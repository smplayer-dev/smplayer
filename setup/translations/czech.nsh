;;
;;  czech.nsh
;;
;;  Czech language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_CZECH} "The installer is already running."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_CZECH} "Unsupported operating system.$\n$(^Name) requires at least Windows 2000 and may not work correctly on your system.$\nDo you really want to continue with the installation?"
LangString SMPLAYER_INSTALLER_NO_ADMIN ${LANG_CZECH} "You must be logged in as an administrator when installing this program."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_CZECH} "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_CZECH} "Binární kodeky podporují formáty, které zatím nejsou implementovány nativne, napr. novejší varianty RealVideo a jiné málo používané formáty.$\nPro vetšinu bežných formátu nejsou potreba (DVD, MPEG-1/2/4, apod.)."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_CZECH} "MPlayer je již nainstalován. Stáhnout znovu?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_CZECH} "Stahuji MPlayer..."
LangString MPLAYER_DL_FAILED ${LANG_CZECH} "Nepovedlo se stáhnout MPlayer:"
LangString MPLAYER_INST_FAILED ${LANG_CZECH} "Nepovedlo se nainstalovat MPlayer. MPlayer je potrebný pro prehrávání."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_CZECH} "Kodeky MPlayeru jsou již nainstalovány. Stáhnout znovu?"
LangString CODECS_IS_DOWNLOADING ${LANG_CZECH} "Instaluji kodeky MPlayeru..."
LangString CODECS_DL_FAILED ${LANG_CZECH} "Nepovedlo se stáhnout kodeky MPlayeru:"
LangString CODECS_INST_FAILED ${LANG_CZECH} "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_CZECH} "Stahuji informace o verzích..."
LangString VERINFO_DL_FAILED ${LANG_CZECH} "Nepovedlo se stáhnout informace o verzích:"
LangString VERINFO_IS_MISSING ${LANG_CZECH} "Verzovací soubor neobsahuje správné informace. Bude použita výchozí verze."

; Uninstaller
LangString UNINSTALL_NO_ADMIN ${LANG_CZECH} "This installation can only be uninstalled by a user with administrator privileges."
LangString UNINSTALL_ABORTED ${LANG_CZECH} "Uninstall aborted by user."
LangString SMPLAYER_NOT_INSTALLED ${LANG_CZECH} "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
LangString APPLICATION_DESCRIPTION ${LANG_CZECH} "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."