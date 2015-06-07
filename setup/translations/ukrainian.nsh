;Language: Ukrainian (1058)
;Ukrainian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Ukrainian" "Українська"

; Startup
${LangFileString} Installer_Is_Running "Встановлювач вже запущений."
${LangFileString} Installer_No_Admin "Ви повинні увійти у систему в якості адміністратора під час встановлення цієї проґрами."
${LangFileString} SMPlayer_Is_Running "Запущено екземпляр SMPlayer. Будь ласка, закрийте SMPlayer і спробуйте ще."

${LangFileString} OS_Not_Supported "Непідтримувана версія операційної системи.$\nSMPlayer ${SMPLAYER_VERSION} потребує як мінімум Windows XP й може працювати неправильно на Вашій системі.$\nВи справді бажаєте продовжити встановлення?"
${LangFileString} Win64_Required "64-розрядна операційна система Windows, необхідна для встановлення цього проґрамного забезпечення."
${LangFileString} Existing_32bitInst "Існує 32-бітове встановлення SMPlayer. Спочатку вилучіть 32-бітовий SMPlayer."
${LangFileString} Existing_64bitInst "Існує 64-бітове встановлення SMPlayer. Спочатку вилучіть 64-бітовий SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "Встановлення $(^NameDA)"
${LangFileString} WelcomePage_Text "Майстер встановлення допоможе встановити $(^NameDA).$\r$\n$\r$\nПеред початком встановлення рекомендується закрити усі працюючі копії SMPlayer. Це дозволить оновити супутні проґрамні файли без перезавантаження комп’ютера.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Ярлик"
${LangFileString} MPlayerGroupTitle "Компоненти MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Рушій відтворення"

${LangFileString} Section_SMPlayer "SMPlayer (обов'язково)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, спільні бібліотеки та документація."

${LangFileString} Section_DesktopShortcut "Стільниця"
${LangFileString} Section_DesktopShortcut_Desc "Створити ярлик SMPlayer на стільниці."

${LangFileString} Section_StartMenu "Меню Пуск"
${LangFileString} Section_StartMenu_Desc "Створити запис для SMPlayer у Меню Пуск."

${LangFileString} Section_MPlayer "MPlayer (обов'язково)"
${LangFileString} Section_MPlayer_Desc "MPlayer; необхідний для відтворення."

${LangFileString} Section_MPlayerCodecs "Бінарні кодеки"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Бінарні кодеки не підтримується цією версією."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Додаткові кодеки для MPlayer. (Необхідне зєднання з Інтернетом для встановлення)"
!endif

${LangFileString} Section_MPV_Desc "Багатий можливостями форк MPlayer і MPlayer2"

${LangFileString} Section_MEncoder_Desc "Супутня MPlayer проґрама, яку можна використовувати для кодування та перетворення підтримуваних аудіо та видиво потоків."

${LangFileString} Section_IconThemes "Теми піктограм"
${LangFileString} Section_IconThemes_Desc "Додаткові теми піктограм для SMPlayer."

${LangFileString} Section_Translations "Мови"
${LangFileString} Section_Translations_Desc "Не англомовні файли для SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Вилучити SMPlayer уподобання, які залишилися від попереднього встановлення."

${LangFileString} MPlayer_Codec_Msg "Пакунки з двійковими кодеками додають підтримку кодеків, не вбудованих у mplayer, наприклад, RealVideo й інших нестандартних форматів.$\nЗверніть увагу, що ці кодеки не потрібні для відтворення більшості звичайних форматів типу DVD, MPEG-1/2/4 тощо."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Виберіть тип встановлення"
${LangFileString} Reinstall_Header_SubText "Оберіть режим Перевстановити чи Вилучити"

${LangFileString} Reinstall_Msg1 "Існує встановлення SMPlayer у такому каталозі:"
${LangFileString} Reinstall_Msg2 "Будь ласка, виберіть, як діяти далі:"
${LangFileString} Reinstall_Overwrite "Перезаписати ($Inst_Type) існуюче встановлення"
${LangFileString} Reinstall_Uninstall "Деінсталювати (вилучити) існуюче встановлення"
${LangFileString} Reinstall_Msg3_1 "Натисніть Пуск, щоб продовжити."
${LangFileString} Reinstall_Msg3_2 "Натисніть Далі, щоб продовжити"
${LangFileString} Reinstall_Msg3_3 "Натисніть Вилучити, щоб продовжити"
${LangFileString} Reinstall_Msg4 "Змінити параметри встановлення"
${LangFileString} Reinstall_Msg5 "Скинути мою конфігурацію SMPlayer"

${LangFileString} Type_Reinstall "перевстановити"
${LangFileString} Type_Downgrade "повернення до попередньої версії"
${LangFileString} Type_Upgrade "оновити"

${LangFileString} StartBtn "Пуск"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Завантаження кодеків MPlayer..."
${LangFileString} Codecs_DL_Retry "Кодеки MPlayer не було вдало встановлено. Повторити спробу?"
${LangFileString} Codecs_DL_Failed "Не вдалося завантажити кодеки MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Не вдалося встановити MPlayer кодеки."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Це встановлення може бути вилучене лише користувачем з правами адміністратора."
${LangFileString} Uninstaller_Aborted "Вилучення перервано користувачем."
${LangFileString} Uninstaller_NotInstalled "Не схоже, що SMPlayer встановлений у каталог '$INSTDIR'.$\r$\nПродовжити все одно (не рекомендується)?"
${LangFileString} Uninstaller_InvalidDirectory "Встановлення SMPlayer не виявлено."
${LangFileString} Uninstaller_64bitOnly "Це встановлення може бути вилучене лише на 64-бітовій Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer - повноцінна оболонка для MPlayer, починаючи від базових функцій типу відтворення видивофайлів, DVD, VCD й закінчуючи просунутішими типу підтримки фільтрів MPlayer, переліків тощо."

; Misc
${LangFileString} Info_Codecs_Backup "Виконується резервне копіювання кодеків з попереднього встановлення..."
${LangFileString} Info_Codecs_Restore "Відновити кодеки з попереднього встановлення"
${LangFileString} Info_Del_Files "Видалення файлів..."
${LangFileString} Info_Del_Registry "Вилучення ключів реєстру..."
${LangFileString} Info_Del_Shortcuts "Вилучення ярликів..."
${LangFileString} Info_Rest_Assoc "Відновлення асоціяцій файлів..."
${LangFileString} Info_RollBack "Відкочування змін..."
${LangFileString} Info_Files_Extract "Витягнення файлів..."
${LangFileString} Info_SMTube_Backup "Резервується SMTube..."
${LangFileString} Info_SMTube_Restore "Відновлення SMTube з попереднього встановлення..."
${LangFileString} Info_MPV_Backup "Backing up MPV..."
${LangFileString} Info_MPV_Restore "Відновлення MPV з попереднього встановлення..."

; MPV
${LangFileString} MPV_DL_Msg "Завантаження MPV..."
${LangFileString} MPV_DL_Retry "Не вдалося встановити MPV. Спробувати ще раз?"
${LangFileString} MPV_DL_Failed "Не вдалося завантажити MPV: '$R0'."
${LangFileString} MPV_Inst_Failed "Помилка при встановленні MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "Не вдалося встановити YouTube-DL. Спробувати ще раз?"
${LangFileString} YTDL_DL_Failed "Не вдалося завантажити Youtube-DL: '$R0'."
