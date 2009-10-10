;Language: Dutch (1043)
;Dutch language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Dutch" "Nederlands"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "The installer is already running."
${LangFileString} SMPLAYER_INSTALLER_PREV_ALL_USERS "SMPlayer has been previously installed for all users.$\nPlease restart the installer with Administrator privileges."
${LangFileString} SMPLAYER_INSTALLER_PREV_VERSION "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "MPlayer is already installed. Re-Download?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Downloading MPlayer..."
  ${LangFileString} MPLAYER_DL_FAILED "Failed to download MPlayer:"
  ${LangFileString} MPLAYER_INST_FAILED "Failed to install MPlayer. MPlayer is required for playback."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "MPlayer codecs are already installed. Re-Download?"
${LangFileString} CODECS_IS_DOWNLOADING "Downloading MPlayer codecs..."
${LangFileString} CODECS_DL_FAILED "Failed to download MPlayer codecs:"
${LangFileString} CODECS_INST_FAILED "Failed to install MPlayer codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Downloading version information..."
${LangFileString} VERINFO_DL_FAILED "Failed to download version info:"
${LangFileString} VERINFO_IS_MISSING "Version file missing version information. Setup will use a default version."

; Uninstaller
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} UNINSTALL_INSTALLED_ALL_USERS "SMPlayer has been installed for all users.$\nPlease restart the uninstaller with Administrator privileges to remove it."
${LangFileString} SMPLAYER_NOT_INSTALLED "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."