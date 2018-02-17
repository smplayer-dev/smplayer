;Language: Romanian (1048)
;Romanian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Romanian" "Română"
!else
!insertmacro LANGFILE "Romanian" = "Română" "Romana"
!endif

; Startup
${LangFileString} Installer_Is_Running "Instalatorul rulează deja."
${LangFileString} Installer_No_Admin "Trebuie să fiți autentificat ca administrator când instalați acest program."
${LangFileString} SMPlayer_Is_Running "O instanță a SMPlayer rulează. Închideți SMPlayer și încercați din nou."

${LangFileString} OS_Not_Supported "Sistem de operare nesuportat.$\nSMPlayer ${SMPLAYER_VERSION} este necesar cel puțin ultimul Windows XP și poate să nu funcționeze corect pe sistemul dumneavoastră.$\n Sigur doriți să continuați cu instalarea?"
${LangFileString} OS_Not_Supported_VistaRequired "Unsupported operating system.$\nSMPlayer ${SMPLAYER_VERSION} requires at least Windows Vista and may not work correctly on your system.$\nDo you really want to continue with the installation?"
${LangFileString} Win64_Required "Un sistem de operare Windows pe 64 biți este necesar pentru instalarea acestei aplicații."
${LangFileString} Existing_32bitInst "Există o instalare SMPlayer de 32-bit. Trebuie să dezinstalați întâi SMPlayer la 32-bit."
${LangFileString} Existing_64bitInst "Există o instalare SMPlayer de 64-bit. Trebuie să dezinstalați întâi SMPlayer la 64-bit."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Configurare"
${LangFileString} WelcomePage_Text "Configurarea vă va ghida prin instalarea $(^NameDA).$\r$\n$\r$\n este recomandat să închideți toate instanțele de SMPlayer înainte să porniți configurarea. Aceasta va face posibil să actualizați fișierele de program relevante fără să fie necesară repornirea computerului.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Scurtături"
${LangFileString} MPlayerGroupTitle "Componente SMPlayer"
${LangFileString} MPlayerMPVGroupTitle "Motor multimedia"

${LangFileString} Section_SMPlayer "SMPlayer (necesar)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, biblioteci partajate și documentație."

${LangFileString} Section_DesktopShortcut "Birou"
${LangFileString} Section_DesktopShortcut_Desc "Creează o scurtătură pentru SMPlayer pe birou."

${LangFileString} Section_StartMenu "Meniu pornire"
${LangFileString} Section_StartMenu_Desc "Creează o intrare în meniu de pornire pentru SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (necesar)"
${LangFileString} Section_MPlayer_Desc "MPlayer; necesar pentru redare."

${LangFileString} Section_MPlayerCodecs "Codecuri binare"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Codecurile binare nu sunt suportate în această versiune."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codecuri suplimentare pentru MPlayer. (Este necesară o conexiune la internet pentru instalare)"
!endif

${LangFileString} Section_MPV_Desc "O versiune îmbunătățită a MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Un program însoțitor pentru MPlayer care poate fi utilizat să codeze sau să transforme fluxurile audio sau video suportate."

${LangFileString} Section_IconThemes "Tematici pictograme"
${LangFileString} Section_IconThemes_Desc "Tematici pictograme suplimentare pentru SMPlayer."

${LangFileString} Section_Translations "Limbi"
${LangFileString} Section_Translations_Desc "Fișiere fără limba engleză pentru SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Șterge preferințele SMPlayer rămase de la instalările anterioare."

${LangFileString} MPlayer_Codec_Msg "Pachetele de codecuri binare adaugă suport pentru codecurile care încă nu sunt implementate nativ, precum noile variante RealVideo și o mulțime de formate particulare.$\n Rețineți că acestea nu sunt necesare pentru redarea celor mai multor formate precum DVD-uri, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Selectați tipul de instalare"
${LangFileString} Reinstall_Header_SubText "Selectați mod suprascriere sau dezinstalare."

${LangFileString} Reinstall_Msg1 "Există o instalare a SMPlayer în următorul dosar:"
${LangFileString} Reinstall_Msg2 "Selectați cum se procedează:"
${LangFileString} Reinstall_Overwrite "Suprascrie ($Inst_Type) instalarea existentă"
${LangFileString} Reinstall_Uninstall "Dezinstalează (elimină) instalarea existentă"
${LangFileString} Reinstall_Msg3_1 "Apăsați pornire când este gata de continuat."
${LangFileString} Reinstall_Msg3_2 "Apăsați următorul când este gata de continuare."
${LangFileString} Reinstall_Msg3_3 "Apăsați dezinstalează când este gata de continuare."
${LangFileString} Reinstall_Msg4 "Modifică configurările de instalare"
${LangFileString} Reinstall_Msg5 "Resetează configurările mele SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Sigur doriți să resetați setările dumneavoastră pentru SMPlayer? Această acțiune nu poate fi revocată."

${LangFileString} Type_Reinstall "reinstalare"
${LangFileString} Type_Downgrade "versiune anterioară"
${LangFileString} Type_Upgrade "actualizare"

${LangFileString} StartBtn "Pornire"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Se descarcă codecurile MPlayer..."
${LangFileString} Codecs_DL_Retry "Codecurile MPlayer nu au fost instalate cu succes. Se reîncearcă?"
${LangFileString} Codecs_DL_Failed "A eșuat descărcarea codecurilor MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "A eșuat instalarea codecurilor MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Această instalare poate fi dezinstalată numai de un utilizator cu drepturi de administrator."
${LangFileString} Uninstaller_Aborted "Dezinstalarea abandonată de utilizator."
${LangFileString} Uninstaller_NotInstalled "Nu apare că SMPlayer este instalat în dosarul '$INSTDIR'.$\r$\n Se continuă oricum (nerecomandat)?"
${LangFileString} Uninstaller_InvalidDirectory "Nu a fost găsită instalarea SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Această instalare poate fi dezinstalată numai pe Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer este o interfață completă pentru MPlayer, de la funcțiile de bază precum redarea video, DVD-uri, VCD-uri la mai multe funcții avansate precum suportul pentru filtrele MPlayer, liste edl și mai multe."

; Misc
${LangFileString} Info_Codecs_Backup "Se recuperează codecurile din instalarea anterioară..."
${LangFileString} Info_Codecs_Restore "Se restaurează codecurile din instalarea anterioară..."
${LangFileString} Info_Del_Files "Se șterg fișiere..."
${LangFileString} Info_Del_Registry "Se șterg cheile regiștrilor..."
${LangFileString} Info_Del_Shortcuts "Se șterg scurtăturile..."
${LangFileString} Info_Rest_Assoc "Se restaurează asocierea fișierelor..."
${LangFileString} Info_RollBack "Se reiau modificările..."
${LangFileString} Info_Files_Extract "Se extrag fișierele..."
${LangFileString} Info_SMTube_Backup "Se recuperează SMTube..."
${LangFileString} Info_SMTube_Restore "Se restaurează SMTube din instalarea anterioară..."
${LangFileString} Info_MPV_Backup "Backing up mpv..."
${LangFileString} Info_MPV_Restore "Restoring mpv from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Se descarcă mpv..."
${LangFileString} MPV_DL_Retry "mpv was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "A eșuat instalarea mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Checking for youtube-dl updates..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Cleaning fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "Cleaning SMPlayer settings..."
