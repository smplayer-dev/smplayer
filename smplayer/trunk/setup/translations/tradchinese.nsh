;Language: Chinese (Traditional) (1028)
;Chinese (Traditional) language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "TradChinese" "漢語(繁體)"

; Startup
${LangFileString} Installer_Is_Running "安裝程式已經在執行。"
${LangFileString} Installer_No_Admin "安裝此程式時，您必須先登入為管理員。"
${LangFileString} SMPlayer_Is_Running "一個 SMPlayer 的執行個體正在運轉。請離開的SMPlayer，然後再試一次。"

${LangFileString} OS_Not_Supported "不支援的操作系統。$\nSMPlayer ${SMPLAYER_VERSION} 至少需要Windows XP，及可能無法在您的系統上正常運作。$\n您真的要繼續安裝嗎?"
${LangFileString} Win64_Required "安裝此軟體，系統必須是64位元的 Windows。"
${LangFileString} Existing_32bitInst "一個現有的32位元已安裝 SMPlayer 存在。您必須先解除安裝32位元的 SMPlayer。"
${LangFileString} Existing_64bitInst "一個現有的64位元已安裝 SMPlayer 存在。您必須先解除安裝64位元的 SMPlayer。"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) 設定"
${LangFileString} WelcomePage_Text "設定程序將引導您完成安裝$(^NameDA)。 $\r$\n$\r$\n建議您在開始安裝前，先關閉所有的 SMPlayer 執行實體。這將使它能夠更新相關的程式檔案，而無需重新啟動您的電腦。 $\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "快速鍵"
${LangFileString} MPlayerGroupTitle "MPlayer 元件"
${LangFileString} MPlayerMPVGroupTitle "多媒體引擎"

${LangFileString} Section_SMPlayer "SMPlayer (必須的)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer、共享庫和文件。"

${LangFileString} Section_DesktopShortcut "桌面"
${LangFileString} Section_DesktopShortcut_Desc "在桌面上建立一個 SMPlayer 的捷徑。"

${LangFileString} Section_StartMenu "開始選單"
${LangFileString} Section_StartMenu_Desc "建立ㄧ 個 SMPlayer 的開始選單項目。"

${LangFileString} Section_MPlayer "MPlayer (必須的)"
${LangFileString} Section_MPlayer_Desc "MPlayer；播放所必須的。"

${LangFileString} Section_MPlayerCodecs "二進制解碼器"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "此版本不支援二進制解碼器。"
!else
${LangFileString} Section_MPlayerCodecs_Desc "給 MPlayer 使用的非必須解碼器。( 需要連接網路以進行安裝 )"
!endif

${LangFileString} Section_MPV_Desc "MPlayer 與 MPlayer2 一個功能豐富的分支"

${LangFileString} Section_MEncoder_Desc "一個伴隨安裝的程式 MPlayer，可以用來解碼或轉換支援的聲音檔或影片檔。"

${LangFileString} Section_IconThemes "圖示主題"
${LangFileString} Section_IconThemes_Desc "適用於 SMPlayer 的附加圖示主題。"

${LangFileString} Section_Translations "語言"
${LangFileString} Section_Translations_Desc "適用於 SMPlayer 的非英語語言檔。"

${LangFileString} Section_ResetSettings_Desc "刪除先前所安裝的 SMPlayer 殘留的偏好設定。"

${LangFileString} MPlayer_Codec_Msg "二進制解碼器套件可增加對新 RealVideo 變種及很多不常見格式的支援。$\n請注意，播放像是 DVD、MPEG-1/2/4 這樣平時常見的格式無需安裝本解碼器套件。"

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "選擇安裝類型"
${LangFileString} Reinstall_Header_SubText "選擇「覆蓋」或「解除安裝」模式。"

${LangFileString} Reinstall_Msg1 "您有一個現有的 SMPlayer 安裝位於以下資料夾："
${LangFileString} Reinstall_Msg2 "請選擇如何處理："
${LangFileString} Reinstall_Overwrite "覆蓋($Inst_Type)現有的安裝"
${LangFileString} Reinstall_Uninstall "解除(移除)現有的安裝"
${LangFileString} Reinstall_Msg3_1 "當準備好進行時，點擊「開始」。"
${LangFileString} Reinstall_Msg3_2 "當準備好進行時，點擊「下一步」。"
${LangFileString} Reinstall_Msg3_3 "當準備好進行時，點擊「解除安裝」"
${LangFileString} Reinstall_Msg4 "更改安裝設定"
${LangFileString} Reinstall_Msg5 "重置 SMPlayer 設定"

${LangFileString} Type_Reinstall "重新安裝"
${LangFileString} Type_Downgrade "降級"
${LangFileString} Type_Upgrade "升級"

${LangFileString} StartBtn "開始"

; Codecs Section
${LangFileString} Codecs_DL_Msg "正在下載 MPlayer 解碼器..."
${LangFileString} Codecs_DL_Retry "MPlayer 解碼器沒有成功安裝。是否重試？"
${LangFileString} Codecs_DL_Failed "無法下載 MPlayer 解碼器：「$R0」。"
${LangFileString} Codecs_Inst_Failed "無法安裝 MPlayer 解碼器。"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "安裝只能由擁有管理員權限的使用者卸載。"
${LangFileString} Uninstaller_Aborted "解除安裝被​​使用者終止。"
${LangFileString} Uninstaller_NotInstalled "沒有發現 SMPlayer 被安裝在目錄「$INSTDIR」中。 $\r$\n仍然繼續嗎(不推薦)？"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer 安裝未找到。"
${LangFileString} Uninstaller_64bitOnly "安裝只能在 64 位元 Windows 中解除。"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer 是一個完整的 MPlayer 前端程式，從播放影片檔、DVD、VCD 這樣的基本功能，到進階功能像是支援 MPlayer 過濾器、EDL 清單，及更多。"

; Misc
${LangFileString} Info_Codecs_Backup "正在從以前的安裝中備份解碼器..."
${LangFileString} Info_Codecs_Restore "正在從以前的安裝中恢復解碼器..."
${LangFileString} Info_Del_Files "正在刪除檔案..."
${LangFileString} Info_Del_Registry "正在刪除登錄機碼..."
${LangFileString} Info_Del_Shortcuts "正在刪除捷徑..."
${LangFileString} Info_Rest_Assoc "正在復原檔案關聯..."
${LangFileString} Info_RollBack "正在復原變更..."
${LangFileString} Info_Files_Extract "正在解壓縮檔案..."
${LangFileString} Info_SMTube_Backup "正在備份 SMTube..."
${LangFileString} Info_SMTube_Restore "正在從以前的安裝中恢復 SMTube..."
${LangFileString} Info_MPV_Backup "正在備份 MPV..."
${LangFileString} Info_MPV_Restore "正在從以前的安裝中恢復 MPV..."

; MPV
${LangFileString} MPV_DL_Msg "正在下載 MPV..."
${LangFileString} MPV_DL_Retry "MPV 沒有成功安裝。是否重試？"
${LangFileString} MPV_DL_Failed "無法下載 MPV：「$R0」。"
${LangFileString} MPV_Inst_Failed "無法安裝 MPV。"

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL 沒有成功安裝。是否重試？"
${LangFileString} YTDL_DL_Failed "無法下載 Youtube-DL：「$R0」。"
