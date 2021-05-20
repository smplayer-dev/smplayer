#! /bin/sh
./get_svn_revision.sh
cd webserver && make
cd ../src
qmake DEFINES+=NO_DEBUG_ON_CONSOLE && make -j4
lrelease smplayer.pro
mkdir smplayer.app/Contents/MacOS/translations
cp translations/*.qm smplayer.app/Contents/MacOS/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
mv smplayer.app SMPlayer.app
macdeployqt SMPlayer.app -dmg
