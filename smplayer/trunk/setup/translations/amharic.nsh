;Language: Amharic (1118)
;Amharic language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Amharic" "አማርኛ"

; Startup
${LangFileString} Installer_Is_Running "መግጠሚያው እየሄደ ነው"
${LangFileString} Installer_No_Admin "እርስዎ ይህን ፕሮግራም ሲገጥሙ እንደ አስተዳዳሪ መግባት አለብዎት "
${LangFileString} SMPlayer_Is_Running "የ SMP ማጫወቻ እየሄደ ነው: እባክዎን ከ SMP ማጫወቻ ይውጡ እና እንደገና ይሞክሩ"

${LangFileString} OS_Not_Supported "ያልተደገፈ አይነት የ መስሪያ ስርአት $\nSMPlayer ${SMPLAYER_VERSION} ያስፈልጋል ቢያንስ Windows XP እና በ እርስዎ መስሪያ ስርአት ላይ በትክክል ላይሰራ ይችላል.$\nበ እርግጥ መግጠሙን መቀጠል ይፈልጋሉ?"
${LangFileString} OS_Not_Supported_VistaRequired "ያልተደገፈ አይነት የ መስሪያ ስርአት $\nSMPlayer ${SMPLAYER_VERSION} ያስፈልጋል ቢያንስ Windows Vista እና በ እርስዎ መስሪያ ስርአት ላይ በትክክል ላይሰራ ይችላል.$\nበ እርግጥ መግጠሙን መቀጠል ይፈልጋሉ?"
${LangFileString} Win64_Required "የ 64-ቢት መስኮት መስሪያ ስርአት ያስፈልጋል ይህን ፕሮግራም ለ መግጠም"
${LangFileString} Existing_32bitInst "ቀደም ያለ የ 32-ቢት የ SMP ማጫወቻ ነበር: መጀመሪያ የ 32-ቢት የ SMP ማጫወቻ ማጥፋት አለብዎት"
${LangFileString} Existing_64bitInst "ቀደም ያለ የ 64-ቢት የ SMP ማጫወቻ ነበር: መጀመሪያ የ 64-ቢት የ SMP ማጫወቻ ማጥፋት አለብዎት"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) ማሰናጃ"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "አቋራጮች"
${LangFileString} MPlayerGroupTitle "የ SMP ማጫወቻ አካላቶች"
${LangFileString} MPlayerMPVGroupTitle "በርካታ መገናኛ ሞተር"

${LangFileString} Section_SMPlayer "SMP ማጫወቻ (ያስፈልጋል)"
${LangFileString} Section_SMPlayer_Desc "የ SMP ማጫወቻ የሚካፈላቸው መጻህፍት ቤት"

${LangFileString} Section_DesktopShortcut "ደስክቶፕ"
${LangFileString} Section_DesktopShortcut_Desc "በ ዴስክቶፕ ላይ አቋራጭ መፍጠሪያ ለ SMP ማጫወቻ"

${LangFileString} Section_StartMenu "ዝርዝር ማስጀመሪያ"
${LangFileString} Section_StartMenu_Desc "Create a Start Menu entry for SMPlayer."

${LangFileString} Section_MPlayer "SMP ማጫወቻ (ያስፈልጋል)"
${LangFileString} Section_MPlayer_Desc "SMP ማጫወቻ; ያስፈልጋል በ ድጋሚ ለማጫወት"

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Optional codecs for MPlayer. (Internet Connection required for installation)"
!endif

; Startup
${LangFileString} Installer_Is_Running "መግጠሚያው እየሄደ ነው"
${LangFileString} Installer_No_Admin "እርስዎ ይህን ፕሮግራም ሲገጥሙ እንደ አስተዳዳሪ መግባት አለብዎት "
${LangFileString} SMPlayer_Is_Running "የ SMP ማጫወቻ እየሄደ ነው: እባክዎን ከ SMP ማጫወቻ ይውጡ እና እንደገና ይሞክሩ"

