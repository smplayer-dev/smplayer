rem cd zlib
rem mingw32-make -fwin32\makefile.gcc clean
rem cd ..

cd src
mingw32-make distclean

cd ..
del src\translations\smplayer_*.qm
del src\object_script.smplayer.Release
del src\object_script.smplayer.Debug
del src\svn_revision.h
del webserver\simple_web_server.exe

rd src\release
rd src\debug
