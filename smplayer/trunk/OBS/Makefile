# This Makefile creates some files necessary to build
# deb packages at build.opensuse.org

PACKAGE = smplayer
VERSION = 16.4.0
DEPENDS = debhelper (>= 5), libqt4-dev, zlib1g-dev

DSC_FILE = $(PACKAGE).dsc
DEBIAN_FILE = debian.tar.gz

all: $(DEBIAN_FILE) $(DSC_FILE)

$(DEBIAN_FILE): Makefile
	svn export ../debian-rvm debian
	mv debian/changelog-orig debian/changelog
	tar -cvzf $(DEBIAN_FILE) debian/

$(DSC_FILE): Makefile
	echo "Format: 1.0" > $(DSC_FILE)
	echo "Source: $(PACKAGE)" >> $(DSC_FILE)
	echo "Binary: $(PACKAGE)" >> $(DSC_FILE)
	echo "Architecture: any" >> $(DSC_FILE)
	echo "Version: $(VERSION)" >> $(DSC_FILE)
	echo "Maintainer: Ricardo Villalba <rvm@users.sourceforge.net>" >> $(DSC_FILE)
	echo "Standards-Version: 3.7.2" >> $(DSC_FILE)
	echo "Build-Depends: $(DEPENDS)" >> $(DSC_FILE)
	echo "DEBTRANSFORM-TAR: $(PACKAGE)-$(VERSION).tar.bz2" >> $(DSC_FILE)
	echo "DEBTRANSFORM-FILES-TAR: $(DEBIAN_FILE)" >> $(DSC_FILE)
	echo "DEBTRANSFORM-RELEASE: 1" >> $(DSC_FILE)

clean:
	- rm debian/*
	- rmdir debian
	- rm $(DEBIAN_FILE) $(DSC_FILE)
