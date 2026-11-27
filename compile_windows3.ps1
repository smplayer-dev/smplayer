<#
.SYNOPSIS
    Automated out-of-source Windows build script for SMPlayer (Qt 5.15.2 + MinGW 8.1.0).

.DESCRIPTION
    Downloads and configures a standalone Qt 5.15.2 (win64_mingw81) and MinGW 8.1.0 64-bit
    toolchain via aqtinstall if not already present (and OpenSSL 1.1.x if still hosted by
    Qt mirrors), organizes them into clean descriptive folders (Qt-5.15.2, MinGW-8.1.0),
    displays a configuration summary with an interactive Y/N confirmation prompt, performs
    a clean out-of-source build of SMPlayer and simple_web_server.exe without polluting the
    Git repository, compiles all SMPlayer translation files (.qm), copies Qt language
    translations (qt_xx.qm) and default shortcuts (*.keys), and packages everything into a
    timestamped output folder so previous builds are never overwritten.

.PARAMETER RepoDir
    Path to the SMPlayer repository root containing src\smplayer.pro.
    Defaults to the directory containing this script (or current working directory).

.PARAMETER DistDir
    Base output directory where timestamped build folders will be created.
    Defaults to an "SMPlayer-Build" sibling folder next to RepoDir.

.PARAMETER BaseDir
    Base directory where Qt-5.15.2 and MinGW-8.1.0 toolchains will be installed.
    Defaults to "<SystemDrive>\SMPlayer-Toolchain" (avoid spaces in path for qmake/MinGW).

.PARAMETER Portable
    Controls whether SMPlayer is compiled with PORTABLE_APP enabled ($true by default).
    Pass -Portable:$false for a standard installation build.

.PARAMETER NoPause
    Skips the final pause prompt (useful for CI/CD or unattended automation).

.EXAMPLE
    .\compile_windows.ps1
    Displays configuration, asks Y/N, and builds into a timestamped folder under SMPlayer-Build.

.EXAMPLE
    .\compile_windows.ps1 -Portable:$false -DistDir "D:\Builds\SMPlayer" -NoPause
    Builds the standard edition into a timestamped folder under D:\Builds\SMPlayer without pausing.
#>

#requires -Version 5.1
[CmdletBinding()]
param(
    # SMPlayer repository root (defaults to the folder containing this script).
    [Parameter(Mandatory = $false)]
    [string]$RepoDir = $(if ($PSScriptRoot) { $PSScriptRoot } else { (Get-Location).Path }),

    # Base output folder for timestamped builds (defaults to "SMPlayer-Build" next to RepoDir).
    [Parameter(Mandatory = $false)]
    [string]$DistDir = (Join-Path -Path (Split-Path -Path $RepoDir -Parent) -ChildPath 'smplayer-build'),

    # Toolchain folder for Qt 5.15.2 + MinGW 8.1.0 (avoid spaces in path).
    [Parameter(Mandatory = $false)]
    [string]$BaseDir = (Join-Path -Path (Split-Path -Path $RepoDir -Parent) -ChildPath 'smplayer-toolchain'),

    # Build Portable edition ($true) or Standard edition ($false).
    [Parameter(Mandatory = $false)]
    [bool]$Portable = $true,

    # Skip the final pause prompt (useful for CI/automation).
    [Parameter(Mandatory = $false)]
    [switch]$NoPause
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

function Write-Step {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Message,

        [Parameter(Mandatory = $false)]
        [ConsoleColor]$Color = [ConsoleColor]::Cyan
    )

    Write-Host ''
    Write-Host "[INFO] $Message" -ForegroundColor $Color
}

function New-TimestampedBuildDirectoryPath {
    param(
        [Parameter(Mandatory = $true)]
        [string]$BaseOutputDir
    )

    # Unicode lookalikes legal in NTFS filenames: U+2044 (Fraction Slash) and U+A789 (Modifier Letter Colon)
    $uSlash = [char]0x2044
    $uColon = [char]0xA789
    $now = Get-Date

    $folderName = ('{0:yyyy}{1}{0:MM}{1}{0:dd} {0:HH}{2}{0:mm}{2}{0:ss}' -f $now, $uSlash, $uColon)
    return (Join-Path -Path $BaseOutputDir -ChildPath $folderName)
}

