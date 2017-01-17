@echo off

:: Reset working dir especially when using 'Run as administrator'
@cd /d "%~dp0"

::                                       ::
::        Command-line Parsing           ::
::                                       ::

set start_dir=%~dp0

set build_themes=yes
set build_skins=yes
set build_pe=
set runinstcmd=yes
set runsvnup=yes

set qmake_defs=
set use_svn_revision=

if defined NUMBER_OF_PROCESSORS (
  set num_jobs=%NUMBER_OF_PROCESSORS%
) else (
  set num_jobs=1
)

set config_file=setup\scripts\win32inst_vars.cmd

:: Default prefix
for /f %%i in ("setup") do set BUILD_PREFIX=%%~fi

:: Default source dirs
set SMPLAYER_SKINS_DIR=..\smplayer-skins
set SMPLAYER_THEMES_DIR=..\smplayer-themes

rem set SMPLAYER_SKINS_DIR=..\..\smplayer-skins\trunk
rem set SMPLAYER_THEMES_DIR=..\..\smplayer-themes\trunk

:cmdline_parsing
if "%1" == ""               goto build_env_info
if "%1" == "-h"             goto usage
if "%1" == "-prefix"        goto prefixTag
if "%1" == "-jobs"          goto cfgJobs
if "%1" == "-nothemes"      goto cfgThemes
if "%1" == "-noskins"       goto cfgSkins
if "%1" == "-noinst"        goto cfgInst
if "%1" == "-noupdate"      goto cfgUpdate
if "%1" == "-portable"      goto cfgPE

echo Unknown option: "%1"
echo.
goto usage

:usage
echo Usage: compile_windows2.cmd [-h] [-prefix (dir)] [-jobs]
echo                             [-portable]
echo                             [-noinst] [-nothemes] [-noskins] [-noupdate]
echo.
echo Configuration:
echo   -h                     display this help and exit
echo.
echo   -prefix (dir)          prefix directory for installation 
echo                          (default prefix: %build_prefix%)
echo.
echo   -jobs                  Specify number of threads (jobs) for mingw32-make (default: %num_jobs%)
echo                          Valid range: 1-32
echo.
echo Optional Features:
echo   -portable              Compile portable executables
echo.
echo Miscellaneous Options:
echo   -noinst                Do not run installation script
echo   -nothemes              Do not compile themes
echo   -noskins               Do not compile skins
echo   -noupdate              Do not update before compiling
echo.
goto end

:prefixTag

shift
set build_prefix=%1
shift

goto cmdline_parsing

:cfgJobs

shift
if %1 geq 1 if %1 leq 32 set num_jobs=%1
shift

goto cmdline_parsing

:cfgPE

set qmake_defs=%qmake_defs% PORTABLE_APP
set build_pe=yes
set build_themes=
set build_skins=
set runinstcmd=
shift

goto cmdline_parsing

:cfgInst

set runinstcmd=
shift

goto cmdline_parsing

:cfgUpdate
set runsvnup=
shift

goto cmdline_parsing

:cfgThemes
set build_themes=
shift

goto cmdline_parsing

:cfgSkins
set build_skins=
shift

goto cmdline_parsing

::                                       ::
::        Build Environment Info         ::
::                                       ::

:build_env_info

:: Command-line Debugging
REM echo build_themes: %build_themes%
REM echo build_skins: %build_skins%
REM echo build_pe: %build_pe%
REM echo runinstcmd: %runinstcmd%
REM echo runsvnup: %runsvnup%
REM echo qmake_defs: %qmake_defs%
REM echo use_svn_revision: %use_svn_revision%
REM goto end

:: GCC Target
for /f "usebackq tokens=2" %%i in (`"gcc -v 2>&1 | find "Target""`) do set gcc_target=%%i
if [%gcc_target%]==[x86_64-w64-mingw32] (
  set X86_64=yes
) else if [%gcc_target%]==[i686-w64-mingw32] (
  set X86_64=no
) else if [%gcc_target%]==[mingw32] (
  set X86_64=no
)

:: MinGW locations from GCC
for /f "usebackq tokens=*" %%d in (`where gcc.exe`) do set MINGW_DIR=%%d
set MINGW_DIR=%MINGW_DIR:~0,-8%

:: Qt locations from QMAKE
for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_PREFIX') do set QT_DIR=%%i
for /f "tokens=*" %%i in ('qmake -query QT_VERSION') do set QT_VER=%%i
set QT_DIR=%QT_DIR:/=\%

