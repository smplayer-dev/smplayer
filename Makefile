
PREFIX=/usr/local
#PREFIX=/tmp/smplayer

CONF_PREFIX=$(PREFIX)

DATA_PATH=$(PREFIX)/share/smplayer
DOC_PATH=$(PREFIX)/share/doc/packages/smplayer
TRANSLATION_PATH=$(PREFIX)/share/smplayer/translations
THEMES_PATH=$(PREFIX)/share/smplayer/themes
SHORTCUTS_PATH=$(PREFIX)/share/smplayer/shortcuts

ICONS_DIR=$(PREFIX)/share/icons/hicolor/
APPLNK_DIR=$(PREFIX)/share/applications/
METAINFO_DIR=$(PREFIX)/share/metainfo/

QMAKE=qmake
LRELEASE=lrelease

DEFS=DATA_PATH=\\\"$(DATA_PATH)\\\" \
     TRANSLATION_PATH=\\\"$(TRANSLATION_PATH)\\\" \
     DOC_PATH=\\\"$(DOC_PATH)\\\" THEMES_PATH=\\\"$(THEMES_PATH)\\\" \
     SHORTCUTS_PATH=\\\"$(SHORTCUTS_PATH)\\\"

all: src/smplayer webserver/simple_web_server

src/smplayer:
	./get_svn_revision.sh
	+cd src && $(QMAKE) $(QMAKE_OPTS) && $(DEFS) make
	cd src && $(LRELEASE) smplayer.pro

webserver/simple_web_server:
	cd webserver && make

clean:
	if [ -f src/Makefile ]; then cd src && make distclean; fi
	-rm src/translations/smplayer_*.qm
	-rm webserver/simple_web_server

install: all
	-install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 src/smplayer $(DESTDIR)$(PREFIX)/bin/
	-install -d $(DESTDIR)$(DATA_PATH)
	install -m 644 src/input.conf $(DESTDIR)$(DATA_PATH)
	-install -d $(DESTDIR)$(TRANSLATION_PATH)
	install -m 644 src/translations/*.qm $(DESTDIR)$(TRANSLATION_PATH)
	-install -d $(DESTDIR)$(DOC_PATH)
	install -m 644 Release_notes.md *.txt $(DESTDIR)$(DOC_PATH)

	install -m 755 webserver/simple_web_server $(DESTDIR)$(PREFIX)/bin/

	-install -d $(DESTDIR)$(DOC_PATH)
	tar -C docs/ --exclude=.svn -c -f - . | tar -C $(DESTDIR)$(DOC_PATH) -x -f -

	-install -d $(DESTDIR)$(SHORTCUTS_PATH)
	cp src/shortcuts/* $(DESTDIR)$(SHORTCUTS_PATH)

#	-install -d $(DESTDIR)$(THEMES_PATH)
#	-tar -C src/themes/ --exclude=.svn -c -f - . | tar -C $(DESTDIR)$(THEMES_PATH) -x -f -

	-install -d $(DESTDIR)$(ICONS_DIR)/512x512/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/256x256/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/192x192/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/128x128/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/64x64/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/32x32/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/22x22/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/16x16/apps/
	-install -d $(DESTDIR)$(ICONS_DIR)/scalable/apps/
	install -m 644 icons/smplayer_icon512.png $(DESTDIR)$(ICONS_DIR)/512x512/apps/smplayer.png
	install -m 644 icons/smplayer_icon256.png $(DESTDIR)$(ICONS_DIR)/256x256/apps/smplayer.png
	install -m 644 icons/smplayer_icon192.png $(DESTDIR)$(ICONS_DIR)/192x192/apps/smplayer.png
	install -m 644 icons/smplayer_icon128.png $(DESTDIR)$(ICONS_DIR)/128x128/apps/smplayer.png
	install -m 644 icons/smplayer_icon64.png $(DESTDIR)$(ICONS_DIR)/64x64/apps/smplayer.png
	install -m 644 icons/smplayer_icon32.png $(DESTDIR)$(ICONS_DIR)/32x32/apps/smplayer.png
	install -m 644 icons/smplayer_icon22.png $(DESTDIR)$(ICONS_DIR)/22x22/apps/smplayer.png
	install -m 644 icons/smplayer_icon16.png $(DESTDIR)$(ICONS_DIR)/16x16/apps/smplayer.png
	install -m 644 icons/smplayer.svg $(DESTDIR)$(ICONS_DIR)/scalable/apps/smplayer.svg
	-install -d $(DESTDIR)$(APPLNK_DIR)
	install -m 644 smplayer.desktop $(DESTDIR)$(APPLNK_DIR)
	install -m 644 smplayer_enqueue.desktop $(DESTDIR)$(APPLNK_DIR)

	-install -d $(DESTDIR)$(METAINFO_DIR)
	./install_appdata.sh $(DESTDIR)$(METAINFO_DIR)/smplayer.appdata.xml

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
	-rm -f $(ICONS_DIR)/64x64/apps/smplayer.png
	-rm -f $(ICONS_DIR)/32x32/apps/smplayer.png
	-rm -f $(ICONS_DIR)/22x22/apps/smplayer.png
	-rm -f $(ICONS_DIR)/16x16/apps/smplayer.png
	-rm -f $(APPLNK_DIR)/smplayer.desktop
	-rm -f $(METAINFO_DIR)/smplayer.appdata.xml
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

