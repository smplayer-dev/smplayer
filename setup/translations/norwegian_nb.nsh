;Language: Norwegian (1044)
;Norwegian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Norwegian" "Norwegian"
!else
!insertmacro LANGFILE "Norwegian" = "Norsk" =
!endif

; Startup
${LangFileString} Installer_Is_Running "Installeringsveiviseren kjører allerede."
${LangFileString} Installer_No_Admin "Du må være logget inn som administrator når du installerer dette programmet."
${LangFileString} SMPlayer_Is_Running "En instans av SMPlayer kjører allerede. Vennligst lukk SMPlayer og prøv igjen."

${LangFileString} OS_Not_Supported "Ditt operativsystem støttes ikke.$\nSMPlayer ${SMPLAYER_VERSION} krever minst Windows XP og vil kanskje ikke fungere riktig på din maskin.$\nVil du virkelig fortsette med installasjonen?"
${LangFileString} OS_Not_Supported_VistaRequired "Ditt operativsystem støttes ikke.$\nSMPlayer ${SMPLAYER_VERSION} krever minst Windows Vista og vil kanskje ikke fungere riktig på din maskin.$\nVil du virkelig fortsette med installasjonen?"
${LangFileString} Win64_Required "Et 64-bits Windows-operativsystem er påkrevd for å installere denne programvaren."
${LangFileString} Existing_32bitInst "Det er allerede en 32-bitinstallasjon av SMPlayer her. Du må avinstallere din 32-bit SMPlayer først."
${LangFileString} Existing_64bitInst "Det er allerede en 64-bitinstallasjon av SMPlayer her. Du må avinstallere din 64-bit SMPlayer først."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA)-oppsett"
${LangFileString} WelcomePage_Text "Oppsettet vil lede deg gjennom installasjonen av $(^NameDA).$\r$\n$\r$\nDet er anbefalt at du lukker all SMPlayer-prosesser før du begynner oppsettet. Dette vil gjøre det mulig å oppdatere relevante programfiler uten at du behøver å starte maskinen din på nytt.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Snarveier"
${LangFileString} MPlayerGroupTitle "MPlayer-komponenter"
${LangFileString} MPlayerMPVGroupTitle "Multimediemotor"

${LangFileString} Section_SMPlayer "SMPlayer (påkrevd)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, delte biblioteker og dokumentasjon."

${LangFileString} Section_DesktopShortcut "Skrivebord"
${LangFileString} Section_DesktopShortcut_Desc "Oppretter en snarvei til SMPlayer på skrivebordet."

${LangFileString} Section_StartMenu "Startmeny"
${LangFileString} Section_StartMenu_Desc "Opprett en startmenyoppføring for SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (påkrevd)"
${LangFileString} Section_MPlayer_Desc "MPlayer; påkrevd for avspilling."

${LangFileString} Section_MPlayerCodecs "Binære kodeker"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Valgfrie kodeker for MPlayer. (Internettilkobling er påkrevd for å installere dem)"
!endif

${LangFileString} Section_MPV_Desc "En funksjonsrik avsporing av MPlayer && MPlayer2"

${LangFileString} Section_YTDL "Youtube support (download required)"
${LangFileString} Section_YTDL_Desc "Provides the possibility to play Youtube videos."

${LangFileString} Section_MEncoder_Desc "Et følgesprogram til MPlayer som kan brukes til å omkode eller transformere støttede lyd- eller videostrømmer."

${LangFileString} Section_IconThemes "Ikonstiler"
${LangFileString} Section_IconThemes_Desc "Ekstra ikonstiler for SMPlayer."

${LangFileString} Section_Translations "Språk"
${LangFileString} Section_Translations_Desc "Språkfiler for SMPlayer for andre språk enn engelsk."

${LangFileString} Section_ResetSettings_Desc "Sletter SMPlayer-innstillinger fra tidligere installasjoner."

${LangFileString} MPlayer_Codec_Msg "De binære kodekpakkene legger til støtte for kodeker som ennå ikke er blitt innebygd, slik som nyere RealVideo-varianter og mange uvanlige formater.$\nVær oppmerksom på at de ikke er nødvendige for å spille av de fleste vanlige formater som DVDer, MPEG-1/2/4, osv."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Velg installeringstype"
${LangFileString} Reinstall_Header_SubText "Velg Overskrivings- eller Avinstallerings-modus."

