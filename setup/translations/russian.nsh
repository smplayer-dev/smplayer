;Language: Russian (1049)
;Russian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Russian" "Русский"

; Startup
${LangFileString} Installer_Is_Running "Программа установки уже запущена."
${LangFileString} Installer_No_Admin "You must be logged in as an administrator when installing this program."

; Components Page
${LangFileString} ShortcutGroupTitle "Shortcuts"
${LangFileString} MPlayerGroupTitle "MPlayer Components"

${LangFileString} Section_SMPlayer "SMPlayer (required)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, shared libraries, and documentation."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Creates a shortcut to SMPlayer on the desktop."

${LangFileString} Section_StartMenu "Start Menu"
${LangFileString} Section_StartMenu_Desc "Create a Start Menu entry for SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (required)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; required for playback."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; required for playback. (Internet Connection required for installation)"
!endif

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
${LangFileString} Section_MPlayerCodecs_Desc "Optional codecs for MPlayer. (Internet Connection required for installation)"
${LangFileString} Section_MPlayerCodecs_Desc_2 "Optional codecs for MPlayer."

${LangFileString} Section_MEncoder "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Icon Themes"
${LangFileString} Section_IconThemes_Desc "Additional icon themes for SMPlayer."

${LangFileString} Section_Translations "Languages"
${LangFileString} Section_Translations_Desc "Non-English language files for SMPlayer."

${LangFileString} MPlayer_Codec_Msg "Пакеты с бинарными кодеками добавляют поддержку кодеков, не встроенных в mplayer, например, RealVideo и других нестандартных форматов.$\nОбратите внимание, что эти кодеки не нужны для воспроизведения большинства обычных форматов вроде DVD, MPEG-1/2/4 и т.п."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Select Install Type"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3 "Click Start when ready to proceed."

${LangFileString} Type_Reinstall "reinstall"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "Загрузка MPlayer..."
${LangFileString} MPlayer_DL_Retry "MPlayer was not successfully installed. Retry?"
${LangFileString} MPlayer_DL_Failed "Не удалось загрузить MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Ошибка при установке MPlayer. MPlayer требуется для воспроизведения."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Загрузка бинарных кодеков для MPlayer..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} Codecs_DL_Failed "Не удалось загрузить бинарные кодеки для MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Ошибка при установке бинарных кодеков для MPlayer."

; Version information
${LangFileString} VerInfo_DL_Msg "Загрузка информации о версии..."
${LangFileString} VerInfo_DL_Failed "Не удалось загрузить информацию о версии: '$R0'. Using a default version."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} Uninstaller_Aborted "Удаление прервано пользователем."
${LangFileString} Uninstaller_NotInstalled "Не похоже, что SMPlayer установлен в каталог '$INSTDIR'.$\r$\nПродолжить всё равно (не рекомендуется)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer - полноценная оболочка для MPlayer, начиная от базовых функций вроде воспроизведения видеофайлов, DVD, VCD и заканчивая более продвинутыми вроде поддержки фильтров MPlayer, списков и т.д."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_Files_Extract "Extracting files..."
