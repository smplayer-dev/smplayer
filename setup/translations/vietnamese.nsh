;Language: Vietnamese (1066)
;Vietnamese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Vietnamese" "Tiếng Việt"
!else
!insertmacro LANGFILE "Vietnamese" = "Tiếng Việt" "Tieng Viet"
!endif

; Startup
${LangFileString} Installer_Is_Running "Trình cài đặt đang chạy."
${LangFileString} Installer_No_Admin "Bạn phải đăng nhập tài khoản quản trị viên khi cài đặt chương trình này."
${LangFileString} SMPlayer_Is_Running "Một tiến trình của SMPlayer đang chạy. Hãy thoát SMPlayer và thử lại."

${LangFileString} OS_Not_Supported "Hệ điều hành không được hỗ trợ.$\nSMPlayer ${SMPLAYER_VERSION} yêu cầu thấp nhất là Windows XP và có thể không hoạt động đúng cách trên hệ thống của bạn.$\nBạn thật sự muốn tiếp tục cài đặt?"
${LangFileString} OS_Not_Supported_VistaRequired "Hệ điều hành không được hỗ trợ.$\nSMPlayer ${SMPLAYER_VERSION} Yêu cầu Windows Vista hoặc mới hơn, có thể không hoạt động đúng cách trên hệ thống của bạn.$\nBạn thật sự muốn tiếp tục cài đặt?"
${LangFileString} Win64_Required "Yêu cầu hệ điều hành Windows 64-bit để cài đặt phần mềm này."
${LangFileString} Existing_32bitInst "Bản cài đặt 32-bit của SMPlayer đang tồn tại. Bạn phải gỡ bỏ bản 32-bit của SMPlayer trước."
${LangFileString} Existing_64bitInst "Bản cài đặt 64-bit của SMPlayer đang tồn tại. Bạn phải gỡ bỏ bản 64-bit của SMPlayer trước."

; Welcome page
${LangFileString} WelcomePage_Title "Cài đặt $(^NameDA)"
${LangFileString} WelcomePage_Text "Trình cài đặt sẽ hướng dẫn bạn trong quá trình cài đặt $(^NameDA).$\r$\n$\r$\nChúng tôi khuyên bạn nên đóng tất cả các tác vụ của SMPlayer trước khi bắt đầu cài đặt. Việc này sẽ giúp cập nhật những tập tin chương trình có liên quan mà không cần khởi động lại máy tính.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Lối tắt"
${LangFileString} MPlayerGroupTitle "Các thành phần của MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Động cơ đa phương tiện"

${LangFileString} Section_SMPlayer "SMPlayer (yêu cầu)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, thư viện và tài liệu được chia sẻ."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Tạo lối tắt đến SMPlayer trên desktop."

${LangFileString} Section_StartMenu "Menu Bắt đầu"
${LangFileString} Section_StartMenu_Desc "Tạo mục Menu Bắt đầu cho SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (yêu cầu)"
${LangFileString} Section_MPlayer_Desc "MPlayer; yêu cầu cho việc phát lại."

${LangFileString} Section_MPlayerCodecs "Bộ mã nhị phân"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Các bộ mã nhị phân chưa được hỗ trợ trong phiên bản này."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Những bộ mã tuỳ chọn cho MPlayer. (Yêu cầu có kết nối Internet để cài đặt)"
!endif

${LangFileString} Section_MPV_Desc "Một bản sao đa tính năng của MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Một chương trình đi kèm với MPlayer, có thể dùng để chuyển mã hoặc chuyển đổi dòng dữ liệu âm thanh hoặc video được hỗ trợ."

${LangFileString} Section_IconThemes "Chủ đề biểu tượng"
${LangFileString} Section_IconThemes_Desc "Những chủ đề biểu tượng khác cho SMPlayer."

${LangFileString} Section_Translations "Ngôn ngữ"
${LangFileString} Section_Translations_Desc "Những tập tin ngôn ngữ không phải tiếng Anh cho SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Xoá những cài đặt SMPlayer còn sót lại từ những lần cài đặt trước đây."

${LangFileString} MPlayer_Codec_Msg "Những gói bộ mã nhị phân thêm hỗ trợ bộ mã không được tích hợp tự nhiên, như các biến thể mới của RealVideo và nhiều định dạng không phổ biến khác.$\nLưu ý rằng chúng không cần thiết để phát những định dạng phổ biến như DVD, MPEG-1/2/4..."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Chọn cách cài đặt"
${LangFileString} Reinstall_Header_SubText "Chọn Chế độ Thay thế hoặc Gỡ bỏ."