set SMPLAYER_DIR=%start_dir%
:: Does string have a trailing slash? if so remove it 
if %SMPLAYER_DIR:~-1%==\ set SMPLAYER_DIR=%SMPLAYER_DIR:~0,-1%

:: Relative paths into full paths
for /f %%i in ("%SMPLAYER_THEMES_DIR%") do set SMPLAYER_THEMES_DIR=%%~fi
for /f %%i in ("%SMPLAYER_SKINS_DIR%") do set SMPLAYER_SKINS_DIR=%%~fi

:: Try to parse if debug or release is uncommented in smplayer.pro
findstr /b /c:"\#CONFIG \+\= release" src\smplayer.pro 2>&1 >nul && (set debug=yes) || (set debug=)
findstr /b /c:"\#CONFIG \+\= debug" src\smplayer.pro 2>&1 >nul && (set debug=) || (set debug=yes)

:: Create var batch file
echo set SMPLAYER_DIR=%SMPLAYER_DIR%>%config_file%
echo set SMPLAYER_THEMES_DIR=%SMPLAYER_THEMES_DIR%>>%config_file%
echo set SMPLAYER_SKINS_DIR=%SMPLAYER_SKINS_DIR%>>%config_file%
echo set QT_DIR=%QT_DIR%>>%config_file%
echo set QT_VER=%QT_VER%>>%config_file%
echo set MINGW_DIR=%MINGW_DIR%>>%config_file%
echo set X86_64=%X86_64%>>%config_file%
echo set BUILD_PREFIX=%BUILD_PREFIX%>>%config_file%
echo set DEBUG=%DEBUG%>>%config_file%

::                                       ::
::          Main Compile Script          ::
::                                       ::

if [%runsvnup%]==[yes] (
  svn up "%SMPLAYER_DIR%" "%SMPLAYER_THEMES_DIR%" "%SMPLAYER_SKINS_DIR%"
  echo.
)

call getrev2.cmd

cd dxlist
if [%build_pe%]==[yes] ( goto skip_dxlist )
for %%F in (directx\d3dtypes.h directx\ddraw.h directx\dsound.h) do if not exist %%F goto skip_dxlist
qmake
mingw32-make

if errorlevel 1 echo Dxlist failed to compile.

:skip_dxlist

cd ..\webserver
if [%build_pe%]==[yes] ( goto skip_webserver )
mingw32-make

if errorlevel 1 echo Webserver failed to compile.

:skip_webserver

cd ..\zlib
mingw32-make -fwin32\makefile.gcc

if errorlevel 1 goto end

cd ..\src
lrelease smplayer.pro
qmake "DEFINES += %qmake_defs%"
mingw32-make -j%num_jobs%

if errorlevel 1 goto end

:: Themes
if [%build_themes%]==[yes] (

  cd "%SMPLAYER_THEMES_DIR%"
  call clean_windows.cmd
  cd themes && mingw32-make

  if errorlevel 1 echo Failed to compile themes.

)

:: Skins
if [%build_skins%]==[yes] (

  cd "%SMPLAYER_SKINS_DIR%"
  call clean_windows.cmd
  cd themes && mingw32-make

  if errorlevel 1 echo Failed to compile skins.

)

:: Installation
if [%runinstcmd%]==[yes] (
  cd "%SMPLAYER_DIR%\setup\scripts"
  call install_smplayer2.cmd
)

if [%build_pe%]==[yes] (
  mkdir "%BUILD_PREFIX%\portable"

  if [%X86_64%]==[yes] (
    copy /y "%SMPLAYER_DIR%\src\release\smplayer.exe" "%BUILD_PREFIX%\portable\smplayer-portable64.exe"
    copy /y "%SMPLAYER_DIR%\src\debug\smplayer.exe" "%BUILD_PREFIX%\portable\smplayer-portable64d.exe"
  ) else ( 
    copy /y "%SMPLAYER_DIR%\src\release\smplayer.exe" "%BUILD_PREFIX%\portable\smplayer-portable.exe"
    copy /y "%SMPLAYER_DIR%\src\debug\smplayer.exe" "%BUILD_PREFIX%\portable\smplayer-portabled.exe"
  )
)

:: Return to starting directory
cd /d "%start_dir%"

:end

:: Reset
set startdir=
set build_themes=
set build_skins=
set build_pe=
set runinstcmd=
set runsvnup=
set qmake_defs=
set use_svn_revision=

if defined NUMBER_OF_PROCESSORS (
  set num_jobs=%NUMBER_OF_PROCESSORS%
) else (
  set num_jobs=1
)
