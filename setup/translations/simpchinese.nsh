;Language: Chinese (Simplified) (2052)
;Chinese (Simplified) language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "SimpChinese" "简体中文"

; Startup
${LangFileString} Installer_Is_Running "安装程序已在运行。"
${LangFileString} Installer_No_Admin "安装此程序时，您必须以管理员身份登录。"
${LangFileString} SMPlayer_Is_Running "SMPlayer 的实例正在运行。请退出 SMPlayer，然后重试。"

${LangFileString} OS_Not_Supported "不支持的操作系统。$\nSMPlayer ${SMPLAYER_VERSION} 至少需要 Windows XP，其在您的系统上可能无法正常工作。$\n您真的要继续安装吗?"
${LangFileString} Win64_Required "安装此软件需要 64 位 Windows 操作系统。"
${LangFileString} Existing_32bitInst "SMPlayer 的 32 位安装已存在。您必须先卸载 32 位的 SMPlayer。"
${LangFileString} Existing_64bitInst "SMPlayer 的 64 位安装已存在。您必须先卸载 64 位的 SMPlayer。"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) 设置"
${LangFileString} WelcomePage_Text "设置程序将引导您完成安装 $(^NameDA)。$\r$\n$\r$\n建议您在开始安装前关闭所有的 SMPlayer 实例。这将使其能够更新相关的程序文件，而无需重新启动您的计算机。$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "快捷方式"
${LangFileString} MPlayerGroupTitle "MPlayer 组件"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "SMPlayer (必需)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer、共享库和文档。"

${LangFileString} Section_DesktopShortcut "桌面"
${LangFileString} Section_DesktopShortcut_Desc "创建 SMPlayer 桌面快捷方式。"

${LangFileString} Section_StartMenu "开始菜单"
${LangFileString} Section_StartMenu_Desc "创建 SMPlayer 开始菜单项。"

${LangFileString} Section_MPlayer "MPlayer (必需)"
${LangFileString} Section_MPlayer_Desc "MPlayer；播放所必需的 。"

${LangFileString} Section_MPlayerCodecs "二进制编解码器"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "此版本不支持二进制解码器"
!else
${LangFileString} Section_MPlayerCodecs_Desc "适用于 MPlayer 的可选解码器。(安装需要使用网络连接)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "MPlayer 的配套程序，可以用来编码或转换支持的音频/视频流。"

${LangFileString} Section_IconThemes "图标主题"
${LangFileString} Section_IconThemes_Desc "适用于 SMPlayer 的附加图标主题。"

${LangFileString} Section_Translations "语言"
${LangFileString} Section_Translations_Desc "适用于 SMPlayer 的非英语语言文件。"

${LangFileString} Section_ResetSettings_Desc "删除上次安装的残留配置"

${LangFileString} MPlayer_Codec_Msg "二进制编解码器包可增加对新 RealVideo 变种和很多不常见格式的支持。$\n请注意，播放像 DVD、MPEG-1/2/4 这样平时常见的格式无需安装本编解码器包。"

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "选择安装类型"
${LangFileString} Reinstall_Header_SubText "选择『覆盖』或『卸载』模式。"

${LangFileString} Reinstall_Msg1 "您有一个现有的 SMPlayer 安装位于以下文件夹:"
${LangFileString} Reinstall_Msg2 "请选择如何继续:"
${LangFileString} Reinstall_Overwrite "覆盖($Inst_Type)现有的安装"
${LangFileString} Reinstall_Uninstall "卸载(移除)现有的安装"
${LangFileString} Reinstall_Msg3_1 "单击『开始』继续。"
${LangFileString} Reinstall_Msg3_2 "单击『下一步』继续。"
${LangFileString} Reinstall_Msg3_3 "单击『卸载』继续。"
${LangFileString} Reinstall_Msg4 "更改安装设置"
${LangFileString} Reinstall_Msg5 "重置 SMPlayer 设置"

${LangFileString} Type_Reinstall "重新安装"
${LangFileString} Type_Downgrade "降级"
${LangFileString} Type_Upgrade "升级"

${LangFileString} StartBtn "开始"

; Codecs Section
${LangFileString} Codecs_DL_Msg "正在下载 MPlayer 编解码器..."
${LangFileString} Codecs_DL_Retry "MPlayer 编解码器没有成功安装。是否重试?"
${LangFileString} Codecs_DL_Failed "无法下载 MPlayer 编解码器: 『$R0』。"
${LangFileString} Codecs_Inst_Failed "无法安装 MPlayer 编解码器。"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "安装只能由拥有管理员权限的用户卸载。"
${LangFileString} Uninstaller_Aborted "卸载被用户终止。"
${LangFileString} Uninstaller_NotInstalled "没有发现 SMPlayer 被安装在目录『$INSTDIR』中。$\r$\n仍然继续吗(不推荐)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer 安装未找到"
${LangFileString} Uninstaller_64bitOnly "安装只能在 64 位 Windows 中卸载。"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer 是一个完备的 MPlayer 前端，从像播放视频、DVD、VCD 这样的基本功能，到支持 MPlayer 过滤器、EDL 列表等更多的高级功能。"

; Misc
${LangFileString} Info_Codecs_Backup "正在从以前的安装中备份解码器..."
${LangFileString} Info_Codecs_Restore "正在从以前的安装中恢复编解码器..."
${LangFileString} Info_Del_Files "正在删除文件..."
${LangFileString} Info_Del_Registry "正在删除注册表项..."
${LangFileString} Info_Del_Shortcuts "正在删除快捷方式..."
${LangFileString} Info_Rest_Assoc "正在恢复文件关联..."
${LangFileString} Info_RollBack "正在回滚更改..."
${LangFileString} Info_Files_Extract "正在提取文件..."
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
