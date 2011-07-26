;Language: Russian (1049)
;Russian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Russian" "Русский"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Программа установки уже запущена."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Для установки этой программы необходимы права администратора."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (необходимо)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, общие библиотеки и документация."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Ярлыки"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Рабочий стол"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Создаёт ярлык SMPlayer на рабочем столе."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Стартовое меню"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Создать пункт в стартовом меню для SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "Компоненты MPlayer"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (необходимо)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; требуется для воспроизведения."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; требуется для воспроизведение. (для установки нужно соединение с Internet)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binary кодеки"
${LangFileString} SMPLAYER_SECCODECS_DESC "Дополнительные кодеки для MPlayer. (для установки нужно соединение с Internet)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Темы иконок"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Дополнительные темы иконок для SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Локализации"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Отличные от английской локализации."

${LangFileString} MPLAYER_CODEC_INFORMATION "Пакеты с бинарными кодеками добавляют поддержку кодеков, не встроенных в mplayer, например, RealVideo и других нестандартных форматов.$\nОбратите внимание, что эти кодеки не нужны для воспроизведения большинства обычных форматов вроде DVD, MPEG-1/2/4 и т.п."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Уже установлено"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Выберите что вам нужно для установки SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Отметьте необходимые вам опции."

${LangFileString} REINSTALL_CHGSETTINGS "Изменить настройки (для продвинутых)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Неизвестное значение PREVIOUS_VERSION_STATE, прерывание"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "На вашей системе установлена старая версия SMPlayer. Выберите необходимую операцию и нажмите Далее для продолжения."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Обновить SMPlayer, используя предыдущие настройки (рекомендуется)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Более новая версия SMPlayer уже установлена! Не рекомендуется переход к более старой версии. Выберите необходимую операцию и нажмите Далее для продолжения."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Понизить версию SMPlayer, используя предыдущие настройки (рекомендуется)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} уже установлен. Выберите необходимую операцию и нажмите Далее для продолжения."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Добавить/Удалить/Изменить компоненты"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Удалить SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Загрузка MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer не был успешно установлен. Повторить?"
${LangFileString} MPLAYER_DL_FAILED "Не удалось загрузить MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Ошибка при установке MPlayer. MPlayer требуется для воспроизведения."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Загрузка бинарных кодеков для MPlayer..."
${LangFileString} CODECS_DL_RETRY "Кодеки для MPlayer не были успешно установлены. Повторить?"
${LangFileString} CODECS_DL_FAILED "Не удалось загрузить бинарные кодеки для MPlayer: '$R0'."
${LangFileString} CODECS_INST_FAILED "Ошибка при установке бинарных кодеков для MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Загрузка информации о версии..."
${LangFileString} VERINFO_DL_FAILED "Не удалось загрузить информацию о версии: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Эта установка может быть удалена только пользователем с правами администратора."
${LangFileString} UNINSTALL_ABORTED "Удаление прервано пользователем."
${LangFileString} SMPLAYER_NOT_INSTALLED "Не похоже, что SMPlayer установлен в каталог '$INSTDIR'.$\r$\nПродолжить всё равно (не рекомендуется)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer - полноценная оболочка для MPlayer, начиная от базовых функций вроде воспроизведения видеофайлов, DVD, VCD и заканчивая более продвинутыми вроде поддержки фильтров MPlayer, списков и т.д."

; Misc
${LangFileString} INFO_DEL_FILES "Удаление файлов..."
${LangFileString} INFO_DEL_REGISTRY "Удаление ключей реестра..."
${LangFileString} INFO_DEL_SHORTCUTS "Удаление ярлыков..."
${LangFileString} INFO_REST_ASSOC "Восстановление ассоциаций файлов..."
${LangFileString} INFO_FILE_EXTRACT "Распаковка файлов..."
