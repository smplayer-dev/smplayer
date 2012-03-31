cd zlib-1.2.6
mingw32-make -fwin32\makefile.gcc clean

cd ..\src
mingw32-make distclean

cd ..
del src\translations\smplayer_*.qm

