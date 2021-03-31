;Language: Slovak (1051)
;Slovak language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Slovak" "Slovenčina"
!else
!insertmacro LANGFILE "Slovak" = "Slovenčina" "Slovencina"
!endif

; Startup
${LangFileString} Installer_Is_Running "Inštalátor je už spustený."
${LangFileString} Installer_No_Admin "Pri inštalovaní tohoto programu musíte byť prihlásení ako administrátor."
${LangFileString} SMPlayer_Is_Running "Je spustená inštancia SMPlayer-u. Ukončite prosím SMPlayer a skúste to znova."

${LangFileString} OS_Not_Supported "Nepodporovaný operačný systém.$\nSMPlayer ${SMPLAYER_VERSION} vyžaduje aspoň systém Windows XP a nemusí fungovať správne vo vašom systéme.$\nNaozaj chcete pokračovať v inštalácii?"
${LangFileString} OS_Not_Supported_VistaRequired "Nepodporovaný operačný systém.$\nSMPlayer ${SMPLAYER_VERSION} vyžaduje aspoň systém Windows Vista a nemusí fungovať správne vo vašom systéme.$\nNaozaj chcete pokračovať v inštalácii?"
${LangFileString} Win64_Required "Na inštaláciu tohto softvéru je potrebný 64-bitový operačný systém Windows."
${LangFileString} Existing_32bitInst "Existuje 32-bitová inštalácia SMPlayer. Najskôr musíte odinštalovať 32-bitový SMPlayer."
${LangFileString} Existing_64bitInst "Existuje 64-bitová inštalácia SMPlayer. Najskôr musíte odinštalovať 64-bitový SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Nastavenie"
${LangFileString} WelcomePage_Text "Nastavenie vás prevedie inštaláciou $(^NameDA).$\r$\n$\r$\nPred spustením inštalácie sa odporúča zavrieť všetky inštancie zariadenia SMPlayer. To umožní aktualizovať príslušné programové súbory bez toho, aby ste museli reštartovať počítač.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Odkazy"
${LangFileString} MPlayerGroupTitle "MPlayer komponenty"
${LangFileString} MPlayerMPVGroupTitle "Multimediálny engine"

${LangFileString} Section_SMPlayer "SMPlayer (nutné)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, zdieľané knižnice a dokumentácia."

${LangFileString} Section_DesktopShortcut "Plocha"
${LangFileString} Section_DesktopShortcut_Desc "Vytvorí odkaz SMPlayer-u na ploche."

${LangFileString} Section_StartMenu "Štart menu"
${LangFileString} Section_StartMenu_Desc "Vytvorí odkaz SMPlayer-u v menu Štart"

${LangFileString} Section_MPlayer "MPlayer (nutné)"
${LangFileString} Section_MPlayer_Desc "MPlayer; nevyhnutné pre prehrávanie"

${LangFileString} Section_MPlayerCodecs "Binárne kodeky"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binárne kodeky nie sú v tejto verzii podporované."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Voliteľné kodeky pre MPlayer. (inštalácia vyžaduje internetové pripojenie)"
!endif

${LangFileString} Section_MPV_Desc "Bohaté funkcie MPlayer && MPlayer2"

${LangFileString} Section_YTDL "Youtube support"
${LangFileString} Section_YTDL_Desc "Provides the possibility to play Youtube videos."

${LangFileString} Section_MEncoder_Desc "Prídavný program k Mplayer-u, ktorý umožňuje enkódovať alebo upraviť podporované audio a video streamy."

${LangFileString} Section_IconThemes "Témy ikon"
${LangFileString} Section_IconThemes_Desc "Prídavné témy ikon pre SMPlayer."

${LangFileString} Section_Translations "Jazyky"
${LangFileString} Section_Translations_Desc "Neanglické jazykové súbory pre SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Vymazať nastavenia SMPlayer-u, kt. zanechali predchádzajúce inštalácie."

${LangFileString} MPlayer_Codec_Msg "Balíky binárnych kodekov pridávajú podporu pre kodeky, ktoré ešte nie sú implementované natívne, ako sú novšie varianty RealVideo a veľa neobvyklých formátov. $\nVšimnite si, že nie je potrebné prehrávať väčšinu bežných formátov, ako sú DVD, MPEG-1/2/4 atď."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Vyberte typ inštalácie"
${LangFileString} Reinstall_Header_SubText "Vyberte možnosť Prepísať alebo Odinštalovať."

