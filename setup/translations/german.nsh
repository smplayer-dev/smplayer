;Language: German (1031)
;German language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "German" "Deutsch"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Das Installationsprogramm läuft bereits."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Es sind Administratorrechte nötig, um dieses Programm zu installieren."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (erforderlich)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, dynamische Bibliotheken und Dokumentation."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Verknüpfung"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Desktop"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Erstellt eine Verknüpfung für SMPlayer auf dem Desktop."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Startmenü"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Erstellt einen Startmenü-Eintrag für SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "MPlayer-Komponenten"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (erforderlich)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer – benötigt für die Wiedergabe."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer – benötigt für die Wiedergabe. (Internetverbindung benötigt für Installation)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binärcodecs"
${LangFileString} SMPLAYER_SECCODECS_DESC "Optionale Codecs für MPlayer. (Internetverbindung benötigt für Installation)"
${LangFileString} SMPLAYER_SECCODECS_DESC_2 "Optionale Codecs für MPlayer."

${LangFileString} SMPLAYER_SECMENCODER_DESC "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Symboldesigns"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Weitere Symboldesigns für SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Lokalisierungen"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Nicht englische Lokalisierungen."

${LangFileString} MPLAYER_CODEC_INFORMATION "Binärcodec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind – wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nHinweis: Die Pakete sind nicht notwendig, um die gängigsten Formate wiederzugeben (DVD, MPEG-1/2/4, etc.)"

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Bereits installiert"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Wählen Sie aus, wie SMPlayer installiert werden soll."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Wählen Sie die auszuführende Wartungsoption aus."

${LangFileString} REINSTALL_CHGSETTINGS "Einstellungen ändern (Fortgeschritten)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unbekannter Wert von PREVIOUS_VERSION_STATE – Abbruch."

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Eine ältere Version von SMPlayer ist auf diesem System installiert. Wählen Sie die Aktion aus, die durchgeführt werden soll und klicken Sie auf Weiter, um fortzufahren."
${LangFileString} REINSTALL_OLDVER_UPGRADE "SMPlayer mit vorherigen Einstellungen aktualisieren (empfohlen)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Eine neuere Version von SMPlayer ist bereits installiert! Es wird nicht empfohlen eine ältere Version zu installieren. Wählen Sie die Aktion aus, die durchgeführt werden soll und klicken Sie auf Weiter, um fortzufahren."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "SMPlayer mit vorherigen Einstellungen herabstufen (downgrade) (empfohlen)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} ist bereits installiert. Wählen Sie die Aktion aus, die durchgeführt werden soll und klicken Sie auf Weiter, um fortzufahren."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Hinzufügen/Entfernen/Neuinstallation von Komponenten"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "SMPlayer deinstallieren"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "MPlayer herunterladen …"
${LangFileString} MPLAYER_DL_RETRY "MPlayer ist nicht erfolgreich installiert worden. Erneut versuchen?"
${LangFileString} MPLAYER_DL_FAILED "Fehler beim Herunterladen von MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "MPlayer-Codecs herunterladen …"
${LangFileString} CODECS_DL_RETRY "Die MPlayer-Codecs sind nicht erfolgreich installiert worden. Erneut versuchen?"
${LangFileString} CODECS_DL_FAILED "Fehler beim Herunterladen der MPlayer-Codecs: '$R0'."
${LangFileString} CODECS_INST_FAILED "Fehler beim Installieren der MPlayer-Codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Informationen der Version herunterladen …"
${LangFileString} VERINFO_DL_FAILED "Fehler beim Herunterladen der Versionsinfo: '$R0'. Standard-Version wird genommen."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Es sind Administratorrechte nötig, um dieses Programm zu deinstallieren."
${LangFileString} UNINSTALL_ABORTED "Die Deinstallation ist vom Benutzer abgebrochen worden."
${LangFileString} SMPLAYER_NOT_INSTALLED "Es scheint, dass SMPlayer nicht im Verzeichnis '$INSTDIR' installiert ist.$\r$\nTrotzdem fortfahren (nicht empfohlen)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer ist ein komplettes grafische Oberfläche für MPlayer, von grundlegenden Funktionen, wie das Abspielen von Videos, DVDs, VCDs, bis zu erweiterten Funktionen, wie die Unterstützung für MPlayer-Filter, edl-Listen und vielem mehr."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
