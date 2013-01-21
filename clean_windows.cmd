set olddir=%CD%

cd zlib
mingw32-make -fwin32\makefile.gcc clean

cd ..\src
mingw32-make distclean

cd ..
del src\translations\smplayer_*.qm
del src\object_script.smplayer.Release
del src\object_script.smplayer.Debug
del src\svn_revision.h
rd src\release
rd src\debug

if exist ..\smtube\clean_windows.cmd (

  cd ..\smtube
  clean_windows.cmd
  :: Return to original directory
  cd %olddir%

) else (

  echo SMTube not found in specified directory... skipping

)
