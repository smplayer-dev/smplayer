;Language: Hungarian (1038)
;Hungarian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Hungarian" "Magyar"
!else
!insertmacro LANGFILE "Hungarian" = "Magyar" =
!endif

; Startup
${LangFileString} Installer_Is_Running "A telepítő már fut."
${LangFileString} Installer_No_Admin "A program telepítéséhez redszergazdai jogosultságra van szükség."
${LangFileString} SMPlayer_Is_Running "Az SMPlayer egy példánya már fut. Kérjük, lépjen ki az SMPlayerből és próbálja újra."

${LangFileString} OS_Not_Supported "Nem támogatott operációs rendszer.$\nAz SMPlayer ${SMPLAYER_VERSION} legalább Windows XP-t igényel, így valószínűleg nem fog megfelelően működni az ön rendszerén.$\nBiztosan folytatja a telepítést?"
${LangFileString} OS_Not_Supported_VistaRequired "Nem támogatott operációs rendszer.$\nAz SMPlayer ${SMPLAYER_VERSION} legalább Windows Vistát igényel, így valószínűleg nem fog megfelelően működni az ön rendszerén.$\nBiztosan folytatja a telepítést?"
${LangFileString} Win64_Required "A szoftver telepítéséhez 64-bites Windows operációs rendszerre van szükség."
${LangFileString} Existing_32bitInst "Egy 32-bites SMPlayer már telepítve van. Előbb el kell távolítania a 32-bites SMPlayer-t."
${LangFileString} Existing_64bitInst "Egy 64-bites SMPlayer már telepítve van. Előbb el kell távolítania a 64-bites SMPlayer-t."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "A telepítő végig fogja vezetni az $(^NameDA) telepítési folyamatán.$\r$\n$\r$\nJavasoljuk, hogy a telepítés megkezdése előtt zárja be az SMPlayer minden megnyitott példányát. Ez lehetővé teszi az adott program fájlok frissítését anélkül, hogy a számítógépét újra kellene indítania.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Parancsikonok"
${LangFileString} MPlayerGroupTitle "MPlayer Összetevők"
${LangFileString} MPlayerMPVGroupTitle "Multimédia-motor"

${LangFileString} Section_SMPlayer "SMPlayer (nélkülözhetetlen)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, megosztott könyvtárak és dokumentáció."

${LangFileString} Section_DesktopShortcut "Asztal"
${LangFileString} Section_DesktopShortcut_Desc "Létrehoz egy SMPlayer parancsikont az asztalon."

${LangFileString} Section_StartMenu "Start Menü"
${LangFileString} Section_StartMenu_Desc "Start Menü bejegyzés létrehozása az SMPlayer számára."

${LangFileString} Section_MPlayer "MPlayer (nélkülözhetetlen)"
${LangFileString} Section_MPlayer_Desc "MPlayer; elengedhetetlen a lejátszáshoz."

${LangFileString} Section_MPlayerCodecs "Bináris Kodekek"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "A bináris kodekek nem támogatottak ebben a verzióban."
!else
${LangFileString} Section_MPlayerCodecs_Desc "További kodekek az MPlayer számára. (Internet Kapcsolat kell a telepítésükhöz)"
!endif

${LangFileString} Section_MPV_Desc "Az MPlayer && MPlayer2 funkciókban gazdag forkja"

${LangFileString} Section_MEncoder_Desc "Az MPlayer segédprogramja, mely audió- és videó sugárzások átkódolására és átalakítására használható."

${LangFileString} Section_IconThemes "Ikontémák"
${LangFileString} Section_IconThemes_Desc "További ikontémák az SMPlayerhez."

${LangFileString} Section_Translations "Nyelvek"
${LangFileString} Section_Translations_Desc "Nem-Angol SMPlayer nyelvi fájlok."

${LangFileString} Section_ResetSettings_Desc "Törli az előző telepítésből megmaradt SMPlayer beállításokat."

${LangFileString} MPlayer_Codec_Msg "A bináris kodekcsomagok támogatást nyújtanak az olyan codecekhez, amelyek még nincsnek natívan implementálva, mint például az újabb RealVideo változatokhoz és rengeteg ritka formátumhoz.$\nMegjegyzendő, hogy ezek nem szükségesek a leggyakorib formátumok lejátszásához, mint például DVD-k, MPEG-1/2/4, stb."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Válassza ki a telepítés típusát"
${LangFileString} Reinstall_Header_SubText "Válassza ki a felülíró vagy eltávolító módot."

