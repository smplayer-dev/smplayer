;Language: Turkish (1055)
;Turkish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Turkish" "Türkçe"
!else
!insertmacro LANGFILE "Turkish" = "Türkçe" "Turkce"
!endif

; Startup
${LangFileString} Installer_Is_Running "Kurulum zaten çalışıyor."
${LangFileString} Installer_No_Admin "Bu programı yüklerken yönetici olarak oturum açmanız gerekir."
${LangFileString} SMPlayer_Is_Running "SMPlayer bir örneği çalışıyor. SMPlayer'ı kapatın ve tekrar deneyin."

${LangFileString} OS_Not_Supported "İşletim sistemi desteklenmiyor.$\nSMPlayer ${SMPLAYER_VERSION} en az Windows XP gerektirir ve sisteminizde düzgün çalışmayabilir.$\nGerçekten kuruluma devam etmek istiyor musunuz?"
${LangFileString} OS_Not_Supported_VistaRequired "İşletim sistemi desteklenmiyor.$\nSMPlayer ${SMPLAYER_VERSION} en az Windows Vista gerektirir ve sisteminizde düzgün çalışmayabilir.$\nGerçekten kuruluma devam etmek istiyor musunuz?"
${LangFileString} Win64_Required "Bu yazılım kurulumu için 64-bit Windows işletim sistemi gerekli."
${LangFileString} Existing_32bitInst "SMPlayer için varolan bir 32-bit yükleme var. Öncelikle 32-bit SMPlayer'ı kaldırmanız gerekir."
${LangFileString} Existing_64bitInst "SMPlayer için varolan bir 64-bit yükleme var. Öncelikle 64-bit SMPlayer'ı kaldırmanız gerekir."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Kurulum"
${LangFileString} WelcomePage_Text "Kurulum $(^NameDA) 'ın kurulumunda size rehberlik edecek.$\r$\n$\r$\nKurulum başlamadan önce SMPlayer'ın tüm benzerlerini kapatmanız önerilir. Bu bilgisayarınızı yeniden başlatmak zorunda kalmadan ilgili program dosyalarını güncellemek mümkün olacaktır.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Kısayollar"
${LangFileString} MPlayerGroupTitle "MPlayer Bileşenleri"
${LangFileString} MPlayerMPVGroupTitle "Multimedya Motoru"

${LangFileString} Section_SMPlayer "SMPlayer (gerekli)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, paylaşımlı kütüphaneleri, ve dökümanı."

${LangFileString} Section_DesktopShortcut "Masaüstü"
${LangFileString} Section_DesktopShortcut_Desc "Masaüstünde SMPlayer kısayolu oluşturacak."

${LangFileString} Section_StartMenu "Başlat Menüsü"
${LangFileString} Section_StartMenu_Desc "SMPlayer için Başlat Menüsünde giriş oluşturulacak."

${LangFileString} Section_MPlayer "MPlayer (gerekli)"
${LangFileString} Section_MPlayer_Desc "MPlayer; oynatmak için gerekli."

${LangFileString} Section_MPlayerCodecs "İkili Kodekleri"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "İkili Kodekler bu sürüm için desteklenmiyor."
!else
${LangFileString} Section_MPlayerCodecs_Desc "MPlayer için isteğe bağlı kodekler. (Kurulum için internet bağlantısı gereklidir)"
!endif

${LangFileString} Section_MPV_Desc "Özellik açısından zengin bir MPlayer && MPlayer2 çatallaması"

${LangFileString} Section_MEncoder_Desc "Desteklenen ses veya video yayınlarını çözmek veya dönüştürmek için kullanılabilen, MPlayer'a eşlik eden bir program."

${LangFileString} Section_IconThemes "Simge Temaları"
${LangFileString} Section_IconThemes_Desc "SMPlayer için ek simge temaları."

${LangFileString} Section_Translations "Diller"
${LangFileString} Section_Translations_Desc "SMPlayer için ingilizce olmayan dil dosyaları."

${LangFileString} Section_ResetSettings_Desc "SMPlayer'in önceki kurulum tercihleri silinir."

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Kurulum Tipini Seç"
${LangFileString} Reinstall_Header_SubText "Üzerine yazma veya kaldırma modunu seç."

