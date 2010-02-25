@echo off
echo This batch file can help you to create a package for SMPlayer without MPlayer
echo Just change the variables at the beginning
echo.
echo Note: It will temporarily rename the smplayer-build directory.
echo Even if the MPlayer files are present they will be excluded.
echo.
echo 7zip command-line (http://7zip.org) is required by this script.
echo.

set /P SMPLAYER_VER="SMPlayer Version: "
if "%SMPLAYER_VER%"=="" exit

set SMPLAYER_DIR=smplayer-build

ren %SMPLAYER_DIR% smplayer-%SMPLAYER_VER%
set SMPLAYER_DIR=smplayer-%SMPLAYER_VER%

7za a -t7z smplayer-%SMPLAYER_VER%_without_mplayer.7z %SMPLAYER_DIR% -xr!mplayer -mx7

ren %SMPLAYER_DIR% smplayer-build
