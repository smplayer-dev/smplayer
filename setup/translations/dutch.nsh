;Language: Dutch (1043)
;Dutch language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Dutch" "Nederlands"

; Startup
${LangFileString} Installer_Is_Running "Het installatiepakket draait al."
${LangFileString} Installer_No_Admin "U moet ingelogd zijn als een administrator tijdens het installeren van het programma."
${LangFileString} SMPlayer_Is_Running "Een venster van SMPlayer is draaiende. Sluit alstublieft SMPlayer af en probeer het opnieuw."

${LangFileString} OS_Not_Supported "Niet-ondersteund besturingssysteem.$\nSMPlayer ${SMPLAYER_VERSION} vereist minimaal Windows XP en zal mogelijk niet juist werken op uw systeem.$\nWeet u zeker dat u de installatie wilt voortzetten?"
${LangFileString} Win64_Required "Een 64-bits-versie van Windows is benodigd om deze software te installeren."
${LangFileString} Existing_32bitInst "Een bestaande 32-bits-installatie van SMPlayer bestaat reeds. U moet deze eerst deïnstalleren."
${LangFileString} Existing_64bitInst "Een bestaande 64-bits-installatie van SMPlayer bestaat reeds. U moet deze eerst deïnstalleren."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Installatie"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Sneltoetsen"
${LangFileString} MPlayerGroupTitle "MPlayer-componenten"

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
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Optionele codecs voor MPlayer. (Internetverbinding vereist voor installatie)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Pictogram-thema's"
${LangFileString} Section_IconThemes_Desc "Extra pictogramthema's voor SMPlayer."

${LangFileString} Section_Translations "Talen"
${LangFileString} Section_Translations_Desc "Niet-Engelse taalbestanden voor SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Verwijdert SMPlayer-voorkeuren die overgebleven zijn van vorige installaties."

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

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
${LangFileString} Uninstaller_No_Admin "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} Uninstaller_Aborted "Uninstall aborted by user."
${LangFileString} Uninstaller_NotInstalled "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer installation not found."
${LangFileString} Uninstaller_64bitOnly "This installation can only be uninstalled on 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."

; Misc
${LangFileString} Info_Codecs_Backup "Backing up codecs from previous installation..."
${LangFileString} Info_Codecs_Restore "Restoring codecs from previous installation..."
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Extracting files..."
