#! /bin/sh
 
ln -s debian-rvm debian
./get_svn_revision.sh
dch -v 0.5.59-`cat svn_revision.txt` "New version"
dpkg-buildpackage -rfakeroot
