@echo off
if exist "turnon_echo" (
  @echo on
)

:: Reset working dir especially when using 'Run as administrator'
@cd /d "%~dp0"

echo This batch file can help you to create a packages for SMPlayer and MPlayer/MPV.
echo.
echo This script will temporarily rename the smplayer-build and mplayer/mpv directories.
echo Make sure these files ^& directories are not opened when running the script.
echo.
echo 1 - NSIS                          10 - NSIS [32-bit/64-bit]
echo 2 - NSIS [64-bit]                 11 - Portable [32-bit/64-bit]
echo 3 - Portable                      20 - Portable SFX [32-bit/64-bit]
echo 4 - Portable [64-bit]
echo 5 - Without MPlayer
echo 6 - Without MPlayer [64-bit]
echo.
echo 31 - NSIS MPV                     40 - NSIS MPV [32-bit/64-bit]
echo 32 - NSIS MPV [64-bit]            41 - Portable MPV [32-bit/64-bit]
echo 33 - Portable MPV
echo 34 - Portable MPV [64-bit]
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
set MPV_DIR=%TOP_LEVEL_DIR%\mpv
set OUTPUT_DIR=%TOP_LEVEL_DIR%\output
set PORTABLE_EXE_DIR=%TOP_LEVEL_DIR%\portable

:reask
set USER_CHOICE=
set /P USER_CHOICE="Choose an action: "
echo.

for %%z in (1 2 3 4 5 6 10 11 20 31 32 33 34 40 41) do if "%USER_CHOICE%" == "%%z" goto pkgver
if "%USER_CHOICE%" == ""  goto superend
goto reask

:pkgver
if exist "pkg_version" (
  for /f "tokens=*" %%i in ('type pkg_version') do set ALL_PKG_VER=%%i
  goto parse_version
)

echo Format: VER_MAJOR.VER_MINOR.VER_BUILD[.VER_REVISION]
echo VER_REVISION is optional (set to 0 if blank)
echo.
:pkgver_manual
set /p ALL_PKG_VER="Version: "
echo.

:parse_version
for /f "tokens=1 delims=." %%j in ("%ALL_PKG_VER%")  do set VER_MAJOR=%%j
for /f "tokens=2 delims=." %%k in ("%ALL_PKG_VER%")  do set VER_MINOR=%%k
for /f "tokens=3 delims=." %%l in ("%ALL_PKG_VER%")  do set VER_BUILD=%%l
for /f "tokens=4 delims=." %%m in ("%ALL_PKG_VER%")  do set VER_REVISION=%%m

if [%VER_MAJOR%]==[] (
  echo Major Version # must be specified [#.x.x]
  echo.
  goto pkgver_manual
)

if [%VER_MINOR%]==[] (
  echo Minor Version # must be specified [x.#.x]
  echo.
  goto pkgver_manual
)

if [%VER_BUILD%]==[] (
  echo Build Version # must be specified [x.x.#]
  echo.
  goto pkgver_manual
)

if [%VER_REVISION%]==[] (
  set VER_REV_CMD=
) else (
  set VER_REV_CMD= /DVER_REVISION=%VER_REVISION%
)

if "%USER_CHOICE%" == "1"  goto nsispkg
if "%USER_CHOICE%" == "2"  goto nsispkg64
if "%USER_CHOICE%" == "3"  goto portable
if "%USER_CHOICE%" == "4"  goto portable64
if "%USER_CHOICE%" == "5"  goto nomplayer
if "%USER_CHOICE%" == "6"  goto nomplayer64
if "%USER_CHOICE%" == "31"  goto testsymboliclink
if "%USER_CHOICE%" == "32"  goto testsymboliclink
if "%USER_CHOICE%" == "33"  goto testsymboliclink
if "%USER_CHOICE%" == "34"  goto testsymboliclink
if "%USER_CHOICE%" == "10"  goto nsispkg
if "%USER_CHOICE%" == "11"  goto portable
if "%USER_CHOICE%" == "20"  goto portablesfx
:: Should not happen
goto end

:nsispkg

echo --- SMPlayer NSIS Package [32-bit] ---
echo.

if exist %TOP_LEVEL_DIR%\smplayer-build (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smplayer.nsi
)

if not "%USER_CHOICE%" == "10"  goto end

:nsispkg64
echo --- SMPlayer NSIS Package [64-bit] ---
echo.

if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
)

goto end

:portable
:: Check for portable exes
echo --- SMPlayer Portable Package [32-bit] ---
echo.

