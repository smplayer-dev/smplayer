
PREFIX=/usr/local
#PREFIX=/tmp/smplayer

CONF_PREFIX=$(PREFIX)

DATA_PATH=$(PREFIX)/share/smplayer
DOC_PATH=$(PREFIX)/share/doc/packages/smplayer
TRANSLATION_PATH=$(PREFIX)/share/smplayer/translations
THEMES_PATH=$(PREFIX)/share/smplayer/themes
SHORTCUTS_PATH=$(PREFIX)/share/smplayer/shortcuts

#KDE_PREFIX=`kde-config --prefix`
#KDE_PREFIX=/tmp/smplayer/kde/
KDE_PREFIX=$(PREFIX)

KDE_ICONS=$(KDE_PREFIX)/share/icons/hicolor/
KDE_APPLNK=$(KDE_PREFIX)/share/applications/

QMAKE=qmake
LRELEASE=lrelease

DEFS=DATA_PATH=\\\"$(DATA_PATH)\\\" \
     TRANSLATION_PATH=\\\"$(TRANSLATION_PATH)\\\" \
     DOC_PATH=\\\"$(DOC_PATH)\\\" THEMES_PATH=\\\"$(THEMES_PATH)\\\" \
     SHORTCUTS_PATH=\\\"$(SHORTCUTS_PATH)\\\"


ifdef KDE_SUPPORT

# KDE paths, change if necessary

KDE_INCLUDE_PATH=`kde-config --prefix`/include/
KDE_LIB_PATH=`kde-config --prefix`/lib/
KDE_LIBS=-lkio

QMAKE_OPTS=DEFINES+=KDE_SUPPORT INCLUDEPATH+=$(KDE_INCLUDE_PATH) \
           LIBS+="$(KDE_LIBS) -L$(KDE_LIB_PATH)"

endif

src/smplayer:
	./get_svn_revision.sh
	cd src && $(QMAKE) $(QMAKE_OPTS) && $(DEFS) make
	cd src && $(LRELEASE) smplayer.pro

clean:
	cd src && make distclean
	-rm src/translations/smplayer_*.qm

install: src/smplayer
	-install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 src/smplayer $(DESTDIR)$(PREFIX)/bin/
	-install -d $(DESTDIR)$(DATA_PATH)
	install -m 644 src/input.conf $(DESTDIR)$(DATA_PATH)
	-install -d $(DESTDIR)$(TRANSLATION_PATH)
	install -m 644 src/translations/*.qm $(DESTDIR)$(TRANSLATION_PATH)
	-install -d $(DESTDIR)$(DOC_PATH)
	install -m 644 Changelog *.txt $(DESTDIR)$(DOC_PATH)

	-install -d $(DESTDIR)$(DOC_PATH)
	tar -C docs/ --exclude=.svn -c -f - . | tar -C $(DESTDIR)$(DOC_PATH) -x -f -

	-install -d $(DESTDIR)$(SHORTCUTS_PATH)
	cp src/shortcuts/* $(DESTDIR)$(SHORTCUTS_PATH)

	-install -d $(DESTDIR)$(KDE_ICONS)/64x64/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/32x32/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/22x22/apps/
	-install -d $(DESTDIR)$(KDE_ICONS)/16x16/apps/
	install -m 644 icons/smplayer_icon64.png $(DESTDIR)$(KDE_ICONS)/64x64/apps/smplayer.png
	install -m 644 icons/smplayer_icon32.png $(DESTDIR)$(KDE_ICONS)/32x32/apps/smplayer.png
	install -m 644 icons/smplayer_icon22.png $(DESTDIR)$(KDE_ICONS)/22x22/apps/smplayer.png
	install -m 644 icons/smplayer_icon16.png $(DESTDIR)$(KDE_ICONS)/16x16/apps/smplayer.png
	-install -d $(DESTDIR)$(KDE_APPLNK)
	install -m 644 smplayer.desktop $(DESTDIR)$(KDE_APPLNK)
	install -m 644 smplayer_enqueue.desktop $(DESTDIR)$(KDE_APPLNK)
	-install -d $(DESTDIR)$(PREFIX)/share/man/man1/
	install -m 644 man/smplayer.1 $(DESTDIR)$(PREFIX)/share/man/man1/
	gzip -9 -f $(DESTDIR)$(PREFIX)/share/man/man1/smplayer.1

uninstall:
	-rm -f $(PREFIX)/bin/smplayer
	-rm -f $(DATA_PATH)/input.conf
	-rm -f $(TRANSLATION_PATH)/*.qm
	-rm -f $(DOC_PATH)/Changelog
	-rm -f $(DOC_PATH)/*.txt
	-rm -f $(SHORTCUTS_PATH)/*.keys
	-rm -f $(KDE_ICONS)/64x64/apps/smplayer.png
	-rm -f $(KDE_ICONS)/32x32/apps/smplayer.png
	-rm -f $(KDE_ICONS)/22x22/apps/smplayer.png
	-rm -f $(KDE_ICONS)/16x16/apps/smplayer.png
	-rm -f $(KDE_APPLNK)/smplayer.desktop
	-rm -f $(PREFIX)/share/man/man1/smplayer.1.gz
	-rmdir $(SHORTCUTS_PATH)/
	-rmdir $(TRANSLATION_PATH)/
#	-for file in docs/*/*; do \
#	    rm -f $(DOC_PATH)/$${file/docs/}; \
#	done;
#	-for file in docs/*; do \
#	    rmdir $(DOC_PATH)/$${file/docs/}; \
#	done;
	-(cd docs && find -iname '*.html') | (cd $(DESTDIR)$(DOC_PATH) && xargs rm)
	-(cd docs && find -type d -name '??') | (cd $(DESTDIR)$(DOC_PATH) && xargs rmdir)
	-rmdir $(DOC_PATH)/
	-rmdir $(DATA_PATH)/