function Find-RepoSubdirectoryWithFiles {
    param(
        [Parameter(Mandatory = $true)]
        [string]$RepoRoot,

        [Parameter(Mandatory = $true)]
        [string[]]$PreferredRelativePaths,

        [Parameter(Mandatory = $true)]
        [string]$FileFilter
    )

    foreach ($rel in $PreferredRelativePaths) {
        $candidate = Join-Path -Path $RepoRoot -ChildPath $rel
        if (Test-Path -LiteralPath $candidate -PathType Container) {
            $files = @(Get-ChildItem -LiteralPath $candidate -Filter $FileFilter -File -ErrorAction SilentlyContinue)
            if ($files.Count -gt 0) {
                return $candidate
            }
        }
    }

    $fallbackFile = Get-ChildItem -LiteralPath $RepoRoot -Filter $FileFilter -File -Recurse -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\\.git\\' } |
        Select-Object -First 1

    if ($null -ne $fallbackFile) {
        return $fallbackFile.DirectoryName
    }

    return $null
}

function Invoke-NativeCommand {
    param(
        [Parameter(Mandatory = $true)]
        [string]$FilePath,

        [Parameter(Mandatory = $false)]
        [string[]]$ArgumentList = @(),

        [Parameter(Mandatory = $false)]
        [string]$WorkingDirectory
    )

    $prevPref = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $pushed = $false
    try {
        if (-not [string]::IsNullOrWhiteSpace($WorkingDirectory)) {
            Push-Location -LiteralPath $WorkingDirectory
            $pushed = $true
        }
        & $FilePath @ArgumentList
        $exitCode = $LASTEXITCODE
    } finally {
        if ($pushed) {
            Pop-Location
        }
        $ErrorActionPreference = $prevPref
    }

    if ($exitCode -ne 0) {
        throw "Command failed with exit code ${exitCode}: $FilePath $($ArgumentList -join ' ')"
    }
}

function Get-PythonExecutable {
    foreach ($candidate in @('python', 'py')) {
        $cmd = Get-Command -Name $candidate -ErrorAction SilentlyContinue
        if ($null -ne $cmd) {
            return $cmd.Source
        }
    }
    throw 'Python was not found in PATH. Install Python 3 to allow automated Qt/MinGW installation via aqtinstall.'
}

function Ensure-AqtInstall {
    param(
        [Parameter(Mandatory = $true)]
        [string]$PythonExe,

        [Parameter(Mandatory = $true)]
        [string]$WorkDir
    )

    $prevPref = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        Push-Location -LiteralPath $WorkDir
        try {
            & $PythonExe -m aqt version *> $null
            $exitCode = $LASTEXITCODE
        } finally {
            Pop-Location
        }
    } finally {
        $ErrorActionPreference = $prevPref
    }

    if ($exitCode -ne 0) {
        Write-Step 'Installing aqtinstall via pip...'
        Invoke-NativeCommand -FilePath $PythonExe -ArgumentList @('-m', 'pip', 'install', '--upgrade', 'aqtinstall') -WorkingDirectory $WorkDir
    }
}

function Invoke-AqtCommand {
    param(
        [Parameter(Mandatory = $true)]
        [string]$PythonExe,

        [Parameter(Mandatory = $true)]
        [string[]]$AqtArgs,

        [Parameter(Mandatory = $true)]
        [string]$WorkDir
    )

    # Run inside $WorkDir so aqtinstall.log is isolated in the temp folder and wiped on exit.
    Invoke-NativeCommand -FilePath $PythonExe -ArgumentList (@('-m', 'aqt') + $AqtArgs) -WorkingDirectory $WorkDir
}

function Ensure-RelocatableQtConf {
    param(
        [Parameter(Mandatory = $true)]
        [string]$QtBinDir
    )

    $qtConfPath = Join-Path -Path $QtBinDir -ChildPath 'qt.conf'
    if (-not (Test-Path -LiteralPath $qtConfPath -PathType Leaf)) {
        [System.IO.File]::WriteAllText($qtConfPath, "[Paths]`r`nPrefix=..`r`n", [System.Text.Encoding]::ASCII)
        return
    }

    $content = [System.IO.File]::ReadAllText($qtConfPath)
    if ($content -match '(?m)^Prefix\s*=') {
        $updated = [regex]::Replace($content, '(?m)^Prefix\s*=.*$', 'Prefix=..')
        if ($updated -ne $content) {
            [System.IO.File]::WriteAllText($qtConfPath, $updated, [System.Text.Encoding]::ASCII)
        }
    } else {
        $updated = $content.TrimEnd() + "`r`nPrefix=..`r`n"
        [System.IO.File]::WriteAllText($qtConfPath, $updated, [System.Text.Encoding]::ASCII)
    }
}

