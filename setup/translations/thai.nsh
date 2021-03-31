;Language: Thai (1054)
;Thai language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Thai" "ไทย"
!else
!insertmacro LANGFILE "Thai" = "ไทย" /* TODO: ISO 11940? */ "Thai"
!endif

; Startup
${LangFileString} Installer_Is_Running "ตัวติดตั้งกำลังรันอยู่แล้ว"
${LangFileString} Installer_No_Admin "คุณต้องลงชื่อเข้าใช้เป็น administrator เมื่อจะติดตั้งโปรแกรมนี้"
${LangFileString} SMPlayer_Is_Running "มี SMPlayer กำลังรันอยู่ โปรดออกจาก SMPlayer แล้วลองอีกครั้ง"

${LangFileString} OS_Not_Supported "ไม่รองรับระบบปฏิบัติการ $\nSMPlayer ${SMPLAYER_VERSION} ต้องการอย่างน้อยคือ Windows XP และอาจจะทำงานได้ไม่ถูกต้องบนระบบของคุณ $\nคุณต้องการติดตั้งต่อไปหรือไม่?"
${LangFileString} OS_Not_Supported_VistaRequired "Unsupported operating system.$\nSMPlayer ${SMPLAYER_VERSION} requires at least Windows Vista and may not work correctly on your system.$\nDo you really want to continue with the installation?"
${LangFileString} Win64_Required "ต้องการระบบปฏิบัติการ 64-bit Windows เพื่อติดตั้งซอฟต์แวร์นี้"
${LangFileString} Existing_32bitInst "มีการติดตั้งแบบ 32-bit ของ SMPlayer อยู่แล้ว คุณต้องถอนการติดตั้งแบบ 32-bit SMPlayer ออกก่อน"
${LangFileString} Existing_64bitInst "มีการติดตั้ง 64-bit ของ SMPlayer อยู่แล้ว คุณต้องถอนการติดตั้งแบบ 64-bit SMPlayer ออกก่อน"

; Welcome page
${LangFileString} WelcomePage_Title "ติดตั้ง $(^NameDA)"
${LangFileString} WelcomePage_Text "ตัวติดตั้งจะแนะนำคุณในการติดตั้ง $(^NameDA).$\r$\n$\r$\nขอแนะนำให้คุณปิดโปรแกรม SMPlayer ก่อนที่จะเริ่มติดตั้ง ซึ่งสิ่งนี้จะทำให้การอัพเดตโปรแกรมเป็นไปได้โดยไม่ต้องบู๊ทคอมพิวเตอร์ของคุณใหม่$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "ทางลัด"
${LangFileString} MPlayerGroupTitle "ส่วนประกอบ MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "SMPlayer (จำเป็น)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, ไลบรารี่ที่แชร์, และเอกสารประกอบ"

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "สร้างทางลัดสู่ SMPlayer บน desktop"

${LangFileString} Section_StartMenu "Start Menu"
${LangFileString} Section_StartMenu_Desc "สร้างทางเข้าที่ Start Menu สำหรับ SMPlayer"

${LangFileString} Section_MPlayer "MPlayer (จำเป็น)"
${LangFileString} Section_MPlayer_Desc "MPlayer; จำเป็นสำหรับการเล่น"

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs ไม่รองรับในเวอร์ชันนี้"
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codecs ทางเลือกสำหรับ MPlayer (ต้องมีการเชื่อมต่ออินเตอร์เน็ตในการติดตั้ง)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_YTDL "Youtube support"
${LangFileString} Section_YTDL_Desc "Provides the possibility to play Youtube videos."

${LangFileString} Section_MEncoder_Desc "โปรแกรมสหายของ MPlayer ที่สามารถใช้เข้ารหัสหรือแปลงกระแสเสียงหรือวิดีโอที่รองรับ"

${LangFileString} Section_IconThemes "รูปลักษณ์ไอคอน"
${LangFileString} Section_IconThemes_Desc "รูปลักษณ์ไอคอนสำหรับ SMPlayer"

${LangFileString} Section_Translations "ภาษา"
${LangFileString} Section_Translations_Desc "ไฟล์ภาษาอื่นที่ไม่ใช่ภาษาอังกฤษสำหรับ SMPlayer"

${LangFileString} Section_ResetSettings_Desc "ลบการตั้งค่าที่ชอบที่เหลืออยู่ของการติดตั้ง SMPlayer ครั้งก่อนหน้า"

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "เลือกประเภทการติดตั้ง"
${LangFileString} Reinstall_Header_SubText "เลือกโหมด เขียนทับ หรือ ถอนการติดตั้ง"

