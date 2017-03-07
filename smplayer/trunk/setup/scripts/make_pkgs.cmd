@echo off
if exist "turnon_echo" (
  @echo on
)

:: Reset working dir especially when using 'Run as administrator'
@cd /d "%~dp0"

echo This batch file can help you to create a packages for SMPlayer and MPlayer.
echo.
echo This script will temporarily rename the smplayer-build and mplayer directories.
echo Make sure these files ^& directories are not opened when running the script.
echo.
echo 1 - NSIS                          10 - NSIS [32-bit/64-bit]
echo 2 - NSIS [64-bit]                 11 - Portable [32-bit/64-bit]
echo 3 - Portable                      20 - Portable SFX [32-bit/64-bit]
echo 4 - Portable [64-bit]
echo 5 - Without MPlayer
echo 6 - Without MPlayer [64-bit]
echo.

:: Relative directory of all the source files to this script
set TOP_LEVEL_DIR=..

:: Reset in case ran again in same command prompt instance
set ALL_PKG_VER=
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=
set VER_REV_CMD=
set MAKENSIS_EXE_PATH=

:: NSIS path
if exist "nsis_path" (
  for /f "tokens=*" %%y in ('type nsis_path') do if exist %%y set MAKENSIS_EXE_PATH="%%y"
)

if not defined MAKENSIS_EXE_PATH (
  for %%x in ("%PROGRAMFILES(X86)%\NSIS\Unicode\makensis.exe" "%PROGRAMFILES%\NSIS\Unicode\makensis.exe") do if exist %%x set MAKENSIS_EXE_PATH=%%x
)

if not defined MAKENSIS_EXE_PATH (
  echo Warning: Unable to locate NSIS in the default path, create the file ^'nsis_path^' with the full correct path
  echo to makensis.exe or the existing ^'nsis_path^' may be incorrect.
  echo.
)

:: Works only in Vista+
where /q where.exe 2>NUL && (
  where /q 7za.exe 2>NUL || (
  echo Warning: 7za.exe not found in path or current directory!
  echo.
  )
)

set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-build
set SMPLAYER_DIR64=%TOP_LEVEL_DIR%\smplayer-build64
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer
set OUTPUT_DIR=%TOP_LEVEL_DIR%\output
set PORTABLE_EXE_DIR=%TOP_LEVEL_DIR%\portable

:cmdline_parsing
if "%1" == ""               goto reask
for %%w in (1 2 3 4 5 6 10 11 20) do (
  if "%1" == "%%w" (
    set USER_CHOICE=%%w
    goto pkgver
  )
)

echo Unknown option: "%1"
echo.
goto superend

:reask
set USER_CHOICE=
set /P USER_CHOICE="Choose an action: "
echo.

for %%z in (1 2 3 4 5 6 10 11 20) do if "%USER_CHOICE%" == "%%z" goto pkgver
if "%USER_CHOICE%" == ""  goto superend
goto reask

:pkgver
if exist "pkg_version" (
  for /f "tokens=*" %%i in ('type pkg_version') do set ALL_PKG_VER=%%i
  goto parse_version
)

echo Format: VER_MAJOR.VER_MINOR.VER_BUILD[.VER_REVISION]
echo VER_REVISION is optional (set to 0 if blank)
:pkgver_manual
echo.
set ALL_PKG_VER=
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=
set VER_REV_CMD=
set /p ALL_PKG_VER="Version: "
echo.

:parse_version
for /f "tokens=1 delims=." %%j in ("%ALL_PKG_VER%")  do set VER_MAJOR=%%j
for /f "tokens=2 delims=." %%k in ("%ALL_PKG_VER%")  do set VER_MINOR=%%k
for /f "tokens=3 delims=." %%l in ("%ALL_PKG_VER%")  do set VER_BUILD=%%l
for /f "tokens=4 delims=." %%m in ("%ALL_PKG_VER%")  do set VER_REVISION=%%m

