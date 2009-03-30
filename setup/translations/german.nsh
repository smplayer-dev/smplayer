;;
;;  german.nsh
;;
;;  Default language strings for the Windows SMPlayer NSIS installer.
;;
;;  Replace all text between the string with the translation.
;;
;;  Do not edit this line!
!insertmacro LANGFILE_EXT "German"
;;

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "Binäre Codec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind, wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nAchtung, nicht notwendig um die gängisten Formate wiederzugeben, wie DVD, MPEG-1/2/4 , etc."

; MPlayer Section
${LangFileString} MPLAYER_IS_INSTALLED "MPlayer ist bereits installiert. Erneut herunterladen?"
${LangFileString} MPLAYER_IS_DOWNLOADING "Lade MPlayer herunter..."
${LangFileString} MPLAYER_DL_FAILED "Fehler beim Herunterladen von MPlayer:"
${LangFileString} MPLAYER_INST_FAILED "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "MPlayer-Codecs sind bereits installiert. Erneut herunterladen?"
${LangFileString} CODECS_IS_DOWNLOADING "Lade MPlayer Codecs runter..."
${LangFileString} CODECS_DL_FAILED "Fehler beim Herunterladen der MPlayer Codecs:"
${LangFileString} CODECS_INST_FAILED "Fehler beim Installieren der MPlayer Codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Lade Informationen der Version runter..."
${LangFileString} VERINFO_DL_FAILED "Fehler beim Herunterladen der Versionsinfo:"
${LangFileString} VERINFO_IS_MISSING "Versionsdatei fehlen Informationen über die Version. Setup nimmt Standardversion."