${LangFileString} OS_Not_Supported "ያልተደገፈ አይነት የ መስሪያ ስርአት $\nSMPlayer ${SMPLAYER_VERSION} ያስፈልጋል ቢያንስ Windows XP እና በ እርስዎ መስሪያ ስርአት ላይ በትክክል ላይሰራ ይችላል.$\nበ እርግጥ መግጠሙን መቀጠል ይፈልጋሉ?"
${LangFileString} OS_Not_Supported_VistaRequired "ያልተደገፈ አይነት የ መስሪያ ስርአት $\nSMPlayer ${SMPLAYER_VERSION} ያስፈልጋል ቢያንስ Windows XP እና በ እርስዎ መስሪያ ስርአት ላይ በትክክል ላይሰራ ይችላል.$\n በ እርግጥ መግጠሙን መቀጠል ይፈልጋሉ?"
${LangFileString} Win64_Required "የ 64-ቢት መስኮት መስሪያ ስርአት ያስፈልጋል ይህን ፕሮግራም ለ መግጠም"
${LangFileString} Existing_32bitInst "ቀደም ያለ የ 32-ቢት የ SMP ማጫወቻ ነበር: መጀመሪያ የ 32-ቢት የ SMP ማጫወቻ ማጥፋት አለብዎት"
${LangFileString} Existing_64bitInst "ቀደም ያለ የ 64-ቢት የ SMP ማጫወቻ ነበር: መጀመሪያ የ 64-ቢት የ SMP ማጫወቻ ማጥፋት አለብዎት"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) ማሰናጃ"
${LangFileString} WelcomePage_Text "ማሰናጃው ይመራዎታል እንዴት እንደሚገጥሙ $(^NameDA).$\r$\n$\r$\n ሌሎች ፕሮግራሞችን ይዝጉ SMPlayer ከ መገጠሙ በፊት: ይህ ማሻሻያ አግባብ ያለው ፕሮግራም ኮምፒዩተሩ እንደገና ሳይጀምር ማሻሻል ያስችለዋል $\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "አቋራጮች"
${LangFileString} MPlayerGroupTitle "የ SMP ማጫወቻ አካላቶች"
${LangFileString} MPlayerMPVGroupTitle "በርካታ መገናኛ ሞተር"

${LangFileString} Section_SMPlayer "SMP ማጫወቻ (ያስፈልጋል)"
${LangFileString} Section_SMPlayer_Desc "የ SMP ማጫወቻ የሚካፈላቸው መጻህፍት ቤት"

${LangFileString} Section_DesktopShortcut "ደስክቶፕ"
${LangFileString} Section_DesktopShortcut_Desc "በ ዴስክቶፕ ላይ አቋራጭ መፍጠሪያ ለ SMP ማጫወቻ"

${LangFileString} Section_StartMenu "ዝርዝር ማስጀመሪያ"
${LangFileString} Section_StartMenu_Desc "ለ SMPlayer የ ማስጀመሪያ ዝርዝር ማስገቢያ መፍጠሪያ"

${LangFileString} Section_MPlayer "SMP ማጫወቻ (ያስፈልጋል)"
${LangFileString} Section_MPlayer_Desc "SMP ማጫወቻ; ያስፈልጋል በ ድጋሚ ለማጫወት"

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "በ ምርጫ codecs ለ MPlayer. (ለ መግጠም የ ኢንተርኔት ግንኙነት ያስፈልጋል)"
!endif

${LangFileString} Section_MPV_Desc "በርካታ ገጽታ ያለው የ MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "የ ምልክት ገጽታዎች"
${LangFileString} Section_IconThemes_Desc "ለ SMP ማጫወቻ ተጨማሪ ገጽታዎች"

${LangFileString} Section_Translations "ቋንቋ"
${LangFileString} Section_Translations_Desc "እንግሊዝኛ-ላልሆኑ ቋንቋዎች ፋይሎች ለ SMP ማጫወቻ"

${LangFileString} Section_ResetSettings_Desc "የ SMP ማጫወቻ ምርጫዎች ካለፈው መግጠሚያ የ ተረፉትን ማጥፊያ"

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "የ መግጠሚያ አይነት ይምረጡ"
${LangFileString} Reinstall_Header_SubText "ይምረጡ በላዩ ላይ ደርቦ መጻፊያ ወይንም ማጥፊያ ዘዴ"

