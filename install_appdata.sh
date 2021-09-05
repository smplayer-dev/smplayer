#! /bin/sh
install -m 644 smplayer.appdata.xml $1

./get_version.sh
sed -e "s/{version}/`cat version`/" -i '' $1
sed -e "s/{date}/`date +%Y-%m-%d`/" -i '' $1
