cd getrev
qmake
mingw32-make 
cd ..
getrev\release\getrev.exe > src\svn_revision.h

cd zlib-1.2.6
mingw32-make -fwin32\makefile.gcc
cd ..

cd src
lrelease smplayer.pro
qmake DEFINES+=PORTABLE_APP
mingw32-make
