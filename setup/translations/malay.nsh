;Language: Malay (1086)
;Malay language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Malay" "Melayu"
!else
!insertmacro LANGFILE "Malay" = "Bahasa Melayu" =
!endif

; Startup
${LangFileString} Installer_Is_Running "Pemasang sudah berjalan."
${LangFileString} Installer_No_Admin "Anda mesti mendaftar masuk sebagai pentadbir bila memasang program ini."
${LangFileString} SMPlayer_Is_Running "Satu kejadian SMPlayer sedang berjalan. Sila keluar SMPlayer dan cuba lagi."

${LangFileString} OS_Not_Supported "Kesian , Sistem pengoperasian tidak disokong.$\nSMPlayer ${SMPLAYER_VERSION} memerlukan sekurang-kurangnya Windows XP dan tidak dapat berfungsi dengan baik pada sistem anda.$\nAnda pasti mahu teruskan pemasangan?"
${LangFileString} OS_Not_Supported_VistaRequired "Sistem pengoperasian tidak disokong.$\nSMPlayer ${SMPLAYER_VERSION} memerlukan sekurang-kurangnya Windows Vista dan tidak dapat berfungsi dengan baik pada sistem anda.$\nAnda pasti mahu teruskan pemasangan?"
${LangFileString} Win64_Required "Sistem pengoperasian Windows 64-bit diperlukan untuk memasang perisian ini."
${LangFileString} Existing_32bitInst "Pemasangan 32-bit SMPlayer sudah wujud. Anda mesti nyahpasang SMPlayer 32-bit dahulu."
${LangFileString} Existing_64bitInst "Pemasangan 64-bit SMPlayer sudah wujud. Anda mesti nyahpasang SMPlayer 64-bit dahulu."

; Welcome page
${LangFileString} WelcomePage_Title "Persediaan $(^NameDA)"
${LangFileString} WelcomePage_Text "Persediaan akan membantu anda melalui pemasangan $(^NameDA).$\r$\n$\r$\nAdalah disarankan anda tutup semua kejadian SMPlayer sebelum memulakan persediaan. Ia bertujuan untuk mengemaskini fail program yang berkaitan tanpa perlu membut semula komputer anda.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Pintasan"
${LangFileString} MPlayerGroupTitle "Komponen MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Enjin Multimedia"

${LangFileString} Section_SMPlayer "SMPlayer (diperlukan)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, pustaka terkongsi, dan dokumentasi."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Cipta pintasan pada SMPlayer pada desktop."

${LangFileString} Section_StartMenu "Menu Mula"
${LangFileString} Section_StartMenu_Desc "Cipta masukan Menu Mula untuk SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (diperlukan)"
${LangFileString} Section_MPlayer_Desc "MPlayer; diperlukan untuk mainbalik."

${LangFileString} Section_MPlayerCodecs "Kodeks Binari"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Kodeks binari tidak disokong dalam versi ini."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Kodeks pilihan untuk MPlayer. (Sambungan Internet diperlukan untuk pemasangan)"
!endif

${LangFileString} Section_MPV_Desc "Cabang kaya-fitur bagi MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Program yang diperlukan oleh MPlayer yang dapat digunakan untuk mengenkod atau jelma audio disokong atau strim video."

${LangFileString} Section_IconThemes "Tema Ikon"
${LangFileString} Section_IconThemes_Desc "Tema ikon tambahan untuk SMPlayer."

${LangFileString} Section_Translations "Bahasa"
${LangFileString} Section_Translations_Desc "Fail bahasa Bukan-Inggeris untuk SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Padam baki keutamaan SMPlayer dari pemasangan terdahulu."

${LangFileString} MPlayer_Codec_Msg "Pakej kodeks binari menambah sokongan untuk kodeks yang belum lagi dilaksana secara tabii, seperti varians RealVideo lebih baharu dan banyak lagi format bukan umum..$\nPerhatian format tersebut tidaklah diperlukan untuk memainkan kebanyakan format umum seperti DVDs, MPEG-1/2/4, dll."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Pilih Jenis Pemasangan"
${LangFileString} Reinstall_Header_SubText "Pilih mod Tulis-ganti atau Nyahpasang."

