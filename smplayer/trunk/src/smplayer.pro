TEMPLATE = app
LANGUAGE = C++

CONFIG += qt warn_on
#CONFIG += release
CONFIG += debug

QT += network xml

RESOURCES = icons.qrc

#DEFINES += EXPERIMENTAL
DEFINES += SINGLE_INSTANCE
DEFINES += FIND_SUBTITLES
DEFINES += VIDEOPREVIEW
DEFINES += YOUTUBE_SUPPORT
DEFINES += YT_USE_SCRIPT
DEFINES += BLURAY_SUPPORT
DEFINES += GUI_CHANGE_ON_RUNTIME
DEFINES += LOG_MPLAYER
DEFINES += LOG_SMPLAYER
DEFINES += MPCGUI
DEFINES += SKINS
DEFINES += UPDATE_CHECKER
DEFINES += CHECK_UPGRADED
DEFINES += REMINDER_ACTIONS
#DEFINES += USE_FONTCONFIG_OPTIONS

isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets gui
	#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040000
	DEFINES -= MPCGUI
}

contains(QT_VERSION, ^4\\.[0-3]\\..*) {
	message("Some features requires Qt > 4.3.")

	contains( DEFINES, SINGLE_INSTANCE ) {
		DEFINES -= SINGLE_INSTANCE
		message("SINGLE_INSTANCE disabled.")
	}

	contains( DEFINES, YOUTUBE_SUPPORT ) {
		DEFINES -= YOUTUBE_SUPPORT
		message("YOUTUBE_SUPPORT disabled.")
	}

	contains( DEFINES, SKINS ) {
		DEFINES -= SKINS
		message("SKINS disabled.")
	}

	contains( DEFINES, FIND_SUBTITLES ) {
		DEFINES -= FIND_SUBTITLES
		message("FIND_SUBTITLES disabled.")
	}

	contains( DEFINES, UPDATE_CHECKER ) {
		DEFINES -= UPDATE_CHECKER
		message("UPDATE_CHECKER disabled.")
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
	prefupdates.h \
	filepropertiesdialog.h \
	multilineinputdialog.h \
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
	autohidewidget.h \
	widgetactions.h \
	toolbareditor.h \
	editabletoolbar.h \
	defaultgui.h \
	minigui.h \
	clhelp.h \
	cleanconfig.h \
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
	prefupdates.cpp \
	filepropertiesdialog.cpp \
	multilineinputdialog.cpp \
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
	autohidewidget.cpp \
	widgetactions.cpp \
	toolbareditor.cpp \
	editabletoolbar.cpp \
	defaultgui.cpp \
	minigui.cpp \
	clhelp.cpp \
	cleanconfig.cpp \
	smplayer.cpp \
	myapplication.cpp \
	main.cpp

FORMS = inputdvddirectory.ui logwindowbase.ui filepropertiesdialog.ui \
        eqslider.ui seekwidget.ui inputurl.ui videoequalizer.ui vdpauproperties.ui \
        preferencesdialog.ui prefgeneral.ui prefdrives.ui prefinterface.ui \
        prefperformance.ui prefinput.ui prefsubtitles.ui prefadvanced.ui \
        prefplaylist.ui preftv.ui prefupdates.ui favoriteeditor.ui \
        about.ui inputmplayerversion.ui errordialog.ui timedialog.ui \
        toolbareditor.ui multilineinputdialog.ui

# qtsingleapplication
contains( DEFINES, SINGLE_INSTANCE ) {
	INCLUDEPATH += qtsingleapplication
	DEPENDPATH += qtsingleapplication

	SOURCES += qtsingleapplication/qtsingleapplication.cpp qtsingleapplication/qtlocalpeer.cpp
	HEADERS += qtsingleapplication/qtsingleapplication.h qtsingleapplication/qtlocalpeer.h
}

# Find subtitles dialog
contains( DEFINES, FIND_SUBTITLES ) {
	DEFINES += DOWNLOAD_SUBS
	#DEFINES += USE_QUAZIP

	INCLUDEPATH += findsubtitles
	DEPENDPATH += findsubtitles

	INCLUDEPATH += findsubtitles/maia
	DEPENDPATH += findsubtitles/maia

	HEADERS += findsubtitles/findsubtitlesconfigdialog.h findsubtitles/findsubtitleswindow.h
	SOURCES += findsubtitles/findsubtitlesconfigdialog.cpp findsubtitles/findsubtitleswindow.cpp
	FORMS += findsubtitles/findsubtitleswindow.ui findsubtitles/findsubtitlesconfigdialog.ui

	# xmlrpc client code to connect to opensubtitles.org
	HEADERS += findsubtitles/maia/maiaObject.h findsubtitles/maia/maiaFault.h findsubtitles/maia/maiaXmlRpcClient.h findsubtitles/osclient.h
	SOURCES += findsubtitles/maia/maiaObject.cpp findsubtitles/maia/maiaFault.cpp findsubtitles/maia/maiaXmlRpcClient.cpp findsubtitles/osclient.cpp
}

# Download subtitles
contains( DEFINES, DOWNLOAD_SUBS ) {
	INCLUDEPATH += findsubtitles/filedownloader
	DEPENDPATH += findsubtitles/filedownloader

	HEADERS += findsubtitles/filedownloader/filedownloader.h findsubtitles/subchooserdialog.h findsubtitles/fixsubs.h
	SOURCES += findsubtitles/filedownloader/filedownloader.cpp findsubtitles/subchooserdialog.cpp findsubtitles/fixsubs.cpp

	FORMS += findsubtitles/subchooserdialog.ui

	contains( DEFINES, USE_QUAZIP ) {
		INCLUDEPATH += findsubtitles/quazip
		DEPENDPATH += findsubtitles/quazip

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
}

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

	HEADERS += youtube/retrieveyoutubeurl.h youtube/ytsig.h
	SOURCES += youtube/retrieveyoutubeurl.cpp youtube/ytsig.cpp

	contains( DEFINES, YT_USE_SCRIPT ) {
		HEADERS += youtube/codedownloader.h
		SOURCES += youtube/codedownloader.cpp
		QT += script
	} else {
		#DEFINES += YTSIG_STATIC
	}
}

# mpcgui
contains( DEFINES, MPCGUI ) {
	INCLUDEPATH += mpcgui
	DEPENDPATH += mpcgui

	HEADERS += mpcgui/mpcgui.h mpcgui/mpcstyles.h
	SOURCES += mpcgui/mpcgui.cpp mpcgui/mpcstyles.cpp
}

# Skins support
contains( DEFINES, SKINS ) {
	INCLUDEPATH += skingui
	DEPENDPATH += skingui

	HEADERS += skingui/myicon.h skingui/mybutton.h skingui/panelseeker.h skingui/playcontrol.h \
               skingui/mediapanel.h skingui/volumecontrolpanel.h skingui/mediabarpanel.h \
               skingui/qpropertysetter.h skingui/actiontools.h skingui/skingui.h
	SOURCES += skingui/myicon.cpp skingui/mybutton.cpp skingui/panelseeker.cpp skingui/playcontrol.cpp \
               skingui/mediapanel.cpp skingui/volumecontrolpanel.cpp skingui/mediabarpanel.cpp \
               skingui/qpropertysetter.cpp skingui/actiontools.cpp skingui/skingui.cpp
	FORMS += skingui/mediapanel.ui skingui/mediabarpanel.ui
}

# Update checker
contains( DEFINES, UPDATE_CHECKER ) {
	HEADERS += updatechecker.h
	SOURCES += updatechecker.CPP
}

# Videopreview
contains( DEFINES, VIDEOPREVIEW ) {
	INCLUDEPATH += videopreview
	DEPENDPATH += videopreview

	HEADERS += videopreview/videopreview.h videopreview/videopreviewconfigdialog.h
	SOURCES += videopreview/videopreview.cpp videopreview/videopreviewconfigdialog.cpp

	FORMS += videopreview/videopreviewconfigdialog.ui
}

contains( DEFINES, REMINDER_ACTIONS ) {
	HEADERS += reminderdialog.h
	SOURCES += reminderdialog.cpp
	FORMS += reminderdialog.ui

	HEADERS += sharedialog.h
	SOURCES += sharedialog.cpp
	FORMS += sharedialog.ui
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
	#DEFINES += AVOID_SCREENSAVER
	#DEFINES += FONTCACHE_DIALOG
	DEFINES += USE_FONTCONFIG_OPTIONS

	contains( DEFINES, SCREENSAVER_OFF ) {
		HEADERS += screensaver.h
		SOURCES += screensaver.cpp
	}

	contains( DEFINES, FONTCACHE_DIALOG ) {
		HEADERS += fontcache.h
		SOURCES += fontcache.cpp
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
               translations/smplayer_da.ts translations/smplayer_hr.ts \
               translations/smplayer_he_IL.ts translations/smplayer_th.ts \
               translations/smplayer_ms_MY.ts translations/smplayer_uz.ts \
               translations/smplayer_nn_NO.ts translations/smplayer_id.ts \
               translations/smplayer_ar.ts translations/smplayer_en_GB.ts
