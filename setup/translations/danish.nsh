;Language: Danish (1030)
;Danish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Danish" "Dansk"

; Startup
${LangFileString} Installer_Is_Running "Installationsprogrammet kører allerede."
${LangFileString} Installer_No_Admin "Du skal være logget på som administrator når du installere dette program."
${LangFileString} SMPlayer_Is_Running "SMPlayer kører allerede. Luk venligst SMPlayer og prøv igen."

${LangFileString} OS_Not_Supported "Styresystemet er ikke understøttet.$\nSMPlayer ${SMPLAYER_VERSION} kræver som minimum Windows XP og vil måske ikke virke korrekt på dit system.$\nEr du sikker på at du vil fortsætte installationen?"
${LangFileString} Win64_Required "Det kræver et 64-bit Windows-styresystem at installere denne software."
${LangFileString} Existing_32bitInst "Der findes allerede en 32-bit installation af SMPlayer. Du skal først afinstallere 32-bit SMPlayer."
${LangFileString} Existing_64bitInst "Der findes allerede en 64-bit installation af SMPlayer. Du skal først afinstallere 64-bit SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) installation"
${LangFileString} WelcomePage_Text "Installationsprogrammet guider dig gennem installationen af $(^NameDA).$\r$\n$\r$\nDet anbefales at du lukker alle forekomster af SMPlayer før installationen startes. Dermed kan de relevante programfiler opdateres uden at skulle genstarte din computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Genveje"
${LangFileString} MPlayerGroupTitle "MPlayer Komponenter"
${LangFileString} MPlayerMPVGroupTitle "Multimedie motor"

${LangFileString} Section_SMPlayer "SMPlayer (påkrævet)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, delte biblioteker, og dokumentation."

${LangFileString} Section_DesktopShortcut "Skrivebord"
${LangFileString} Section_DesktopShortcut_Desc "Lav en genvej til SMPlayer på skrivebordet."

${LangFileString} Section_StartMenu "Menuen Start"
${LangFileString} Section_StartMenu_Desc "Tilføj SMPlayer i menuen Start."

${LangFileString} Section_MPlayer "MPlayer (påkrævet)"
${LangFileString} Section_MPlayer_Desc "MPlayer; er påkrævet for at afspille."

${LangFileString} Section_MPlayerCodecs "Binære-codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binære-codecs er ikke understøttet i denne version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Yderligere codecs til MPlayer. (kræver internetforbindelse for at installere)"
!endif

${LangFileString} Section_MPV_Desc "En funktionsrig fork af MPlayer og MPlayer2"

${LangFileString} Section_MEncoder_Desc "Et medfølgende program til MPlayer der kan bruges til at indkode eller transformere understøttede lyd- og videostreams."

${LangFileString} Section_IconThemes "Ikon-temaer"
${LangFileString} Section_IconThemes_Desc "Yderligere ikon-temaer til SMPlayer."

${LangFileString} Section_Translations "Sprog"
${LangFileString} Section_Translations_Desc "Ikke-engelske sprogfiler til SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Sletter SMPlayer indstillinger fra tidligere installationer."

${LangFileString} MPlayer_Codec_Msg "De binære-codec pakker tilføjer understøttelse af codecs som endnu ikke er implementeret fra starten, såsom nyere RealVideo-varianter og mange ualmindelige formater.$\nBemærk at de ikke kræves for at kunne afspiller de mest almindelige formater såsom DVD'er, MPEG-1/2/4, osv."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Vælg installationstype"
${LangFileString} Reinstall_Header_SubText "Vælg Overskriv eller Afinstaller."

${LangFileString} Reinstall_Msg1 "Du har en eksisterende installation af SMPlayer i følgende mappe:"
${LangFileString} Reinstall_Msg2 "Vælg venligst hvordan du vil fortsætte:"
${LangFileString} Reinstall_Overwrite "Overskriv ($Inst_Type) den eksisterende installation"
${LangFileString} Reinstall_Uninstall "Afinstaller (fjern) den eksisterende installation"
${LangFileString} Reinstall_Msg3_1 "Klik på Start når du er klar til at fortsætte."
${LangFileString} Reinstall_Msg3_2 "Klik på Næste når du er klar til at fortsætte."
${LangFileString} Reinstall_Msg3_3 "Klik på Afinstaller når du er klar til at fortsætte."
${LangFileString} Reinstall_Msg4 "Skift installationsindstillinger"
${LangFileString} Reinstall_Msg5 "Nulstil mine SMPlayer indstillinger"

${LangFileString} Type_Reinstall "geninstaller"
${LangFileString} Type_Downgrade "nedgrader"
${LangFileString} Type_Upgrade "opgrader"

${LangFileString} StartBtn "Start"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Henter MPlayer-codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs blev ikke installeret korrekt. Prøv igen?"
${LangFileString} Codecs_DL_Failed "Hentning af MPlayer-codecs mislykkedes: '$R0'."
${LangFileString} Codecs_Inst_Failed "Installation af MPlayer-codecs mislykkedes."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Denne installation kan kun afinstalleres af en bruger med administrator tilladelser."
${LangFileString} Uninstaller_Aborted "Afinstallation afbrudt af bruger."
${LangFileString} Uninstaller_NotInstalled "Det ser ikke ud til at SMPlayer er installeret i mappen '$INSTDIR'.$\r$\nFortsæt alligevel (anbefales ikke)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer-installation blev ikke fundet."
${LangFileString} Uninstaller_64bitOnly "Denne installation kan kun afinstalleres på 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer er en komplet brugerflade til MPlayer, fra grundlæggende funktionalitet såsom at afspille videoer, DVD'er, VCD'er til avanceret funktionalitet såsom understøttelse af MPlayer filtre, EDL lister, og andet."

; Misc
${LangFileString} Info_Codecs_Backup "Foretager backup af codecs fra tidligere installation..."
${LangFileString} Info_Codecs_Restore "Gendanner codecs fra tidligere installation..."
${LangFileString} Info_Del_Files "Sletter filer..."
${LangFileString} Info_Del_Registry "Sletter registreringsdatabasenøgler..."
${LangFileString} Info_Del_Shortcuts "Sletter genveje..."
${LangFileString} Info_Rest_Assoc "Gendanner filtilknytninger..."
${LangFileString} Info_RollBack "Gendanner ændringer..."
${LangFileString} Info_Files_Extract "Udpakker filer..."
${LangFileString} Info_SMTube_Backup "Opretter backup af SMTube..."
${LangFileString} Info_SMTube_Restore "Gendanner SMTube fra tidligere installation..."
${LangFileString} Info_MPV_Backup "Opretter backup af MPV..."
${LangFileString} Info_MPV_Restore "Gendanner MPV fra tidligere installation..."

; MPV
${LangFileString} MPV_DL_Msg "Henter MPV..."
${LangFileString} MPV_DL_Retry "MPV blev ikke korrekt installeret. Prøv igen?"
${LangFileString} MPV_DL_Failed "Hentning af MPV mislykkedes: '$R0'."
${LangFileString} MPV_Inst_Failed "Installation af MPV mislykkedes."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL blev ikke korrekt installeret. Forsøg igen?"
${LangFileString} YTDL_DL_Failed "Download af YouTube-DL mislykkedes: '$R0'."
