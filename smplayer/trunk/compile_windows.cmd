call getrev.cmd

cd zlib
mingw32-make -fwin32\makefile.gcc
cd ..

cd src
lrelease smplayer.pro
qmake
mingw32-make
