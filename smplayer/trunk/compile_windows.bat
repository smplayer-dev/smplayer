cd getrev
qmake
mingw32-make 
release\getrev.exe > ..\src\svn_revision.h

cd ..

cd src
lrelease smplayer.pro
qmake
mingw32-make
