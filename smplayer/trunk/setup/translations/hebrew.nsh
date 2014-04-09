;Language: Hebrew (1037)
;Hebrew language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Hebrew" "עברית"

; Startup
${LangFileString} Installer_Is_Running "המתקין כבר מורץ."
${LangFileString} Installer_No_Admin "עליך להתחבר בתור מנהל בעת התקנת יישום זה."
${LangFileString} SMPlayer_Is_Running "תהליך של SMPlayer הינו מורץ כעת. אנא צא מתוך SMPlayer ונסה שוב."

${LangFileString} OS_Not_Supported "מערכת הפעלה לא נתמכת.$\nSMPlayer ${SMPLAYER_VERSION} מצריך לפחות Windows XP ועשוי שלא לעבוד כראוי על המערכת הנוכחית.$\nהאם אתה בטוח כי ברצונך להמשיך עם ההתקנה?"
${LangFileString} Win64_Required "מערכת הפעלה Windows מטיפוס  64-ביט נדרשת לשם התקנת תוכנה זו."
${LangFileString} Existing_32bitInst "התקנה קיימת מטיפוס 32-ביט של SMPlayer זמינה. עליך לבטל התקנת SMPlayer מטיפוס 32-ביט תחילה."
${LangFileString} Existing_64bitInst "התקנה קיימת מטיפוס 64-ביט של SMPlayer זמינה. עליך לבטל התקנת SMPlayer מטיפוס 64-ביט תחילה."

; Welcome page
${LangFileString} WelcomePage_Title "התקנת $(^NameDA)"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "קיצורי דרך"
${LangFileString} MPlayerGroupTitle "רכיבי MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (חובה)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, ספריות משותפות, ותיעוד."

${LangFileString} Section_DesktopShortcut "שולחן עבודה"
${LangFileString} Section_DesktopShortcut_Desc "צור קיצור דרך אל SMPlayer על שולחן העבודה."

${LangFileString} Section_StartMenu "תפריט התחלה"
${LangFileString} Section_StartMenu_Desc "צור רשומת תפריט התחלה עבור SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (חובה)"
${LangFileString} Section_MPlayer_Desc "MPlayer; נדרש לשם נגינה."

${LangFileString} Section_MPlayerCodecs "קודקים של בינארי"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "קודקים של בינארי אינם נתמכים בגרסא זו."
!else
${LangFileString} Section_MPlayerCodecs_Desc "קודקי לא מחייבים (רשות) עבור MPlayer. (חיבור אינטרנט נדרש לשם התקנה)"
!endif

${LangFileString} Section_MEncoder_Desc "תוכנית משותפת ליישום MPlayer אשר מסוגלת לשמש כדי לקודד או לשנות זרמי אודיו או וידאו נתמכים."

${LangFileString} Section_IconThemes "סמלי ערכות נושא"
${LangFileString} Section_IconThemes_Desc "סמלי ערכות נושא נוספים עבור SMPlayer."

${LangFileString} Section_Translations "שפות"
${LangFileString} Section_Translations_Desc "קבצים של שפות לא-אנגליות עבור SMPlayer."

${LangFileString} Section_ResetSettings_Desc "מחיקת העדפות SMPlayer אשר נותרו מתוך התקנות קודמות."

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "בחר טיפוס התקנה"
${LangFileString} Reinstall_Header_SubText "בחר צורת שכתוב או ביטול התקנת"

${LangFileString} Reinstall_Msg1 "יש לך התקנה קיימת של SMPlayer בתוך התיקייה הבאה:"
${LangFileString} Reinstall_Msg2 "אנא בחר כיצד להמשיך:"
${LangFileString} Reinstall_Overwrite "שכתב ($Inst_Type) את ההתקנה הקיימת"
${LangFileString} Reinstall_Uninstall "בטל התקנה (הסר) את ההתקנה הקיימת"
${LangFileString} Reinstall_Msg3_1 "לחץ התחל כאשר אתה מוכן להמשיך."
${LangFileString} Reinstall_Msg3_2 "לחץ הבא כאשר אתה מוכן להמשיך."
${LangFileString} Reinstall_Msg3_3 "לחץ בטל התקנה כאשר אתה מוכן להמשיך."
${LangFileString} Reinstall_Msg4 "שנה הגדרות התקנה"
${LangFileString} Reinstall_Msg5 "אפס תצורת SMPlayer"

${LangFileString} Type_Reinstall "התקן מחדש"
${LangFileString} Type_Downgrade "הנמך"
${LangFileString} Type_Upgrade "שדרג"

${LangFileString} StartBtn "התחל"

; Codecs Section
${LangFileString} Codecs_DL_Msg "מוריד קודקים של MPlayer..."
${LangFileString} Codecs_DL_Retry "קודקי MPlayer לא הותקנו בהצלחה. לנסות שוב?"
${LangFileString} Codecs_DL_Failed "נכשל להוריד קודקי MPlayer. סיבה: '$R0'."
${LangFileString} Codecs_Inst_Failed "נכשל להתקין קודקי MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "התקנה זו ניתנת לביטול על ידי משתמש עם הרשאות מנהל."
${LangFileString} Uninstaller_Aborted "ביטל התקנת ננטש על ידי משתמש."
${LangFileString} Uninstaller_NotInstalled "לא נראה כי SMPlayer הינו מותקן בתוך התיקייה '$INSTDIR'.$\r$\nלהמשיך בכל זאת (לא מומלץ)?"
${LangFileString} Uninstaller_InvalidDirectory "התקנת SMPlayer לא נמצאה."
${LangFileString} Uninstaller_64bitOnly "התקנה זו ניתנת לביטול רק על Windows מטיפוס 64-ביט."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer הינו מגשר-חזיתי מקיף עבור MPlayer, מתכונות בסיסיות כגון נגינת סרטוני וידאו, DVD, VCD לתכונות יותר מתקדמות כגון תמיכה עבור מסנני MPlayer, רשימות edl, ועוד."

; Misc
${LangFileString} Info_Codecs_Backup "מגבה קודקים מתוך ההתקנה הקודמת..."
${LangFileString} Info_Codecs_Restore "משחזר קודקים מתוך ההתקנה הקודמת..."
${LangFileString} Info_Del_Files "מוחק קבצים..."
${LangFileString} Info_Del_Registry "מוחק מפתחות Registry..."
${LangFileString} Info_Del_Shortcuts "מוחק קיצורי דרך..."
${LangFileString} Info_Rest_Assoc "משחזר שיוכי קובץ..."
${LangFileString} Info_RollBack "משחזר שינויים..."
${LangFileString} Info_Files_Extract "מחלץ קבצים..."
