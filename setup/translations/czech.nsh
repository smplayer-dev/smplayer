;Language: Czech (1029)
;Czech language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Czech" "Čeština"

; Startup
${LangFileString} Installer_Is_Running "Instalátor již běží."
${LangFileString} Installer_No_Admin "Instalaci tohoto programu je potřeba provést s právy administrátora."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "Zástupci"
${LangFileString} MPlayerGroupTitle "Součásti MPlayeru"

${LangFileString} Section_SMPlayer "SMPlayer (vyžadován)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, sdílené knihovny a dokumentace."

${LangFileString} Section_DesktopShortcut "Plocha"
${LangFileString} Section_DesktopShortcut_Desc "Vytvoří zástupce SMPlayeru na ploše."

${LangFileString} Section_StartMenu "Nabídka Start"
${LangFileString} Section_StartMenu_Desc "Vytvoří zástupce SMPlayeru v nabídce Start."

${LangFileString} Section_MPlayer "MPlayer (vyžadován)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; vyžadován pro přehrávání."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; vyžadován pro přehrávání. (Pro instalaci je potřeba připojení k Internetu)"
!endif

${LangFileString} Section_MPlayerCodecs "Binární kodeky"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Přídavné kodeky MPlayeru."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Přídavné kodeky MPlayeru. (Pro instalaci je potřeba připojení k Internetu)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Témata ikon"
${LangFileString} Section_IconThemes_Desc "Přídavná témata ikon pro SMPlayer."

${LangFileString} Section_Translations "Lokalizace"
${LangFileString} Section_Translations_Desc "Neanglické lokalizace."

${LangFileString} MPlayer_Codec_Msg "Binární kodeky podporují formáty, které zatím nejsou implementovány nativně, např. novější varianty RealVideo a jiné málo používané formáty.$\nPro většinu běžných formátů nejsou potřeba (DVD, MPEG-1/2/4, apod.)."

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
${LangFileString} MPlayer_DL_Msg "Stahuji MPlayer..."
${LangFileString} MPlayer_DL_Retry "MPlayer se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} MPlayer_DL_Failed "Nepovedlo se stáhnout MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Nepovedlo se nainstalovat MPlayer. MPlayer je potřebný pro přehrávání."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Instaluji kodeky MPlayeru..."
${LangFileString} Codecs_DL_Retry "Kodeky MPlayeru se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} Codecs_DL_Failed "Nepovedlo se stáhnout kodeky MPlayeru: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
${LangFileString} VerInfo_DL_Msg "Stahuji informace o verzích..."
${LangFileString} VerInfo_DL_Failed "Nepovedlo se stáhnout informace o verzích: '$R0'. Užita výchozí verze."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Odinstalaci je potřeba provést s právy administrátora."
${LangFileString} Uninstaller_Aborted "Odinstalace přerušena uživatelem."
${LangFileString} Uninstaller_NotInstalled "V adresáři '$INSTDIR' není SMPlayer nainstalován .$\r$\nPokračovat (nedoporučeno)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer je kompletní frontend pro MPlayer, podporuje vše od základních funkcí jako přehrávání videí, DVD, VCD až po pokročilé funkce jako filtry MPlayeru, edl a více."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Extracting files..."
