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

${LangFileString} OS_Not_Supported "Неподржан оперативни систем.$\nСМПлејер ${SMPLAYER_VERSION} захтева најмање Windows XP и можда неће радити на вашем систему.$\nДа ли стварно желите да наставите инсталацију?"
${LangFileString} Win64_Required "64-битни Windows оперативни систем је потребан за инсталацију овог програма."
${LangFileString} Existing_32bitInst "Тренутно је инсталирана 32-битна верзија СМПлејера. Морате прво деинсталирати 32-битни СМПлејер."
${LangFileString} Existing_64bitInst "Тренутно је инсталирана 64-битна верзија СМПлејера. Морате прво деинсталирати 64-битни СМПлејер."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) програм инсталације"
${LangFileString} WelcomePage_Text "Програм инсталације ће вас водити кроз инсталацију $(^NameDA).$\r$\n$\r$\nПрепорука је да затворите претходно отворени СМПлејер пре почетка инсталације. То ће омогућити ажурирање битних фајлова без потребе за поновним покретањем вашег рачунара.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Пречице"
${LangFileString} MPlayerGroupTitle "МПлејер компоненте"
${LangFileString} MPlayerMPVGroupTitle "Погон мултимедије"

${LangFileString} Section_SMPlayer "СМПлејер (потребно)"
${LangFileString} Section_SMPlayer_Desc "СМПлејер, заједничке библиотеке и документација."

${LangFileString} Section_DesktopShortcut "Радна површина"
${LangFileString} Section_DesktopShortcut_Desc "Направи пречицу СМПлејера на радној површини."

${LangFileString} Section_StartMenu "Старт мени"
${LangFileString} Section_StartMenu_Desc "Направи ставку у старт менију за СМПлејер."

${LangFileString} Section_MPlayer "МПлејер (потребно)"
${LangFileString} Section_MPlayer_Desc "МПлејер; потребно за репродукцију."

${LangFileString} Section_MPlayerCodecs "Бинарни кодеци"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Бинарни кодеци нису подржани у овој верзији."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Опциони кодеци за МПлејер. (Интернет веза је потребна за инсталацију)"
!endif

${LangFileString} Section_MPV_Desc "Могућностима богат наследник МПлејера2"

${LangFileString} Section_MEncoder_Desc "Пратећи програм за МПлејер који се може користити за кодирање или трансформацију подржаних звучних или видео токова."

${LangFileString} Section_IconThemes "Теме икона"
${LangFileString} Section_IconThemes_Desc "Додатне теме икона за СМПлејер."

${LangFileString} Section_Translations "Језици"
${LangFileString} Section_Translations_Desc "Не-енглески језички фајлови за СМПлејер."

${LangFileString} Section_ResetSettings_Desc "Уклања СМПлејер подешавања заостала од претходних инсталација."

${LangFileString} MPlayer_Codec_Msg "Пакет бинарних кодека даје подршку за кодеке који још нису изворно уграђени, попут новијих RealVideo верзија и још много неуобичајених формата.$\nИмајте на уму да нису неопходни за репродукцију најчешћих формата попут ДВД-a, МПЕГ-1/2/4, итд."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Одаберите врсту инсталације"
${LangFileString} Reinstall_Header_SubText "Одаберите режим пребрисавања или деинсталације."

${LangFileString} Reinstall_Msg1 "Откривена је постојећа инсталација СМПлејера у следећој фасцикли:"
${LangFileString} Reinstall_Msg2 "Одаберите наставак инсталације:"
${LangFileString} Reinstall_Overwrite "Препиши ($Inst_Type) преко постојеће инсталације"
${LangFileString} Reinstall_Uninstall "Деинсталирај (уклони) постојећу инсталацију"
${LangFileString} Reinstall_Msg3_1 "Кликните Покрени када сте спремни за наставак."
${LangFileString} Reinstall_Msg3_2 "Кликните Следеће када сте спремни за наставак."
${LangFileString} Reinstall_Msg3_3 "Кликните Деинсталирај када сте спремни за наставак."
${LangFileString} Reinstall_Msg4 "Промени инсталациона подешавања"
${LangFileString} Reinstall_Msg5 "Врати СМПлејер на почетна подешавања"

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
${LangFileString} Uninstaller_No_Admin "Ову инсталацију може деинсталирати само корисник са администраторским овлашћењима."
${LangFileString} Uninstaller_Aborted "Деинсталацију је прекинуо корисник."
${LangFileString} Uninstaller_NotInstalled "Чини се да СМПлејер није инсталиран у фасцикли '$INSTDIR'.$\r$\nИпак настављате (није препоручљиво)?"
${LangFileString} Uninstaller_InvalidDirectory "СМПлејер инсталација није пронађена."
${LangFileString} Uninstaller_64bitOnly "Ова инсталација може бити деинсталирана само на 64-битном Windowsu."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "СМПлејер је комплетно прочеље за МПлејер, од основних функција попут репродукције видео снимака, ДВД-а, ВЦД-а, до напреднијих као што су подршке за МПлејер филтере, едл листе и још много тога."

; Misc
${LangFileString} Info_Codecs_Backup "Правим резервну копију кодека од претходних инсталација..."
${LangFileString} Info_Codecs_Restore "Враћање копије кодека од претходне инсталације..."
${LangFileString} Info_Del_Files "Уклањање фајлова..."
${LangFileString} Info_Del_Registry "Уклањање кључева регистра..."
${LangFileString} Info_Del_Shortcuts "Уклањање пречица..."
${LangFileString} Info_Rest_Assoc "Враћам придруживање фајлова..."
${LangFileString} Info_RollBack "Враћање промена..."
${LangFileString} Info_Files_Extract "Распакивање фајлова..."
${LangFileString} Info_SMTube_Backup "Прављење резервних копија SMTube..."
${LangFileString} Info_SMTube_Restore "Враћање SMTube од претходне инсталације..."
${LangFileString} Info_MPV_Backup "Правим резерву за MPV..."
${LangFileString} Info_MPV_Restore "Враћам MPV од претходне инсталације..."

; MPV
${LangFileString} MPV_DL_Msg "Преузимам MPV..."
${LangFileString} MPV_DL_Retry "MPV није успешно инсталиран. Покушајте поново?"
${LangFileString} MPV_DL_Failed "Неуспешно преузимање MPV: '$R0'."
${LangFileString} MPV_Inst_Failed "Неуспела инсталација MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL није успешно инсталиран. Да покушам поново?"
${LangFileString} YTDL_DL_Failed "Неуспешно преузимање Youtube-DL: '$R0'."