${LangFileString} Reinstall_Msg1 "እርስዎ በቅድሚያ የተገጠመ የ SMPlayer በሚቀጥለው ፎልደር ውስጥ አለዎት"
${LangFileString} Reinstall_Msg2 "እባክዎን እንዴት መቀጠል እንደሚፈልጉ ይምረጡ"
${LangFileString} Reinstall_Overwrite " በላዩ ላይ ደርቦ መጻፊያ ($Inst_Type) በ ነበረው መግጠሚያ ላይ"
${LangFileString} Reinstall_Uninstall "ማጥፊያ (ማስወገጃ) የ ነበረውን መግጠሚያ"
${LangFileString} Reinstall_Msg3_1 "ይጫኑ ማስጀመሪያውን ለ መቀጠል ዝግጁ ሲሆኑ "
${LangFileString} Reinstall_Msg3_2 "ይጫኑ ማስጀመሪያውን ለ መቀጠል ዝግጁ ሲሆኑ "
${LangFileString} Reinstall_Msg3_3 "ይጫኑ ማጥፊያውን ለ መቀጠል ዝግጁ ሲሆኑ "
${LangFileString} Reinstall_Msg4 "የ መግጠሚያ ማሰናጃ መቀየሪያ"
${LangFileString} Reinstall_Msg5 "እንደ ነበር መመለሻ የ እኔን SMPlayer ማዋቀሪያ"

${LangFileString} Remove_Settings_Confirmation "Are you sure you want to reset your SMPlayer settings? This action cannot be reversed."

${LangFileString} Type_Reinstall "እንደገና መግጠሚያ"
${LangFileString} Type_Downgrade "ዝቅ ማድረጊያ"
${LangFileString} Type_Upgrade "ማሻሻያ"

${LangFileString} StartBtn "ማስጀመሪያ"

; Codecs Section
${LangFileString} Codecs_DL_Msg "የ MPlayer Codecs በ ማውረድ ላይ..."
${LangFileString} Codecs_DL_Retry "የ MPlayer codecs ተሳክቶ አልተገጠመም: እንደገና ለ መግጠም ልሞክር?"
${LangFileString} Codecs_DL_Failed "ማውረድ አልተቻለም የ MPlayer codecs: '$R0'."
${LangFileString} Codecs_Inst_Failed "የ MPlayer codecs. መግጠም አልተቻለም"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} Uninstaller_Aborted "ማጥፋቱ በ ተጠቃሚ ተቋርጧል"
${LangFileString} Uninstaller_NotInstalled "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"
${LangFileString} Uninstaller_InvalidDirectory "የ SMPlayer መግጠሚያ አልተገኘም"
${LangFileString} Uninstaller_64bitOnly "ይህን መግጠሚያ ማስወገድ የሚቻለው በ 64-ቢት መስኮቶች ውስጥ ብቻ ነው"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."

; Misc
${LangFileString} Info_Codecs_Backup "በ ቅድሚያ የ ተገጠመውን codecs ተተኪ በ መፍጠር ላይ"
${LangFileString} Info_Codecs_Restore "በ ቅድሚያ የ ተገጠመውን codecs ተተኪ እንደ ነበር መመለሻ"
${LangFileString} Info_Del_Files "ፋይሎች በማጥፋት ላይ..."
${LangFileString} Info_Del_Registry "Registry Keys በማጥፋት ላይ..."
${LangFileString} Info_Del_Shortcuts "አቋራጮች በማጥፋት ላይ..."
${LangFileString} Info_Rest_Assoc "እንደ ነበር በ መመለስ ላይ የሚዛመዱ ፋይሎችን..."
${LangFileString} Info_RollBack "ለውጦችን እንደ ነበር መመለሻ..."
${LangFileString} Info_Files_Extract "ፋይሎች በ ማራገፍ ላይ..."
${LangFileString} Info_SMTube_Backup "ለ SMTube ተተኪ መፍጠሪያ..."
${LangFileString} Info_SMTube_Restore "በ ቅድሚያ የ ተገጠመውን SMTube ተተኪ እንደ ነበር መመለሻ"
${LangFileString} Info_MPV_Backup "ተተኪ በማሰናዳት ላይ የ mpv..."
${LangFileString} Info_MPV_Restore "በ ቅድሚያ የ ተገጠመውን mpv ተተኪ እንደ ነበር መመለሻ"

; MPV
${LangFileString} MPV_DL_Msg "በ ማውረድ ላይ mpv..."
${LangFileString} MPV_DL_Retry "mpv ተሳክቶ አልተገጠመም: እንደገና ልሞክር?"
${LangFileString} MPV_DL_Failed "ማውረድ አልተቻለም: '$R0'."
${LangFileString} MPV_Inst_Failed "መግጠም አልተቻለም mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "ማውረድ አልተቻለም ከ youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "የ ዩቲዩብ-dl ማሻሻያ በ መመርመር ላይ..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Cleaning fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "በማጽዳት ላይ የ SMPlayer ማሰናጃ"
