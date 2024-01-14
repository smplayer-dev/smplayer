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
qmake DEFINES+="SMTUBE_LIB"
make -j$(sysctl -n hw.logicalcpu)
cd ../src

echo Compiling smplayer
lrelease smplayer.pro
qmake DEFINES+="USE_SMTUBE_LIB NO_PLAYPAUSE_WORKAROUND"
make -j$(sysctl -n hw.logicalcpu)
cp ../smtube/translations/*.qm smplayer.app/Contents/Resources/translations/
cp ../webserver/simple_web_server smplayer.app/Contents/MacOS/
../Mac/create_plist > smplayer.app/Contents/Info.plist
cp ../Mac/smplayer-document.icns smplayer.app/Contents/Resources/
cd ..

echo Creating package
mkdir -p package
rm -r package/SMPlayer.app
mv src/smplayer.app package/SMPlayer.app
mkdir -p package/SMPlayer.app/Contents/translations
cp `qmake -query QT_INSTALL_TRANSLATIONS`/qt_*.qm package/SMPlayer.app/Contents/translations/
cp `qmake -query QT_INSTALL_TRANSLATIONS`/qtbase_*.qm package/SMPlayer.app/Contents/translations/
rm package/SMPlayer.app/Contents/translations/qt_help_*
macdeployqt package/SMPlayer.app
