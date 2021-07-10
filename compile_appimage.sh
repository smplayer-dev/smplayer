#! /bin/sh
./get_svn_revision.sh

echo Compiling web server
cd webserver && make
cd ..

echo Compiling smplayer
cd src
lrelease smplayer.pro
qmake DEFINES+="NO_DEBUG_ON_CONSOLE USE_SHM USE_GL_WINDOW NO_MPLAYER"
make -j4
cd ..

echo Installing
mkdir -p appdir/usr/bin/
cp src/smplayer appdir/usr/bin/
cp webserver/simple_web_server appdir/usr/bin/
mkdir -p appdir/usr/bin/translations
cp src/smplayer/translations/*.qm appdir/usr/bin/translations/
mkdir -p appdir/usr/share/applications
cp smplayer.desktop appdir/usr/share/applications/

