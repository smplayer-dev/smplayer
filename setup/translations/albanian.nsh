;Language: Albanian (1052)
;Albanian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Albanian" "Gjuha shqipe"

; Startup
${LangFileString} Installer_Is_Running "Instaluesi eshte ne egzekutim."
${LangFileString} Installer_No_Admin "Ju duhet te jeni loguar si administator per te instaluar kete program."
${LangFileString} SMPlayer_Is_Running "Një instancë e SMPlayer është në egzekutim. Ju lutem mbyllni SMPlayer dhe riprovoni."

${LangFileString} OS_Not_Supported "Ky sistem operativë nuk suportohet.$\nSMPlayer ${SMPLAYER_VERSION} kërkon të paktën Windows XP dhe mund të mos punoj në mënyrë korrekte në sistemin tuaj.$\nDëshironi akoma të vazhdoni me installimin?"
${LangFileString} Win64_Required "Një sistem operativë Windows 64-bit nevojitet për të instaluar këtë software."
${LangFileString} Existing_32bitInst "Egziston një installim 32-bit i SMPlayer. Duhet të disinstaloni më parë SMPlayer 32-bit."
${LangFileString} Existing_64bitInst "Egziston një installim 64-bit i SMPlayer. Duhet të disinstaloni më parë SMPlayer 64-bit."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "Programi i instalimit do ju ndihmoj hap pas hapi ne instalimin e $(^NameDA).$\r$\n$\r$\nRekomandohet qe te mbyllni te gjitha dritaret e hapura te SMPlayer perpara se te filloni instalimin. Kjo do te bej te mundur instalimin e update-ve pa pasur nevoj te riavioni/ristartoni kompjuterin tuaj.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Shortcuts"
${LangFileString} MPlayerGroupTitle "Komponentet e MPlayer."
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "SMPlayer (nevojitet)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, libraritë dinamike, dhe dokumentacioni."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Krijon një shortcut të SMPlayer në desktop."

${LangFileString} Section_StartMenu "Menuja Start"
${LangFileString} Section_StartMenu_Desc "Krijo një menu të re për SMPlayer tek Menuja Start."

${LangFileString} Section_MPlayer "MPlayer (nevojitet)"
${LangFileString} Section_MPlayer_Desc "MPlayer; nevojitet per leximin/shikimin e videos dhe audios."

${LangFileString} Section_MPlayerCodecs "Codec-et binare."
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Codec-ët binary nuk suportohen në këtë version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codec-ët shtesë për MPlayer. (Nevojitet Lidhje në Internet për instalimin)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Nje program qe perdor MPlayer mund te perdoret per te enkoduar ose transformuar file audio ose video te suportuar."

${LangFileString} Section_IconThemes "Tema e Ikonave."
${LangFileString} Section_IconThemes_Desc "Tema te tjera te ikonave per SMPlayer."

${LangFileString} Section_Translations "Gjuhët"
${LangFileString} Section_Translations_Desc "File për gjuhët jo Ingleze të SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Fshij file e preferencave te instalimit te meparshem te SMPlayer."

${LangFileString} MPlayer_Codec_Msg "Paketa software e codec-ve binare shton suportin per codec-et qe nuk jane implementuar akoma ne menyre native, si versione e reja te RealVideo dhe shume formateve video jo te famshem.$\nShenim: keto codec-ek nuk jane te domosdoshem per te pare/shikuar file video me formate te famshem si DVDs, MPEG-1/2/4, etj."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Zgjidh tipin e instalimit"
${LangFileString} Reinstall_Header_SubText "Zgjidh mënyrën e Mbishkruarjes ose Disinstallimit."

${LangFileString} Reinstall_Msg1 "Ju keni një instalim të SMPlayer në këtë direktori:"
${LangFileString} Reinstall_Msg2 "Ju lutem zgjidhni si të veproni:"
${LangFileString} Reinstall_Overwrite "Mbishkruani ($Inst_Type) instalimi egzistues"
${LangFileString} Reinstall_Uninstall "Disinstalo (fshij) instalimin eksistues"
${LangFileString} Reinstall_Msg3_1 "Kliko Start kur te jeni gati për të vazhduar."
${LangFileString} Reinstall_Msg3_2 "Kliko Next kur te jeni gati për të vazhduar."
${LangFileString} Reinstall_Msg3_3 "Kliko Uninstall kur te jeni gati për të vazhduar."
${LangFileString} Reinstall_Msg4 "Ndrysho karakteristikat e Instalimit"
${LangFileString} Reinstall_Msg5 "Reseto configurimin tim te SMPlayer"

${LangFileString} Type_Reinstall "riinstallo"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

${LangFileString} StartBtn "Fillo"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Duke shkarkuar MPlayer Codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs nuk u installuan me sukses. Riprovo?"
${LangFileString} Codecs_DL_Failed "Nuk u arrit shkarkimi i codec-vë te MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Dështim gjatë instalimit të MPlayer codecs."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Ky instalim mund të disinstalohet nga një përdorues me të drejta administratori."
${LangFileString} Uninstaller_Aborted "Disinstalimi u ndërpre nga përdoruesi."
${LangFileString} Uninstaller_NotInstalled "SMPlayer nuk është instaluar në këtë direktori '$INSTDIR'.$\r$\nDëshironi të vazhdoni gjithsesi (nuk rekomandohet)?"
${LangFileString} Uninstaller_InvalidDirectory "Nuk u gjend instalimi i SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Ky instalim mund të disinstalohet vetëm në 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer mbështet tek MPlayer, nga veprimtaritë bazë si shikimi i video-ve, DVD-ve, VCD-ve deri te veprimtaritë më të avancuara të MPlayer, si filtratë, listat edl, etj."

; Misc
${LangFileString} Info_Codecs_Backup "Krijimi i një kopje rezervë për codec-ët nga instalimi i mëparshëm."
${LangFileString} Info_Codecs_Restore "Duke gjetur codec-ët nga instalimi i vjetër..."
${LangFileString} Info_Del_Files "Duke fshire File..."
${LangFileString} Info_Del_Registry "Duke fshire Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Duke fshire Shortcuts..."
${LangFileString} Info_Rest_Assoc "Duke gjetur file associations..."
${LangFileString} Info_RollBack "Kthimi mbrapa i ndryshimeve..."
${LangFileString} Info_Files_Extract "Ekstratimi i file..."
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
