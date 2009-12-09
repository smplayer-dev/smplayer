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

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "MPlayer ist bereits installiert. Erneut herunterladen?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Lade MPlayer herunter..."
  ${LangFileString} MPLAYER_DL_RETRY "MPlayer was not successfully installed. Retry?"
  ${LangFileString} MPLAYER_DL_FAILED "Fehler beim Herunterladen von MPlayer: '$R0'."
  ${LangFileString} MPLAYER_INST_FAILED "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "MPlayer-Codecs sind bereits installiert. Erneut herunterladen?"
${LangFileString} CODECS_IS_DOWNLOADING "Lade MPlayer Codecs runter..."
${LangFileString} CODECS_DL_RETRY "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} CODECS_DL_FAILED "Fehler beim Herunterladen der MPlayer Codecs: '$R0'."
${LangFileString} CODECS_INST_FAILED "Fehler beim Installieren der MPlayer Codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Lade Informationen der Version runter..."
${LangFileString} VERINFO_DL_FAILED "Fehler beim Herunterladen der Versionsinfo: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Administratorrechte sind nötig um dieses Programm zu deinstallieren."
${LangFileString} UNINSTALL_ABORTED "Deinstallieren vom Benutzer abgebrochen."
${LangFileString} SMPLAYER_NOT_INSTALLED "Es scheint, das SMPlayer nicht in dem Verzeichnis installiert ist '$INSTDIR'.$\r$\nTrotzdem fortfahren (nicht empfohlen)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer ist eine komplettes Front-End für MPlayer, von grundlegenden Funktionen, wie das Abspielen von Videos, DVDs, VCDs, bis erweiterte Funktionen wie die Unterstützung für MPlayer Filter, edl-Listen und vieles mehr.."