${LangFileString} Reinstall_Msg1 "Anda sudah mempunyai pemasangan SMPlayer di dalam folder berikut:"
${LangFileString} Reinstall_Msg2 "Sila pilih bagaimana hendak diteruskan:"
${LangFileString} Reinstall_Overwrite "Tulis-ganti pemasangan sedia ada ($Inst_Type)"
${LangFileString} Reinstall_Uninstall "Nyapasang (buang) pemasangan sedia ada"
${LangFileString} Reinstall_Msg3_1 "Klik Mula bila sedia diteruskan."
${LangFileString} Reinstall_Msg3_2 "Klik Berikutnya bila sedia diteruskan."
${LangFileString} Reinstall_Msg3_3 "Klik Nyahpasang bila sedia diteruskan."
${LangFileString} Reinstall_Msg4 "Ubah Tetapan Pemasangan"
${LangFileString} Reinstall_Msg5 "Tetap semula konfigurasi SMPlayer saya"

${LangFileString} Remove_Settings_Confirmation "Anda pasti mahu menetap semula tetapan SMPlayer anda? Tindakan ini tidak boleh dikembalikan."

${LangFileString} Type_Reinstall "pasang semula"
${LangFileString} Type_Downgrade "nyahtatar"
${LangFileString} Type_Upgrade "tatar"

${LangFileString} StartBtn "Mula"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Memuat Turun Kodeks MPlayer..."
${LangFileString} Codecs_DL_Retry "Kodeks MPlayer gagal dipasang. Cuba lagi?"
${LangFileString} Codecs_DL_Failed "Gagal memuat turun kodeks MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Gagal memasang kodeks MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Pemasangan ini hanya boleh dinyahpasang oleh pengguna dengan kelayakan pentadbir."
${LangFileString} Uninstaller_Aborted "Nyahpasang dihenti-paksa oleh pengguna."
${LangFileString} Uninstaller_NotInstalled "Kelihatan SMPlayer tidak dipasang dalam direktori '$INSTDIR'.$\r$\nTeruskan jua (tidak disarankan)?"
${LangFileString} Uninstaller_InvalidDirectory "Pemasangan SMPlayer tidak ditemui."
${LangFileString} Uninstaller_64bitOnly "Pemasangan ini hanya boleh dinyahpasang pada Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer merupakan bahagian-hadapan MPlayer, dari fitur asas seperti memainkan video, DVD. VCD sehinggalah fitur lanjutan seperti sokongan untuk penapis MPlayer, senarai edl, dan banyak lagi."

; Misc
${LangFileString} Info_Codecs_Backup "Menyandar kodeks dari pemasangan terdahulu..."
${LangFileString} Info_Codecs_Restore "Memulihkan kodeks dari pemasangan terdahulu..."
${LangFileString} Info_Del_Files "Memadam Fail..."
${LangFileString} Info_Del_Registry "Memadam Kunci Registry..."
${LangFileString} Info_Del_Shortcuts "Memadam Pintasan..."
${LangFileString} Info_Rest_Assoc "Memulihkan perkaitan fail..."
${LangFileString} Info_RollBack "Mengembali perubahan..."
${LangFileString} Info_Files_Extract "Mengekstrak fail..."
${LangFileString} Info_SMTube_Backup "Menyandar SMTube..."
${LangFileString} Info_SMTube_Restore "Memulihkan SMTube dari pemasangan terdahulu..."
${LangFileString} Info_MPV_Backup "Menyandar mpv..."
${LangFileString} Info_MPV_Restore "Memulihkan mpv dari pemasangan terdahulu..."

; MPV
${LangFileString} MPV_DL_Msg "Memuat turun mpv..."
${LangFileString} MPV_DL_Retry "mpv gagal dipasang. Cuba lagi?"
${LangFileString} MPV_DL_Failed "Gagal memuat turun mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Gagal memasang mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl gagal dipasang. Cuba lagi?"
${LangFileString} YTDL_DL_Failed "Gagal memuat turun youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Memeriksa kemaskini youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Membersihkan cache konfig fon..."
${LangFileString} Info_Cleaning_SMPlayer "Membersihkan tetapan SMPlayer..."