${LangFileString} Reinstall_Msg1 "Bạn đã có một bản cài đặt SMPlayer trong thư mục sau:"
${LangFileString} Reinstall_Msg2 "Hãy chọn cách tiến hành:"
${LangFileString} Reinstall_Overwrite "Thay thế ($Inst_Type) bản cài đặt đang có"
${LangFileString} Reinstall_Uninstall "Gỡ bỏ (xoá) bản cài đặt đang có"
${LangFileString} Reinstall_Msg3_1 "Bấm Bắt đầu khi sẵn sàng tiến hành."
${LangFileString} Reinstall_Msg3_2 "Bấm Tiếp theo khi sẵn sàng tiến hành."
${LangFileString} Reinstall_Msg3_3 "Bấm Gỡ bỏ khi sẵn sàng tiến hành."
${LangFileString} Reinstall_Msg4 "Thay đổi Thiết lập Cài đặt"
${LangFileString} Reinstall_Msg5 "Đặt lại cấu hình SMPlayer của tôi"

${LangFileString} Remove_Settings_Confirmation "Bạn có thật sự muốn xoá thiết lập của SMPlayer không? Sau khi đã xoá sẽ không thể khôi phục lại"

${LangFileString} Type_Reinstall "cài lại"
${LangFileString} Type_Downgrade "hạ cấp"
${LangFileString} Type_Upgrade "nâng cấp"

${LangFileString} StartBtn "Bắt đầu"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Đang tải xuống bộ mã MPlayer..."
${LangFileString} Codecs_DL_Retry "Các bộ mã MPlayer chưa được cài đặt thành công. Thử lại?"
${LangFileString} Codecs_DL_Failed "Không tải tải xuống các bộ mã MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Không thể cài đặt các bộ mã MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Bản cài đặt này chỉ có thể được gỡ bỏ bởi người dùng có quyền quản trị."
${LangFileString} Uninstaller_Aborted "Quá trình gỡ bỏ bị huỷ bỏ bởi người dùng."
${LangFileString} Uninstaller_NotInstalled "Có vẻ như SMPlayer không được cài đặt trong thư mục '$INSTDIR'.$\r$\nCứ tiếp tục (không khuyến cáo)?"
${LangFileString} Uninstaller_InvalidDirectory "Không phát hiện bản cài đặt SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Bản cài đặt này chỉ có thể được gỡ bỏ trên Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer là bản hoàn thiện của MPlayer cho người dùng, từ những tính năng cơ bản như phát video, DVD, VCD đến nhiều tính năng nâng cao như hỗ trợ những bộ lọc MPlayer, danh sách edl và nhiều hơn nữa."

; Misc
${LangFileString} Info_Codecs_Backup "Đang sao lưu các bộ mã từ bản cài đặt trước..."
${LangFileString} Info_Codecs_Restore "Đang phục hồi các bộ mã  từ bản cài đặt trước..."
${LangFileString} Info_Del_Files "Đang xoá tập tin..."
${LangFileString} Info_Del_Registry "Đang xoá khoá registry..."
${LangFileString} Info_Del_Shortcuts "Đang xoá lối tắt..."
${LangFileString} Info_Rest_Assoc "Đang phục hồi liên kết tập tin..."
${LangFileString} Info_RollBack "Đang phục hồi thay đổi..."
${LangFileString} Info_Files_Extract "Đang trích xuất tập tin..."
${LangFileString} Info_SMTube_Backup "Đang sao lưu SMTube..."
${LangFileString} Info_SMTube_Restore "Đang phục hồi SMTube từ bản cài đặt trước..."
${LangFileString} Info_MPV_Backup "Sao lưu mpv..."
${LangFileString} Info_MPV_Restore "Phục hồi MPV từ lần cài đặt trước..."

; MPV
${LangFileString} MPV_DL_Msg "Đang tải MPV xuống..."
${LangFileString} MPV_DL_Retry "mpv chưa được cài đặt thành công. Thử lại?"
${LangFileString} MPV_DL_Failed "Không thể tải xuống mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Không thể cài đặt mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl chưa được cài đặt thành công. Thử lại?"
${LangFileString} YTDL_DL_Failed "Không thể tải xuống youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Đang tìm cập nhật cho youtube-dl"
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Đang xoá cache của fontconfig..."
${LangFileString} Info_Cleaning_SMPlayer "Đang xoá thiết lập của SMPlayer..."
