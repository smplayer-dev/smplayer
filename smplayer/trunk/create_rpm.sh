#!/bin/sh
#svn up
./get_svn_revision.sh

SVN_REVISION=`cat svn_revision.txt | sed -e 's/SVN-/svn_/g'`
SMPVERSION=`cat src/version.cpp | grep "#define VERSION " | sed -e 's/#define VERSION "//g' -e 's/ /_/g' -e 's/"$//g'`

svn export . /tmp/smplayer-${SMPVERSION}_${SVN_REVISION}
CURDIR=`pwd`
cd /tmp
tar cvjf smplayer-${SMPVERSION}_${SVN_REVISION}.tar.bz2 smplayer-${SMPVERSION}_${SVN_REVISION}/
rm -r /tmp/smplayer-${SMPVERSION}_${SVN_REVISION}
cat ${CURDIR}/smplayer.spec | sed -e 's/%define version [a-zA-Z0-9\.]*$/%define version '${SMPVERSION}'_'${SVN_REVISION}'/' > /tmp/smplayer.spec
PCKGDIR=/usr/src/packages/
if [ -e /etc/fedora-release ]; then
    PCKGDIR=/usr/src/redhat/
fi
if [ -e /etc/mandrake-release ]; then
    PCKGDIR=/usr/src/rpm/
fi
cp /tmp/smplayer-${SMPVERSION}_${SVN_REVISION}.tar.bz2 ${PCKGDIR}SOURCES/
rpmbuild -bb --clean --rmsource smplayer.spec