${LangFileString} Reinstall_Msg1 "Už máte existujúcu inštaláciu SMPlayer-u v priečinku:"
${LangFileString} Reinstall_Msg2 "Prosím vyberte, ako chcete pokračovať:"
${LangFileString} Reinstall_Overwrite "Prepísať ($Inst_Type) existujúcu inštaláciu"
${LangFileString} Reinstall_Uninstall "Odinštalovať (odstrániť) existujúcu inštaláciu"
${LangFileString} Reinstall_Msg3_1 "Kliknite na Spustiť, keď budete pripravený pokračovať."
${LangFileString} Reinstall_Msg3_2 "Kliknite na Ďalej, keď budete pripravený pokračovať."
${LangFileString} Reinstall_Msg3_3 "Kliknite na Odinštalovať, keď budete pripravený pokračovať."
${LangFileString} Reinstall_Msg4 "Zmeniť nastavenia inštalácie"
${LangFileString} Reinstall_Msg5 "Zresetovať konfiguráciu SMPlayeru"

${LangFileString} Remove_Settings_Confirmation "Naozaj chcete obnoviť nastavenia SMPlayer? Táto akcia sa nedá zvrátiť."

${LangFileString} Type_Reinstall "preinštalovať"
${LangFileString} Type_Downgrade "nahradiť staršou verziou"
${LangFileString} Type_Upgrade "aktualizovať"

${LangFileString} StartBtn "Spustiť"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Sťahujú sa kodeky MPlayer-u..."
${LangFileString} Codecs_DL_Retry "MPlayer kodeky neboli úspešné nainštalované. Znovu?"
${LangFileString} Codecs_DL_Failed "Nepodarilo sa stiahnuť kodek MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nepodarilo sa nainštalovať kodek MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Táto inštalácia môže byť odinštalovaná iba používateľom s oprávneniami správcu."
${LangFileString} Uninstaller_Aborted "Odinštalovanie zrušené používateľom."
${LangFileString} Uninstaller_NotInstalled "Zdá sa, že SMPlayer nie je nainštalovaný v adresári '$INSTDIR'.$\r$\nPokračovať napriek tomu (neodporúča sa)?"
${LangFileString} Uninstaller_InvalidDirectory "Inštalácia SMPlayer nebola nájdená."
${LangFileString} Uninstaller_64bitOnly "Táto inštalácia môže byť odinštalovaná len v 64-bitovom systéme Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer je kompletný front-end pre MPlayer, od základných vlastností ako prehrávanie videí, DVD, VCD, po viac pokročilé vlastnosti ako MPlayer filtre, edl zoznamy, a viac."

; Misc
${LangFileString} Info_Codecs_Backup "Zálohovanie kodekov z predchádzajúcej inštalácie..."
${LangFileString} Info_Codecs_Restore "Obnovenie kodekov z predchádzajúcej inštalácie..."
${LangFileString} Info_Del_Files "Odstraňujú sa súbory..."
${LangFileString} Info_Del_Registry "Odstraňú sa kľúče databázy Registry..."
${LangFileString} Info_Del_Shortcuts "Odstránenie ikon..."
${LangFileString} Info_Rest_Assoc "Obnovenie priradenie súborov..."
${LangFileString} Info_RollBack "Prebieha vrátenie zmien..."
${LangFileString} Info_Files_Extract "Rozbaľovanie súborov ..."
${LangFileString} Info_SMTube_Backup "Zálohuje sa SMTube..."
${LangFileString} Info_SMTube_Restore "Obnovenie SMTube z predchádzajúcej inštalácie..."
${LangFileString} Info_MPV_Backup "Zálohovanie mpv..."
${LangFileString} Info_MPV_Restore "Obnovenie mpv z predchádzajúcej inštalácie..."

; MPV
${LangFileString} MPV_DL_Msg "Sťahujem mpv..."
${LangFileString} MPV_DL_Retry "mpv sa nepodarilo úspešne nainštalovať. Skúsiť znova?"
${LangFileString} MPV_DL_Failed "Nepodarilo sa stiahnuť mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Nepodarilo sa nainštalovať mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl sa nepodarilo úspešne nainštalovať. Skúsiť znova?"
${LangFileString} YTDL_DL_Failed "Nepodarilo sa prevziať youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Kontrola aktualizácií youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Čistenie fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "Čistenie nastavení SMPlayer..."
