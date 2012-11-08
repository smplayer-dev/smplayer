TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on
CONFIG += release
#CONFIG += debug

QT += network xml

RESOURCES = icons.qrc

INCLUDEPATH += mpcgui
DEPENDPATH += mpcgui

#DEFINES += EXPERIMENTAL
DEFINES += SINGLE_INSTANCE
DEFINES += FIND_SUBTITLES
DEFINES += VIDEOPREVIEW
DEFINES += YOUTUBE_SUPPORT
DEFINES += GUI_CHANGE_ON_RUNTIME
DEFINES += LOG_MPLAYER
DEFINES += LOG_SMPLAYER
DEFINES += SKINS

# Disable SINGLE_INSTANCE if Qt < 4.4
contains( DEFINES, SINGLE_INSTANCE ) {
	contains(QT_VERSION, ^4\\.[0-3]\\..*) {
		message("SINGLE_INSTANCE requires Qt > 4.3. Disabled.")
		DEFINES -= SINGLE_INSTANCE
	}
}

# Disable SKINS if Qt < 4.4
contains( DEFINES, SKINS ) {
	contains(QT_VERSION, ^4\\.[0-3]\\..*) {
		message("SKINS requires Qt > 4.3. Disabled.")
		DEFINES -= SKINS
	}
}

HEADERS += guiconfig.h \
	config.h \
	constants.h \
	svn_revision.h \
	version.h \
	chapters.h \
	global.h \
	paths.h \
	helper.h \
	colorutils.h \
	translator.h \
	subtracks.h \
	tracks.h \
	titletracks.h \
	discname.h \
	extensions.h \
	desktopinfo.h \
	myprocess.h \
	mplayerversion.h \
	mplayerprocess.h \
	infoprovider.h \
	mplayerwindow.h \
	mediadata.h \
	audioequalizerlist.h \
	mediasettings.h \
	assstyles.h \
	filters.h \
	preferences.h \
	filesettingsbase.h \
	filesettings.h \
	filesettingshash.h \
	filehash.h \
	tvsettings.h \
	images.h \
	inforeader.h \
	deviceinfo.h \
	recents.h \
	urlhistory.h \
	core.h \
	logwindow.h \
	infofile.h \
	seekwidget.h \
	mytablewidget.h \
	shortcutgetter.h \
	actionseditor.h \
	filechooser.h \
	vdpauproperties.h \
	preferencesdialog.h \
	lineedit_with_icon.h \
	mylineedit.h \
	mycombobox.h \
	tristatecombo.h \
	languages.h \
	selectcolorbutton.h \
	simplehttp.h \
	prefwidget.h \
	prefgeneral.h \
	prefdrives.h \
	prefinterface.h \
	prefperformance.h \
	prefinput.h \
	prefsubtitles.h \
	prefadvanced.h \
	prefplaylist.h \
	preftv.h \
	filepropertiesdialog.h \
	playlist.h \
	playlistdock.h \
	verticaltext.h \
	eqslider.h \
	videoequalizer.h \
	audioequalizer.h \
	myslider.h \
	timeslider.h \
	inputdvddirectory.h \
	inputurl.h \
	myaction.h \
	myactiongroup.h \
	filedialog.h \
	inputmplayerversion.h \
	about.h \
	errordialog.h \
	timedialog.h \
	favorites.h \
	tvlist.h \
	favoriteeditor.h \
	basegui.h \
	baseguiplus.h \
	floatingwidget.h \
	widgetactions.h \
	toolbareditor.h \
	editabletoolbar.h \
	defaultgui.h \
	minigui.h \
	mpcgui/mpcgui.h \
	mpcgui/mpcstyles.h \
	clhelp.h \
	smplayer.h \
	myapplication.h


