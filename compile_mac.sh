#! /bin/sh
./get_svn_revision.sh

echo Compiling web server
cd webserver && make
cd ..

echo Compiling create_plist
cd Mac
qmake && make
cd ..

echo Compiling smtube
ln -sf ../smtube/src smtube
cd smtube
lrelease smtube.pro
qmake DEFINES+="NO_DEBUG_ON_CONSOLE SMTUBE_LIB"
make -j4
cd ../src

echo Compiling smplayer
lrelease smplayer.pro
qmake DEFINES+="NO_DEBUG_ON_CONSOLE USE_SMTUBE_LIB"
make -j4
cp ../smtube/translations/*.qm smplayer.app/Contents/Resources/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
../Mac/create_plist > smplayer.app/Contents/Info.plist
cp ../Mac/smplayer-document.icns smplayer.app/Contents/Resources/
cd ..

echo Creating package
mkdir package
rm -r package/SMPlayer.app
mv src/smplayer.app package/SMPlayer.app
macdeployqt package/SMPlayer.app
