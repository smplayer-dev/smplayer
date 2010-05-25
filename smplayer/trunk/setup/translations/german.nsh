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
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (erforderlich)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, Laufzeit-Bibliotheken und Dokumentation."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Verknüpfung"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Desktop"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Erstellt eine Verknüpfung zu SMPlayer auf dem Desktop."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Startmenü"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Erstellt einen Startmenü Eintrag für SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "MPlayer Komponenten"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (erforderlich)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; benötigt für die Wiedergabe."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; benötigt für die Wiedergabe. (Internet Verbindung benötigt für Installation)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binärcodecs"
${LangFileString} SMPLAYER_SECCODECS_DESC "Optionale Codecs für MPlayer. (Internet Verbindung benötigt für Installation)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Icon Themen"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Weitere Icon-Themen für SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Lokalisierungen"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Non-English Lokalisierungen."

${LangFileString} MPLAYER_CODEC_INFORMATION "Binäre Codec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind, wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nAchtung, nicht notwendig um die gängigsten Formate wiederzugeben, wie DVD, MPEG-1/2/4 , etc."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Bereits installiert"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Auswahl wie SMPlayer installiert werden soll."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Auswahl der auszuführenden Wartungsoption."

${LangFileString} REINSTALL_CHGSETTINGS "Einstellungen ändern (Fortgeschritten)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unbekannter Zustandswert der vorangehenden Version, Abbruch."

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Eine ältere Version von SMPlayer ist auf diesem System installiert. Auswahl der Operation die durchführt werden soll und auf Weiter klicken, um fortzufahren."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Aktualisieren von SMPlayer mit vorherigen Einstellungen (empfohlen)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Eine neuere Version von SMPlayer ist bereits installiert! Es wird nicht empfohlen eine ältere Version zu installieren. Auswahl der Operation die durchführt werden soll und auf Weiter klicken, um fortzufahren."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Herabstufung von SMPlayer mit vorherigen Einstellungen (empfohlen)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} ist bereits installiert. Auswahl der Operation die durchführt werden soll und auf Weiter klicken, um fortzufahren."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Hinzufügen/Entfernen/Neuinstallation der Komponenten"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "SMPlayer deinstallieren"

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