SOURCES	+= version.cpp \
	global.cpp \
	paths.cpp \
	chapters.cpp \
	helper.cpp \
	colorutils.cpp \
	translator.cpp \
	subtracks.cpp \
	tracks.cpp \
	titletracks.cpp \
	discname.cpp \
	extensions.cpp \
	desktopinfo.cpp \
	myprocess.cpp \
	mplayerversion.cpp \
	mplayerprocess.cpp \
	infoprovider.cpp \
	mplayerwindow.cpp \
	mediadata.cpp \
	mediasettings.cpp \
	assstyles.cpp \
	filters.cpp \
	preferences.cpp \
	filesettingsbase.cpp \
	filesettings.cpp \
	filesettingshash.cpp \
	filehash.cpp \
	tvsettings.cpp \
	images.cpp \
	inforeader.cpp \
	deviceinfo.cpp \
	recents.cpp \
	urlhistory.cpp \
	core.cpp \
	logwindow.cpp \
	infofile.cpp \
	seekwidget.cpp \
	mytablewidget.cpp \
	shortcutgetter.cpp \
	actionseditor.cpp \
	filechooser.cpp \
	vdpauproperties.cpp \
	preferencesdialog.cpp \
	lineedit_with_icon.cpp \
	mylineedit.cpp \
	mycombobox.cpp \
	tristatecombo.cpp \
	languages.cpp \
	selectcolorbutton.cpp \
	simplehttp.cpp \
	prefwidget.cpp \
	prefgeneral.cpp \
	prefdrives.cpp \
	prefinterface.cpp \
	prefperformance.cpp \
	prefinput.cpp \
	prefsubtitles.cpp \
	prefadvanced.cpp \
	prefplaylist.cpp \
	preftv.cpp \
	filepropertiesdialog.cpp \
	playlist.cpp \
	playlistdock.cpp \
	verticaltext.cpp \
	eqslider.cpp \
	videoequalizer.cpp \
	audioequalizer.cpp \
	myslider.cpp \
	timeslider.cpp \
	inputdvddirectory.cpp \
	inputurl.cpp \
	myaction.cpp \
	myactiongroup.cpp \
	filedialog.cpp \
	inputmplayerversion.cpp \
	about.cpp \
	errordialog.cpp \
	timedialog.cpp \
	favorites.cpp \
	tvlist.cpp \
	favoriteeditor.cpp \
	basegui.cpp \
	baseguiplus.cpp \
	floatingwidget.cpp \
	widgetactions.cpp \
	toolbareditor.cpp \
	editabletoolbar.cpp \
	defaultgui.cpp \
	minigui.cpp \
	mpcgui/mpcgui.cpp \
	mpcgui/mpcstyles.cpp \
	clhelp.cpp \
	smplayer.cpp \
	myapplication.cpp \
	main.cpp

FORMS = inputdvddirectory.ui logwindowbase.ui filepropertiesdialog.ui \
        eqslider.ui seekwidget.ui inputurl.ui vdpauproperties.ui \
        preferencesdialog.ui prefgeneral.ui prefdrives.ui prefinterface.ui \
        prefperformance.ui prefinput.ui prefsubtitles.ui prefadvanced.ui \
        prefplaylist.ui preftv.ui favoriteeditor.ui \
        about.ui inputmplayerversion.ui errordialog.ui timedialog.ui \
        toolbareditor.ui

# qtsingleapplication
contains( DEFINES, SINGLE_INSTANCE ) {
	INCLUDEPATH += qtsingleapplication
	DEPENDPATH += qtsingleapplication

	SOURCES += qtsingleapplication.cpp qtlocalpeer.cpp
	HEADERS += qtsingleapplication.h qtlocalpeer.h
}

# Find subtitles dialog
contains( DEFINES, FIND_SUBTITLES ) {
	DEFINES += DOWNLOAD_SUBS

	INCLUDEPATH += findsubtitles
	DEPENDPATH += findsubtitles

	HEADERS += osparser.h findsubtitlesconfigdialog.h findsubtitleswindow.h
	SOURCES += osparser.cpp findsubtitlesconfigdialog.cpp findsubtitleswindow.cpp
	FORMS += findsubtitleswindow.ui findsubtitlesconfigdialog.ui
}

# Download subtitles
contains( DEFINES, DOWNLOAD_SUBS ) {
	INCLUDEPATH += findsubtitles/filedownloader findsubtitles/quazip
	DEPENDPATH += findsubtitles/filedownloader findsubtitles/quazip

	HEADERS += filedownloader.h subchooserdialog.h fixsubs.h
	SOURCES += filedownloader.cpp subchooserdialog.cpp fixsubs.cpp

	FORMS += subchooserdialog.ui

	HEADERS += crypt.h \
	           ioapi.h \
	           quazip.h \
	           quazipfile.h \
	           quazipfileinfo.h \
	           quazipnewinfo.h \
	           unzip.h \
	           zip.h

	SOURCES += ioapi.c \
	           quazip.cpp \
	           quazipfile.cpp \
	           quazipnewinfo.cpp \
	           unzip.c \
	           zip.c

	LIBS += -lz
	
	win32 {
		INCLUDEPATH += ..\\zlib
		LIBS += -L..\\zlib
	}
}

