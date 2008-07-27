#!/bin/sh

# This file is taken from the MPlayer sources, and adapted a little bit.
# It gets the SVN revision number and then saves it in two files:
# src/svn_revision.h and svn_revision

test "$1" && extra="-$1"

svn_revision=`LC_ALL=C svn info 2> /dev/null | grep Revision | cut -d' ' -f2`
test $svn_revision || svn_revision=`cd "$1" && grep revision .svn/entries 2>/dev/null | cut -d '"' -f2`
test $svn_revision || svn_revision=`cd "$1" && sed -n -e '/^dir$/{n;p;q}' .svn/entries 2>/dev/null`
test $svn_revision || svn_revision=0UNKNOWN

NEW_REVISION="#define SVN_REVISION \"SVN-r${svn_revision}${extra}\""
OLD_REVISION=`cat src/svn_revision.h 2> /dev/null`

# Update version.h only on revision changes to avoid spurious rebuilds
if test "$NEW_REVISION" != "$OLD_REVISION"; then
    echo "$NEW_REVISION" > src/svn_revision.h
	echo "SVN-r${svn_revision}${extra}" > svn_revision
fi