${LangFileString} Reinstall_Msg1 "Du har en eksisterende installasjon av SMPlayer i den følgende mappen:"
${LangFileString} Reinstall_Msg2 "Vennligst velg hvordan du vil fortsette:"
${LangFileString} Reinstall_Overwrite "Skriv over ($Inst_Type) den nåværende installasjonen"
${LangFileString} Reinstall_Uninstall "Avinstaller (fjern) den nåværende installasjonen"
${LangFileString} Reinstall_Msg3_1 "Trykk Start når du ønsker å fortsette."
${LangFileString} Reinstall_Msg3_2 "Trykk neste når du ønsker å fortsette."
${LangFileString} Reinstall_Msg3_3 "Trykk Avinstaller når du ønsker å fortsette"
${LangFileString} Reinstall_Msg4 "Endre installasjonsinnstillinger"
${LangFileString} Reinstall_Msg5 "Tilbakestill mitt SMPlayer-oppsett"

${LangFileString} Remove_Settings_Confirmation "Er du sikker på at du vil tilbakestille ditt SMPlayer-oppsett? Denne handlingen kan ikke bli gjenopprettet senere."

${LangFileString} Type_Reinstall "installer på nytt"
${LangFileString} Type_Downgrade "nedgrader"
${LangFileString} Type_Upgrade "oppgrader"

${LangFileString} StartBtn "Start"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Laster ned MPlayer-kodeker..."
${LangFileString} Codecs_DL_Retry "MPlayer-kodeker ble ikke riktig installert. Vil du prøve på nytt?"
${LangFileString} Codecs_DL_Failed "Klarte ikke å laste ned MPlayer-kodeker: '$R0'."
${LangFileString} Codecs_Inst_Failed "Klarte ikke å installere MPlayer-kodeker."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Denne installasjonen kan bare avinstalleres av en bruker med administratorrettigheter."
${LangFileString} Uninstaller_Aborted "Avinstalleringen ble avbrutt av brukeren."
${LangFileString} Uninstaller_NotInstalled "Det ser ikke ut til at SMPlayer er installert under filplasseringen '$INSTDIR'.$\r$\nVil du fortsette likevel (ikke anbefalt)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer-installasjonen ble ikke funnet."
${LangFileString} Uninstaller_64bitOnly "Denne installasjonen kan kun avinstalleres på en 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer er et komplett grensesnitt for MPlayer, fra grunnleggende funksjoner som å spille av videoer, DVDer og VCDer, til mer avanserte funksjoner som støtte for MPlayer-filtre, EDL-lister og mer til."

; Misc
${LangFileString} Info_Codecs_Backup "Tar backup av kodeker fra en tidligere installasjon..."
${LangFileString} Info_Codecs_Restore "Tilbakestiller kodeker fra en tidligere installasjon..."
${LangFileString} Info_Del_Files "Sletter filer..."
${LangFileString} Info_Del_Registry "Sletter registernøkler..."
${LangFileString} Info_Del_Shortcuts "Sletter snarveier..."
${LangFileString} Info_Rest_Assoc "Gjenoppretter filtilknytninger..."
${LangFileString} Info_RollBack "Tilbakestiller endringer..."
${LangFileString} Info_Files_Extract "Pakker ut filer..."
${LangFileString} Info_SMTube_Backup "Tar backup av SMTube..."
${LangFileString} Info_SMTube_Restore "Gjenoppretter SMTube fra en tidligere installasjon..."
${LangFileString} Info_MPV_Backup "Tar backup av mpv..."
${LangFileString} Info_MPV_Restore "Gjenoppretter mpv fra en tidligere installasjon..."

; MPV
${LangFileString} MPV_DL_Msg "Laster ned mpv..."
${LangFileString} MPV_DL_Retry "mpv ble ikke riktig installert. Vil du prøve på nytt?"
${LangFileString} MPV_DL_Failed "Mislyktes i å installere mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Mislyktes i å installere mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl ble ikke riktig installert. Vil du prøve på nytt?"
${LangFileString} YTDL_DL_Failed "Klarte ikke å laste ned youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Leter etter oppdateringer for youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Fjerner mellomlageret for fontoppsett..."
${LangFileString} Info_Cleaning_SMPlayer "Fjerner SMPlayer-innstillinger..."
