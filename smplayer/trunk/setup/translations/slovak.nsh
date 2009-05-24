;;
;;  slovak.nsh
;;
;;  Slovak language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_SLOVAK} "The installer is already running."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_SLOVAK} "Unsupported operating system.$\n$(^Name) requires at least Windows 2000 and may not work correctly on your system.$\nDo you really want to continue with the installation?"
LangString SMPLAYER_INSTALLER_NO_ADMIN ${LANG_SLOVAK} "You must be logged in as an administrator when installing this program."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_SLOVAK} "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_SLOVAK} "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_SLOVAK} "MPlayer is already installed. Re-Download?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_SLOVAK} "Downloading MPlayer..."
LangString MPLAYER_DL_FAILED ${LANG_SLOVAK} "Failed to download MPlayer:"
LangString MPLAYER_INST_FAILED ${LANG_SLOVAK} "Failed to install MPlayer. MPlayer is required for playback."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_SLOVAK} "MPlayer codecs are already installed. Re-Download?"
LangString CODECS_IS_DOWNLOADING ${LANG_SLOVAK} "Downloading MPlayer codecs..."
LangString CODECS_DL_FAILED ${LANG_SLOVAK} "Failed to download MPlayer codecs:"
LangString CODECS_INST_FAILED ${LANG_SLOVAK} "Failed to install MPlayer codecs."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_SLOVAK} "Downloading version information..."
LangString VERINFO_DL_FAILED ${LANG_SLOVAK} "Failed to download version info:"
LangString VERINFO_IS_MISSING ${LANG_SLOVAK} "Version file missing version information. Setup will use a default version."

; Uninstaller
LangString UNINSTALL_NO_ADMIN ${LANG_SLOVAK} "This installation can only be uninstalled by a user with administrator privileges."
LangString SMPLAYER_NOT_INSTALLED ${LANG_SLOVAK} "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"