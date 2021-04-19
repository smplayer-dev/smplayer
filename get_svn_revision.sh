#!/bin/sh

# This file is taken from the MPlayer sources, and adapted a little bit.
# It gets the SVN revision number and then saves it in two files:
# src/svn_revision.h and svn_revision

svn_revision=`LC_ALL=C svn info 2> /dev/null | grep Revision | cut -d' ' -f2`
test $svn_revision || svn_revision=`git rev-list --count HEAD`
test $svn_revision || svn_revision=0UNKNOWN

NEW_REVISION="#define SVN_REVISION \"${svn_revision}\""
OLD_REVISION=`cat src/svn_revision.h 2> /dev/null`

# Update svn_revision.h only on revision changes to avoid spurious rebuilds
if test "$NEW_REVISION" != "$OLD_REVISION"; then
    echo "$NEW_REVISION" > src/svn_revision.h
fi
echo "${svn_revision}" > svn_revision
