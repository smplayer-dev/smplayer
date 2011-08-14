;Language: Czech (1029)
;Czech language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Czech" "Čeština"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Instalátor již běží."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Instalaci tohoto programu je potřeba provést s právy administrátora."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (vyžadován)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, sdílené knihovny a dokumentace."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Zástupci"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Plocha"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Vytvoří zástupce SMPlayeru na ploše."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Nabídka Start"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Vytvoří zástupce SMPlayeru v nabídce Start."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "Součásti MPlayeru"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (vyžadován)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; vyžadován pro přehrávání."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; vyžadován pro přehrávání. (Pro instalaci je potřeba připojení k Internetu)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binární kodeky"
${LangFileString} SMPLAYER_SECCODECS_DESC "Přídavné kodeky MPlayeru. (Pro instalaci je potřeba připojení k Internetu)"
${LangFileString} SMPLAYER_SECCODECS_DESC_2 "Přídavné kodeky MPlayeru."

${LangFileString} SMPLAYER_SECMENCODER_DESC "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Témata ikon"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Přídavná témata ikon pro SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Lokalizace"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Neanglické lokalizace."

${LangFileString} MPLAYER_CODEC_INFORMATION "Binární kodeky podporují formáty, které zatím nejsou implementovány nativně, např. novější varianty RealVideo a jiné málo používané formáty.$\nPro většinu běžných formátů nejsou potřeba (DVD, MPEG-1/2/4, apod.)."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Již nainstalováno"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Zvolte si, jak chce SMPlayer nainstalovat."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Zvolte metodu údržby."

${LangFileString} REINSTALL_CHGSETTINGS "Změna nastavení (pokročilé)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Neznámá hodnota PREVIOUS_VERSION_STATE, ukončuji"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Na vašem systému je nainstalována starší verze SMPlayeru. Vyberte jednu z možností a stikněte Další."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Provést upgrade pomocí předchozích nastavení (doporučeno)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Máte nainstalovánu novější verzi SMPlayeru! Downgrade na starší verzi se nedoporučuje. Vyberte jednu z možností a stikněte Další."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Provést downgrade pomocí předchozích nastavení (doporučeno)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer verze ${SMPLAYER_VERSION} je již nainstalován. Vyberte jednu z možností a stikněte Další."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Přidat/Odstranit/reinstalovat součásti"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Odinstalovat SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Stahuji MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} MPLAYER_DL_FAILED "Nepovedlo se stáhnout MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Nepovedlo se nainstalovat MPlayer. MPlayer je potřebný pro přehrávání."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Instaluji kodeky MPlayeru..."
${LangFileString} CODECS_DL_RETRY "Kodeky MPlayeru se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} CODECS_DL_FAILED "Nepovedlo se stáhnout kodeky MPlayeru: '$R0'."
${LangFileString} CODECS_INST_FAILED "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Stahuji informace o verzích..."
${LangFileString} VERINFO_DL_FAILED "Nepovedlo se stáhnout informace o verzích: '$R0'. Užita výchozí verze."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Odinstalaci je potřeba provést s právy administrátora."
${LangFileString} UNINSTALL_ABORTED "Odinstalace přerušena uživatelem."
${LangFileString} SMPLAYER_NOT_INSTALLED "V adresáři '$INSTDIR' není SMPlayer nainstalován .$\r$\nPokračovat (nedoporučeno)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer je kompletní frontend pro MPlayer, podporuje vše od základních funkcí jako přehrávání videí, DVD, VCD až po pokročilé funkce jako filtry MPlayeru, edl a více."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
