#! /bin/sh
install -m 644 smplayer.appdata.xml $1

./get_version.sh
DATE_FMT="+%Y-%m-%d"
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(date +%s)}"
DATE=`date -u -d "@$SOURCE_DATE_EPOCH" "$DATE_FMT" 2>/dev/null || date -u -r "$SOURCE_DATE_EPOCH" "$DATE_FMT" 2>/dev/null || date -u "$DATE_FMT"`
sed -i -e "s/{version}/`cat version`/" $1
sed -i -e "s/{date}/$DATE/" $1

