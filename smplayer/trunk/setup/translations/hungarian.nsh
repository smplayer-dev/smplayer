;;
;;  hungarian.nsh
;;
;;  Hungarian language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_HUNGARIAN} "The installer is already running."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_HUNGARIAN} "Unsupported operating system.$\n$(^Name) requires at least Windows 2000 and may not work correctly on your system.$\nDo you really want to continue with the installation?"
LangString SMPLAYER_INSTALLER_NO_ADMIN ${LANG_HUNGARIAN} "You must be logged in as an administrator when installing this program."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_HUNGARIAN} "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_HUNGARIAN} "A bináris kodek csomagok támogatást nyújtanak natívan még nem támogatott kodekekhez, mint pl. az újabb RealVideo variánsok és sok ritka formátum.$\nNem szükségesek a legtöbb gyakori formátum lejátszásához, mint a DVD-k, MPEG-1/2/4, stb."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_HUNGARIAN} "Az MPlayer már telepítve van. Újra letöltsem?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_HUNGARIAN} "Az MPlayer letöltése..."
LangString MPLAYER_DL_FAILED ${LANG_HUNGARIAN} "Az MPlayer letöltése nem sikerült:"
LangString MPLAYER_INST_FAILED ${LANG_HUNGARIAN} "Az MPlayer telepítése nem sikerült. Az MPlayerre szükség van a lejátszáshoz."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_HUNGARIAN} "Az MPlayer kodekek már telepítve vannak. Újra letöltsem?"
LangString CODECS_IS_DOWNLOADING ${LANG_HUNGARIAN} "MPlayer kodekek letöltése..."
LangString CODECS_DL_FAILED ${LANG_HUNGARIAN} "Az MPlayer kodekek letöltése nem sikerült:"
LangString CODECS_INST_FAILED ${LANG_HUNGARIAN} "Az MPlayer kodekek telepítése nem sikerült."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_HUNGARIAN} "Verzió információ letöltése..."
LangString VERINFO_DL_FAILED ${LANG_HUNGARIAN} "Verzió információ letöltése nem sikerült:"
LangString VERINFO_IS_MISSING ${LANG_HUNGARIAN} "A verzió fájlból hiányzik a verzió információ. A telepítõ egy alapértelmezett verziót fog használni."

; Uninstaller
LangString UNINSTALL_NO_ADMIN ${LANG_HUNGARIAN} "This installation can only be uninstalled by a user with administrator privileges."
LangString SMPLAYER_NOT_INSTALLED ${LANG_HUNGARIAN} "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"