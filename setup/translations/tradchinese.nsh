;Language: Chinese (Traditional) (1028)
;Chinese (Traditional) language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "TradChinese" "漢語(繁體)"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "The installer is already running."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "You must be logged in as an administrator when installing this program."

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
# MUI Headers
${LangFileString} REINSTALL_HEADER_TEXT "Already Installed"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Choose how you want to install SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Choose the maintenance option to perform."
# Misc
${LangFileString} REINSTALL_CHGSETTINGS "Change settings (advanced)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unknown value of PREVIOUS_VERSION_STATE, aborting"
# Older version detected
${LangFileString} REINSTALL_OLDVER_DESCRIPTION "An older version of SMPlayer is installed on your system. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Upgrade SMPlayer using previous settings (recommended)"
# Newer version detected
${LangFileString} REINSTALL_NEWVER_DESCRIPTION "A newer version of SMPlayer is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Downgrade SMPlayer using previous settings (recommended)"
# Same version detected
${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} is already installed. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Add/Remove/Reinstall components"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Uninstall SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Downloading MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer was not successfully installed. Retry?"
${LangFileString} MPLAYER_DL_FAILED "Failed to download MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Failed to install MPlayer. MPlayer is required for playback."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Downloading MPlayer codecs..."
${LangFileString} CODECS_DL_RETRY "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} CODECS_DL_FAILED "Failed to download MPlayer codecs: '$R0'."
${LangFileString} CODECS_INST_FAILED "Failed to install MPlayer codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Downloading version information..."
${LangFileString} VERINFO_DL_FAILED "Failed to download version info: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} SMPLAYER_NOT_INSTALLED "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."