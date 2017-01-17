@echo off

for /f %%d in ('wmic os get localdatetime ^|findstr /b [0-9]') do @set datetime=%%d

REM Read from generated file during compile if exist
REM Relative paths are relative to install_smplayer

if exist win32inst_vars.cmd (

  call win32inst_vars.cmd

  set MPLAYER_DIR=mplayer
  set MPV_DIR=mpv

) else (

  echo This batch file can help you to create a directory with all required files
  echo Just change the variables at the beginning
  echo.
  echo Warning: it will only work with sources from the SVN and the command svn has to be in the path
  echo.

  set /P QT_VER="Qt Version (Default: 5.6.0): "
  if "%QT_VER%"=="" set QT_VER=5.6.0

  set SMPLAYER_DIR=svn\smplayer
  REM set SMTUBE_DIR=svn\smtube
  set SMPLAYER_THEMES_DIR=svn\smplayer-themes
  set SMPLAYER_SKINS_DIR=svn\smplayer-skins
  set MPLAYER_DIR=mplayer
  set MPV_DIR=mpv
  set QT_DIR=C:\Qt\%QT_VER%
  set MINGW_DIR=C:\Qt\%QT_VER%\Tools\mingw492_32
  set X86_64=
  set BUILD_PREFIX=..
  set DEBUG=

)

if [%X86_64%]==[yes] (
  set BASE_BUILD_DIR=smplayer-build64
  set OUTPUT_DIR=%BUILD_PREFIX%\smplayer-build64
  set OPENSSL_DIR=openssl64
) else (
  set BASE_BUILD_DIR=smplayer-build
  set OUTPUT_DIR=%BUILD_PREFIX%\smplayer-build
  set OPENSSL_DIR=openssl
)

if exist "%OUTPUT_DIR%" (
  ren "%OUTPUT_DIR%" "%BASE_BUILD_DIR%.%datetime%"
)

if [%DEBUG%]==[yes] (
  set QT_DEBUG_DLL_SUFFIX=d
  set QT_BUILD_DIR=debug
) else (
  set QT_DEBUG_DLL_SUFFIX=
  set QT_BUILD_DIR=release
)

:begin
echo.
echo --      SMPlayer, QT libs      --
echo.

mkdir %OUTPUT_DIR%
copy "%SMPLAYER_DIR%\src\%QT_BUILD_DIR%\smplayer.exe" "%OUTPUT_DIR%"
copy "%SMPLAYER_DIR%\dxlist\release\dxlist.exe" "%OUTPUT_DIR%"
copy "%SMPLAYER_DIR%\webserver\simple_web_server.exe" "%OUTPUT_DIR%"
copy "%SMPLAYER_DIR%\zlib\zlib1.dll" "%OUTPUT_DIR%"
copy "%SMPLAYER_DIR%\*.txt" "%OUTPUT_DIR%"
REM copy "%SMPLAYER_DIR%\setup\sample.avi" "%OUTPUT_DIR%"

REM Core files
if %QT_VER% lss 5.0.0 (

  copy "%QT_DIR%\bin\QtCore%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\QtDBus%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\QtGui%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\QtNetwork%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\QtXml%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\QtScript%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%"

) else if %QT_VER% geq 5.0.0 (

  REM copy "%QT_DIR%\bin\icudt*.dll" "%OUTPUT_DIR%"
  REM copy "%QT_DIR%\bin\icuin*.dll" "%OUTPUT_DIR%"
  REM copy "%QT_DIR%\bin\icuuc*.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Core%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  REM copy "%QT_DIR%\bin\Qt5DBus%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Gui%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Network%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Widgets%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Xml%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"
  copy "%QT_DIR%\bin\Qt5Script%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%"

)

REM Qt Plugins
mkdir "%OUTPUT_DIR%\imageformats"
if %QT_VER% lss 5.0.0 (

  copy "%QT_DIR%\plugins\imageformats\qjpeg%QT_DEBUG_DLL_SUFFIX%4.dll" "%OUTPUT_DIR%\imageformats\"

) else if %QT_VER% geq 5.0.0 (

  mkdir "%OUTPUT_DIR%\platforms"
  copy "%QT_DIR%\plugins\imageformats\qjpeg%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%\imageformats\"
  copy "%QT_DIR%\plugins\platforms\qwindows%QT_DEBUG_DLL_SUFFIX%.dll" "%OUTPUT_DIR%\platforms\"

)

