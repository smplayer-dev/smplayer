#! /bin/sh
./get_svn_revision.sh

echo Compiling web server
cd webserver && make
cd ..

echo Compiling create_plist
cd Mac
qmake && make
cd ../src

echo Compiling smplayer
qmake DEFINES+="NO_DEBUG_ON_CONSOLE USE_SHM USE_GL_WINDOW"
make -j4
lrelease smplayer.pro
mkdir smplayer.app/Contents/MacOS/translations
cp translations/*.qm smplayer.app/Contents/MacOS/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
../Mac/create_plist > smplayer.app/Contents/Info.plist
cd ..

echo Creating package
mkdir package
rm -r package/SMPlayer.app
mv src/smplayer.app package/SMPlayer.app
macdeployqt package/SMPlayer.app
