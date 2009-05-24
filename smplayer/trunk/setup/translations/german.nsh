;;
;;  german.nsh
;;
;;  German language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_GERMAN} "The installer is already running."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_GERMAN} "Unsupported operating system.$\n$(^Name) requires at least Windows 2000 and may not work correctly on your system.$\nDo you really want to continue with the installation?"
LangString SMPLAYER_INSTALLER_NO_ADMIN ${LANG_GERMAN} "You must be logged in as an administrator when installing this program."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_GERMAN} "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_GERMAN} "Binäre Codec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind, wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nAchtung, nicht notwendig um die gängisten Formate wiederzugeben, wie DVD, MPEG-1/2/4 , etc."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_GERMAN} "MPlayer ist bereits installiert. Erneut herunterladen?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_GERMAN} "Lade MPlayer herunter..."
LangString MPLAYER_DL_FAILED ${LANG_GERMAN} "Fehler beim Herunterladen von MPlayer:"
LangString MPLAYER_INST_FAILED ${LANG_GERMAN} "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_GERMAN} "MPlayer-Codecs sind bereits installiert. Erneut herunterladen?"
LangString CODECS_IS_DOWNLOADING ${LANG_GERMAN} "Lade MPlayer Codecs runter..."
LangString CODECS_DL_FAILED ${LANG_GERMAN} "Fehler beim Herunterladen der MPlayer Codecs:"
LangString CODECS_INST_FAILED ${LANG_GERMAN} "Fehler beim Installieren der MPlayer Codecs."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_GERMAN} "Lade Informationen der Version runter..."
LangString VERINFO_DL_FAILED ${LANG_GERMAN} "Fehler beim Herunterladen der Versionsinfo:"
LangString VERINFO_IS_MISSING ${LANG_GERMAN} "Versionsdatei fehlen Informationen über die Version. Setup nimmt Standardversion."

; Uninstaller
LangString UNINSTALL_NO_ADMIN ${LANG_GERMAN} "This installation can only be uninstalled by a user with administrator privileges."
LangString SMPLAYER_NOT_INSTALLED ${LANG_GERMAN} "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"