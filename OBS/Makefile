# This Makefile creates some files necessary to build
# deb packages at build.opensuse.org

PACKAGE = smplayer
VERSION = 21.1.0
#DEPENDS = debhelper (>= 5), zlib1g-dev, qtbase5-dev, qt5-qmake, qtscript5-dev, qttools5-dev-tools, qtbase5-private-dev, qt5-default | nano
DEPENDS = debhelper (>= 5), zlib1g-dev, libqt4-dev

DSC_FILE = $(PACKAGE).dsc
DEBIAN_FILE = debian.tar.gz

all: $(DEBIAN_FILE) $(DSC_FILE)

$(DEBIAN_FILE): Makefile
	svn export ../debian-orig debian
	mv debian/changelog-orig debian/changelog
	tar -cvzf $(DEBIAN_FILE) debian/

$(DSC_FILE): Makefile
	echo "Format: 1.0" > $(DSC_FILE)
	echo "Source: $(PACKAGE)" >> $(DSC_FILE)
	echo "Binary: $(PACKAGE)" >> $(DSC_FILE)
	echo "Architecture: any" >> $(DSC_FILE)
	echo "Version: $(VERSION)" >> $(DSC_FILE)
	echo "Maintainer: Ricardo Villalba <ricardo@smplayer.info>" >> $(DSC_FILE)
	echo "Standards-Version: 3.9.7" >> $(DSC_FILE)
	echo "Build-Depends: $(DEPENDS)" >> $(DSC_FILE)
	echo "DEBTRANSFORM-TAR: $(PACKAGE)-$(VERSION).tar.bz2" >> $(DSC_FILE)
	echo "DEBTRANSFORM-FILES-TAR: $(DEBIAN_FILE)" >> $(DSC_FILE)
	echo "DEBTRANSFORM-RELEASE: 1" >> $(DSC_FILE)

clean:
	- rm -r debian/*
	- rmdir debian
	- rm $(DEBIAN_FILE) $(DSC_FILE)