${LangFileString} Reinstall_Msg1 "คุณมีการติดตั้ง SMPlayer อยู่แล้วในโฟลเดอร์ต่อไปนี้:"
${LangFileString} Reinstall_Msg2 "โปรดเลือกว่าจะดำเนินการอย่างไร:"
${LangFileString} Reinstall_Overwrite "เขียนทับ ($Inst_Type) การติดตั้งที่มีอยู่เดิม"
${LangFileString} Reinstall_Uninstall "ถอนการติดตั้ง (ลบออก) การติดตั้งที่มีอยู่เดิม"
${LangFileString} Reinstall_Msg3_1 "คลิก เริ่ม เมื่อพร้อมดำเนินการ"
${LangFileString} Reinstall_Msg3_2 "คลิก ถัดไป เมื่อพร้อมดำเนินการ"
${LangFileString} Reinstall_Msg3_3 "คลิก ถอนการติดตั้ง เมื่อพร้อมดำเนินการ"
${LangFileString} Reinstall_Msg4 "เปลี่ยนการตั้งค่าการติดตั้ง"
${LangFileString} Reinstall_Msg5 "คืนค่าการกำหนดค่า SMPlayer ของฉัน"

${LangFileString} Remove_Settings_Confirmation "Are you sure you want to reset your SMPlayer settings? This action cannot be reversed."

${LangFileString} Type_Reinstall "ติดตั้งอีกครั้ง"
${LangFileString} Type_Downgrade "ลดระดับรุ่น"
${LangFileString} Type_Upgrade "เพิ่มระดับรุ่น"

${LangFileString} StartBtn "เริ่ม"

; Codecs Section
${LangFileString} Codecs_DL_Msg "กำลังดาวน์โหลด MPlayer Codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs ติดตั้งไม่สำเร็จ ลองอีกครั้งหรือไม่?"
${LangFileString} Codecs_DL_Failed "ล้มเหลวในการดาวน์โหลด MPlayer codecs: '$R0'."
${LangFileString} Codecs_Inst_Failed "ล้มเหลวในการติดตั้ง MPlayer codecs"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "การติดตั้งนี้สามารถถอนการติดตั้งได้เฉพาะผู้ใช้ที่มีสิทธิเป็น administrator"
${LangFileString} Uninstaller_Aborted "การถอนการติดตั้ง ถูกยกเลิกโดยผู้ใช้"
${LangFileString} Uninstaller_NotInstalled "ไม่ปรากฏว่า SMPlayer ได้ถูกติดตั้งในเส้นทาง '$INSTDIR'.$\r$\nจะทำต่อไปหรือเปล่า (ไม่แนะนำ)?"
${LangFileString} Uninstaller_InvalidDirectory "ไม่พบการติดตั้ง SMPlayer"
${LangFileString} Uninstaller_64bitOnly "การติดตั้งนี้สามารถถอนการติดตั้งเฉพาะบน 64-bit Windows"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer คือ front-end ที่สมบูรณ์สำหรับ MPlayer, มีคุณลักษณะพื้นฐานอย่างเช่น การเล่นวิดีโอ, DVD, VCD จนถึงคุณลักษณะขั้นสูงอย่างเช่น การรองรับสำหรับตัวกรอง MPlayer, บัญชี edl, และอื่นๆ"

; Misc
${LangFileString} Info_Codecs_Backup "กำลังสำรอง codecs จากการติดตั้งก่อนหน้า..."
${LangFileString} Info_Codecs_Restore "กำลังคืนค่าเดิมของ codecs จากการติดตั้งก่อนหน้า"
${LangFileString} Info_Del_Files "กำลังลบไฟล์..."
${LangFileString} Info_Del_Registry "กำลังลบคีย์ทะเบียน..."
${LangFileString} Info_Del_Shortcuts "กำลังลบทางลัด..."
${LangFileString} Info_Rest_Assoc "กำลังคืนค่าการสัมพันธ์ไฟล์..."
${LangFileString} Info_RollBack "กำลังคืนกลับการเปลี่ยนแปลง..."
${LangFileString} Info_Files_Extract "กำลังแยกไฟล์..."
${LangFileString} Info_SMTube_Backup "Backing up SMTube..."
${LangFileString} Info_SMTube_Restore "Restoring SMTube from previous installation..."
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
