;Language: Ukrainian (1058)
;Ukrainian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Ukrainian" "Українська"

; Startup
${LangFileString} Installer_Is_Running "Встановлювач вже запущений."
${LangFileString} Installer_No_Admin "Ви повинні увійти в систему в якості адміністратора під час встановлення цієї програми."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} OS_Not_Supported "Unsupported operating system.$\nSMPlayer ${SMPLAYER_VERSION} requires at least Windows XP and may not work correctly on your system.$\nDo you really want to continue with the installation?"
${LangFileString} Win64_Required "64-розрядна операційна система Windows, необхідна для встановлення цього програмного забезпечення."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Встановлення"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Лейба"
${LangFileString} MPlayerGroupTitle "Компоненти MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

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

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Теми піктограм"
${LangFileString} Section_IconThemes_Desc "Додаткові теми піктограм для SMPlayer."

${LangFileString} Section_Translations "Мови"
${LangFileString} Section_Translations_Desc "Не англомовні файли для SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Видалити SMPlayer уподобання, які залишились від попереднього встановлення."

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Виберіть тип встановлення"
${LangFileString} Reinstall_Header_SubText "Оберіть режим Перевстановити чи Видалити"

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Будь ласка, виберіть, як діяти далі:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3_1 "Натисніть Пуск, щоб продовжити."
${LangFileString} Reinstall_Msg3_2 "Натисніть Далі, щоб продовжити"
${LangFileString} Reinstall_Msg3_3 "Натисніть Видалити, щоб продовжити"
${LangFileString} Reinstall_Msg4 "Змінити параметри встановлення"
${LangFileString} Reinstall_Msg5 "Скинути мою конфігурацію SMPlayer"

${LangFileString} Type_Reinstall "перевстановити"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "оновити"

${LangFileString} StartBtn "Пуск"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Завантаження кодеків MPlayer..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} Codecs_DL_Failed "Не вдалося завантажити кодеки MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Не вдалося встановити MPlayer кодеки."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} Uninstaller_Aborted "Видалення перервано користувачем."
${LangFileString} Uninstaller_NotInstalled "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer installation not found."
${LangFileString} Uninstaller_64bitOnly "This installation can only be uninstalled on 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."

; Misc
${LangFileString} Info_Codecs_Backup "Backing up codecs from previous installation..."
${LangFileString} Info_Codecs_Restore "Відновити кодеки з попереднього встановлення"
${LangFileString} Info_Del_Files "Видалення файлів..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Витягнення файлів..."
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
