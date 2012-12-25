@echo off

set script_name=%0
set build_smtube=true
set smtube_svn_dir=..\..\smtube
set smtube_params=
set qmake_defs=

:arg_loop
if [%1]==[] (

  goto compile

) else if [%1]==[pe] (

  set qmake_defs="DEFINES+=PORTABLE_APP"
  set smtube_params=pe

) else if [%1]==[nosmtube] (

  set build_smtube=false

) else if [%1]==[-h] (

  echo How to use:
  echo.
  echo Add ^`pe^' to compile portable.
  echo Add ^`nosmtube^' to disable compiling smtube.
  echo.
  echo To compile SMPlayer/SMTube non-portable, enter no arguments.
  echo.
  echo ex: %script_name% pe nosmtube
  goto end

) else (

  echo configure: error: unrecognized option: `%1'
  echo Try `%script_name% -h' for more information
  goto end

) 

shift
goto arg_loop

:compile

call getrev.cmd

cd zlib
mingw32-make -fwin32\makefile.gcc

cd ..\src
lrelease smplayer.pro
qmake %qmake_defs%
mingw32-make

if [%errorlevel%]==[0] (
  if [%build_smtube%]==[true] (
    if exist %smtube_svn_dir%\compile_windows.cmd (

      cd %smtube_svn_dir%
      compile_windows.cmd %smtube_params%

    ) else (

      echo SMTube not found in specified directory... skipping
    )
  )
) else (
echo Compilation error, script aborted
:: Resets errorlevel to 0
ver >nul
)

:end