${LangFileString} Reinstall_Msg1 "Már van egy meglévő SMPlayer telepítése a következő mappában:"
${LangFileString} Reinstall_Msg2 "Válassza ki, hogyan folytatódjon:"
${LangFileString} Reinstall_Overwrite "Írja felül ($Inst_Type) a megfelelő telepítést"
${LangFileString} Reinstall_Uninstall "Távolítsa el (törölje) a meglévő telepítést"
${LangFileString} Reinstall_Msg3_1 "A folytatáshoz kattintson a Startra."
${LangFileString} Reinstall_Msg3_2 "A folytatáshoz kattintson a Továbbra."
${LangFileString} Reinstall_Msg3_3 "A folytatáshoz kattintson az Eltávolításra."
${LangFileString} Reinstall_Msg4 "Telepítési beállítások megváltoztatása"
${LangFileString} Reinstall_Msg5 "SMPlayer konfigurációjának alaphelyzetbe állítása"

${LangFileString} Remove_Settings_Confirmation "Biztos benne, hogy visszaállítja az SMPlayer beállításait? Ez a művelet nem visszafordítható."

${LangFileString} Type_Reinstall "újratelepítés"
${LangFileString} Type_Downgrade "visszaminősítés"
${LangFileString} Type_Upgrade "frissítés"

${LangFileString} StartBtn "Start"

; Codecs Section
${LangFileString} Codecs_DL_Msg "MPlayer kodekek letöltése..."
${LangFileString} Codecs_DL_Retry "Nem sikerült telepíteni az MPlayer kodekeket. Újra?"
${LangFileString} Codecs_DL_Failed "Nem sikerült letölteni az MPlayer kodekeket: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nem sikerült feltelepíteni az MPlayer kodekeket."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Ez a telepítés csak egy rendszergazdai jogosultságokkal rendelkező felhasználó által távolítható el."
${LangFileString} Uninstaller_Aborted "Az eltávolítást a felhasználó leállította."
${LangFileString} Uninstaller_NotInstalled "Nem úgy tűnik, hogy az SMPlayer telepítve lenne a '$INSTDIR' mappába.$\r$\nMégis folytatja? (nem ajánlott)"
${LangFileString} Uninstaller_InvalidDirectory "Az SMPlayer telepítés nem található."
${LangFileString} Uninstaller_64bitOnly "A telepítés csak 64-bites Windowson távolítható el."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "Az SMPlayer egy átfogó front-end az MPlayerhez, mely rengeteg mindent tartalmaz az alap funkcióktól kezdve, mint a videók, DVD-k és VCD-k lejátszása, az olyan fejlett tulajdonságokig, mint támogatás az MPlayer szűrőkhöz, edl listákhoz és még sok máshoz."

; Misc
${LangFileString} Info_Codecs_Backup "Kodekek biztonsági mentése az előző telepítésből..."
${LangFileString} Info_Codecs_Restore "Kodekek visszaállítása az előző telepítésből..."
${LangFileString} Info_Del_Files "Fájlök törlése..."
${LangFileString} Info_Del_Registry "Regisztrációs kulcsok törlése..."
${LangFileString} Info_Del_Shortcuts "Parancsikonok törlése..."
${LangFileString} Info_Rest_Assoc "Fájltársítások visszaállítása..."
${LangFileString} Info_RollBack "Változtatások visszavonása..."
${LangFileString} Info_Files_Extract "Fájlok kicsomagolása..."
${LangFileString} Info_SMTube_Backup "SMTube biztonsági mentése folyamatban..."
${LangFileString} Info_SMTube_Restore "SMTube visszaállítása az előző telepítésből..."
${LangFileString} Info_MPV_Backup "mpv biztonsági mentése folyamatban..."
${LangFileString} Info_MPV_Restore "mpv visszaállítása az előző telepítésből..."

; MPV
${LangFileString} MPV_DL_Msg "mpv letöltés alatt..."
${LangFileString} MPV_DL_Retry "Az mpv telepítése nem sikerült. Újra próbála?"
${LangFileString} MPV_DL_Failed "Az mpv letöltése sikertelen: '$R0'."
${LangFileString} MPV_Inst_Failed "Az mpv telepítése sikertelen."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "A youtube-dl telepítése nem sikerült. Újra próbálja?"
${LangFileString} YTDL_DL_Failed "A youtube-dl letöltése sikertelen: '$R0'."
${LangFileString} YTDL_Update_Check "youtube-dl frissítések keresése..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "fontconfig gyorsítótár tisztítása..."
${LangFileString} Info_Cleaning_SMPlayer "SMPlayer beállítások tisztítása..."