REM Toolchain specific files
copy "%QT_DIR%\bin\mingwm10.dll" "%OUTPUT_DIR%"
copy "%QT_DIR%\bin\libgcc_s_*.dll" "%OUTPUT_DIR%"
copy "%QT_DIR%\bin\libstdc++-6.dll" "%OUTPUT_DIR%"
copy "%QT_DIR%\bin\libwinpthread-1.dll" "%OUTPUT_DIR%"

REM OpenSSL
copy "%OPENSSL_DIR%\*.dll" "%OUTPUT_DIR%"

if not exist "%QT_DIR%\bin\libeay32.dll" goto skip_toolchain_ssl
if not exist "%QT_DIR%\bin\ssleay32.dll" goto skip_toolchain_ssl
copy /y "%QT_DIR%\bin\libeay32.dll" "%OUTPUT_DIR%"
copy /y "%QT_DIR%\bin\ssleay32.dll" "%OUTPUT_DIR%"

:skip_toolchain_ssl

REM echo.
REM echo --           Fonts             --
REM echo.
REM mkdir "%OUTPUT_DIR%\open-fonts"
REM copy "open-fonts\*.*" "%OUTPUT_DIR%\open-fonts\"

echo.
echo --        Translations         --
echo.
mkdir "%OUTPUT_DIR%\translations"
copy "%SMPLAYER_DIR%\src\translations\*.qm" "%OUTPUT_DIR%\translations"

echo.
echo --       Qt Translations       --
echo.
copy "%QT_DIR%\translations\qt_*.qm" %OUTPUT_DIR%\translations
copy "%QT_DIR%\translations\qtbase*.qm" %OUTPUT_DIR%\translations
copy "%SMPLAYER_DIR%\qt-translations\qt_*.qm" "%OUTPUT_DIR%\translations"
del "%OUTPUT_DIR%\translations\qt_help_*.qm"

echo.
echo --         Shortcuts           --
echo.
mkdir "%OUTPUT_DIR%\shortcuts"
copy "%SMPLAYER_DIR%\src\shortcuts\*.keys" "%OUTPUT_DIR%\shortcuts"

echo.
echo --        Documentation        --
echo.
svn export --force "%SMPLAYER_DIR%\docs" "%OUTPUT_DIR%\docs"

echo.
echo --         Icon Themes         --
echo.
for /f "tokens=*" %%a in ('dir /ad /b ^"%SMPLAYER_THEMES_DIR%\themes^"') do (
xcopy "%SMPLAYER_THEMES_DIR%\themes\%%a\*.rcc" "%OUTPUT_DIR%\themes\%%a\"
xcopy "%SMPLAYER_THEMES_DIR%\themes\%%a\*.qss" "%OUTPUT_DIR%\themes\%%a\"
xcopy "%SMPLAYER_THEMES_DIR%\themes\%%a\README.txt" "%OUTPUT_DIR%\themes\%%a\"
)

echo.
echo --            Skins            --
echo.
for /f "tokens=*" %%b in ('dir /ad /b ^"%SMPLAYER_SKINS_DIR%\themes^"') do (
xcopy "%SMPLAYER_SKINS_DIR%\themes\%%b\*.rcc" "%OUTPUT_DIR%\themes\%%b\"
xcopy "%SMPLAYER_SKINS_DIR%\themes\%%b\main.css" "%OUTPUT_DIR%\themes\%%b\"
)

echo.
echo --           MPlayer           --
echo.
for /f %%i in ("%MPLAYER_DIR%") do set MPLAYER_DIR=%%~fi
mklink /D "%OUTPUT_DIR%\mplayer" "%MPLAYER_DIR%"
if not errorlevel 0 (
  xcopy "%MPLAYER_DIR%" "%OUTPUT_DIR%\mplayer\" /E
)

echo.
echo --           MPV           --
echo.
for /f %%i in ("%MPV_DIR%") do set MPV_DIR=%%~fi
mklink /D "%OUTPUT_DIR%\mpv" "%MPV_DIR%"
if not errorlevel 0 (
  xcopy "%MPV_DIR%" "%OUTPUT_DIR%\mpv\" /E
)

echo.

:end

set SMPLAYER_DIR=
set SMTUBE_DIR=
set SMPLAYER_THEMES_DIR=
set SMPLAYER_SKINS_DIR=
set MPLAYER_DIR=
set MPV_DIR=
set QT_DIR=
set QT_VER=
set MINGW_DIR=
set X86_64=
set BUILD_PREFIX=

set QT_DEBUG_DLL_SUFFIX=
set QT_BUILD_DIR=
