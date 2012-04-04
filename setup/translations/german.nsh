;Language: German (1031)
;German language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "German" "Deutsch"

; Startup
${LangFileString} Installer_Is_Running "Das Installationsprogramm läuft bereits."
${LangFileString} Installer_No_Admin "Es sind Administratorrechte nötig, um dieses Programm zu installieren."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "Verknüpfung"
${LangFileString} MPlayerGroupTitle "MPlayer-Komponenten"

${LangFileString} Section_SMPlayer "SMPlayer (erforderlich)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, dynamische Bibliotheken und Dokumentation."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Erstellt eine Verknüpfung für SMPlayer auf dem Desktop."

${LangFileString} Section_StartMenu "Startmenü"
${LangFileString} Section_StartMenu_Desc "Erstellt einen Startmenü-Eintrag für SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (erforderlich)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer – benötigt für die Wiedergabe."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer – benötigt für die Wiedergabe. (Internetverbindung benötigt für Installation)"
!endif

${LangFileString} Section_MPlayerCodecs "Binärcodecs"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Optionale Codecs für MPlayer."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Optionale Codecs für MPlayer. (Internetverbindung benötigt für Installation)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Symboldesigns"
${LangFileString} Section_IconThemes_Desc "Weitere Symboldesigns für SMPlayer."

${LangFileString} Section_Translations "Lokalisierungen"
${LangFileString} Section_Translations_Desc "Nicht englische Lokalisierungen."

${LangFileString} MPlayer_Codec_Msg "Binärcodec-Pakete werden eingesetzt für Codecs, die noch nicht nativ implementiert sind – wie neuere Varianten von RealVideo und viele ungewöhnliche Formate.$\nHinweis: Die Pakete sind nicht notwendig, um die gängigsten Formate wiederzugeben (DVD, MPEG-1/2/4, etc.)"

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Select Install Type"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3_1 "Click Start when ready to proceed."
${LangFileString} Reinstall_Msg3_2 "Click Next when ready to proceed."
${LangFileString} Reinstall_Msg3_3 "Click Uninstall when ready to proceed."
${LangFileString} Reinstall_Msg4 "Change Installation Settings"

${LangFileString} Type_Reinstall "reinstall"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

${LangFileString} StartBtn "Start"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "MPlayer herunterladen …"
${LangFileString} MPlayer_DL_Retry "MPlayer ist nicht erfolgreich installiert worden. Erneut versuchen?"
${LangFileString} MPlayer_DL_Failed "Fehler beim Herunterladen von MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Fehler beim Installieren von MPlayer. MPlayer ist erforderlich für die Wiedergabe."

; Codecs Section
${LangFileString} Codecs_DL_Msg "MPlayer-Codecs herunterladen …"
${LangFileString} Codecs_DL_Retry "Die MPlayer-Codecs sind nicht erfolgreich installiert worden. Erneut versuchen?"
${LangFileString} Codecs_DL_Failed "Fehler beim Herunterladen der MPlayer-Codecs: '$R0'."
${LangFileString} Codecs_Inst_Failed "Fehler beim Installieren der MPlayer-Codecs."

; Version information
${LangFileString} VerInfo_DL_Msg "Informationen der Version herunterladen …"
${LangFileString} VerInfo_DL_Failed "Fehler beim Herunterladen der Versionsinfo: '$R0'. Standard-Version wird genommen."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Es sind Administratorrechte nötig, um dieses Programm zu deinstallieren."
${LangFileString} Uninstaller_Aborted "Die Deinstallation ist vom Benutzer abgebrochen worden."
${LangFileString} Uninstaller_NotInstalled "Es scheint, dass SMPlayer nicht im Verzeichnis '$INSTDIR' installiert ist.$\r$\nTrotzdem fortfahren (nicht empfohlen)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer ist ein komplettes grafische Oberfläche für MPlayer, von grundlegenden Funktionen, wie das Abspielen von Videos, DVDs, VCDs, bis zu erweiterten Funktionen, wie die Unterstützung für MPlayer-Filter, edl-Listen und vielem mehr."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Extracting files..."
