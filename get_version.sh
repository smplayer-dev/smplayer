#! /bin/sh
./get_svn_revision.sh
echo "21.10.0.`cat svn_revision`" > version
#echo "21.10.0" > version
