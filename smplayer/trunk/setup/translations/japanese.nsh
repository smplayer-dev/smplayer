;Language: Japanese (1041)
;Japanese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Japanese" "日本語"

; Startup
${LangFileString} Installer_Is_Running "インストーラーは既に実行中です。"
${LangFileString} Installer_No_Admin "このプログラムをインストールするときは管理者としてログインする必要があります。"
${LangFileString} SMPlayer_Is_Running "SMPlayer のインスタンスが実行中です。SMPlayer を終了してやり直してください。"

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "ショートカット"
${LangFileString} MPlayerGroupTitle "MPlayer コンポーネント"

${LangFileString} Section_SMPlayer "SMPlayer (必須)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer、共有ライブラリ、およびドキュメントです。"

${LangFileString} Section_DesktopShortcut "デスクトップ"
${LangFileString} Section_DesktopShortcut_Desc "デスクトップに SMPlayer へのショートカットを作成します。"

${LangFileString} Section_StartMenu "[スタート] メニュー"
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

${LangFileString} Section_MEncoder_Desc "サポートされたオーディオまたはビデオ ストリームのエンコードや変換に使用できる MPlayer の姉妹プログラムです。"

${LangFileString} Section_IconThemes "アイコン テーマ"
${LangFileString} Section_IconThemes_Desc "SMPlayer の追加アイコン テーマです。"

${LangFileString} Section_Translations "言語"
${LangFileString} Section_Translations_Desc "SMPlayer の英語以外のの言語ファイルです。"

${LangFileString} MPlayer_Codec_Msg "バイナリ コーデック パッケージは新しい RealVideo 派生や多くの希少なフォーマットなど、まだ自然には実装されていないコーデックへのサポートを追加します。$\nDVD、MPEG-1/2/4 などの最も一般的なフォーマットの再生には必要でないことにご注意ください。"

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "インストールの種類の選択"
${LangFileString} Reinstall_Header_SubText "上書きまたはアンインストール モードを選択します。"

${LangFileString} Reinstall_Msg1 "以下のフォルダーに既存 SMPlayer のインストールがあります:"
${LangFileString} Reinstall_Msg2 "続行する方法を選択してください:"
${LangFileString} Reinstall_Overwrite "既存のインストールを上書き ($Inst_Type) する"
${LangFileString} Reinstall_Uninstall "既存のインストールをアンインストール (削除) する"
${LangFileString} Reinstall_Msg3_1 "続行するには [開始] をクリックします。"
${LangFileString} Reinstall_Msg3_2 "続行するには [次へ] をクリックします。"
${LangFileString} Reinstall_Msg3_3 "続行するには [アンインストール] をクリックします。"
${LangFileString} Reinstall_Msg4 "インストール設定を変更する"

${LangFileString} Type_Reinstall "再インストール"
${LangFileString} Type_Downgrade "ダウングレード"
${LangFileString} Type_Upgrade "アップグレード"

${LangFileString} StartBtn "開始"

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
${LangFileString} VerInfo_DL_Failed "バージョン情報のダウンロードに失敗しました: '$R0'。既定のバージョンを使用します。"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "このインストールは管理者特権のあるユーザーによってのみアンインストールできます。"
${LangFileString} Uninstaller_Aborted "アンインストールはユーザーによって中止されました。"
${LangFileString} Uninstaller_NotInstalled "ディレクトリ '$INSTDIR' に SMPlayer がインストールされているように見えません。$\r$\nこのまま続行しますか (推奨されません)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer はビデオ、DVD、VCD の再生のような基本的な機能から MPlayer フィルター、edl リストなどへのサポートのような高度な機能まで、MPlayer の完全なフロントエンドです。"

; Misc
${LangFileString} Info_Del_Files "ファイルを削除しています..."
${LangFileString} Info_Del_Registry "レジストリ キーを削除しています..."
${LangFileString} Info_Del_Shortcuts "ショートカットを削除しています..."
${LangFileString} Info_Rest_Assoc "ファイルの関連付けを復元しています..."
${LangFileString} Info_RollBack "変更を元に戻しています..."
${LangFileString} Info_Files_Extract "ファイルを展開しています..."
