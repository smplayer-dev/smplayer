#! /bin/sh
./get_svn_revision.sh
echo "24.5.0.`cat svn_revision`" > version
#echo "24.5.0" > version
