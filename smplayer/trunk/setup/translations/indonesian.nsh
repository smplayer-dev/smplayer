;Language: Indonesian (1057)
;Indonesian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Indonesian" "Bahasa Indonesia"
!else
!insertmacro LANGFILE "Indonesian" = "Bahasa Indonesia" =
!endif

; Startup
${LangFileString} Installer_Is_Running "Pemasang telah berjalan."
${LangFileString} Installer_No_Admin "Anda harus masuk sebagai pengurus ketika memasang program."
${LangFileString} SMPlayer_Is_Running "Sebuah instansi SMPlayer masih berjalan. Tolong tutup SMPlayer dan coba lagi."

${LangFileString} OS_Not_Supported "Sistem operasi tidak didukung.$\nSMPlayer ${SMPLAYER_VERSION} membutuhkan setidaknya Windows XP dan mungkin tidak bekerja dengan baik pada sistem Anda.$\n Apakah Anda ingin melanjutkan pemasangan?"
${LangFileString} OS_Not_Supported_VistaRequired "Sistem operasi tak didukung.$\nSMPlayer ${SMPLAYER_VERSION} perlu di Windows Vista terkini dan mungkin tidak bekerja dengan benar pada sistemmu.$\nApakah kamu benar-benar ingin melanjutkan pemasangan?"
${LangFileString} Win64_Required "Sistem operasi Windows 64-bit diperlukan untuk memasang perangkat lunak ini."
${LangFileString} Existing_32bitInst "Pemasang SMPlayer 32-bit masih terpasang. Anda harus mencopot SMPlayer 32-bit terlebih dahulu."
${LangFileString} Existing_64bitInst "SMPlayer 64-bit masih terpasang. Anda harus mencopot SMPlayer 64-bit terlebih dahulu."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Wisaya"
${LangFileString} WelcomePage_Text "Wisaya ini akan memandu Anda melalui pemasangan $(^NameDA).$\r$\n$\r$\n Disarankan agar Anda menutup semua instansi SMPlayer sebelum memulai wisaya. Hal ini akan memungkinkan untuk memperbarui berkas program tanpa harus memulai ulang komputer Anda.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Pintasan"
${LangFileString} MPlayerGroupTitle "Komponen MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Mesin Multimedia"

${LangFileString} Section_SMPlayer "SMPlayer (diperlukan)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, pustaka berbagi dan dokumentasi."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Buat pintasan untuk SMPlayer pada desktop."

${LangFileString} Section_StartMenu "Start Menu"
${LangFileString} Section_StartMenu_Desc "Buat sebuah entri Start Menu untuk SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (dibutuhkan)"
${LangFileString} Section_MPlayer_Desc "MPlayer; dibutuhkan untuk pemutaran."

${LangFileString} Section_MPlayerCodecs "Kodek Binari"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Kodek binari tidak didukung pada versi ini."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Kodek tambahan untuk MPlayer. (Koneksi Internet diperlukan untuk pemasangan)"
!endif

${LangFileString} Section_MPV_Desc "Sebuah cabang yang kaya fitur dari MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Teman Aplikasi dari MPlater yang dapat menggunakan encode atau transformasi audio yang didukung atau video streaming."

${LangFileString} Section_IconThemes "Tema Ikon"
${LangFileString} Section_IconThemes_Desc "Tema ikon tambahan untuk SMPlayer."

${LangFileString} Section_Translations "Bahasa"
${LangFileString} Section_Translations_Desc "Berkas Bahasa Non-Inggris untuk SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Hapus pengaturan SMPlayer yang tersisa ada pemasangan sebelumnya."

${LangFileString} MPlayer_Codec_Msg "Paket kodek binari menambahkan dukungan untuk kodek yang tidak dijalankan secara umum, seperti varian terbaru RealVideo dan berbagai format lainnya yang belum dapat diputar.$\nCatat bahwa semua kodek itu tidak diperlukan untuk memainkan format yang sering dipakai seperti DVD, MPEG-1/2/4, dsb."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Pilih Tipe Pemasangan"
${LangFileString} Reinstall_Header_SubText "Pilih mode Penimpaan atau Pencopotan."