function Find-OpenSslDll {
    param(
        [Parameter(Mandatory = $true)]
        [string]$SearchRoot,

        [Parameter(Mandatory = $true)]
        [string]$DllName
    )

    if (-not (Test-Path -LiteralPath $SearchRoot -PathType Container)) {
        return $null
    }

    $match = Get-ChildItem -LiteralPath $SearchRoot -Filter $DllName -File -Recurse -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -notmatch '\\_smplayer_obj_tmp\\' } |
        Select-Object -First 1
    if ($null -ne $match) {
        return $match.FullName
    }
    return $null
}

function Try-InstallOpenSsl11 {
    param(
        [Parameter(Mandatory = $true)]
        [string]$PythonExe,

        [Parameter(Mandatory = $true)]
        [string]$StageDir,

        [Parameter(Mandatory = $true)]
        [string]$TargetDir,

        [Parameter(Mandatory = $true)]
        [string]$WorkDir
    )

    $prevPref = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        Push-Location -LiteralPath $WorkDir
        try {
            $toolsOutput = & $PythonExe -m aqt list-tool windows desktop 2>$null
            if ($LASTEXITCODE -ne 0) {
                return $false
            }

            $toolName = ($toolsOutput -split '\s+' | Where-Object {
                $_ -match '^tools_openssl' -and $_ -notmatch 'v3' -and $_ -notmatch 'x86$'
            } | Select-Object -First 1)

            if ([string]::IsNullOrWhiteSpace($toolName)) {
                return $false
            }

            $variantsOutput = & $PythonExe -m aqt list-tool windows desktop $toolName 2>$null
            if ($LASTEXITCODE -ne 0) {
                return $false
            }

            $variant = ($variantsOutput -split '\s+' | Where-Object {
                $_ -match 'openssl' -and $_ -match 'x64'
            } | Select-Object -First 1)

            if ([string]::IsNullOrWhiteSpace($variant)) {
                return $false
            }

            Write-Step "Installing OpenSSL 1.1.x ($toolName / $variant) into $TargetDir..."
            & $PythonExe -m aqt install-tool -O $StageDir windows desktop $toolName $variant
            if ($LASTEXITCODE -ne 0) {
                return $false
            }

            $crypto = Get-ChildItem -LiteralPath $StageDir -Filter 'libcrypto-1_1-x64.dll' -File -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
            $ssl    = Get-ChildItem -LiteralPath $StageDir -Filter 'libssl-1_1-x64.dll' -File -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
            if (($null -ne $crypto) -and ($null -ne $ssl)) {
                if (-not (Test-Path -LiteralPath $TargetDir -PathType Container)) {
                    $null = New-Item -Path $TargetDir -ItemType Directory -Force
                }
                Copy-Item -LiteralPath $crypto.FullName -Destination $TargetDir -Force
                Copy-Item -LiteralPath $ssl.FullName -Destination $TargetDir -Force
                return $true
            }
            return $false
        } finally {
            Pop-Location
        }
    } finally {
        $ErrorActionPreference = $prevPref
    }
}

