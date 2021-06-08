#! /bin/sh
./get_svn_revision.sh
cd webserver && make
cd ../src
qmake DEFINES+="NO_DEBUG_ON_CONSOLE USE_SHM USE_GL_WINDOW"
make -j4
lrelease smplayer.pro
mkdir smplayer.app/Contents/MacOS/translations
cp translations/*.qm smplayer.app/Contents/MacOS/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
cd ..
mkdir package
mv src/smplayer.app package/SMPlayer.app
macdeployqt package/SMPlayer.app
