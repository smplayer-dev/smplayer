;;
;;  russian.nsh
;;
;;  Russian language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_RUSSIAN} "Программа установки уже запущена."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_RUSSIAN} "Неподдерживаемая операционная система.$\n$(^Name) требует как минимум Windows 2000 и может некорректно работать на вашей системе.$\nВы действительно хотите продолжить установку?"
LangString SMPLAYER_INSTALLER_PREV_ALL_USERS ${LANG_RUSSIAN} "SMPlayer уже установлен глобально для всех пользователей.$\nПожалуйста, перезапустите программу установки с привилегиями администратора."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_RUSSIAN} "SMPlayer уже установлен.$\nВы хотите удалить предыдущую версию перед установкой $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_RUSSIAN} "Пакеты с бинарными кодеками добавляют поддержку кодеков, не встроенных в mplayer, например, RealVideo и других нестандартных форматов.$\nОбратите внимание, что эти кодеки не нужны для воспроизведения большинства обычных форматов вроде DVD, MPEG-1/2/4 и т.п."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_RUSSIAN} "MPlayer уже установлен. Загрузить заново?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_RUSSIAN} "Загрузка MPlayer..."
LangString MPLAYER_DL_FAILED ${LANG_RUSSIAN} "Не удалось загрузить MPlayer:"
LangString MPLAYER_INST_FAILED ${LANG_RUSSIAN} "Ошибка при установке MPlayer. MPlayer требуется для воспроизведения."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_RUSSIAN} "Бинарные кодеки для MPlayer уже установлены. Загрузить заново?"
LangString CODECS_IS_DOWNLOADING ${LANG_RUSSIAN} "Загрузка бинарных кодеков для MPlayer..."
LangString CODECS_DL_FAILED ${LANG_RUSSIAN} "Не удалось загрузить бинарные кодеки для MPlayer:"
LangString CODECS_INST_FAILED ${LANG_RUSSIAN} "Ошибка при установке бинарных кодеков для MPlayer."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_RUSSIAN} "Загрузка информации о версии..."
LangString VERINFO_DL_FAILED ${LANG_RUSSIAN} "Не удалось загрузить информацию о версии:"
LangString VERINFO_IS_MISSING ${LANG_RUSSIAN} "Файл версии не содержит нужной информации. Программа установки будет использовать версию по умолчанию."

; Uninstaller
LangString UNINSTALL_ABORTED ${LANG_RUSSIAN} "Удаление прервано пользователем."
LangString UNINSTALL_INSTALLED_ALL_USERS ${LANG_RUSSIAN} "SMPlayer установлен глобально для всех пользователей.$\nПожалуйста, перезапустите программу удаления с привилегиями администратора."
LangString SMPLAYER_NOT_INSTALLED ${LANG_RUSSIAN} "Не похоже, что SMPlayer установлен в каталог '$INSTDIR'.$\r$\nПродолжить всё равно (не рекомендуется)?"

; Vista & Later Default Programs Registration
LangString APPLICATION_DESCRIPTION ${LANG_RUSSIAN} "SMPlayer - полноценная оболочка для MPlayer, начиная от базовых функций вроде воспроизведения видеофайлов, DVD, VCD и заканчивая более продвинутыми вроде поддержки фильтров MPlayer, списков и т.д."