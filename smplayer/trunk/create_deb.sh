#! /bin/sh
 
ln -s debian-rvm debian
cp debian-rvm/changelog-orig debian-rvm/changelog

./get_svn_revision.sh
dch -v 0.5.61+`cat svn_revision.txt` "New version"

dpkg-buildpackage -rfakeroot

rm debian-rvm/changelog
rm debian