function Show-WelcomeAndConfirm {
    param(
        [Parameter(Mandatory = $true)]
        [string]$RepoPath,

        [Parameter(Mandatory = $true)]
        [string]$BuildOutPath,

        [Parameter(Mandatory = $true)]
        [string]$QtPath,

        [Parameter(Mandatory = $true)]
        [string]$MingwPath,

        [Parameter(Mandatory = $true)]
        [string]$TempPath,

        [Parameter(Mandatory = $true)]
        [string]$QtVer,

        [Parameter(Mandatory = $true)]
        [string]$QtArchitecture,

        [Parameter(Mandatory = $true)]
        [bool]$IsPortable
    )

    $editionLabel = if ($IsPortable) { 'Portable (PORTABLE_APP)' } else { 'Standard' }

    Write-Host ''
    Write-Host '============================================================' -ForegroundColor Cyan
    Write-Host '          SMPlayer Windows Build - Configuration            ' -ForegroundColor Cyan
    Write-Host '============================================================' -ForegroundColor Cyan
    Write-Host "  Repository : $RepoPath" -ForegroundColor White
    Write-Host "  Output Dir : $BuildOutPath" -ForegroundColor White
    Write-Host "  Qt Dir     : $QtPath" -ForegroundColor White
    Write-Host "  MinGW Dir  : $MingwPath" -ForegroundColor White
    Write-Host "  Temp Build : $TempPath" -ForegroundColor DarkGray
    Write-Host "  Toolchain  : Qt $QtVer ($QtArchitecture) + MinGW 8.1.0 x64" -ForegroundColor White
    Write-Host "  Edition    : $editionLabel" -ForegroundColor White
    Write-Host '============================================================' -ForegroundColor Cyan
    Write-Host ''
    Write-Host -NoNewline 'Continue? [Y/N]: ' -ForegroundColor Yellow

    while ($true) {
        $key = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
        $ch = [char]::ToUpperInvariant($key.Character)
        if ($ch -eq 'Y') {
            Write-Host 'Y' -ForegroundColor Green
            return $true
        }
        if ($ch -eq 'N') {
            Write-Host 'N' -ForegroundColor Red
            return $false
        }
    }
}

# Toolchain layout inside $BaseDir
$QtVersion      = '5.15.2'
$QtArch         = 'win64_mingw81'
$QtRoot         = Join-Path -Path $BaseDir -ChildPath "Qt-$QtVersion"
$QtBin          = Join-Path -Path $QtRoot -ChildPath 'bin'
$QtPlugins      = Join-Path -Path $QtRoot -ChildPath 'plugins'
$QtTranslations = Join-Path -Path $QtRoot -ChildPath 'translations'
$MingwRoot      = Join-Path -Path $BaseDir -ChildPath 'MinGW-8.1.0'
$MingwBin       = Join-Path -Path $MingwRoot -ChildPath 'bin'
$OpenSslRoot    = Join-Path -Path $BaseDir -ChildPath 'OpenSSL-1.1'
$ObjDir         = Join-Path -Path $BaseDir -ChildPath '_smplayer_obj_tmp'
$AqtStageDir    = Join-Path -Path $ObjDir -ChildPath '_aqt_stage'

$userAborted = $false

