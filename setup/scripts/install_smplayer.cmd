@echo off
echo This batch file can help you to create a directory with all required files
echo Just change the variables at the beginning
echo.
echo Warning: it will only work with sources from the SVN and the command svn has to be in the path
echo.

set /P QTVER="Qt Version (Default: 4.5.1): "
if "%QTVER%"=="" set QTVER=4.5.1

set OUTPUT_DIR=smplayer-build

set SMPLAYER_DIR=svn\smplayer
set SMPLAYER_THEMES_DIR=svn\smplayer-themes
set MINGW_DIR=C:\MinGW
set MPLAYER_DIR=mplayer
set QT_DIR=C:\Qt\%QTVER%
set QXT_DIR=C:\development\libqxt

echo.
echo ######      SMPlayer, QT libs      #######
echo.

mkdir %OUTPUT_DIR%
copy %SMPLAYER_DIR%\src\release\smplayer.exe %OUTPUT_DIR%
copy %SMPLAYER_DIR%\dxlist\release\dxlist.exe %OUTPUT_DIR%
copy %SMPLAYER_DIR%\*.txt %OUTPUT_DIR%
copy %QT_DIR%\bin\QtCore4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtGui4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtNetwork4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtXml4.dll %OUTPUT_DIR%
copy %MINGW_DIR%\bin\mingwm10.dll %OUTPUT_DIR%
if %QTVER% geq 4.6.0 (
copy %MINGW_DIR%\bin\libgcc_s_dw2-1.dll %OUTPUT_DIR%
)

mkdir %OUTPUT_DIR%\imageformats
copy %QT_DIR%\plugins\imageformats\qjpeg4.dll %OUTPUT_DIR%\imageformats\

echo.
echo ######           Libqxt            #######
echo.
copy %QXT_DIR%\lib\QxtCore.dll %OUTPUT_DIR%

echo.
echo ######        Translations         #######
echo.

mkdir %OUTPUT_DIR%\translations
copy %SMPLAYER_DIR%\src\translations\*.qm %OUTPUT_DIR%\translations

echo.
echo ######       Qt Translations       #######
echo.
copy %QT_DIR%\translations\*.qm %OUTPUT_DIR%\translations

echo.
echo ######         Shortcuts           #######
echo.
mkdir %OUTPUT_DIR%\shortcuts
copy %SMPLAYER_DIR%\src\shortcuts\*.keys %OUTPUT_DIR%\shortcuts

echo.
echo ######        Documentation        #######
echo.
svn export --force %SMPLAYER_DIR%\docs %OUTPUT_DIR%\docs

echo.
echo ######         Icon Themes         #######
echo.
svn export --force %SMPLAYER_THEMES_DIR%\themes %OUTPUT_DIR%\themes

echo.
echo ######           MPlayer           #######
echo.
xcopy %MPLAYER_DIR% %OUTPUT_DIR%\mplayer\ /E

echo.