echo %VER_MAJOR%|findstr /r /c:"^[0-9][0-9]*$" >nul
if errorlevel 1 (
  echo Invalid version string. VER_MAJOR is not defined or is not a number [#.x.x]
  goto pkgver_manual & ver>nul
)

echo %VER_MINOR%|findstr /r /c:"^[0-9][0-9]*$" >nul
if errorlevel 1 (
  echo Invalid version string. VER_MINOR is not defined or is not a number [x.#.x]
  goto pkgver_manual & ver>nul
)
echo %VER_BUILD%|findstr /r /c:"^[0-9][0-9]*$" >nul
if errorlevel 1 (
  echo Invalid version string. VER_BUILD is not defined or is not a number [x.x.#]
  goto pkgver_manual & ver>nul
)

if defined VER_REVISION (
  echo %VER_REVISION%|findstr /r /c:"^[0-9][0-9]*$" >nul
  if errorlevel 1 (
    echo Invalid version string. VER_REVISION is not a number [x.x.x.#]
    goto pkgver_manual & ver>nul
  ) else (
    set VER_REV_CMD=/DVER_REVISION=%VER_REVISION% & ver>nul
  )
) else (
  set VER_REV_CMD=
)

if not defined VER_REVISION (
  set PORTABLE_PKG_VER=%ALL_PKG_VER%.0
) else (
  set PORTABLE_PKG_VER=%ALL_PKG_VER%
)

if "%USER_CHOICE%" == "1"  goto nsispkg
if "%USER_CHOICE%" == "2"  goto nsispkg64
if "%USER_CHOICE%" == "3"  goto portable
if "%USER_CHOICE%" == "4"  goto portable64
if "%USER_CHOICE%" == "5"  goto nomplayer
if "%USER_CHOICE%" == "6"  goto nomplayer64
if "%USER_CHOICE%" == "10"  goto nsispkg
if "%USER_CHOICE%" == "11"  goto portable
if "%USER_CHOICE%" == "20"  goto portablesfx
:: Should not happen
goto end

:nsispkg

echo --- SMPlayer NSIS Package [32-bit] ---
echo.

if exist %TOP_LEVEL_DIR%\smplayer-build (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% %VER_REV_CMD% %TOP_LEVEL_DIR%\smplayer.nsi
)

if not "%USER_CHOICE%" == "10"  goto end

:nsispkg64
echo --- SMPlayer NSIS Package [64-bit] ---
echo.

if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% %VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
)

goto end

:portable
echo --- SMPlayer Portable Package [32-bit] ---
echo.

if not exist %SMPLAYER_DIR% (
  echo SMPlayer sources missing.
  goto end
)

REM Check for portable exes
if not exist "%PORTABLE_EXE_DIR%\smplayer-portable.exe" (
  echo SMPlayer portable EXE not found!
  goto end
)

REM if not exist "%PORTABLE_EXE_DIR%\smtube-portable.exe" (
REM  echo Warning: SMTube portable EXE not found!
REM )

if exist "%SMPLAYER_DIR%\QtCore4.dll" (
  set PORTABLE_DIR_NAME=smplayer-portable-%PORTABLE_PKG_VER%-Qt4
) else (
  set PORTABLE_DIR_NAME=smplayer-portable-%PORTABLE_PKG_VER%
)

ren "%SMPLAYER_DIR%" "%PORTABLE_DIR_NAME%"
set SMPLAYER_PORTABLE_DIR="%TOP_LEVEL_DIR%\%PORTABLE_DIR_NAME%"

if not exist "%SMPLAYER_PORTABLE_DIR%" (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

echo Backing up files...

ren "%SMPLAYER_PORTABLE_DIR%\smplayer.exe" smplayer.bak
REM ren "%SMPLAYER_PORTABLE_DIR%\smtube.exe" smtube.bak

echo Creating screenshots dir...

mkdir "%SMPLAYER_PORTABLE_DIR%\screenshots"

echo Creating smplayer.ini...

echo [%%General]> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo screenshot_directory=.\\screenshots>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo.>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo [smplayer]>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo check_if_upgraded=false>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"

echo Creating smplayer_orig.ini...

echo [%%General]> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo screenshot_directory=.\\screenshots>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo.>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo [smplayer]>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo check_if_upgraded=false>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"

REM echo Creating mplayer config...

REM echo ^<cachedir^>../fontconfig^</cachedir^>> "%SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf"

echo Copying portable .exe...

copy /y "%PORTABLE_EXE_DIR%\smplayer-portable.exe" "%SMPLAYER_PORTABLE_DIR%\smplayer.exe"
REM copy /y "%PORTABLE_EXE_DIR%\smtube-portable.exe" "%SMPLAYER_PORTABLE_DIR%\smtube.exe"

echo Finalizing package...
7za a -t7z "%OUTPUT_DIR%\%PORTABLE_DIR_NAME%.7z" "%SMPLAYER_PORTABLE_DIR%" -xr!*.bak* -xr!qxtcore.dll -xr!mplayer64.exe -xr!mencoder.exe -xr!mencoder64.exe -xr!mplayer64.exe.debug -xr!mencoder64.exe.debug -xr!mplayer.exe.debug -xr!mencoder.exe.debug -xr!gdb.exe -xr!gdb64.exe -xr!vfw2menc.exe -xr!buildinfo -xr!buildinfo64 -xr!buildinfo-mencoder-32 -xr!buildinfo-mencoder-debug-32 -xr!buildinfo-mplayer-32 -xr!buildinfo-mplayer-debug-32 -xr!buildinfo-mencoder-64 -xr!buildinfo-mencoder-debug-64 -xr!buildinfo-mplayer-64 -xr!buildinfo-mplayer-debug-64 -xr!mpv64.exe -xr!mpv64.com -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir "%SMPLAYER_PORTABLE_DIR%\screenshots"
del "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
del "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
del "%SMPLAYER_PORTABLE_DIR%\smplayer.exe"
REM del "%SMPLAYER_PORTABLE_DIR%\smtube.exe"
REM del "%SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf"
ren "%SMPLAYER_PORTABLE_DIR%\smplayer.bak" smplayer.exe
REM ren "%SMPLAYER_PORTABLE_DIR%\smtube.bak" smtube.exe
ren "%SMPLAYER_PORTABLE_DIR%" smplayer-build

if not "%USER_CHOICE%" == "11"  goto end

:portable64
echo --- SMPlayer Portable Package [64-bit] ---
echo.

if not exist %SMPLAYER_DIR64% (
  echo SMPlayer sources missing.
  goto end
)

REM Check for portable exes
if not exist "%PORTABLE_EXE_DIR%\smplayer-portable64.exe" (
  echo SMPlayer portable EXE not found!
  goto end
)

REM if not exist "%PORTABLE_EXE_DIR%\smtube-portable64.exe" (
REM  echo Warning: SMTube portable EXE not found!
REM )

set PORTABLE_DIR_NAME=smplayer-portable-%PORTABLE_PKG_VER%-x64

ren "%SMPLAYER_DIR64%" "%PORTABLE_DIR_NAME%"
set SMPLAYER_PORTABLE_DIR="%TOP_LEVEL_DIR%\%PORTABLE_DIR_NAME%"

if not exist "%SMPLAYER_PORTABLE_DIR%" (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

echo Backing up files...

ren "%SMPLAYER_PORTABLE_DIR%\smplayer.exe" smplayer.bak
REM ren "%SMPLAYER_PORTABLE_DIR%\smtube.exe" smtube.bak
ren "%SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe" mplayer.exe.bak32
ren "%SMPLAYER_PORTABLE_DIR%\mplayer\mplayer64.exe" mplayer.exe

if exist "%SMPLAYER_PORTABLE_DIR%\mpv\fonts" (
  ren "%SMPLAYER_PORTABLE_DIR%\mpv\fonts" fonts.bak32
)
if exist "%SMPLAYER_PORTABLE_DIR%\mpv\mpv" (
  ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv" mpv.bak32
)

ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.exe" mpv.exe.bak32
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.com" mpv.com.bak32
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv64.exe" mpv.exe
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv64.com" mpv.com

echo Creating screenshots dir...

mkdir "%SMPLAYER_PORTABLE_DIR%\screenshots"

echo Creating smplayer.ini...

echo [%%General]> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo screenshot_directory=.\\screenshots>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo.>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo [smplayer]>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
echo check_if_upgraded=false>> "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"

echo Creating smplayer_orig.ini...

echo [%%General]> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo screenshot_directory=.\\screenshots>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo.>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo [smplayer]>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
echo check_if_upgraded=false>> "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"

REM echo Creating mplayer config...

REM echo ^<cachedir^>../fontconfig^</cachedir^>> "%SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf"

echo Copying portable .exe...

copy /y "%PORTABLE_EXE_DIR%\smplayer-portable64.exe" "%SMPLAYER_PORTABLE_DIR%\smplayer.exe"
REM copy /y "%PORTABLE_EXE_DIR%\smtube-portable64.exe" "%SMPLAYER_PORTABLE_DIR%\smtube.exe"

echo Finalizing package...
7za a -t7z "%OUTPUT_DIR%\%PORTABLE_DIR_NAME%.7z" "%SMPLAYER_PORTABLE_DIR%" -xr!*.bak* -xr!qxtcore.dll -xr!mencoder.exe -xr!mencoder64.exe  -xr!mplayer64.exe.debug -xr!mencoder64.exe.debug -xr!mplayer.exe.debug -xr!mencoder.exe.debug -xr!gdb.exe -xr!gdb64.exe -xr!vfw2menc.exe -xr!codecs -xr!buildinfo -xr!buildinfo64 -xr!buildinfo-mencoder-32 -xr!buildinfo-mencoder-debug-32 -xr!buildinfo-mplayer-32 -xr!buildinfo-mplayer-debug-32 -xr!buildinfo-mencoder-64 -xr!buildinfo-mencoder-debug-64 -xr!buildinfo-mplayer-64 -xr!buildinfo-mplayer-debug-64 -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir "%SMPLAYER_PORTABLE_DIR%\screenshots"
del "%SMPLAYER_PORTABLE_DIR%\smplayer.ini"
del "%SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini"
del "%SMPLAYER_PORTABLE_DIR%\smplayer.exe"
REM del "%SMPLAYER_PORTABLE_DIR%\smtube.exe"
ren "%SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe" mplayer64.exe
ren "%SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe.bak32" mplayer.exe
REM del "%SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf"
ren "%SMPLAYER_PORTABLE_DIR%\smplayer.bak" smplayer.exe
REM ren "%SMPLAYER_PORTABLE_DIR%\smtube.bak" smtube.exe

ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.exe" mpv64.exe
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.com" mpv64.com
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.exe.bak32" mpv.exe
ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.com.bak32" mpv.com

if exist "%SMPLAYER_PORTABLE_DIR%\mpv\fonts.bak32" (
  ren "%SMPLAYER_PORTABLE_DIR%\mpv\fonts.bak32" fonts
)
if exist "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.bak32" (
  ren "%SMPLAYER_PORTABLE_DIR%\mpv\mpv.bak32" mpv
)

ren "%SMPLAYER_PORTABLE_DIR%" smplayer-build64

goto end

:portablesfx

if exist %PORTABLE_EXE_DIR%\smplayer-portable.exe (
  if exist %TOP_LEVEL_DIR%\smplayer-build (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% %VER_REV_CMD% %TOP_LEVEL_DIR%\smportable.nsi
  )
)

if exist %PORTABLE_EXE_DIR%\smplayer-portable64.exe (
  if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% %VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smportable.nsi
  )
)

goto end

:nomplayer
echo --- Creating SMPlayer w/o MPlayer Package [32-bit] ---
echo.

ren %SMPLAYER_DIR% smplayer-%ALL_PKG_VER%
set SMPLAYER_NOMP_DIR=%TOP_LEVEL_DIR%\smplayer-%ALL_PKG_VER%

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-%ALL_PKG_VER%_without_mplayer.7z %SMPLAYER_NOMP_DIR% -xr!mplayer -mx9 >nul

ren %SMPLAYER_NOMP_DIR% smplayer-build

echo.
echo Restoring source folder(s) back to its original state....

goto end

:nomplayer64
echo --- Creating SMPlayer w/o MPlayer Package [64-bit] ---
echo.

ren %SMPLAYER_DIR64% smplayer-%ALL_PKG_VER%-x64
set SMPLAYER_NOMP_DIR=%TOP_LEVEL_DIR%\smplayer-%ALL_PKG_VER%-x64

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-%ALL_PKG_VER%-x64_without_mplayer.7z %SMPLAYER_NOMP_DIR% -xr!mplayer -mx9 >nul

ren %SMPLAYER_NOMP_DIR% smplayer-build64

echo.
echo Restoring source folder(s) back to its original state....

goto end

:end

timeout /t 10

:superend
set ALL_PKG_VER=
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=
