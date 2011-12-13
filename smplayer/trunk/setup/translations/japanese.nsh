;Language: Japanese (1041)
;Japanese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Japanese" "日本語"

; Startup
${LangFileString} Installer_Is_Running "インストーラーは既に実行中です。"
${LangFileString} Installer_No_Admin "このプログラムのインストール時には管理者としてログインする必要があります。"

; Components Page
${LangFileString} ShortcutGroupTitle "ショートカット"
${LangFileString} MPlayerGroupTitle "MPlayer コンポーネント"

${LangFileString} Section_SMPlayer "SMPlayer (必須)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer、共有ライブラリ、およびドキュメントです。"

${LangFileString} Section_DesktopShortcut "デスクトップ"
${LangFileString} Section_DesktopShortcut_Desc "デスクトップに SMPlayer へのショートカットを作成します。"

${LangFileString} Section_StartMenu "スタートメニュー"
${LangFileString} Section_StartMenu_Desc "SMPlayer の [スタート] メニュー エントリを作成します。"

${LangFileString} Section_MPlayer "MPlayer (必須)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "再生のために必要な MPlayer です。"
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "再生のために必要な MPlayer です。(インストールにはインターネット接続が必要です)"
!endif

${LangFileString} Section_MPlayerCodecs "バイナリ コーデック"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "MPlayer のオプション コーデックです。"
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "MPlayer のオプション コーデックです。(インストールにはインターネット接続が必要です)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "アイコン テーマ"
${LangFileString} Section_IconThemes_Desc "SMPlayer の追加アイコン テーマです。"

${LangFileString} Section_Translations "ローカライズ"
${LangFileString} Section_Translations_Desc "非英語のローカライズです。"

${LangFileString} MPlayer_Codec_Msg "バイナリ コーデック パッケージは新しい RealVideo 派生や多くの希少なフォーマットなど、まだ自然には実装されていないコーデックへのサポートを追加します。$\nDVD、MPEG-1/2/4などの最も一般的なフォーマットの再生には必要でないことにご注意ください。"

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

${LangFileString} StartBtn "スタート"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "MPlayer をダウンロードしています..."
${LangFileString} MPlayer_DL_Retry "MPlayer は正常にインストールされませんでした。再試行しますか?"
${LangFileString} MPlayer_DL_Failed "MPlayer のダウンロードに失敗しました: '$R0'。"
${LangFileString} MPlayer_Inst_Failed "MPlayer のインストールに失敗しました。MPlayer は再生に必要です。"

; Codecs Section
${LangFileString} Codecs_DL_Msg "MPlayer コーデックをダウンロードしています..."
${LangFileString} Codecs_DL_Retry "MPlayer コーデックは正常にインストールされませんでした。再試行しますか?"
${LangFileString} Codecs_DL_Failed "MPlayer コーデックのダウンロードに失敗しました: '$R0'。"
${LangFileString} Codecs_Inst_Failed "MPlayer コーデックのインストールに失敗しました。"

; Version information
${LangFileString} VerInfo_DL_Msg "バージョン情報をダウンロードしています..."
${LangFileString} VerInfo_DL_Failed "バージョン情報のダウンロードに失敗しました: '$R0'。既定のバージョンを使用しています。"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "このインストールは管理者特権のあるユーザーによってのみアンインストールできます。"
${LangFileString} Uninstaller_Aborted "アンインストールはユーザーによって注視されました。"
${LangFileString} Uninstaller_NotInstalled "ディレクトリ '$INSTDIR' に SMPlayer がインストールされているように見えません。$\r$\nこのまま続行しますか (推奨されません)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer はビデオ、DVD、VCD の再生のような基本的な機能から MPlayer フィルター、edl リストなどへのサポートのような高度な機能まで、MPlayer の完全なフロントエンドです。"

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_Files_Extract "Extracting files..."
