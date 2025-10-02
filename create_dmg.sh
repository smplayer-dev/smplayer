#! /bin/sh

rm package/*.dmg

./get_version.sh
VERSION=`cat version`

create-dmg \
  --volname "SMPlayer $VERSION" \
  --background "Mac/installer_background.png" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon-size 100 \
  --icon "SMPlayer.app" 128 190 \
  --hide-extension "SMPlayer.app" \
  --app-drop-link 472 185 \
  "package/smplayer-$VERSION.dmg" \
  "package/SMPlayer.app/"
