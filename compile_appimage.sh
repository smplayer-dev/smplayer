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
cp src/translations/*.qm appdir/usr/bin/translations/
mkdir -p appdir/usr/share/applications
cp smplayer.desktop appdir/usr/share/applications/

install -d appdir/usr/share/icons/hicolor/512x512/apps/
install -d appdir/usr/share/icons/hicolor/256x256/apps/
install -d appdir/usr/share/icons/hicolor/192x192/apps/
install -d appdir/usr/share/icons/hicolor/128x128/apps/
install -d appdir/usr/share/icons/hicolor/64x64/apps/
install -d appdir/usr/share/icons/hicolor/32x32/apps/
install -d appdir/usr/share/icons/hicolor/22x22/apps/
install -d appdir/usr/share/icons/hicolor/16x16/apps/
install -d appdir/usr/share/icons/hicolor/scalable/apps/
install -m 644 icons/smplayer_icon512.png appdir/usr/share/icons/hicolor//512x512/apps/smplayer.png
install -m 644 icons/smplayer_icon256.png appdir/usr/share/icons/hicolor/256x256/apps/smplayer.png
install -m 644 icons/smplayer_icon192.png appdir/usr/share/icons/hicolor/192x192/apps/smplayer.png
install -m 644 icons/smplayer_icon128.png appdir/usr/share/icons/hicolor/128x128/apps/smplayer.png
install -m 644 icons/smplayer_icon64.png appdir/usr/share/icons/hicolor/64x64/apps/smplayer.png
install -m 644 icons/smplayer_icon32.png appdir/usr/share/icons/hicolor/32x32/apps/smplayer.png
install -m 644 icons/smplayer_icon22.png appdir/usr/share/icons/hicolor/22x22/apps/smplayer.png
install -m 644 icons/smplayer_icon16.png appdir/usr/share/icons/hicolor/16x16/apps/smplayer.png
install -m 644 icons/smplayer.svg appdir/usr/share/icons/hicolor/scalable/apps/smplayer.svg