try {
    # 1. Validate repository and prepare timestamped output directory
    $RepoDir = (Resolve-Path -LiteralPath $RepoDir).Path
    $ProFile = Join-Path -Path $RepoDir -ChildPath 'src\smplayer.pro'
    if (-not (Test-Path -LiteralPath $ProFile -PathType Leaf)) {
        throw "smplayer.pro not found in '$RepoDir'. Run this script from the SMPlayer repository root or specify -RepoDir."
    }

    $BuildOutDir = New-TimestampedBuildDirectoryPath -BaseOutputDir $DistDir
    $OutExe      = Join-Path -Path $BuildOutDir -ChildPath 'smplayer.exe'

    # 2. Show configuration summary and require Y or N exclusively
    $confirmed = Show-WelcomeAndConfirm `
        -RepoPath $RepoDir `
        -BuildOutPath $BuildOutDir `
        -QtPath $QtRoot `
        -MingwPath $MingwRoot `
        -TempPath $ObjDir `
        -QtVer $QtVersion `
        -QtArchitecture $QtArch `
        -IsPortable $Portable

    if (-not $confirmed) {
        $userAborted = $true
        Write-Step 'Build cancelled by user.' -Color Yellow
        return
    }

    # 3. Create base and isolated temporary build directories early (for clean logs, staging & build output)
    if (-not (Test-Path -LiteralPath $BaseDir -PathType Container)) {
        Write-Step "Creating toolchain directory: $BaseDir"
        $null = New-Item -Path $BaseDir -ItemType Directory -Force
    }

    Write-Step "Preparing isolated temporary build directory: $ObjDir"
    if (Test-Path -LiteralPath $ObjDir) {
        Remove-Item -LiteralPath $ObjDir -Recurse -Force
    }
    $null = New-Item -Path $ObjDir -ItemType Directory -Force

    # 4. Install Qt 5.15.2, MinGW 8.1.0, and OpenSSL 1.1.x (best-effort) into clean descriptive folders
    $QmakeExe      = Join-Path -Path $QtBin -ChildPath 'qmake.exe'
    $MakeExe       = Join-Path -Path $MingwBin -ChildPath 'mingw32-make.exe'
    $GccExe        = Join-Path -Path $MingwBin -ChildPath 'gcc.exe'
    $OpenSslCrypto = Find-OpenSslDll -SearchRoot $BaseDir -DllName 'libcrypto-1_1-x64.dll'
    $OpenSslSsl    = Find-OpenSslDll -SearchRoot $BaseDir -DllName 'libssl-1_1-x64.dll'

    $needQt      = -not (Test-Path -LiteralPath $QmakeExe -PathType Leaf)
    $needMingw   = -not (Test-Path -LiteralPath $MakeExe -PathType Leaf)
    $needOpenSsl = ($null -eq $OpenSslCrypto) -or ($null -eq $OpenSslSsl)

    if ($needQt -or $needMingw -or $needOpenSsl) {
        $PythonExe = Get-PythonExecutable
        Ensure-AqtInstall -PythonExe $PythonExe -WorkDir $ObjDir
        $null = New-Item -Path $AqtStageDir -ItemType Directory -Force

        if ($needQt) {
            Write-Step "Installing Qt $QtVersion ($QtArch) into $QtRoot..."
            Invoke-AqtCommand -PythonExe $PythonExe -WorkDir $ObjDir -AqtArgs @(
                'install-qt', '-O', $AqtStageDir,
                'windows', 'desktop', $QtVersion, $QtArch
            )

            $stagedQt = Join-Path -Path $AqtStageDir -ChildPath "$QtVersion\mingw81_64"
            if (-not (Test-Path -LiteralPath $stagedQt -PathType Container)) {
                throw "Expected staged Qt folder not found: $stagedQt"
            }
            if (Test-Path -LiteralPath $QtRoot) {
                Remove-Item -LiteralPath $QtRoot -Recurse -Force
            }
            Move-Item -LiteralPath $stagedQt -Destination $QtRoot
            Ensure-RelocatableQtConf -QtBinDir $QtBin
        }

        if ($needMingw) {
            Write-Step "Installing MinGW 8.1.0 (64-bit) into $MingwRoot..."
            Invoke-AqtCommand -PythonExe $PythonExe -WorkDir $ObjDir -AqtArgs @(
                'install-tool', '-O', $AqtStageDir,
                'windows', 'desktop', 'tools_mingw', 'qt.tools.win64_mingw810'
            )

            $stagedMingw = Join-Path -Path $AqtStageDir -ChildPath 'Tools\mingw810_64'
            if (-not (Test-Path -LiteralPath $stagedMingw -PathType Container)) {
                throw "Expected staged MinGW folder not found: $stagedMingw"
            }
            if (Test-Path -LiteralPath $MingwRoot) {
                Remove-Item -LiteralPath $MingwRoot -Recurse -Force
            }
            Move-Item -LiteralPath $stagedMingw -Destination $MingwRoot
        }

        if ($needOpenSsl) {
            $installedSsl = Try-InstallOpenSsl11 -PythonExe $PythonExe -StageDir $AqtStageDir -TargetDir $OpenSslRoot -WorkDir $ObjDir
            if ($installedSsl) {
                $OpenSslCrypto = Find-OpenSslDll -SearchRoot $BaseDir -DllName 'libcrypto-1_1-x64.dll'
                $OpenSslSsl    = Find-OpenSslDll -SearchRoot $BaseDir -DllName 'libssl-1_1-x64.dll'
            }
        }
    }

    # 5. Verify installed toolchain binaries and platform plugin
    $QWindowsDll = Join-Path -Path $QtPlugins -ChildPath 'platforms\qwindows.dll'
    foreach ($requiredFile in @($QmakeExe, $MakeExe, $GccExe, $QWindowsDll)) {
        if (-not (Test-Path -LiteralPath $requiredFile -PathType Leaf)) {
            throw "Required toolchain file not found: $requiredFile"
        }
    }
    Ensure-RelocatableQtConf -QtBinDir $QtBin

    # 6. Prepend toolchain directories to process PATH
    $env:PATH = "$QtBin;$MingwBin;$env:PATH"

    # 7. Generate svn_revision.h inside temporary build directory
    Write-Step 'Generating svn_revision.h inside temporary build directory...'
    $Rev = '0'
    $GitCmd = Get-Command -Name 'git' -ErrorAction SilentlyContinue
    if ($null -ne $GitCmd) {
        $gitOutput = & $GitCmd.Source -C $RepoDir rev-list --count HEAD 2>$null
        if ($LASTEXITCODE -eq 0 -and -not [string]::IsNullOrWhiteSpace($gitOutput)) {
            $Rev = $gitOutput.Trim()
        }
    }

    $SvnRevFile = Join-Path -Path $ObjDir -ChildPath 'svn_revision.h'
    [System.IO.File]::WriteAllText($SvnRevFile, "#define SVN_REVISION `"$Rev`"`r`n", [System.Text.Encoding]::ASCII)

    # 8. Run out-of-source qmake and mingw32-make
    $ObjDirPosix = $ObjDir -replace '\\', '/'
    $QmakeArgs = @($ProFile, "INCLUDEPATH+=$ObjDirPosix")
    if ($Portable) {
        $QmakeArgs += 'DEFINES+=PORTABLE_APP'
    }

    Push-Location -LiteralPath $ObjDir
    try {
        Write-Step 'Running out-of-source qmake...'
        Invoke-NativeCommand -FilePath $QmakeExe -ArgumentList $QmakeArgs

        $Jobs = [Environment]::ProcessorCount
        Write-Step "Compiling SMPlayer with $Jobs threads..."
        Invoke-NativeCommand -FilePath $MakeExe -ArgumentList @("-j$Jobs")
    } finally {
        Pop-Location
    }

    $CompiledExe = Join-Path -Path $ObjDir -ChildPath 'release\smplayer.exe'
    if (-not (Test-Path -LiteralPath $CompiledExe -PathType Leaf)) {
        throw "Compiled executable not found: $CompiledExe"
    }

    # 9. Compile simple_web_server.exe out-of-source (supports main.c + mongoose.c or single-file layouts)
    $WebServerDir = Find-RepoSubdirectoryWithFiles -RepoRoot $RepoDir -PreferredRelativePaths @('webserver', 'src\webserver') -FileFilter '*.c'
    $CompiledWebServer = Join-Path -Path $ObjDir -ChildPath 'simple_web_server.exe'
    if ($null -ne $WebServerDir) {
        $webServerSources = @(Get-ChildItem -LiteralPath $WebServerDir -Filter '*.c' -File | ForEach-Object { $_.FullName })
        if ($webServerSources.Count -gt 0) {
            Write-Step "Compiling simple_web_server.exe from $WebServerDir..."
            $gccArgs = @('-O2', "-I$WebServerDir") + $webServerSources + @('-o', $CompiledWebServer, '-lws2_32')
            Invoke-NativeCommand -FilePath $GccExe -ArgumentList $gccArgs
        }
    } else {
        Write-Host '[WARN] webserver C sources were not found in the repository; skipping simple_web_server.exe.' -ForegroundColor Yellow
    }

    # 10. Create fresh timestamped output directory (never deletes or overwrites previous builds)
    Write-Step "Creating output directory: $BuildOutDir"
    $null = New-Item -Path $BuildOutDir -ItemType Directory -Force

    Copy-Item -LiteralPath $CompiledExe -Destination $OutExe -Force
    if (Test-Path -LiteralPath $CompiledWebServer -PathType Leaf) {
        Copy-Item -LiteralPath $CompiledWebServer -Destination (Join-Path -Path $BuildOutDir -ChildPath 'simple_web_server.exe') -Force
    }

    # 11. Copy required and optional Qt5 DLLs
    Write-Step 'Copying Qt5 DLLs...'
    $RequiredQtDlls = @('Qt5Core.dll', 'Qt5Gui.dll', 'Qt5Network.dll', 'Qt5Qml.dll', 'Qt5Widgets.dll', 'Qt5Xml.dll')
    foreach ($dll in $RequiredQtDlls) {
        $srcDll = Join-Path -Path $QtBin -ChildPath $dll
        if (-not (Test-Path -LiteralPath $srcDll -PathType Leaf)) {
            throw "Missing required Qt5 DLL: $srcDll"
        }
        Copy-Item -LiteralPath $srcDll -Destination $BuildOutDir -Force
    }

    $OptionalQtDlls = @('Qt5Svg.dll', 'Qt5DBus.dll', 'libEGL.dll', 'libGLESv2.dll', 'd3dcompiler_47.dll', 'opengl32sw.dll')
    foreach ($dll in $OptionalQtDlls) {
        $srcDll = Join-Path -Path $QtBin -ChildPath $dll
        if (Test-Path -LiteralPath $srcDll -PathType Leaf) {
            Copy-Item -LiteralPath $srcDll -Destination $BuildOutDir -Force
        }
    }

    # 12. Copy OpenSSL 1.1.x DLLs (optional; required for HTTPS in Qt5)
    Write-Step 'Copying OpenSSL 1.1.x DLLs...'
    if (($null -ne $OpenSslCrypto) -and ($null -ne $OpenSslSsl)) {
        Copy-Item -LiteralPath $OpenSslCrypto -Destination $BuildOutDir -Force
        Copy-Item -LiteralPath $OpenSslSsl -Destination $BuildOutDir -Force
    } else {
        Write-Host '[WARN] OpenSSL 1.1.x DLLs (libcrypto-1_1-x64.dll / libssl-1_1-x64.dll) are not available on Qt mirrors.' -ForegroundColor Yellow
        Write-Host "[WARN] Build will succeed, but HTTPS features require placing those two DLLs in '$OpenSslRoot' or next to smplayer.exe." -ForegroundColor Yellow
    }

    # 13. Copy MinGW runtime DLLs
    Write-Step 'Copying MinGW runtime DLLs...'
    $RuntimeDlls = @('libgcc_s_seh-1.dll', 'libstdc++-6.dll', 'libwinpthread-1.dll')
    foreach ($dll in $RuntimeDlls) {
        $fromMingw = Join-Path -Path $MingwBin -ChildPath $dll
        $fromQt    = Join-Path -Path $QtBin -ChildPath $dll

        if (Test-Path -LiteralPath $fromMingw -PathType Leaf) {
            Copy-Item -LiteralPath $fromMingw -Destination $BuildOutDir -Force
        } elseif (Test-Path -LiteralPath $fromQt -PathType Leaf) {
            Copy-Item -LiteralPath $fromQt -Destination $BuildOutDir -Force
        } else {
            throw "Missing required MinGW runtime DLL: $dll"
        }
    }

    # 14. Copy Qt5 plugins
    Write-Step 'Copying Qt5 plugins...'
    $PlatformsDst = Join-Path -Path $BuildOutDir -ChildPath 'platforms'
    $null = New-Item -Path $PlatformsDst -ItemType Directory -Force
    Copy-Item -LiteralPath $QWindowsDll -Destination $PlatformsDst -Force

    $OptionalPlugins = @{
        'styles'       = @('qwindowsvistastyle.dll')
        'iconengines'  = @('qsvgicon.dll')
        'bearer'       = @('qgenericbearer.dll')
        'imageformats' = @('qgif.dll', 'qicns.dll', 'qico.dll', 'qjpeg.dll', 'qsvg.dll', 'qtga.dll', 'qtiff.dll', 'qwbmp.dll', 'qwebp.dll')
    }

    foreach ($pluginDir in $OptionalPlugins.Keys) {
        $dstDir = Join-Path -Path $BuildOutDir -ChildPath $pluginDir
        foreach ($dll in $OptionalPlugins[$pluginDir]) {
            $srcFile = Join-Path -Path (Join-Path -Path $QtPlugins -ChildPath $pluginDir) -ChildPath $dll
            if (Test-Path -LiteralPath $srcFile -PathType Leaf) {
                if (-not (Test-Path -LiteralPath $dstDir -PathType Container)) {
                    $null = New-Item -Path $dstDir -ItemType Directory -Force
                }
                Copy-Item -LiteralPath $srcFile -Destination $dstDir -Force
            }
        }
    }

    # 15. Compile SMPlayer translations in ASCII temp dir (lrelease.exe uses ANSI argv and mangles Unicode paths)
    $TransDst = Join-Path -Path $BuildOutDir -ChildPath 'translations'
    $null = New-Item -Path $TransDst -ItemType Directory -Force

    $LreleaseExe = Join-Path -Path $QtBin -ChildPath 'lrelease.exe'
    $TsDir       = Join-Path -Path $RepoDir -ChildPath 'src\translations'
    if ((Test-Path -LiteralPath $LreleaseExe -PathType Leaf) -and (Test-Path -LiteralPath $TsDir -PathType Container)) {
        Write-Step 'Compiling SMPlayer translations...'
        $TempTransDir = Join-Path -Path $ObjDir -ChildPath 'translations'
        $null = New-Item -Path $TempTransDir -ItemType Directory -Force

        $tsFiles = @(Get-ChildItem -LiteralPath $TsDir -Filter '*.ts' -File)
        foreach ($ts in $tsFiles) {
            $qmTemp = Join-Path -Path $TempTransDir -ChildPath "$($ts.BaseName).qm"
            Invoke-NativeCommand -FilePath $LreleaseExe -ArgumentList @('-silent', $ts.FullName, '-qm', $qmTemp)
        }

        Get-ChildItem -LiteralPath $TempTransDir -Filter '*.qm' -File |
            Copy-Item -Destination $TransDst -Force
    }

    # Copy strictly qt_xx.qm / qt_xx_XX.qm from Qt toolchain and repo qt-translations
    Write-Step 'Copying Qt language translations (qt_xx.qm)...'
    $qtTranslationDirs = @($QtTranslations, (Join-Path -Path $RepoDir -ChildPath 'qt-translations'))
    foreach ($qtDir in $qtTranslationDirs) {
        if (Test-Path -LiteralPath $qtDir -PathType Container) {
            $qtLangFiles = @(
                Get-ChildItem -LiteralPath $qtDir -Filter 'qt_*.qm' -File -ErrorAction SilentlyContinue |
                    Where-Object { $_.Name -match '^qt_[a-z]{2}(_[A-Z]{2})?\.qm$' }
            )
            foreach ($qm in $qtLangFiles) {
                Copy-Item -LiteralPath $qm.FullName -Destination $TransDst -Force
            }
        }
    }

    # 16. Copy default shortcut presets (*.keys)
    $ShortcutsSrc = Find-RepoSubdirectoryWithFiles -RepoRoot $RepoDir -PreferredRelativePaths @('src\shortcuts', 'shortcuts') -FileFilter '*.keys'
    if ($null -ne $ShortcutsSrc) {
        Write-Step "Copying shortcut presets from $ShortcutsSrc..."
        $ShortcutsDst = Join-Path -Path $BuildOutDir -ChildPath 'shortcuts'
        $null = New-Item -Path $ShortcutsDst -ItemType Directory -Force
        $keyFiles = @(Get-ChildItem -LiteralPath $ShortcutsSrc -Filter '*.keys' -File)
        foreach ($kf in $keyFiles) {
            Copy-Item -LiteralPath $kf.FullName -Destination $ShortcutsDst -Force
        }
    } else {
        Write-Host '[WARN] Shortcut preset files (*.keys) were not found in the repository.' -ForegroundColor Yellow
    }

    Write-Host ''
    Write-Host '[OK] Clean build succeeded.' -ForegroundColor Green
    Write-Host "[OK] Toolchain folder: $BaseDir" -ForegroundColor Green
    Write-Host "[OK] Output folder:    $BuildOutDir" -ForegroundColor Green
    Write-Host "[OK] Repository clean: $RepoDir" -ForegroundColor Green
} catch {
    Write-Host ''
    Write-Host "[ERROR] $($_.Exception.Message)" -ForegroundColor Red
    exit 1
} finally {
    if (Test-Path -LiteralPath $ObjDir) {
        Write-Step 'Cleaning up temporary build directory...' -Color DarkGray
        Remove-Item -LiteralPath $ObjDir -Recurse -Force -ErrorAction SilentlyContinue
    }
    if (-not $NoPause) {
        Write-Host ''
        Pause
    }
    if ($userAborted) {
        exit 1
    }
}