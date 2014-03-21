;Language: Bulgarian (1026)
;Bulgarian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Bulgarian" "Български"

; Startup
${LangFileString} Installer_Is_Running "Инсталаторът вече работи."
${LangFileString} Installer_No_Admin "Трябва да сте вписан като администратор, когато инсталирате тази програма."
${LangFileString} SMPlayer_Is_Running "Има работеща инстанция на SMPlayer. Моля, излезте от SMPlayer и опитайте отново."

${LangFileString} OS_Not_Supported "Неподдържана операционна система.$\nSMPlayer ${SMPLAYER_VERSION} изисква поне Windows XP и може към момента да не работи с Вашата система.$\nНаистина ли искате да продължите с инсталацията?"
${LangFileString} Win64_Required "Изисква се 64-битова операционна система Windows, за да инсталирате този софтуер. "
${LangFileString} Existing_32bitInst "Има инсталирана 32-битова версия на SMPlayer. Трябва първо да деинсталирате 32-битовия SMPlayer."
${LangFileString} Existing_64bitInst "Има инсталирана 64-битова версия на SMPlayer. Трябва първо да деинсталирате 64-битовия SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) инсталатор"
${LangFileString} WelcomePage_Text "Инсталаторът ще Ви упътва по време на инсталацията на $(^NameDA).$\r$\n$\r$\nПрепоръчително е да затворите всички инстанции на SMPlayer преди да пуснете инсталатора. Това ще позволи да обновявате обвързани програмни файлове, без да рестартирате компютъра си.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Преки пътища"
${LangFileString} MPlayerGroupTitle "Компоненти на MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (необходим)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, споделени библиотеки и документация."

${LangFileString} Section_DesktopShortcut "Работен плот"
${LangFileString} Section_DesktopShortcut_Desc "Създава пряк път до SMPlayer на работния плот."

${LangFileString} Section_StartMenu "Старт меню"
${LangFileString} Section_StartMenu_Desc "Създава запис за SMPlayer в Старт менюто."

${LangFileString} Section_MPlayer "MPlayer (необходим)"
${LangFileString} Section_MPlayer_Desc "MPlayer; необходим за възпроизвеждане."

${LangFileString} Section_MPlayerCodecs "Бинарни кодеци"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Бинарните кодеци не са поддържани за тази версия."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Допълнителни кодеци за MPlayer. (За инсталацията се изисква връзка с Интернет)"
!endif

${LangFileString} Section_MEncoder_Desc "Допълваща програма на MPlayer, която може да кодира или пренася поддържани звукови или видео потоци."

${LangFileString} Section_IconThemes "Теми с икони"
${LangFileString} Section_IconThemes_Desc "Допълнителни теми с икони за SMPlayer."

${LangFileString} Section_Translations "Езици"
${LangFileString} Section_Translations_Desc "Неанглийски езикови файлове за SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Изтрива остатъчните настройки на SMPlayer от предишната инсталация."

${LangFileString} MPlayer_Codec_Msg "Бинарните пакети с кодеци добавят поддръжка за кодеци, които все още не са вградени, като по-новите RealVideo варианти и много от необикновените формати.$\nИмайте предвид, че не са необходими, за възпроизвеждане на най-масовите формати, като например DVD-та, MPEG-1/2/4."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Изберете начин на инсталиране"
${LangFileString} Reinstall_Header_SubText "Изберете режим на презаписване или деинсталиране."

${LangFileString} Reinstall_Msg1 "Имате съществуваща инсталация на SMPlayer в следната папка:"
${LangFileString} Reinstall_Msg2 "Моля, изберете как искате да продължите:"
${LangFileString} Reinstall_Overwrite "Презаписване на ($Inst_Type) на съществуващата инсталация"
${LangFileString} Reinstall_Uninstall "Деинсталиране (премахване) на съществуващата инсталация"
${LangFileString} Reinstall_Msg3_1 "Кликнете Старт, когато сте готови да продължите."
${LangFileString} Reinstall_Msg3_2 "Кликнете Следващ, когато сте готови да продължите."
${LangFileString} Reinstall_Msg3_3 "Кликнете Деинсталиране, когато сте готови да продължите."
${LangFileString} Reinstall_Msg4 "Промяна на настройките за инсталацията"
${LangFileString} Reinstall_Msg5 "Анулиране на моите настройки на SMPlayer"

${LangFileString} Type_Reinstall "преинсталиране"
${LangFileString} Type_Downgrade "връщане на версия"
${LangFileString} Type_Upgrade "подобряване на версия"

${LangFileString} StartBtn "Старт"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Изтегляне на кодеци за MPlayer..."
${LangFileString} Codecs_DL_Retry "Неуспех в инсталирането на кодеци за MPlayer. Искате ли да опитате отново?"
${LangFileString} Codecs_DL_Failed "Неуспех в изтеглянето на следните кодеци за MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Неуспех в инсталирането на кодеци за MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Тази инсталация може да бъде деинсталирана само от потребител с администраторски права."
${LangFileString} Uninstaller_Aborted "Деинсталацията е прекъсната от потребителя."
${LangFileString} Uninstaller_NotInstalled "Изглежда, че SMPlayer не е инсталиран в директория '$INSTDIR'.$\r$\nПродължаване въпреки това (не е препоръчително)?"
${LangFileString} Uninstaller_InvalidDirectory "Не е намерена инсталация на SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Тази инсталация може да бъде деинсталирана само на 64-битов Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer е напълно съвместима обвивка за MPlayer, от основни възможности като възпроизвеждане на видеа, DVD-та, VCD-та до по-големи възможности като поддръжка на MPlayer филтри, edl-списъци и други."

; Misc
${LangFileString} Info_Codecs_Backup "Съхраняване на кодеци от предишна инсталация..."
${LangFileString} Info_Codecs_Restore "Възстановяване на кодеци от предишна инсталация..."
${LangFileString} Info_Del_Files "Изтриване на файлове..."
${LangFileString} Info_Del_Registry "Изтриване на ключове от регистъра..."
${LangFileString} Info_Del_Shortcuts "Изтриване на преки пътища..."
${LangFileString} Info_Rest_Assoc "Възстановяване на файлови асоциации..."
${LangFileString} Info_RollBack "Връщане на промените..."
${LangFileString} Info_Files_Extract "Разархивиране на файлове..."
