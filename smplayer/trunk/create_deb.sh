#! /bin/sh
 
ln -s debian-rvm debian
dpkg-buildpackage -rfakeroot
