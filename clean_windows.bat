cd getrev
mingw32-make distclean

cd ..\src
mingw32-make distclean

cd ..
del src\translations\smplayer_*.qm

