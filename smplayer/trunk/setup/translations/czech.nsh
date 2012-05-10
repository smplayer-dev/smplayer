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

${LangFileString} Win64_Required "Pro instalaci tohoto software je zapotřebí mít 64-bitový oprační systém Windows."
${LangFileString} Existing_32bitInst "Máte nainstalovánu 32-bitovou verzi SMPlayeru. Musíte nejprve odinstalovat 32-bitový SMPlayer."
${LangFileString} Existing_64bitInst "Máte nainstalovánu 64-bitovou verzi SMPlayeru. Musíte nejprve odinstalovat 64-bitový SMPlayer."

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

${LangFileString} Section_MEncoder_Desc "Dodatkový program pro MPlayer, který se využívá k enkódování nebo transformaci audio/video souborů."

${LangFileString} Section_IconThemes "Témata ikon"
${LangFileString} Section_IconThemes_Desc "Přídavná témata ikon pro SMPlayer."

${LangFileString} Section_Translations "Lokalizace"
${LangFileString} Section_Translations_Desc "Neanglické lokalizace."

${LangFileString} MPlayer_Codec_Msg "Binární kodeky podporují formáty, které zatím nejsou implementovány nativně, např. novější varianty RealVideo a jiné málo používané formáty.$\nPro většinu běžných formátů nejsou potřeba (DVD, MPEG-1/2/4, apod.)."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Vyberte typ instalace"
${LangFileString} Reinstall_Header_SubText "Vyberte přepsání nebo odinstalaci."

${LangFileString} Reinstall_Msg1 "SMPlayer již máte nainstalován v tomto adresáři:"
${LangFileString} Reinstall_Msg2 "Prosím vyberte jak pokračovat:"
${LangFileString} Reinstall_Overwrite "Přepsat ($Inst_Type) existující instalaci"
${LangFileString} Reinstall_Uninstall "Odinstalovat (odebrat) existující instalaci"
${LangFileString} Reinstall_Msg3_1 "Stiskněte Start jste-li připraveni."
${LangFileString} Reinstall_Msg3_2 "Stiskněte Další jste-li připraveni."
${LangFileString} Reinstall_Msg3_3 "Stiskněte Odinstalovat jste-li připraveni."
${LangFileString} Reinstall_Msg4 "Změnit nastavení instalace"

${LangFileString} Type_Reinstall "reinstalovat"
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
${LangFileString} Uninstaller_64bitOnly "Tato instalace jde odinstalovat pouze na 64-bitové verzi Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer je kompletní frontend pro MPlayer, podporuje vše od základních funkcí jako přehrávání videí, DVD, VCD až po pokročilé funkce jako filtry MPlayeru, edl a více."

; Misc
${LangFileString} Info_Del_Files "Mažu soubory..."
${LangFileString} Info_Del_Registry "Mažu záznamy registru..."
${LangFileString} Info_Del_Shortcuts "Mažu zástupce..."
${LangFileString} Info_Rest_Assoc "Obnovuji asociace souborů..."
${LangFileString} Info_RollBack "Beru zpět změny..."
${LangFileString} Info_Files_Extract "Extrahuji soubory..."
