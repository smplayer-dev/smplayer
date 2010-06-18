@echo off
echo This batch file can help you to create a package for SMPlayer Portable
echo Just change the variables at the beginning
echo.
echo Note: It will temporarily rename the smplayer-build directory.
echo Be sure to have a compiled portable smplayer.exe, renamed as
echo `smplayer-portable.exe` in the same directory as this script.
echo.
echo 7zip command-line (http://7zip.org) is required by this script.
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" exit

set SMPLAYER_DIR=smplayer-build

ren %SMPLAYER_DIR% smplayer-portable-%SMPLAYER_VER%
set SMPLAYER_DIR=smplayer-portable-%SMPLAYER_VER%

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

copy /y .\smplayer-portable.exe %SMPLAYER_DIR%\smplayer.exe

echo.
echo ######  Creating portable package  #######
echo.
7za a -t7z smplayer-portable-%SMPLAYER_VER%.7z %SMPLAYER_DIR% -xr!*.bak -xr!qxtcore.dll -mx7

echo.
echo ######   Cleanup and restoration   #######
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
