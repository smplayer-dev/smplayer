#! /bin/sh
 
ln -s debian-orig debian
cp debian-orig/changelog-orig debian-orig/changelog

./get_svn_revision.sh
dch -v 18.4.0-`cat svn_revision` "New version"

#dpkg-buildpackage -rfakeroot

# This should be faster:
rm build-stamp
rm src/smplayer
fakeroot debian/rules build
fakeroot debian/rules binary

dh_clean
rm debian-orig/changelog
rm debian