${LangFileString} Reinstall_Msg1 "Aşağıdaki klasörde SMPlayer'ın var olan bir kurulumu var:"
${LangFileString} Reinstall_Msg2 "Lütfen nasıl devam edeceğinizi seçin:"
${LangFileString} Reinstall_Overwrite "Önceki kurulum ($Inst_Type) üzerine yaz"
${LangFileString} Reinstall_Uninstall "Varolan yüklemeyi kaldırın (sil)"
${LangFileString} Reinstall_Msg3_1 "Hazır olduğunuzda devam etmek başlata tıklatın."
${LangFileString} Reinstall_Msg3_2 "Hazır olduğunuzda devam etmek ileri'yi tıklatın."
${LangFileString} Reinstall_Msg3_3 "Hazır olduğunuzda devam etmek Kaldır'ı tıklatın."
${LangFileString} Reinstall_Msg4 "Kurulum Ayarlarını Değiştir"
${LangFileString} Reinstall_Msg5 "SMPlayer yapılandırmasını sıfırla"

${LangFileString} Remove_Settings_Confirmation "SMPlayer ayarlarını sıfırlamak istediğine emin misin? Bu eylem geri alınamaz."

${LangFileString} Type_Reinstall "tekrar kur"
${LangFileString} Type_Downgrade "gerilet"
${LangFileString} Type_Upgrade "güncelle"

${LangFileString} StartBtn "Başlat"

; Codecs Section
${LangFileString} Codecs_DL_Msg "MPlayer Kodekleri İndiriliyor..."
${LangFileString} Codecs_DL_Retry "MPlayer kodekleri kurulumu başarısız oldu. Tekrar denensin mi?"
${LangFileString} Codecs_DL_Failed "MPlayer kodekleri indirilmesi başarısız oldu: '$R0'."
${LangFileString} Codecs_Inst_Failed "MPlayer kodekleri kurulumu başarısız oldu."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Bu kurulum sadece yönetici ayrıcalıklarına sahip bir kullanıcı tarafından kaldırılabilir."
${LangFileString} Uninstaller_Aborted "Kaldırma kullanıcı tarafından iptal edildi."
${LangFileString} Uninstaller_NotInstalled "SMPlayerin '$INSTDIR' dizininde yüklü olduğu görülmüyor. $\r$\nHer koşulda devam etsin mi (önerilmez)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer kurulumu bulunamadı."
${LangFileString} Uninstaller_64bitOnly "Bu yükleme yalnızca 64-bit Windows üzerinde kaldırılabilir."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer, video, DVD ve VCD oynatmak gibi temel özelliklerden, MPlayer süzgeçleri, edl listeleri ve daha çoğu için gelişmiş özelliklere sahip eksiksiz bir MPlayer ön ucudur"

; Misc
${LangFileString} Info_Codecs_Backup "Önceki kurulumdan kodekler yedekleniyor..."
${LangFileString} Info_Codecs_Restore "Önceki kurulumdaki kodekleri yenileniyor..."
${LangFileString} Info_Del_Files "Dosyalar Siliniyor..."
${LangFileString} Info_Del_Registry "Kayıt Anahtarları Siliniyor..."
${LangFileString} Info_Del_Shortcuts "Kısayollar Siliniyor..."
${LangFileString} Info_Rest_Assoc "Dosya ilişkileri yenileniyor..."
${LangFileString} Info_RollBack "Değişiklikler geri alınıyor..."
${LangFileString} Info_Files_Extract "Dosyaları ayıklanıyor..."
${LangFileString} Info_SMTube_Backup "SMTube Yedekleniyor..."
${LangFileString} Info_SMTube_Restore "SMTube önceki kurulumdan geri yükleniyor..."
${LangFileString} Info_MPV_Backup "Mpv yedekleniyor..."
${LangFileString} Info_MPV_Restore "Mpv önceki kurulumdan geri yükleniyor..."

; MPV
${LangFileString} MPV_DL_Msg "Mpv indiriliyor..."
${LangFileString} MPV_DL_Retry "mpv kurulamadı. Tekrar denemek ister misiniz?"
${LangFileString} MPV_DL_Failed "Mpv indirilirken bir hata oldu: '$R0'."
${LangFileString} MPV_Inst_Failed "Mpv kurulamadı."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl kurulamadı. Tekrar denemek ister misiniz?"
${LangFileString} YTDL_DL_Failed "Youtube-dl indirilemedi : '$R0'."
${LangFileString} YTDL_Update_Check "Youtube-dl için güncelleme kontrolü... "
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Yazı türü yapılandırma önbelleğini temizleme"
${LangFileString} Info_Cleaning_SMPlayer "SMPlayer ayarlarını temizleme..."
