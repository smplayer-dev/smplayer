;Language: Serbian (3098)
;Serbian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Serbian" "Српски"

; Startup
${LangFileString} Installer_Is_Running "Инсталациони програм је већ покренут."
${LangFileString} Installer_No_Admin "Морате бити пријављени као администратор када инсталирате овај програм."
${LangFileString} SMPlayer_Is_Running "СМПлејер је већ покренут. Затворите СМПлејер и покушајте поново."

${LangFileString} OS_Not_Supported "Неподржан оперативни систем.$\nСМПлејер ${SMPLAYER_VERSION} захтева најмање Windows XP и можда неће радити на вашем систему.$\nДа ли стварно желите наставити инсталацију?"
${LangFileString} Win64_Required "64-битни Windows оперативни систем је потребан за инсталацију овог програма."
${LangFileString} Existing_32bitInst "Тренутно је инсталирана 32-битна верзија СМПлејера. Морате прво деинсталирати 32-битни СМПлејер."
${LangFileString} Existing_64bitInst "Тренутно је инсталирана 64-битна верзија СМПлејера. Морате прво деинсталирати 64-битни СМПлејер."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) програм инсталације"
${LangFileString} WelcomePage_Text "Програм инсталације ће вас водити кроз инсталацију $(^NameDA).$\r$\n$\r$\nПрепорука је да затворите претходно отворени СМПлејер пре почетка инсталације. То ће омогућити ажурирање битних фајлова без потребе за поновним покретањем вашег рачунара.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Пречице"
${LangFileString} MPlayerGroupTitle "МПлејер компоненте"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "СМПлејер (потребно)"
${LangFileString} Section_SMPlayer_Desc "СМПлејер, заједничке библиотеке и документација."

${LangFileString} Section_DesktopShortcut "Радна површина"
${LangFileString} Section_DesktopShortcut_Desc "Створи пречицу СМПлејера на радној површини."

${LangFileString} Section_StartMenu "Основни мени"
${LangFileString} Section_StartMenu_Desc "Створи ставку у менију за СМПлејер."

${LangFileString} Section_MPlayer "МПлејер (потребно)"
${LangFileString} Section_MPlayer_Desc "МПлејер; потребно за репродукцију."

${LangFileString} Section_MPlayerCodecs "Бинарни кодеци"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Бинарни кодеци нису подржани у овој верзији."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Опциони кодеци за МПлејер. (Интернет веза је потребна за инсталацију)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Пратећи програм за МПлејер који се може користити за енкодовање или трансформацију подржаних звучних или видео стримова."

${LangFileString} Section_IconThemes "Сетови икона"
${LangFileString} Section_IconThemes_Desc "Додатни сетови икона за СМПлејер."

${LangFileString} Section_Translations "Језици"
${LangFileString} Section_Translations_Desc "Не-енглески језички фајлови за СМПлејер."

${LangFileString} Section_ResetSettings_Desc "Уклања СМПлејер подешавања заостала од претходних инсталација."

${LangFileString} MPlayer_Codec_Msg "Пакет бинарних кодека даје подршку за кодеке који још нису изворно уграђени, попут новијих RealVideo верзија и још много неуобичајених формата.$\nИмајте на уму да нужно не морају репродуковати уобичајене формате попут ДВД-a, МПЕГ-1/2/4, итд."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Одаберите врсту инсталације"
${LangFileString} Reinstall_Header_SubText "Одаберите начин преписивања или деинсталације."

${LangFileString} Reinstall_Msg1 "Откривене је постојећа инсталација СМПлејера у следећој фасцикли:"
${LangFileString} Reinstall_Msg2 "Одаберите наставак инсталације:"
${LangFileString} Reinstall_Overwrite "Препиши ($Inst_Type) преко постојеће инсталације"
${LangFileString} Reinstall_Uninstall "Деинсталирај (уклони) постојећу инсталацију"
${LangFileString} Reinstall_Msg3_1 "Кликните Покрени када сте спремни за наставак."
${LangFileString} Reinstall_Msg3_2 "Кликните Следеће када сте спремни за наставак."
${LangFileString} Reinstall_Msg3_3 "Кликните Деинсталирај када сте спремни за наставак."
${LangFileString} Reinstall_Msg4 "Промени инсталациона подешавања"
${LangFileString} Reinstall_Msg5 "Врати на почетна СМПлејер подешавања"

${LangFileString} Type_Reinstall "реинсталирај"
${LangFileString} Type_Downgrade "врати на старију верзију"
${LangFileString} Type_Upgrade "надогради"

${LangFileString} StartBtn "Покрени"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Преузимање МПлејер кодека..."
${LangFileString} Codecs_DL_Retry "МПлејер кодеци нису успешно инсталирани. Покушајте поново?"
${LangFileString} Codecs_DL_Failed "Неуспешно преузимање МПлејер кодека: '$R0'."
${LangFileString} Codecs_Inst_Failed "Неуспела инсталација МПлејер кодека."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Ову инсталацију може деинсталирати само корисник са админ овлашћењима."
${LangFileString} Uninstaller_Aborted "Деинсталацију прекинуо корисник."
${LangFileString} Uninstaller_NotInstalled "Чини се да СМПлејер није инсталиран у фасцикли '$INSTDIR'.$\r$\nИпак настављате (није препоручљиво)?"
${LangFileString} Uninstaller_InvalidDirectory "СМПлејер инсталација није пронађена."
${LangFileString} Uninstaller_64bitOnly "Ова инсталације може бити деинсталирана само на 64-битном Windowsu."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "СМПлејер је комплетно сучеље за МПлејер, са основним функцијама попут репродукције видео снимака, ДВД-а,ВЦД-а, до напреднијих као што су подршке за МПлејер филтере, едл листе и још много тога."

; Misc
${LangFileString} Info_Codecs_Backup "Стварање сигурносне копије кодека од претходних инсталација..."
${LangFileString} Info_Codecs_Restore "Враћање копије кодека од претходне инсталације..."
${LangFileString} Info_Del_Files "Уклањање фајлова..."
${LangFileString} Info_Del_Registry "Уклањање кључева регистра..."
${LangFileString} Info_Del_Shortcuts "Уклањање пречица..."
${LangFileString} Info_Rest_Assoc "Враћање на почетно придруживање фајлова..."
${LangFileString} Info_RollBack "Враћање промена..."
${LangFileString} Info_Files_Extract "Распакивање фајлова..."
${LangFileString} Info_SMTube_Backup "Backing up SMTube..."
${LangFileString} Info_SMTube_Restore "Restoring SMTube from previous installation..."
${LangFileString} Info_MPV_Backup "Backing up MPV..."
${LangFileString} Info_MPV_Restore "Restoring MPV from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Downloading MPV..."
${LangFileString} MPV_DL_Retry "MPV was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download MPV: '$R0'."
${LangFileString} MPV_Inst_Failed "Failed to install MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download Youtube-DL: '$R0'."
