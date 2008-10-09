cd getrev
qmake
mingw32-make 

cd ..

getrev\release\getrev.exe > src\svn_revision.h

cd src
lrelease smplayer.pro
qmake DEFINES+=USE_QXT
mingw32-make
