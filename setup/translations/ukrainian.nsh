;Language: Ukrainian (1058)
;Ukrainian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Ukrainian" "Українська"
!else
!insertmacro LANGFILE "Ukrainian" = "Українська" "Ukrayins'ka"
!endif

; Startup
${LangFileString} Installer_Is_Running "Встановлювач вже працює."
${LangFileString} Installer_No_Admin "Ви повинні увійти до системи в якості розпорядника під час встановлення цієї програми."
${LangFileString} SMPlayer_Is_Running "Примірник SMPlayer працює. Будь ласка, закрийте SMPlayer і спробуйте ще."

${LangFileString} OS_Not_Supported "Непідтримувана версія операційної системи.$\nSMPlayer ${SMPLAYER_VERSION} потребує щонайменш Windows XP і може працювати неправильно у Вашій системі.$\nВи справді бажаєте продовжити встановлення?"
${LangFileString} OS_Not_Supported_VistaRequired "Непідтримувана операційна система.$\nSMPlayer ${SMPLAYER_VERSION} потребує принаймні Windows Vista та може працювати неправильно у Вашій системі.$\nВи справді бажаєте продовжити встановлення?"
${LangFileString} Win64_Required "Для встановлення цього програмного забезпечення потрібна 64-бітова операційна система Windows."
${LangFileString} Existing_32bitInst "Існує 32-бітове встановлення SMPlayer. Спочатку вилучіть 32-бітовий SMPlayer."
${LangFileString} Existing_64bitInst "Існує 64-бітове встановлення SMPlayer. Спочатку вилучіть 64-бітовий SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "Встановлення $(^NameDA)"
${LangFileString} WelcomePage_Text "Майстер встановлення допоможе встановити $(^NameDA).$\r$\n$\r$\nПеред початком встановлення радиться закрити всі активні копії SMPlayer. Це дозволить оновити супутні програмні файли без перезавантаження комп’ютера.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Ярлик"
${LangFileString} MPlayerGroupTitle "Компоненти MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Рушій відтворення"

${LangFileString} Section_SMPlayer "SMPlayer (обов'язково)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, спільні бібліотеки та документація."

${LangFileString} Section_DesktopShortcut "Стільниця"
${LangFileString} Section_DesktopShortcut_Desc "Створити посилання SMPlayer на стільниці."

${LangFileString} Section_StartMenu "Меню Пуск"
${LangFileString} Section_StartMenu_Desc "Створити запис для SMPlayer у Меню Пуск."

${LangFileString} Section_MPlayer "MPlayer (обов'язково)"
${LangFileString} Section_MPlayer_Desc "MPlayer; необхідний для відтворення."

${LangFileString} Section_MPlayerCodecs "Двійкові кодеки"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Двійкові кодеки не підтримуються у цій версії."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Додаткові кодеки для MPlayer. (Необхідне зєднання з Інтернетом для встановлення)"
!endif

${LangFileString} Section_MPV_Desc "Багате можливостями відгалуження(форк) MPlayer і MPlayer2"

${LangFileString} Section_YTDL "Підтримка Youtube"
${LangFileString} Section_YTDL_Desc "Дозволяє відтворювати відео з Youtube."

${LangFileString} Section_MEncoder_Desc "Супутня MPlayer програма, яку можна використовувати для кодування та перетворення підтримуваних звукових і відеопотоків."

${LangFileString} Section_IconThemes "Теми піктограм"
${LangFileString} Section_IconThemes_Desc "Додаткові теми піктограм для SMPlayer."

${LangFileString} Section_Translations "Мови"
${LangFileString} Section_Translations_Desc "Не англомовні файли для SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Вилучити уподобання SMPlayer, що залишилися від попереднього встановлення."

${LangFileString} MPlayer_Codec_Msg "Пакунки з двійковими кодеками додають підтримку кодеків, не вбудованих у mplayer, наприклад, RealVideo й инших нестандартних форматів.$\nЗверніть увагу, що ці кодеки не потрібні для відтворення більшости звичайних форматів типу DVD, MPEG-1/2/4 тощо."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Оберіть тип встановлення"
${LangFileString} Reinstall_Header_SubText "Оберіть режим Перевстановити чи Вилучити"

