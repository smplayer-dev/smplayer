#! /bin/sh
install -m 644 smplayer.appdata.xml $1

./get_version.sh
sed -i -e "s/{version}/`cat version`/" $1
sed -i -e "s/{date}/`date +%Y-%m-%d`/" $1

