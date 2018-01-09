;Language: Farsi (1065)
;Farsi language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Farsi" "فارسی"
!else
!insertmacro LANGFILE "Farsi" "Persian" "فارسی" "Farsi"
!endif

; Startup
${LangFileString} Installer_Is_Running "برنامه نصب کننده هم اکنون در حال اجراست."
${LangFileString} Installer_No_Admin "برای نصب این نرم‌افزار باید با یک حساب کاربری با دسترسی مدیریت وارد شده باشید."
${LangFileString} SMPlayer_Is_Running "برنامه SMPlayer هم اکنون در حال اجراست. ابتدا نرم‌افزار در حال اجرا را بسته و سپس دوباره تلاش کنید."

${LangFileString} OS_Not_Supported "سیستم عامل پشتیبانی نمی‌شود.$\nنرم‌افزار SMPlayer ${SMPLAYER_VERSION} حداقل به سیستم عامل Windows XP برای اجرا نیاز دارد و ممکن است بر روی سیستم شما به درستی کار نکند.$\nآیا با این حال فرایند نصب برنامه را دنبال می‌کنید؟"
${LangFileString} OS_Not_Supported_VistaRequired "سیستم عامل پشتیبانی نمی‌شود.$\nنرم‌افزار SMPlayer ${SMPLAYER_VERSION} حداقل به سیستم عامل Windows Vista برای اجرا نیاز دارد و ممکن است بر روی سیستم شما به درستی کار نکند.$\nآیا با این حال فرایند نصب برنامه را دنبال می‌کنید؟"
${LangFileString} Win64_Required "برای نصب این نرم‌فزار به یک سیستم عامل Windows با ساختار 64 بیتی نیاز است."
${LangFileString} Existing_32bitInst "یک نسخه 32 بیتی از SMPlayer نصب شده است ، ابتدا آن نسخه 32 بیتی را حذف نمایید."
${LangFileString} Existing_64bitInst "یک نسخه 64 بیتی از smplayer نصب شده است . شما باید ابتدا این نسخه 64 بیتی smplayer را حذف کنید."

; Welcome page
${LangFileString} WelcomePage_Title "نصب $(^NameDA)"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "میانبرها"
${LangFileString} MPlayerGroupTitle "اجزاء mplayer "
${LangFileString} MPlayerMPVGroupTitle "موتور چندرسانه‌ای"

${LangFileString} Section_SMPlayer "smplayer(خواسته شده است)"
${LangFileString} Section_SMPlayer_Desc "smplayer کتابخانه ها و اسناد را به اشتراک گذاشته است"

${LangFileString} Section_DesktopShortcut "دسکتاپ"
${LangFileString} Section_DesktopShortcut_Desc "ساختن یک میانبر برای smplayer روی دسکتاپ"

${LangFileString} Section_StartMenu "فهرست شروع"
${LangFileString} Section_StartMenu_Desc "ایجاد یک فهرست شروع برای smplayer"

${LangFileString} Section_MPlayer "mplayer(خواسته شده است)"
${LangFileString} Section_MPlayer_Desc "برای پخش به MPLayer نیاز هست"

${LangFileString} Section_MPlayerCodecs "کدک‌های باینری"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "نسخه باینری کدک‌ها از این نسخه پشتیبانی به عمل نمی‌آورند."
!else
${LangFileString} Section_MPlayerCodecs_Desc "کدک‌های اختیاری برای MPLayer . ( اتصال به اینترنت نیاز است )"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "تم آیکون"
${LangFileString} Section_IconThemes_Desc "بسته تیم آیکون اضافه برای SMPlayer"

${LangFileString} Section_Translations "زبان‌ها"
${LangFileString} Section_Translations_Desc "زبان‌های غیر انگلیسی برای SMPlayer"

${LangFileString} Section_ResetSettings_Desc "پاک شدن ترجیحات باقی مانده از نصب‌های قبلی"

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "انتخاب شیوه نصب"
${LangFileString} Reinstall_Header_SubText "بازنویسی یا حذف نصب را انتخاب کنید."

${LangFileString} Reinstall_Msg1 "شما یک نسخه نصب شده از SMPlayer را نصب شده دارید."
${LangFileString} Reinstall_Msg2 "لطفا شیوه ادامه را انتخاب کنید:"
${LangFileString} Reinstall_Overwrite "بازنویسی ($Inst_Type) روی نسخه نصب شده"
${LangFileString} Reinstall_Uninstall "حذف (remove) نسخه موجود"
${LangFileString} Reinstall_Msg3_1 "برای ادامه روی شروع کلیک کنید"
${LangFileString} Reinstall_Msg3_2 "برای ادامه روی بعدی کلیک کنید"
${LangFileString} Reinstall_Msg3_3 "برای ادامه روی حذف کلیک کنید"
${LangFileString} Reinstall_Msg4 "تغییر تنظیمات نصب"
${LangFileString} Reinstall_Msg5 "برگرداندن پیکره‌بندی‌ها به حالت پیشفرض"

${LangFileString} Remove_Settings_Confirmation "Are you sure you want to reset your SMPlayer settings? This action cannot be reversed."

${LangFileString} Type_Reinstall "نصب مجدد"
${LangFileString} Type_Downgrade "تنزیل‌"
${LangFileString} Type_Upgrade "ارتقاء"

${LangFileString} StartBtn "شروع"

; Codecs Section
${LangFileString} Codecs_DL_Msg "بارگذاری کدک‌های MPlayer"
${LangFileString} Codecs_DL_Retry "کدک‌های MPLayer با موفقیت نصب نشده‌اند. سعی مجدد نمایید؟"
${LangFileString} Codecs_DL_Failed "خطا در بارگذاری کدک‌های MPLayer:'$R0'"
${LangFileString} Codecs_Inst_Failed "نصب کد های mplayer با موفقیت انجام نشد"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "نسخه نصب شده تنها توسط کاربری با دسترسی مدیر سیستم قابل حذف شدن خواهد بود."
${LangFileString} Uninstaller_Aborted "حذف توسط کاربر لغو شد"
${LangFileString} Uninstaller_NotInstalled "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"
${LangFileString} Uninstaller_InvalidDirectory "نسخه نصب شده از SMPlayer یافت نشد."
${LangFileString} Uninstaller_64bitOnly "نسخه نصب شده تنها بر روی ویندوز 64 بیتی قابلیت حذف شدن را دارد."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."

; Misc
${LangFileString} Info_Codecs_Backup "پشتیبان‌گیری از کدک‌ها نسخه نصب شده قبلی"
${LangFileString} Info_Codecs_Restore "بازیابی کدک‌ها از نسخه نصب شده قبلی"
${LangFileString} Info_Del_Files "حذف فایل‌ها"
${LangFileString} Info_Del_Registry "حذف کلیدهای رجیستری"
${LangFileString} Info_Del_Shortcuts "حذف میانبرها"
${LangFileString} Info_Rest_Assoc "بازیابی فایل انجمن"
${LangFileString} Info_RollBack "برگرداندن تغییرات به حالت اولیه"
${LangFileString} Info_Files_Extract "گشودن فایل‌ها"
${LangFileString} Info_SMTube_Backup "پشتیبان SMTube..."
${LangFileString} Info_SMTube_Restore "بازیابی SMTube از نسخه نصب شده قبلی"
${LangFileString} Info_MPV_Backup "Backing up mpv..."
${LangFileString} Info_MPV_Restore "Restoring mpv from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Downloading mpv..."
${LangFileString} MPV_DL_Retry "mpv was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Failed to install mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Checking for youtube-dl updates..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Cleaning fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "Cleaning SMPlayer settings..."
