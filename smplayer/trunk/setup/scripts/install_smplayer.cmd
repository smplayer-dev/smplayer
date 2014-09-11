@echo off
echo This batch file can help you to create a directory with all required files
echo Just change the variables at the beginning
echo.
echo Warning: it will only work with sources from the SVN and the command svn has to be in the path
echo.

set /P QTVER="Qt Version (Default: 4.8.6): "
if "%QTVER%"=="" set QTVER=4.8.6

set OUTPUT_DIR=smplayer-build

set SMPLAYER_DIR=svn\smplayer
set SMTUBE_DIR=svn\smtube
set SMPLAYER_THEMES_DIR=svn\smplayer-themes
set SMPLAYER_SKINS_DIR=svn\smplayer-skins
set MPLAYER_DIR=mplayer
rem set QT_DIR=C:\QtSDK\Desktop\Qt\%QTVER%\mingw
set QT_DIR=C:\Qt\%QTVER%

echo.
echo ######      SMPlayer, QT libs      #######
echo.

mkdir %OUTPUT_DIR%
copy %SMPLAYER_DIR%\src\release\smplayer.exe %OUTPUT_DIR%
copy %SMPLAYER_DIR%\dxlist\release\dxlist.exe %OUTPUT_DIR%
copy %SMPLAYER_DIR%\zlib\zlib1.dll %OUTPUT_DIR%
copy %SMPLAYER_DIR%\setup\sample.avi %OUTPUT_DIR%
copy %SMPLAYER_DIR%\*.txt %OUTPUT_DIR%
copy %QT_DIR%\bin\QtCore4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtGui4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtNetwork4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtXml4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\QtScript4.dll %OUTPUT_DIR%
copy %QT_DIR%\bin\mingwm10.dll %OUTPUT_DIR%
if %QTVER% geq 4.6.0 (
copy %QT_DIR%\bin\libgcc_s_dw2-1.dll %OUTPUT_DIR%
)
copy openssl\*.dll %OUTPUT_DIR%

mkdir %OUTPUT_DIR%\imageformats
copy %QT_DIR%\plugins\imageformats\qjpeg4.dll %OUTPUT_DIR%\imageformats\

echo.
echo ######        Translations         #######
echo.

mkdir %OUTPUT_DIR%\translations
copy %SMPLAYER_DIR%\src\translations\*.qm %OUTPUT_DIR%\translations
copy %SMPLAYER_DIR%\qt-translations\*.qm %OUTPUT_DIR%\translations

echo.
echo ######       Qt Translations       #######
echo.
copy %QT_DIR%\translations\qt_*.qm %OUTPUT_DIR%\translations
del %OUTPUT_DIR%\translations\qt_help_*.qm

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

mkdir %OUTPUT_DIR%\themes\

mkdir %OUTPUT_DIR%\themes\Gartoon
copy %SMPLAYER_THEMES_DIR%\themes\Gartoon\Gartoon.rcc %OUTPUT_DIR%\themes\Gartoon\
copy %SMPLAYER_THEMES_DIR%\themes\Gartoon\README.txt %OUTPUT_DIR%\themes\Gartoon\

mkdir %OUTPUT_DIR%\themes\Gnome
copy %SMPLAYER_THEMES_DIR%\themes\Gnome\Gnome.rcc %OUTPUT_DIR%\themes\Gnome\
copy %SMPLAYER_THEMES_DIR%\themes\Gnome\README.txt %OUTPUT_DIR%\themes\Gnome\

mkdir %OUTPUT_DIR%\themes\Monochrome
copy %SMPLAYER_THEMES_DIR%\themes\Monochrome\Monochrome.rcc %OUTPUT_DIR%\themes\Monochrome\
copy %SMPLAYER_THEMES_DIR%\themes\Monochrome\README.txt %OUTPUT_DIR%\themes\Monochrome\

mkdir %OUTPUT_DIR%\themes\Noia
copy %SMPLAYER_THEMES_DIR%\themes\Noia\Noia.rcc %OUTPUT_DIR%\themes\Noia\
copy %SMPLAYER_THEMES_DIR%\themes\Noia\README.txt %OUTPUT_DIR%\themes\Noia\

mkdir %OUTPUT_DIR%\themes\Numix-remix
copy %SMPLAYER_THEMES_DIR%\themes\Numix-remix\Numix-remix.rcc %OUTPUT_DIR%\themes\Numix-remix\
copy %SMPLAYER_THEMES_DIR%\themes\Numix-remix\README.txt %OUTPUT_DIR%\themes\Numix-remix\

mkdir %OUTPUT_DIR%\themes\Numix-uTouch
copy %SMPLAYER_THEMES_DIR%\themes\Numix-uTouch\Numix-uTouch.rcc %OUTPUT_DIR%\themes\Numix-uTouch\
copy %SMPLAYER_THEMES_DIR%\themes\Numix-uTouch\README.txt %OUTPUT_DIR%\themes\Numix-uTouch\

