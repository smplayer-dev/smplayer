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

set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-build
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
echo Format: VER_MAJOR.VER_MINOR.VER_BUILD.VER_REVISION
echo Example: 0.8.1.0
echo.
echo Note: VER_REVISION must be defined as '0' if not used
echo.

:: Reset in case ran again in same command prompt instance
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=

set /P VER_MAJOR="VER_MAJOR: "
set /P VER_MINOR="VER_MINOR: "
set /P VER_BUILD="VER_BUILD: "
set /P VER_REVISION="VER_REVISION: "

echo.

if %VER_REVISION% neq 0 (

%NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% /DVER_REVISION=%VER_REVISION% %TOP_LEVEL_DIR%\smplayer.nsi
rem %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% /DVER_REVISION=%VER_REVISION% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
rem %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% /DVER_REVISION=%VER_REVISION% /DWIN64 %TOP_LEVEL_DIR%\smplayer.full.nsi

) else (

%NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% %TOP_LEVEL_DIR%\smplayer.nsi
rem %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.nsi
rem %NSIS_PATH%\makensis.exe /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD% /DWIN64 %TOP_LEVEL_DIR%\smplayer.full.nsi

)

echo.

goto end

:portable
echo --- Creating SMPlayer Portable Package ---
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" goto end
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

ren %SMPLAYER_DIR% smplayer-portable-%SMPLAYER_VER%
set SMPLAYER_PORTABLE_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%SMPLAYER_VER%

if not exist %TOP_LEVEL_DIR%\smplayer-portable-%SMPLAYER_VER% (
  echo Oops! Unable to find renamed directory, make sure no files are opened.
	goto end
)

echo.
echo ######      Backing up files       #######
echo.

ren %SMPLAYER_PORTABLE_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_PORTABLE_DIR%\smtube.exe smtube.bak
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config config.bak

echo.
echo ######   Create screenshots dir.   #######
echo.

mkdir %SMPLAYER_PORTABLE_DIR%\screenshots

echo.
echo ######    Create mplayer config    #######
echo.

echo ## MPlayer Windows configuration>> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo.>> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo subfont=c:\windows\fonts\arial.ttf>> %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf

echo.
echo ######    Copying portable .exe    #######
echo.

copy /y %PORTABLE_EXE_DIR%\smplayer-portable.exe %SMPLAYER_PORTABLE_DIR%\smplayer.exe
copy /y %PORTABLE_EXE_DIR%\smtube-portable.exe %SMPLAYER_PORTABLE_DIR%\smtube.exe

echo.
echo ######  Creating portable package  #######
echo.
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%SMPLAYER_VER%.7z %SMPLAYER_PORTABLE_DIR% -xr!*.bak -xr!qxtcore.dll -mx9

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir %SMPLAYER_PORTABLE_DIR%\screenshots
del %SMPLAYER_PORTABLE_DIR%\smplayer.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer_orig.ini
del %SMPLAYER_PORTABLE_DIR%\smplayer.exe
del %SMPLAYER_PORTABLE_DIR%\smtube.exe
del %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config
del %SMPLAYER_PORTABLE_DIR%\mplayer\fonts\local.conf
ren %SMPLAYER_PORTABLE_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_PORTABLE_DIR%\smtube.bak smtube.exe
ren %SMPLAYER_PORTABLE_DIR%\mplayer\mplayer\config.bak config
ren %SMPLAYER_PORTABLE_DIR% smplayer-build

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
