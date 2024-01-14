#! /bin/sh
./get_svn_revision.sh

echo Compiling web server
cd webserver && make
cd ..

SM_DEFINES="USE_SHM USE_GL_WINDOW NO_PLAYPAUSE_WORKAROUND"

if [ -n "$NO_MPLAYER" ]; then
  SM_DEFINES="${SM_DEFINES} NO_MPLAYER"
fi

if [ -n "$USE_SMTUBE_LIB" ]; then
  SM_DEFINES="${SM_DEFINES} USE_SMTUBE_LIB"

  echo Compiling smtube
  ln -sf ../smtube/src smtube
  cd smtube
  lrelease smtube.pro
  qmake DEFINES+="SMTUBE_LIB"
  make -j$(nproc)
  cd ..
fi

echo Compiling smplayer
cd src
lrelease smplayer.pro
qmake DEFINES+="$SM_DEFINES"
make -j$(nproc)
cd ..

echo Installing
mkdir -p appdir/usr/bin/
cp src/smplayer appdir/usr/bin/
cp webserver/simple_web_server appdir/usr/bin/
mkdir -p appdir/usr/bin/translations
cp src/translations/*.qm appdir/usr/bin/translations/
mkdir -p appdir/usr/share/applications
cp smplayer.desktop appdir/usr/share/applications/

install -d appdir/usr/share/icons/hicolor/scalable/apps/
install -m 644 icons/smplayer.svg appdir/usr/share/icons/hicolor/scalable/apps/smplayer.svg

if [ -n "$USE_SMTUBE_LIB" ]; then
  cp smtube/translations/*.qm appdir/usr/bin/translations/
fi
