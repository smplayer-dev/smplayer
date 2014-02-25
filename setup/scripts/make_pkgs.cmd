@echo off
echo This batch file can help you to create a packages for SMPlayer and MPlayer.
echo.
echo Note: It will temporarily rename the smplayer-build or mplayer directory.
echo Be sure to have a compiled portable smplayer.exe, renamed as
echo `smplayer-portable.exe` in the same directory as this script or an
echo alternate location you specify in this script when creating the portable
echo packages.
echo.
echo Configure your build environment at the beginning of this script.
echo.
echo 7zip command-line (http://7zip.org) is required by this script.
echo.
echo 1 - NSIS SMPlayer Packages
echo 2 - Portable SMPlayer Package
echo 3 - SMPlayer Package w/o MPlayer
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
set USER_CHOICE=

:: NSIS path
if exist "%PROGRAMFILES(X86)%\NSIS\Unicode\makensis.exe" (
  set MAKENSIS_EXE_PATH="%PROGRAMFILES(X86)%\NSIS\Unicode\makensis.exe"
) else if exist "%PROGRAMFILES%\NSIS\Unicode\makensis.exe" (
  set MAKENSIS_EXE_PATH="%PROGRAMFILES%\NSIS\Unicode\makensis.exe"
)

set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-build
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer
set OUTPUT_DIR=%TOP_LEVEL_DIR%\output
set PORTABLE_EXE_DIR=%TOP_LEVEL_DIR%\portable

:reask
set /P USER_CHOICE="Choose an action: "
echo.

if "%USER_CHOICE%" == "1"  goto pkgver
if "%USER_CHOICE%" == "2"  goto pkgver
if "%USER_CHOICE%" == "3"  goto pkgver
goto reask

:pkgver

echo Format: VER_MAJOR.VER_MINOR.VER_BUILD[.VER_REVISION]
echo VER_REVISION is optional (set to 0 if blank)
echo.

:pkgver_again
Set /p ALL_PKG_VER="Version: "
echo.

for /f "tokens=1 delims=." %%j in ("%ALL_PKG_VER%")  do set VER_MAJOR=%%j
for /f "tokens=2 delims=." %%k in ("%ALL_PKG_VER%")  do set VER_MINOR=%%k
for /f "tokens=3 delims=." %%l in ("%ALL_PKG_VER%")  do set VER_BUILD=%%l
for /f "tokens=4 delims=." %%m in ("%ALL_PKG_VER%")  do set VER_REVISION=%%m

if [%VER_MAJOR%]==[] (
  echo Major Version # must be specified [#.x.x]
  echo.
  goto pkgver_again
)

if [%VER_MINOR%]==[] (
  echo Minor Version # must be specified [x.#.x]
  echo.
  goto pkgver_again
)

if [%VER_BUILD%]==[] (
  echo Build Version # must be specified [x.x.#]
  echo.
  goto pkgver_again
)

if [%VER_REVISION%]==[] (
  set VER_REV_CMD=
) else (
  set VER_REV_CMD= /DVER_REVISION=%VER_REVISION%
)

if "%USER_CHOICE%" == "1"  goto nsispkg
if "%USER_CHOICE%" == "2"  goto portable
if "%USER_CHOICE%" == "3"  goto nomplayer
:: Should not happen
goto end

:nsispkg

echo --- Creating SMPlayer NSIS Packages ---
echo.

if exist %TOP_LEVEL_DIR%\smplayer-build (
  %MAKENSIS_EXE_PATH% /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smplayer.nsi
)

if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %MAKENSIS_EXE_PATH% /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
)

goto end

:portable
echo --- Creating SMPlayer Portable Package ---
echo.

:: Check for portable exes
if not exist %PORTABLE_EXE_DIR%\smplayer-portable.exe (
  echo SMPlayer portable EXE not found!
	goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable.exe (
  echo SMTube portable EXE not found!
	goto end
)

ren %SMPLAYER_DIR% smplayer-portable-%ALL_PKG_VER%
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER%

if not exist %TOP_LEVEL_DIR%\smplayer-portable-%ALL_PKG_VER% (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
	goto end
)

echo.
echo Backing up files...
echo.

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak

echo.
echo Creating screenshots dir...
echo.

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

echo.
echo Creating smplayer.ini...
echo.

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer.ini

echo.
echo Creating smplayer_orig.ini...
echo.

echo [%%General]> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo.>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo [advanced]>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini

echo.
echo Creating mplayer config...
echo.

echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf

echo.
echo Copying portable .exe...
echo.

copy /y %PORTABLE_EXE_DIR%\smplayer-portable.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

echo.
echo Finalizing portable package...
echo.
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%ALL_PKG_VER%.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!qxtcore.dll -xr!mplayer64.exe -xr!mencoder.exe -xr!mencoder64.exe -mx9

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

goto end

:nomplayer
echo --- Creating SMPlayer w/o MPlayer Package ---
echo.

ren %SMPLAYER_DIR% smplayer-%ALL_PKG_VER%
set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-%ALL_PKG_VER%

7za a -t7z %OUTPUT_DIR%\smplayer-%ALL_PKG_VER%_without_mplayer.7z %SMPLAYER_DIR% -xr!mplayer -mx9

ren %SMPLAYER_DIR% smplayer-build

echo.
echo Restoring source folder(s) back to its original state....

goto end

:end

pause
