#! /bin/sh
./get_svn_revision.sh
echo "21.1.0.`cat svn_revision`" > version