mkdir %OUTPUT_DIR%\themes\Nuvola
copy %SMPLAYER_THEMES_DIR%\themes\Nuvola\Nuvola.rcc %OUTPUT_DIR%\themes\Nuvola\
copy %SMPLAYER_THEMES_DIR%\themes\Nuvola\README.txt %OUTPUT_DIR%\themes\Nuvola\

mkdir %OUTPUT_DIR%\themes\Oxygen
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen\Oxygen.rcc %OUTPUT_DIR%\themes\Oxygen\
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen\README.txt %OUTPUT_DIR%\themes\Oxygen\

mkdir %OUTPUT_DIR%\themes\Oxygen-Air
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen-Air\Oxygen-Air.rcc %OUTPUT_DIR%\themes\Oxygen-Air\
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen-Air\README.txt %OUTPUT_DIR%\themes\Oxygen-Air\

mkdir %OUTPUT_DIR%\themes\Oxygen-Refit
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen-Refit\Oxygen-Refit.rcc %OUTPUT_DIR%\themes\Oxygen-Refit\
copy %SMPLAYER_THEMES_DIR%\themes\Oxygen-Refit\README.txt %OUTPUT_DIR%\themes\Oxygen-Refit\

mkdir %OUTPUT_DIR%\themes\Silk
copy %SMPLAYER_THEMES_DIR%\themes\Silk\Silk.rcc %OUTPUT_DIR%\themes\Silk\
copy %SMPLAYER_THEMES_DIR%\themes\Silk\README.txt %OUTPUT_DIR%\themes\Silk\

mkdir %OUTPUT_DIR%\themes\Tango
copy %SMPLAYER_THEMES_DIR%\themes\Tango\Tango.rcc %OUTPUT_DIR%\themes\Tango\
copy %SMPLAYER_THEMES_DIR%\themes\Tango\README.txt %OUTPUT_DIR%\themes\Tango\

mkdir %OUTPUT_DIR%\themes\blackPanther-Light
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-Light\blackPanther-Light.rcc %OUTPUT_DIR%\themes\blackPanther-Light\
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-Light\README.txt %OUTPUT_DIR%\themes\blackPanther-Light\

mkdir %OUTPUT_DIR%\themes\blackPanther-Real
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-Real\blackPanther-Real.rcc %OUTPUT_DIR%\themes\blackPanther-Real\
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-Real\README.txt %OUTPUT_DIR%\themes\blackPanther-Real\

mkdir %OUTPUT_DIR%\themes\blackPanther-VistaLike
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-VistaLike\blackPanther-VistaLike.rcc %OUTPUT_DIR%\themes\blackPanther-VistaLike\
copy %SMPLAYER_THEMES_DIR%\themes\blackPanther-VistaLike\README.txt %OUTPUT_DIR%\themes\blackPanther-VistaLike\


echo.
echo ######         Skins Themes         #######
echo.

mkdir %OUTPUT_DIR%\themes\Black
copy %SMPLAYER_SKINS_DIR%\themes\Black\Black.rcc %OUTPUT_DIR%\themes\Black\
copy %SMPLAYER_SKINS_DIR%\themes\Black\main.css %OUTPUT_DIR%\themes\Black\

mkdir %OUTPUT_DIR%\themes\Gonzo
copy %SMPLAYER_SKINS_DIR%\themes\Gonzo\Gonzo.rcc %OUTPUT_DIR%\themes\Gonzo\
copy %SMPLAYER_SKINS_DIR%\themes\Gonzo\main.css %OUTPUT_DIR%\themes\Gonzo\

mkdir %OUTPUT_DIR%\themes\Mac
copy %SMPLAYER_SKINS_DIR%\themes\Mac\Mac.rcc %OUTPUT_DIR%\themes\Mac\
copy %SMPLAYER_SKINS_DIR%\themes\Mac\main.css %OUTPUT_DIR%\themes\Mac\

mkdir %OUTPUT_DIR%\themes\Modern
copy %SMPLAYER_SKINS_DIR%\themes\Modern\Modern.rcc %OUTPUT_DIR%\themes\Modern\
copy %SMPLAYER_SKINS_DIR%\themes\Modern\main.css %OUTPUT_DIR%\themes\Modern\

mkdir %OUTPUT_DIR%\themes\Vista
copy %SMPLAYER_SKINS_DIR%\themes\Vista\Vista.rcc %OUTPUT_DIR%\themes\Vista\
copy %SMPLAYER_SKINS_DIR%\themes\Vista\main.css %OUTPUT_DIR%\themes\Vista\


echo.
echo ######           MPlayer           #######
echo.
xcopy %MPLAYER_DIR% %OUTPUT_DIR%\mplayer\ /E

echo.
echo ######           SMTUBE           #######
echo.
copy %SMTUBE_DIR%\src\release\smtube.exe %OUTPUT_DIR%
copy %SMTUBE_DIR%\src\translations\*.qm %OUTPUT_DIR%\translations
mkdir %OUTPUT_DIR%\docs\smtube
copy %SMTUBE_DIR%\*.txt %OUTPUT_DIR%\docs\smtube

echo.
