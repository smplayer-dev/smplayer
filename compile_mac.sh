#! /bin/sh
./get_svn_revision.sh
cd webserver && make
cd ../src
qmake && make
lrelease smplayer.pro
mkdir smplayer.app/Contents/MacOS/translations
cp translations/*.qm smplayer.app/Contents/MacOS/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