${LangFileString} Reinstall_Msg1 "Існує встановлення SMPlayer у такому каталозі:"
${LangFileString} Reinstall_Msg2 "Будь ласка, оберіть, що робити далі:"
${LangFileString} Reinstall_Overwrite "Перезаписати ($Inst_Type) наявне встановлення"
${LangFileString} Reinstall_Uninstall "Деінсталювати (вилучити) наявне встановлення"
${LangFileString} Reinstall_Msg3_1 "Натисніть Пуск, щоб продовжити."
${LangFileString} Reinstall_Msg3_2 "Натисніть Далі, щоб продовжити"
${LangFileString} Reinstall_Msg3_3 "Натисніть Вилучити, щоб продовжити"
${LangFileString} Reinstall_Msg4 "Змінити параметри встановлення"
${LangFileString} Reinstall_Msg5 "Скинути мої налаштування SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Ви справді волієте скинути налаштування SMPlayer? Дану дію не може бути скасовано."

${LangFileString} Type_Reinstall "перевстановити"
${LangFileString} Type_Downgrade "повернення до попередньої версії"
${LangFileString} Type_Upgrade "оновити"

${LangFileString} StartBtn "Пуск"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Завантаження кодеків MPlayer..."
${LangFileString} Codecs_DL_Retry "Кодеки MPlayer встановити не вийшло. Повторити спробу?"
${LangFileString} Codecs_DL_Failed "Завантажити кодеки MPlayer: '$R0' не вийшло."
${LangFileString} Codecs_Inst_Failed "Встановити MPlayer кодеки не вийшло."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Це встановлення може бути вилучено лише користувачем із правами розпорядника."
${LangFileString} Uninstaller_Aborted "Вилучення перервано користувачем."
${LangFileString} Uninstaller_NotInstalled "Не схоже, що SMPlayer встановлено в каталог '$INSTDIR'.$\r$\nПродовжити попри все(не радиться)?"
${LangFileString} Uninstaller_InvalidDirectory "Встановлення SMPlayer не виявлено."
${LangFileString} Uninstaller_64bitOnly "Це встановлення може бути вилучено лише на 64-бітовій Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer — повноцінна оболонка для MPlayer, починаючи від базових функцій типу відтворення відеофайлів, DVD, VCD і закінчуючи просунутішими, типу підтримки фільтрів MPlayer, переліків тощо."

; Misc
${LangFileString} Info_Codecs_Backup "Виконується резервне копіювання кодеків із попереднього встановлення..."
${LangFileString} Info_Codecs_Restore "Відновити кодеки з попереднього встановлення"
${LangFileString} Info_Del_Files "Видалення файлів..."
${LangFileString} Info_Del_Registry "Вилучення ключів реєстру..."
${LangFileString} Info_Del_Shortcuts "Вилучення посилань..."
${LangFileString} Info_Rest_Assoc "Відновлення асоціацій файлів..."
${LangFileString} Info_RollBack "Відкочування змін..."
${LangFileString} Info_Files_Extract "Видобування файлів..."
${LangFileString} Info_SMTube_Backup "Резервується SMTube..."
${LangFileString} Info_SMTube_Restore "Відновлення SMTube з попереднього встановлення..."
${LangFileString} Info_MPV_Backup "Створення резервної копії MPV..."
${LangFileString} Info_MPV_Restore "Відновлення MPV із попереднього встановлення..."

; MPV
${LangFileString} MPV_DL_Msg "Завантаження MPV..."
${LangFileString} MPV_DL_Retry "Встановити MPV не вийшло. Спробувати ще раз?"
${LangFileString} MPV_DL_Failed "Завантажити MPV: '$R0' не вийшло."
${LangFileString} MPV_Inst_Failed "Помилка при встановленні MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "Встановити YouTube-DL не вийшло. Спробувати ще раз?"
${LangFileString} YTDL_DL_Failed "Завантажити Youtube-DL: '$R0' не вийшло."
${LangFileString} YTDL_Update_Check "Перевірка наявности оновлень Youtube-DL..."
${LangFileString} YTDL_Error_Msg1 "Попередження: ненормальний вихід youtube-dl з кодом: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Для youtube-dl потрібен Visual C++ 2010 Runtime (x86)."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "Поточне встановлення SMTube несумісне з цією версією SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Будь ласка, оновіться до нової версії SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Очищення кешу шрифтів..."
${LangFileString} Info_Cleaning_SMPlayer "Очищення налаштувань SMPlayer..."
