;Language: German (1031)
;German language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "German" "Deutsch"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Installationsprogramm läuft bereits."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Administratorrechte sind nötig um dieses Programm zu installieren."

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "Binäre Codec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind, wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nAchtung, nicht notwendig um die gängisten Formate wiederzugeben, wie DVD, MPEG-1/2/4 , etc."

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
${LangFileString} MPLAYER_IS_DOWNLOADING "Lade MPlayer herunter..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer wurde nicht erfolgreich installiert. Wiederholung?"
${LangFileString} MPLAYER_DL_FAILED "Fehler beim Herunterladen von MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Lade MPlayer Codecs runter..."
${LangFileString} CODECS_DL_RETRY "MPlayer-Codecs wurden nicht erfolgreich installiert.. Wiederholung?"
${LangFileString} CODECS_DL_FAILED "Fehler beim Herunterladen der MPlayer Codecs: '$R0'."
${LangFileString} CODECS_INST_FAILED "Fehler beim Installieren der MPlayer Codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Lade Informationen der Version runter..."
${LangFileString} VERINFO_DL_FAILED "Fehler beim Herunterladen der Versionsinfo: '$R0'. Standard-Version wird genommen."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Administratorrechte sind nötig um dieses Programm zu deinstallieren."
${LangFileString} UNINSTALL_ABORTED "Deinstallieren vom Benutzer abgebrochen."
${LangFileString} SMPLAYER_NOT_INSTALLED "Es scheint, das SMPlayer nicht in dem Verzeichnis installiert ist '$INSTDIR'.$\r$\nTrotzdem fortfahren (nicht empfohlen)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer ist ein komplettes Front-End für MPlayer, von grundlegenden Funktionen, wie das Abspielen von Videos, DVDs, VCDs, bis zu erweiterten Funktionen wie die Unterstützung für MPlayer Filter, edl-Listen und vieles mehr.."
