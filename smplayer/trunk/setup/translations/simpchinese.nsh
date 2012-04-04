;Language: Chinese (Simplified) (2052)
;Chinese (Simplified) language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "SimpChinese" "简体中文"

; Startup
${LangFileString} Installer_Is_Running "安装程序已经运行。"
${LangFileString} Installer_No_Admin "安装本程序时，您必须以管理员身份登录。"
${LangFileString} SMPlayer_Is_Running "一个 SMPlayer 实例正在运行。请退出 SMPlayer，然后再试一次。"

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "快捷方式"
${LangFileString} MPlayerGroupTitle "MPlayer 组件"

${LangFileString} Section_SMPlayer "SMPlayer (必需)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer 主程序、共享库和文档。"

${LangFileString} Section_DesktopShortcut "桌面"
${LangFileString} Section_DesktopShortcut_Desc "创建 SMPlayer 桌面快捷方式。"

${LangFileString} Section_StartMenu "开始菜单"
${LangFileString} Section_StartMenu_Desc "创建 SMPlayer 开始菜单项。"

${LangFileString} Section_MPlayer "MPlayer (必需)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "播放媒体文件所必需的 MPlayer 内核。"
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "播放媒体文件所必需的 MPlayer 内核。(安装需要使用网络连接)"
!endif

${LangFileString} Section_MPlayerCodecs "二进制编解码器"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "用于 MPlayer 的可选编解码器包。"
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "用于 MPlayer 的可选编解码器包。(安装需要使用网络连接)"
!endif

${LangFileString} Section_MEncoder_Desc "MPlayer 伴侣程序，可用于编码或转换支持的音频/视频流。"

${LangFileString} Section_IconThemes "图标主题"
${LangFileString} Section_IconThemes_Desc "用于 SMPlayer 的附加图标主题。"

${LangFileString} Section_Translations "语言"
${LangFileString} Section_Translations_Desc "用于 SMPlayer 的语言文件 (除英语)。"

${LangFileString} MPlayer_Codec_Msg "二进制编解码器包可增加对新 RealVideo 变种和很多不常见格式的支持。$\n请注意，播放像 DVD, MPEG-1/2/4 这样平时常见的格式无需安装本编解码器包。"

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "选择安装类型"
${LangFileString} Reinstall_Header_SubText "选择覆盖或卸载模式。"

${LangFileString} Reinstall_Msg1 "您有一个已存在的 SMPlayer 安装位于以下文件夹:"
${LangFileString} Reinstall_Msg2 "请选择如何继续:"
${LangFileString} Reinstall_Overwrite "覆盖 ($Inst_Type) 现有的安装"
${LangFileString} Reinstall_Uninstall "卸载 (移除) 现有的安装"
${LangFileString} Reinstall_Msg3_1 "单击“开始”继续。"
${LangFileString} Reinstall_Msg3_2 "单击“下一步”继续。"
${LangFileString} Reinstall_Msg3_3 "单击“卸载”继续。"
${LangFileString} Reinstall_Msg4 "更改安装设置"

${LangFileString} Type_Reinstall "重新安装"
${LangFileString} Type_Downgrade "降级"
${LangFileString} Type_Upgrade "升级"

${LangFileString} StartBtn "开始"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "正在下载 MPlayer..."
${LangFileString} MPlayer_DL_Retry "MPlayer 没有成功安装。是否重试?"
${LangFileString} MPlayer_DL_Failed "无法下载 MPlayer: '$R0'。"
${LangFileString} MPlayer_Inst_Failed "无法安装 MPlayer。要播放媒体文件必须安装 MPlayer。"

; Codecs Section
${LangFileString} Codecs_DL_Msg "正在下载 MPlayer 编解码器..."
${LangFileString} Codecs_DL_Retry "MPlayer 编解码器没有成功安装。是否重试?"
${LangFileString} Codecs_DL_Failed "无法下载 MPlayer 编解码器: '$R0'。"
${LangFileString} Codecs_Inst_Failed "无法安装 MPlayer 编解码器。"

; Version information
${LangFileString} VerInfo_DL_Msg "正在下载版本信息..."
${LangFileString} VerInfo_DL_Failed "无法下载版本信息: '$R0'。将使用默认版本。"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "本安装只能由具有管理员权限的用户卸载。"
${LangFileString} Uninstaller_Aborted "卸载由用户中止。"
${LangFileString} Uninstaller_NotInstalled "没有发现 SMPlayer 被安装在目录 '$INSTDIR' 中。$\r$\n仍要继续吗? (不推荐)"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer 是一个从播放视频、DVD、VCD 等基本特性到支持 MPlayer 过滤器、EDL 列表等高级特性的完整 MPlayer 前端程序。"

; Misc
${LangFileString} Info_Del_Files "正在删除文件..."
${LangFileString} Info_Del_Registry "正在删除注册表项..."
${LangFileString} Info_Del_Shortcuts "正在删除快捷方式..."
${LangFileString} Info_Rest_Assoc "正在恢复文件关联..."
${LangFileString} Info_RollBack "正在回滚更改..."
${LangFileString} Info_Files_Extract "正在提取文件..."