${LangFileString} Reinstall_Msg1 "Anda memiliki pemasang SMPlayer yang masih ada di direktori ini:"
${LangFileString} Reinstall_Msg2 "Harap memilih proses:"
${LangFileString} Reinstall_Overwrite "Timpa ($Inst_Type) pemasang yang ada"
${LangFileString} Reinstall_Uninstall "Copot (lepas) pemasang yang ada"
${LangFileString} Reinstall_Msg3_1 "Klik Mulai untuk mempersiapkan proses."
${LangFileString} Reinstall_Msg3_2 "Klik Berikutnya ketika siap untuk proses."
${LangFileString} Reinstall_Msg3_3 "Klik Copot ketika siap untuk diproses."
${LangFileString} Reinstall_Msg4 "Ubah Setelan Pemasangan"
${LangFileString} Reinstall_Msg5 "Setel ulang konfigurasi SMPlayer saya"

${LangFileString} Remove_Settings_Confirmation "Apakah kamu yakin ingin menyetel ulang setelan SMPlayer-mu? Tindakan ini tidak dapat dikembalikan."

${LangFileString} Type_Reinstall "pasang ulang"
${LangFileString} Type_Downgrade "penurunan"
${LangFileString} Type_Upgrade "peningkatan"

${LangFileString} StartBtn "Mulai"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Mengunduh Kodek MPlayer..."
${LangFileString} Codecs_DL_Retry "Kodek MPlayer tidak berhasil dipasang. Coba lagi?"
${LangFileString} Codecs_DL_Failed "Gagal mengunduh kodek MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Gagal memasang kodek MPlayer"

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Pemasangan ini hanya bisa dicopot oleh pengguna dengan izin pengurus."
${LangFileString} Uninstaller_Aborted "Pencopotan dibatalkan oleh pengguna."
${LangFileString} Uninstaller_NotInstalled "Sepertinya tidak terlihat bahwa SMPlayer terinstal didirektori '$INSTDIR'.$\r$\nLanjutkan saja(tidak direkomendasikan)?"
${LangFileString} Uninstaller_InvalidDirectory "Pemasangan SMPlayer tidak tersedia."
${LangFileString} Uninstaller_64bitOnly "Pemasangan hanya bisa dicopot pada Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMplayer adalah aplikasi antarmuka lengkap untuk MPlayer, dari fitur dasar seperti memutar video, DVD, VCD ke fitur lanjutan seperti mendukung penyaringan MPlayer, daftar edl, dan banyak lainnya."

; Misc
${LangFileString} Info_Codecs_Backup "Mencadangkan kodek dari pemasangan sebelumnya..."
${LangFileString} Info_Codecs_Restore "Mengembalikan kodek dari pemasangan sebelumnya..."
${LangFileString} Info_Del_Files "Menghapus berkas..."
${LangFileString} Info_Del_Registry "Menghapus Kunci Registri..."
${LangFileString} Info_Del_Shortcuts "Menghapus Pintasan..."
${LangFileString} Info_Rest_Assoc "Mengembalikan asosiasi berkas..."
${LangFileString} Info_RollBack "Mengembalikan pengubahan..."
${LangFileString} Info_Files_Extract "Mengekstrak berkas..."
${LangFileString} Info_SMTube_Backup "Mencadangkan SMTube..."
${LangFileString} Info_SMTube_Restore "Mengembalikan SMTube dari pemasangan sebelumnya..."
${LangFileString} Info_MPV_Backup "Mencadangkan mpv..."
${LangFileString} Info_MPV_Restore "Mengembalikan mpv dari pemasangan sebelumnya..."

; MPV
${LangFileString} MPV_DL_Msg "Mengunduh mpv..."
${LangFileString} MPV_DL_Retry "mpv tidak sukses dipasang. Coba kembali?"
${LangFileString} MPV_DL_Failed "Gagal mengunduh mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Gagal memasang mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl tidak sukses dipasang. Coba kembali?"
${LangFileString} YTDL_DL_Failed "Gagal mengunduh youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Memeriksa pembaruan youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Membersihkan singgahan fontconfig..."
${LangFileString} Info_Cleaning_SMPlayer "Membersihkan setelan SMPlayer..."
