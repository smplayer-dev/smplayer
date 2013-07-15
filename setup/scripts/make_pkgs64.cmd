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
echo 4 - MPlayer Package

echo.

:: Relative directory of all the source files to this script
set TOP_LEVEL_DIR=..

:: Reset in case ran again in same command prompt instance
set SMPLAYER_VER=

:: NSIS path
set NSIS_PATH="C:\Program Files (x86)\NSIS\Unicode"

set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-build64
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer
set OUTPUT_DIR=%TOP_LEVEL_DIR%\output
set PORTABLE_EXE_DIR=%TOP_LEVEL_DIR%\portable

:reask
set /P USER_CHOICE="Choose an action: "
echo.

if "%USER_CHOICE%" == "1" (
  goto nsispkg

) else if "%USER_CHOICE%" == "2" (
  goto portable

) else if "%USER_CHOICE%" == "3" (
  goto nomplayer

) else if "%USER_CHOICE%" == "4" (
  goto mplayer

) else (
  goto reask
)

:nsispkg

echo --- Creating SMPlayer NSIS Packages ---
echo.
echo Format: VER_MAJOR.VER_MINOR.VER_BUILD[.VER_REVISION]
echo VER_REVISION is optional (set to 0 if blank)
echo.


:: Reset in case ran again in same command prompt instance
set NSIS_PKG_VER=
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=
set VER_REV_CMD=

:nsispkgver_again
Set /p NSIS_PKG_VER="Version: "

for /f "tokens=1 delims=." %%j in ("%NSIS_PKG_VER%")  do set VER_MAJOR=%%j
for /f "tokens=2 delims=." %%k in ("%NSIS_PKG_VER%")  do set VER_MINOR=%%k
for /f "tokens=3 delims=." %%l in ("%NSIS_PKG_VER%")  do set VER_BUILD=%%l
for /f "tokens=4 delims=." %%m in ("%NSIS_PKG_VER%")  do set VER_REVISION=%%m

if [%VER_MAJOR%]==[] (
  echo Major Version # must be specified [#.x.x]
  echo.
  goto nsispkgver_again
)

if [%VER_MINOR%]==[] (
  echo Minor Version # must be specified [x.#.x]
  echo.
  goto nsispkgver_again
)

if [%VER_BUILD%]==[] (
  echo Build Version # must be specified [x.x.#]
  echo.
  goto nsispkgver_again
)

if [%VER_REVISION%]==[] (
  set VER_REV_CMD=
) else (
  set VER_REV_CMD= /DVER_REVISION=%VER_REVISION%
)

if exist %TOP_LEVEL_DIR%\smplayer-build (
  %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smplayer.nsi
)

if exist %TOP_LEVEL_DIR%\smplayer-build64 (
  %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
)

goto end

:portable
echo --- Creating SMPlayer Portable Package ---
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" goto end
echo.

:: Check for portable exes
if not exist %PORTABLE_EXE_DIR%\smplayer-portable64.exe (
  echo SMPlayer portable EXE not found!
	goto end
)

if not exist %PORTABLE_EXE_DIR%\smtube-portable64.exe (
  echo SMTube portable EXE not found!
	goto end
)

ren %SMPLAYER_DIR% smplayer-portable-%SMPLAYER_VER%-x64
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%SMPLAYER_VER%-x64

if not exist %TOP_LEVEL_DIR%\smplayer-portable-%SMPLAYER_VER%-x64 (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
	goto end
)

echo.
echo Backing up files...
echo.

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer.exe mplayer.exe.bak32
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer64.exe mplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config config.bak

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

echo ## MPlayer Windows configuration> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo.>> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo subfont=c:\windows\fonts\arial.ttf>> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf

echo.
echo Copying portable .exe...
echo.

copy /y %PORTABLE_EXE_DIR%\smplayer-portable64.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable64.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

echo.
echo Finalizing portable package...
echo.
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%SMPLAYER_VER%-x64.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak* -xr!qxtcore.dll -mx9

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
del %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
del %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config.bak config
ren %SMPLAYER_PORTABLE_DIR% smplayer-build64

goto end

:nomplayer
echo --- Creating SMPlayer w/o MPlayer Package ---
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" goto end
echo.

ren %SMPLAYER_DIR% smplayer-%SMPLAYER_VER%
set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-%SMPLAYER_VER%

7za a -t7z %OUTPUT_DIR%\smplayer-%SMPLAYER_VER%_without_mplayer.7z %SMPLAYER_DIR% -xr!mplayer -mx9

ren %SMPLAYER_DIR% smplayer-build

echo.
echo Restoring source folder(s) back to its original state....

goto end

:mplayer
echo.
echo --- Creating MPlayer Package ---
echo.

set /P MP_REV="MPlayer Revision: "

ren %MPLAYER_DIR% mplayer-svn-%MP_REV%
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer-svn-%MP_REV%

7za a -t7z %OUTPUT_DIR%\mplayer-svn-%MP_REV%.7z %MPLAYER_DIR% -xr!mencoder.exe -mx9

ren %MPLAYER_DIR% mplayer
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer

echo.
echo Restoring source folder(s) back to its original state...

goto end

:end

pause
