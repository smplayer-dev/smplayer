@echo off
echo This batch file can help you to create a packages for SMPlayer and MPlayer.
echo.
echo Note: It will temporarily rename the smplayer-build or mplayer directory.
echo Be sure to have a compiled portable smplayer.exe, renamed as
echo `smplayer-portable.exe` in the same directory as this script or an
echo alternate location you specify in this script.
echo.
echo Configure your build environment at the beginning of this script.
echo.
echo 7zip command-line (http://7zip.org) is required by this script.
echo.
echo * Release Packages
echo.
echo 1 - Portable SMPlayer Package
echo 2 - SMPlayer Package w/o MPlayer
echo 3 - MPlayer Package
echo.
echo * Update Packages
echo.
echo 4 - SMPlayer SVN Update Package
echo 5 - Qt DLL Package

echo.

set TOP_LEVEL_DIR=..

set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-build
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer
set OUTPUT_DIR=%TOP_LEVEL_DIR%\output
set PORTABLE_EXE_DIR=%TOP_LEVEL_DIR%\portable
set QT_DIR=E:\Buildsys\Qt\%QTVER%

:reask
set /P USER_CHOICE="Choose an action: "
echo.

if "%USER_CHOICE%" == "1"     goto portable
if "%USER_CHOICE%" == "2"     goto nomplayer
if "%USER_CHOICE%" == "3"     goto mplayer
if "%USER_CHOICE%" == "4"     goto svn_updpkg
if "%USER_CHOICE%" == "5"     goto qtdlls
if "%USER_CHOICE%" == ""      goto end

:portable
echo --- Creating SMPlayer Portable Package ---
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" exit
echo.

ren %SMPLAYER_DIR% smplayer-portable-%SMPLAYER_VER%
set SMPLAYER_DIR=%TOP_LEVEL_DIR%\smplayer-portable-%SMPLAYER_VER%

echo.
echo ######      Backing up files       #######
echo.

ren %SMPLAYER_DIR%\smplayer.exe smplayer.bak
ren %SMPLAYER_DIR%\mplayer\mplayer\config config.bak

echo.
echo ######   Create screenshots dir.   #######
echo.

mkdir %SMPLAYER_DIR%\screenshots

echo.
echo ######     Create smplayer.ini     #######
echo.

echo [%%General]>> %SMPLAYER_DIR%\smplayer.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_DIR%\smplayer.ini
echo.>> %SMPLAYER_DIR%\smplayer.ini
echo [advanced]>> %SMPLAYER_DIR%\smplayer.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_DIR%\smplayer.ini

echo.
echo ######  Create smplayer_orig.ini   #######
echo.

echo [%%General]>> %SMPLAYER_DIR%\smplayer_orig.ini
echo screenshot_directory=.\\screenshots>> %SMPLAYER_DIR%\smplayer_orig.ini
echo.>> %SMPLAYER_DIR%\smplayer_orig.ini
echo [advanced]>> %SMPLAYER_DIR%\smplayer_orig.ini
echo mplayer_additional_options=-nofontconfig>> %SMPLAYER_DIR%\smplayer_orig.ini

echo.
echo ######    Create mplayer config    #######
echo.

echo ## MPlayer Windows configuration>> %SMPLAYER_DIR%\mplayer\mplayer\config
echo.>> %SMPLAYER_DIR%\mplayer\mplayer\config
echo subfont=c:\windows\fonts\arial.ttf>> %SMPLAYER_DIR%\mplayer\mplayer\config
echo ^<cachedir^>../fontconfig^</cachedir^>> %SMPLAYER_DIR%\mplayer\fonts\local.conf

echo.
echo ######    Copying portable .exe    #######
echo.

copy /y %PORTABLE_EXE_DIR%\smplayer-portable.exe %SMPLAYER_DIR%\smplayer.exe

echo.
echo ######  Creating portable package  #######
echo.
7za a -t7z %OUTPUT_DIR%\smplayer-portable-%SMPLAYER_VER%.7z %SMPLAYER_DIR% -xr!*.bak -xr!qxtcore.dll -mx9

echo.
echo Restoring source folder(s) back to its original state...
echo.
rmdir %SMPLAYER_DIR%\screenshots
del %SMPLAYER_DIR%\smplayer.ini
del %SMPLAYER_DIR%\smplayer_orig.ini
del %SMPLAYER_DIR%\smplayer.exe
del %SMPLAYER_DIR%\mplayer\mplayer\config
del %SMPLAYER_DIR%\mplayer\fonts\local.conf
ren %SMPLAYER_DIR%\smplayer.bak smplayer.exe
ren %SMPLAYER_DIR%\mplayer\mplayer\config.bak config
ren %SMPLAYER_DIR% smplayer-build

goto end

:nomplayer
echo --- Creating SMPlayer w/o MPlayer Package ---
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" exit
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

7za a -t7z %OUTPUT_DIR%\mplayer-svn-%MP_REV%.7z %MPLAYER_DIR% -mx9

ren %MPLAYER_DIR% mplayer
set MPLAYER_DIR=%TOP_LEVEL_DIR%\mplayer

echo.
echo Restoring source folder(s) back to its original state...

goto end

:svn_updpkg
echo.
echo ---  Creating SVN Update Package  ---
echo.

set /P SMPLAYER_SVN="SMPlayer SVN Revision: "
if "%SMPLAYER_SVN%"=="" exit

7za a -t7z %OUTPUT_DIR%\smplayer_update_svn_r%SMPLAYER_SVN%.7z %SMPLAYER_DIR%\smplayer.exe %SMPLAYER_DIR%\translations -mx9

goto end

:qtdlls

set /P QTVER="Qt Version: "
if "%QTVER%"=="" exit

echo.
echo ---  Creating Qt DLL Package  ---
echo.

mkdir imageformats
copy %QT_DIR%\%QTVER%\plugins\imageformats\qjpeg4.dll imageformats
7za a -t7z %OUTPUT_DIR%\qt_%QTVER%_dlls.7z %QT_DIR%\%QTVER%\bin\libgcc_s_dw2-1.dll %QT_DIR%\%QTVER%\bin\mingwm10.dll %QT_DIR%\%QTVER%\bin\QtCore4.dll %QT_DIR%\%QTVER%\bin\QtGui4.dll %QT_DIR%\%QTVER%\bin\QtNetwork4.dll %QT_DIR%\%QTVER%\bin\QtXml4.dll imageformats -mx9
rd /s /q imageformats

goto end

:end

pause
