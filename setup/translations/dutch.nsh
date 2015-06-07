;Language: Dutch (1043)
;Dutch language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Dutch" "Nederlands"

; Startup
${LangFileString} Installer_Is_Running "Het installatiepakket draait al."
${LangFileString} Installer_No_Admin "U moet ingelogd zijn als beheerder tijdens het installeren van het programma."
${LangFileString} SMPlayer_Is_Running "Een venster van SMPlayer is draaiende. Sluit SMPlayer en probeer het opnieuw."

${LangFileString} OS_Not_Supported "Niet-ondersteund besturingssysteem.$\nSMPlayer ${SMPLAYER_VERSION} vereist minimaal Windows XP en zal mogelijk niet juist werken op uw systeem.$\nWeet u zeker dat u de installatie wilt voortzetten?"
${LangFileString} Win64_Required "Een 64-bits-versie van Windows is benodigd om deze software te installeren."
${LangFileString} Existing_32bitInst "Een bestaande 32-bits-installatie van SMPlayer bestaat reeds. U moet deze eerst deïnstalleren."
${LangFileString} Existing_64bitInst "Een bestaande 64-bits-installatie van SMPlayer bestaat reeds. U moet deze eerst deïnstalleren."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Installatie"
${LangFileString} WelcomePage_Text "Setup zal u door de installatie van $(^NameDA) begeleiden.$\r$\n$\r$\nHet wordt aanbevolen dat u alle instanties van SMPlayer sluit voordat u de setup start. Dit maakt het mogelijk om relevante programmabestanden te updaten zonder uw computer te herstarten.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Sneltoetsen"
${LangFileString} MPlayerGroupTitle "MPlayer-componenten"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "SMPlayer (vereist)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, gedeelde bibliotheken en documentatie."

${LangFileString} Section_DesktopShortcut "Bureablad"
${LangFileString} Section_DesktopShortcut_Desc "Maakt een snelkoppeling van SMPlayer aan op het bureaublad."

${LangFileString} Section_StartMenu "Start-menu"
${LangFileString} Section_StartMenu_Desc "Maakt een Start-menu-verwijzing aan voor SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (vereist)"
${LangFileString} Section_MPlayer_Desc "MPlayer; vereist voor afspelen."

${LangFileString} Section_MPlayerCodecs "Binary-codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binaire codecs worden niet ondersteund in deze versie."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Optionele codecs voor MPlayer. (Internetverbinding vereist voor installatie)"
!endif

${LangFileString} Section_MPV_Desc "Een rijke functie code kopie van MPlayer en MPlayer2"

${LangFileString} Section_MEncoder_Desc "Een metgezel programma voor MPlayer dat gebruikt kan worden om ondersteunende audio of video streams te coderen of transformeren."

${LangFileString} Section_IconThemes "Pictogram-thema's"
${LangFileString} Section_IconThemes_Desc "Extra pictogramthema's voor SMPlayer."

${LangFileString} Section_Translations "Talen"
${LangFileString} Section_Translations_Desc "Niet-Engelse taalbestanden voor SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Verwijdert SMPlayer-voorkeuren die overgebleven zijn van vorige installaties."

${LangFileString} MPlayer_Codec_Msg "De binaire codec pakketten voegen ondersteuning toe voor codecs die nog niet zijn geïmplementeerd, zoals de nieuwere RealVideo varianten en veel ongewone formaten.$\nDeze zijn niet nodig om de meest gangbare formaten af te spelen zoals DVDs, MPEG-1/2/4, enzovoorts."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Installatietype selecteren"
${LangFileString} Reinstall_Header_SubText "Overschrijven- of Deïnstallatie-modus selecteren"

${LangFileString} Reinstall_Msg1 "U heeft een bestaande installatie van SMPlayer in de volgende map:"
${LangFileString} Reinstall_Msg2 "Selecteer hoe verder te verder te gaan:"
${LangFileString} Reinstall_Overwrite "Overschrijf ($Inst_Type) de huidige installatie"
${LangFileString} Reinstall_Uninstall "Deïnstalleer (verwijder) de huidige installatie"
${LangFileString} Reinstall_Msg3_1 "Klik op Beginnen wanneer u klaar bent om door te gaan."
${LangFileString} Reinstall_Msg3_2 "Klik op Volgende wanneer u klaar bent om door te gaan."
${LangFileString} Reinstall_Msg3_3 "Klik op Deïnstalleren wanneer u klaar bent om door te gaan."
${LangFileString} Reinstall_Msg4 "Installatie-voorkeuren wijzigen"
${LangFileString} Reinstall_Msg5 "Standaardwaarden voor mijn SMPlayer-configuratie"

${LangFileString} Type_Reinstall "herinstalleren"
${LangFileString} Type_Downgrade "versie verlagen"
${LangFileString} Type_Upgrade "versie opwaarderen"

${LangFileString} StartBtn "Beginnen"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Downloaden van MPlayer-codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer-codecs waren niet succesvol geïnstalleerd. Opnieuw proberen?"
${LangFileString} Codecs_DL_Failed "Mislukt om MPlayer-codecs te downloaden: '$R0'."
${LangFileString} Codecs_Inst_Failed "Mislukt om MPlayer-codecs te installeren."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Deze installatie kan alleen worden verwijderd door een gebruiker met administratie bevoegdheden."
${LangFileString} Uninstaller_Aborted "Installatie geannuleerd door gebruiker."
${LangFileString} Uninstaller_NotInstalled "Het ziet ernaar uit dat SMPlayer niet is geïnstalleerd in de directory '$INSTDIR'.$\r$\nWilt u toch doorgaan (niet aanbevolen)?"
${LangFileString} Uninstaller_InvalidDirectory "Kan SMPlayer installatie niet vinden."
${LangFileString} Uninstaller_64bitOnly "Deze installatie kan alleen worden verwijderd op 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is een complete voorkant voor MPlayer, van basis functies zoals  het afspelen van videos, DVDs, VCDS tot meer geavanceerde functies zoals ondersteuning voor Mplayer filters, edl lijsten, en meer."

; Misc
${LangFileString} Info_Codecs_Backup "Reservekopie maken van codecs uit vorige installatie..."
${LangFileString} Info_Codecs_Restore "Herstellen van codecs uit vorige installatie..."
${LangFileString} Info_Del_Files "Verwijderen van bestanden..."
${LangFileString} Info_Del_Registry "Registersleutel verwijderen..."
${LangFileString} Info_Del_Shortcuts "Snelkoppelingen verwijderen..."
${LangFileString} Info_Rest_Assoc "Herstellen van bestandskoppelingen..."
${LangFileString} Info_RollBack "Veranderingen ongedaan maken..."
${LangFileString} Info_Files_Extract "Uitpakken van bestanden..."
${LangFileString} Info_SMTube_Backup "Back-up SMTube..."
${LangFileString} Info_SMTube_Restore "Herstel SMTube van de oude installatie..."
${LangFileString} Info_MPV_Backup "Bak-up MPV..."
${LangFileString} Info_MPV_Restore "Herstel MPV van de oude installatie..."

; MPV
${LangFileString} MPV_DL_Msg "Download MPV..."
${LangFileString} MPV_DL_Retry "MPV was niet succesvol geïnstalleerd. Opnieuw?"
${LangFileString} MPV_DL_Failed "De download van MPV is mislukt: '$R0'."
${LangFileString} MPV_Inst_Failed "De installatie van MPV is mislukt."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL was niet succesvol geïnstalleerd. Opniew?"
${LangFileString} YTDL_DL_Failed "De download van Youtube-DL is mislukt: '$R0'."