if not exist %PORTABLE_EXE_DIR%\smplayer-portable.exe (
  echo SMPlayer portable EXE not found!
  goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable.exe (
  echo Warning: SMTube portable EXE not found!
)

ren %SMPLAYER_DIR% smplayer-portable-%ALL_PKG_VER%
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER%

if not exist %TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER% (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

::
echo Backing up files...

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak

::
echo Creating screenshots dir...

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

::
echo Creating smplayer.ini...

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini

::
echo Creating smplayer_orig.ini...

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini

::
echo Creating mplayer config...

echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf

::
echo Copying portable .exe...

copy /y %PORTABLE_EXE_DIR%\smplayer-portable.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%ALL_PKG_VER%.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!qxtcore.dll -xr!mplayer64.exe -xr!mencoder.exe -xr!mencoder64.exe -xr!mplayer64.exe.debug -xr!mencoder64.exe.debug -xr!mplayer.exe.debug -xr!mencoder.exe.debug -xr!buildinfo -xr!buildinfo64 -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir %SMPLAYER_PORTABLE_DIR%\screenshots
del %SMPLAYER_PORTABLE_DIR%\smplayer.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer.exe
del %SMPLAYER_PORTABLE_DIR%\smtube.exe
del %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR% smplayer-build

if not "%USER_CHOICE%" == "11"  goto end

:portable64
echo --- SMPlayer Portable Package [64-bit] ---
echo.

:: Check for portable exes
if not exist %PORTABLE_EXE_DIR%\smplayer-portable64.exe (
  echo SMPlayer portable EXE not found!
  goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable64.exe (
  echo Warning: SMTube portable EXE not found!
)

ren %SMPLAYER_DIR64% smplayer-portable-%ALL_PKG_VER%-x64
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER%-x64

if not exist %TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER%-x64 (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

::
echo Backing up files...

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe mplayer.exe.bak32
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer64.exe mplayer.exe

::
echo Creating screenshots dir...

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

::
echo Creating smplayer.ini...

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini

::
echo Creating smplayer_orig.ini...

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini

::
echo Creating mplayer config...

echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf

::
echo Copying portable .exe...

copy /y %PORTABLE_EXE_DIR%\smplayer-portable64.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable64.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%ALL_PKG_VER%-x64.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!qxtcore.dll -xr!mencoder.exe -xr!mencoder64.exe  -xr!mplayer64.exe.debug -xr!mencoder64.exe.debug -xr!mplayer.exe.debug -xr!mencoder.exe.debug -xr!codecs -xr!buildinfo -xr!buildinfo64 -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir %SMPLAYER_PORTABLE_DIR%\screenshots
del %SMPLAYER_PORTABLE_DIR%\smplayer.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer.exe
del %SMPLAYER_PORTABLE_DIR%\smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe mplayer64.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe.bak32 mplayer.exe
del %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR% smplayer-build64

goto end

:portablesfx
if exist %PORTABLE_EXE_DIR%\smplayer-portable.exe (
  if exist %TOP_LEVEL_DIR%\smplayer-build (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smportable.nsi
  )
)

if exist %PORTABLE_EXE_DIR%\smplayer-portable64.exe (
  if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smportable.nsi
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

:testsymboliclink

set SymbolicTestDir1=SymbolicTestDir1-%RANDOM%
set SymbolicTestDir2=SymbolicTestDir2-%RANDOM%
mkdir "%TEMP%\%SymbolicTestDir1%" >NUL 2>&1
mklink /D "%TEMP%\%SymbolicTestDir2%" "%TEMP%\%SymbolicTestDir1%" >NUL 2>&1 || (
  echo Unable to create symbolic links. Run the script elevated ^(Right-click -^> Run as administrator^), enable SeCreateSymbolicLinkPrivilege on your account in the Local Security Policy Editor, or support may be missing from your OS.
  goto end
)

if not exist %MPV_DIR% (
  echo MPV directory not found!
  goto end
)

if "%USER_CHOICE%" == "31"  goto nsispkgmpv
if "%USER_CHOICE%" == "32"  goto nsispkgmpv64
if "%USER_CHOICE%" == "33"  goto portablempv
if "%USER_CHOICE%" == "34"  goto portablempv64
goto end

:nsispkgmpv
echo --- SMPlayer NSIS MPV Package [32-bit] ---
echo.

ren %SMPLAYER_DIR%\mplayer mplayer.bak
mklink /D %SMPLAYER_DIR%\mplayer %MPV_DIR%

%MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smplayer.nsi

rmdir %SMPLAYER_DIR%\mplayer
ren %SMPLAYER_DIR%\mplayer.bak mplayer

if not "%USER_CHOICE%" == "40" goto end

:nsispkgmpv64
echo --- SMPlayer NSIS MPV Package [64-bit] ---
echo.

ren %SMPLAYER_DIR64%\mplayer mplayer.bak
mklink /D %SMPLAYER_DIR64%\mplayer %MPV_DIR%

%MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi

rmdir %SMPLAYER_DIR64%\mplayer
ren %SMPLAYER_DIR64%\mplayer.bak mplayer

goto end

:portablempv
echo --- SMPlayer Portable Package [32-bit] ---
echo.

:: Check for portable exes
if not exist %PORTABLE_EXE_DIR%\smplayer-portable.exe (
  echo SMPlayer portable EXE not found!
  goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable.exe (
  echo Warning: SMTube portable EXE not found!
)

ren %SMPLAYER_DIR% smplayer-mpv-portable-%ALL_PKG_VER%
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-mpv-portable-%ALL_PKG_VER%

if not exist %TOP_LEVEL_DIR%\smplayer-mpv-portable-%ALL_PKG_VER% (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

::
echo Backing up files...

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak
ren %SMPLAYER_PORTABLE_DIR%\mplayer mplayer.bak

::
echo Creating screenshots dir...

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

::
echo Copying portable .exe...

copy /y %PORTABLE_EXE_DIR%\smplayer-portable.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

::
echo Creating symbolic link to MPV...
mklink /D %SMPLAYER_PORTABLE_DIR%\mplayer %MPV_DIR%

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-mpv-portable-%ALL_PKG_VER%.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!open-fonts -xr!docs -xr!imageformats -xr!shortcuts -xr!Finding_subtitles.txt -xr!Not_so_obvious_things.txt -xr!Watching_TV.txt -xr!sample.avi -xr!Notes_about_mpv.txt -xr!dvdmenus.txt -xr!mpv64.exe -xr!mpv64.com -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
rem DO NOT use 'rmdir /q /s' to delete directory symbolic links
rmdir %SMPLAYER_PORTABLE_DIR%\mplayer
rmdir %SMPLAYER_PORTABLE_DIR%\screenshots
del %SMPLAYER_PORTABLE_DIR%\smplayer.exe
del %SMPLAYER_PORTABLE_DIR%\smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer.bak mplayer
ren %SMPLAYER_PORTABLE_DIR% smplayer-build

if not "%USER_CHOICE%" == "41"  goto end

:portablempv64
echo --- SMPlayer Portable Package [64-bit] ---
echo.

:: Check for portable exes
if not exist %PORTABLE_EXE_DIR%\smplayer-portable64.exe (
  echo SMPlayer portable EXE not found!
  goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable64.exe (
  echo Warning: SMTube portable EXE not found!
)

for %%F in (%MPV_DIR%\mpv64.exe %MPV_DIR%\mpv64.com) do if not exist %%F (
  echo 64-bit MPV executables not found!
  goto end
)

ren %SMPLAYER_DIR64% smplayer-mpv-portable-%ALL_PKG_VER%-x64
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-mpv-portable-%ALL_PKG_VER%-x64

if not exist %TOP_LEVEL_DIR%\smplayer-mpv-portable-%ALL_PKG_VER%-x64 (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
  goto end
)

::
echo Backing up files...

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak
ren %SMPLAYER_PORTABLE_DIR%\mplayer mplayer.bak

::
echo Creating screenshots dir...

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

::
echo Copying portable .exe...

copy /y %PORTABLE_EXE_DIR%\smplayer-portable64.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable64.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

::
echo Creating symbolic link to MPV...
mklink /D %SMPLAYER_PORTABLE_DIR%\mplayer %MPV_DIR%
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.exe mpv.exe.bak32
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.com mpv.com.bak32
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv64.exe mpv.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv64.com mpv.com

::
echo Finalizing package...
7za a -t7z %OUTPUT_DIR%\smplayer-mpv-portable-%ALL_PKG_VER%-x64.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!open-fonts -xr!docs -xr!imageformats -xr!shortcuts -xr!Finding_subtitles.txt -xr!Not_so_obvious_things.txt -xr!Watching_TV.txt -xr!sample.avi -xr!Notes_about_mpv.txt -xr!dvdmenus.txt -mx9 >nul

echo.
echo Restoring source folder(s) back to its original state...
echo.
del %SMPLAYER_PORTABLE_DIR%\smplayer.exe
del %SMPLAYER_PORTABLE_DIR%\smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.exe mpv64.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.com mpv64.com
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.exe.bak32 mpv.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mpv.com.bak32 mpv.com
rem DO NOT use 'rmdir /q /s' to delete directory symbolic links
rmdir %SMPLAYER_PORTABLE_DIR%\mplayer
rmdir %SMPLAYER_PORTABLE_DIR%\screenshots
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer.bak mplayer
ren %SMPLAYER_PORTABLE_DIR% smplayer-build64

goto end

:end

rmdir "%TEMP%\%SymbolicTestDir1%" 2>NUL
rmdir "%TEMP%\%SymbolicTestDir2%" 2>NUL

pause

:superend
