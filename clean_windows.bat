del src\release\smplayer.exe
del src\translations\smplayer_*.qm

cd getrev
mingw32-make clean

cd ..\src
mingw32-make clean

