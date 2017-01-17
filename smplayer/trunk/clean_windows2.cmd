set olddir=%CD%

cd dxlist
mingw32-make distclean

cd ..\zlib
mingw32-make -fwin32\makefile.gcc clean

cd ..\src
mingw32-make distclean

cd ..
del setup\scripts\win32inst_vars.cmd
del src\translations\smplayer_*.qm
del src\object_script.smplayer.Release
del src\object_script.smplayer.Debug
del src\svn_revision.h
del webserver\simple_web_server.exe
rd dxlist\release
rd dxlist\debug
rd src\release
rd src\debug