# Youtube support
contains( DEFINES, YOUTUBE_SUPPORT ) {
	INCLUDEPATH += youtube
	DEPENDPATH += youtube

	HEADERS += retrieveyoutubeurl.h
	SOURCES += retrieveyoutubeurl.cpp
}

# Skins support
contains( DEFINES, SKINS ) {
	INCLUDEPATH += skingui
	DEPENDPATH += skingui

	HEADERS += myicon.h mybutton.h panelseeker.h playcontrol.h \
               mediapanel.h volumecontrolpanel.h mediabarpanel.h \
               qpropertysetter.h actiontools.h skingui.h
	SOURCES += myicon.cpp mybutton.cpp panelseeker.cpp playcontrol.cpp \
               mediapanel.cpp volumecontrolpanel.cpp mediabarpanel.cpp \
               qpropertysetter.cpp actiontools.cpp skingui.cpp
	FORMS += mediapanel.ui mediabarpanel.ui
}

# Videopreview
contains( DEFINES, VIDEOPREVIEW ) {
	INCLUDEPATH += videopreview
	DEPENDPATH += videopreview

	HEADERS += videopreview.h videopreviewconfigdialog.h
	SOURCES += videopreview.cpp videopreviewconfigdialog.cpp

	FORMS += videopreviewconfigdialog.ui
}

# 
contains( DEFINES, FONTCACHE_DIALOG ) {
	HEADERS += fontcache.h
	SOURCES += fontcache.cpp
}

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj

	DEFINES += DATA_PATH=$(DATA_PATH)
	DEFINES += DOC_PATH=$(DOC_PATH)
	DEFINES += TRANSLATION_PATH=$(TRANSLATION_PATH)
	DEFINES += THEMES_PATH=$(THEMES_PATH)
	DEFINES += SHORTCUTS_PATH=$(SHORTCUTS_PATH)
	#DEFINES += NO_DEBUG_ON_CONSOLE
}

win32 {
	DEFINES += SCREENSAVER_OFF
	DEFINES += FONTCACHE_DIALOG

	contains( DEFINES, SCREENSAVER_OFF ) {
		HEADERS += screensaver.h
		SOURCES += screensaver.cpp
	}

	!contains( DEFINES, PORTABLE_APP ) {
		DEFINES += USE_ASSOCIATIONS
	}
	
	contains( DEFINES, USE_ASSOCIATIONS ) {
		HEADERS += prefassociations.h winfileassoc.h
		SOURCES += prefassociations.cpp winfileassoc.cpp
		FORMS += prefassociations.ui
	}

	contains(TEMPLATE,vcapp) {
		LIBS += ole32.lib user32.lib
	} else {
		LIBS += libole32
	}
	
	RC_FILE = smplayer.rc
	DEFINES += NO_DEBUG_ON_CONSOLE
	#debug {
	#	CONFIG += console
	#}
}

os2 {
	DEFINES += SCREENSAVER_OFF
	INCLUDEPATH += .
	contains( DEFINES, SCREENSAVER_OFF ) {
		HEADERS += screensaver.h
		SOURCES += screensaver.cpp
	}
	RC_FILE = smplayer_os2.rc
}


TRANSLATIONS = translations/smplayer_es.ts translations/smplayer_de.ts \
               translations/smplayer_sk.ts translations/smplayer_it.ts \
               translations/smplayer_fr.ts translations/smplayer_zh_CN.ts \
               translations/smplayer_ru_RU.ts translations/smplayer_hu.ts \
               translations/smplayer_en_US.ts translations/smplayer_pl.ts \
               translations/smplayer_ja.ts translations/smplayer_nl.ts \
               translations/smplayer_uk_UA.ts translations/smplayer_pt_BR.ts \
               translations/smplayer_ka.ts translations/smplayer_cs.ts \
               translations/smplayer_bg.ts translations/smplayer_tr.ts \
               translations/smplayer_sv.ts translations/smplayer_sr.ts \
               translations/smplayer_zh_TW.ts translations/smplayer_ro_RO.ts \
               translations/smplayer_pt.ts translations/smplayer_el_GR.ts \
               translations/smplayer_fi.ts translations/smplayer_ko.ts \
               translations/smplayer_mk.ts translations/smplayer_eu.ts \
               translations/smplayer_ca.ts translations/smplayer_sl_SI.ts \
               translations/smplayer_ar_SY.ts translations/smplayer_ku.ts \
               translations/smplayer_gl.ts translations/smplayer_vi_VN.ts \
               translations/smplayer_et.ts translations/smplayer_lt.ts \
               translations/smplayer_da.ts translations/smplayer_hr.ts
