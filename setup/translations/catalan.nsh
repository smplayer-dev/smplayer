;Language: Catalan (1027)
;Catalan language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Catalan" "Català"
!else
!insertmacro LANGFILE "Catalan" = "Català" "Catala"
!endif

; Startup
${LangFileString} Installer_Is_Running "L'instal·lador ja està en execució."
${LangFileString} Installer_No_Admin "Heu d'iniciar la sessió com a administrador quan instal·leu aquest programa."
${LangFileString} SMPlayer_Is_Running "Una instància de SMPlayer s'està executant. Si us plau, sortiu de SMPlayer i torneu-ho a intentar."

${LangFileString} OS_Not_Supported "El sistema operatiu no és compatible.$\nSMPlayer ${SMPLAYER_VERSION} requereix com a mínim Windows XP i pot ser que no funcioni correctament en el vostre sistema.$\nRealment voleu continuar amb la instal·lació?"
${LangFileString} OS_Not_Supported_VistaRequired "El sistema operatiu no és compatible.$\nSMPlayer ${SMPLAYER_VERSION} requereix com a mínim Windows Vista i pot ser que no funcioni correctament en el vostre sistema.$\nRealment voleu continuar amb la instal·lació?"
${LangFileString} Win64_Required "Es requereix un sistema operatiu Windows de 64 bits per instal·lar aquest programari."
${LangFileString} Existing_32bitInst "Existeix una instal·lació de 32 bits de SMPlayer. Primer heu de desinstal·lar SMPlayer de 32 bits."
${LangFileString} Existing_64bitInst "Existeix una instal·lació de 64 bits de SMPlayer. Primer heu de desinstal·lar SMPlayer de 64 bits."

; Welcome page
${LangFileString} WelcomePage_Title "Preparació $(^NameDA)"
${LangFileString} WelcomePage_Text "La preparació us guiarà a través de la instal·lació de $(^NameDA).$\r$\n$\r$\nEs recomana que tanqueu totes les instàncies de SMPlayer abans de començar la preparació. Això farà que sigui possible l'actualització dels de programa pertinents sense haver de reiniciar el vostre ordinador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Dreceres"
${LangFileString} MPlayerGroupTitle "Components de MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Motor multimèdia"

${LangFileString} Section_SMPlayer "SMPlayer (requerit)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, biblioteques compatides i documentació."

${LangFileString} Section_DesktopShortcut "Escriptori"
${LangFileString} Section_DesktopShortcut_Desc "Crea una drecera a SMPlayer en l'escriptori."

${LangFileString} Section_StartMenu "Menú d'inici"
${LangFileString} Section_StartMenu_Desc "Crea una entrada de menú d'inici per a SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (requerit)"
${LangFileString} Section_MPlayer_Desc "MPlayer; requerit per a la reproducció."

${LangFileString} Section_MPlayerCodecs "Còdecs binaris"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Els còdecs binaris no són compatibles amb aquesta versió."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Còdecs opcionals per a MPlayer. (es requereix una connexió a Internet per a la instal·lació)"
!endif

${LangFileString} Section_MPV_Desc "Un fork ric en característiques de MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Un programa que acompanya MPlayer que es pot utilitzar per codificar o transformar els fluxos compatibles d'àudio o de vídeo."

${LangFileString} Section_IconThemes "Temes d'icones"
${LangFileString} Section_IconThemes_Desc "Temes d'icones addicionals per a SMPlayer."

${LangFileString} Section_Translations "Idiomes"
${LangFileString} Section_Translations_Desc "Els fitxers dels idiomes no anglesos per a SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Elimina les preferències sobrants de SMPlayer de les instal·lacions anteriors."

${LangFileString} MPlayer_Codec_Msg "Els paquets de còdecs binaris afegeixen compatibilitat per als còdecs que encara no estan implementats de forma nativa, com ara les variants més noves de RealVideo i una gran quantitat de formats poc comuns.$\nRecordeu que no són necessaris per reproduir els formats més comuns, com DVD, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Selecció del tipus d'instal·lació"
${LangFileString} Reinstall_Header_SubText "Seleccioneu el mode Sobreescriu o Desinstal·la."

${LangFileString} Reinstall_Msg1 "Teniu una instal·lació existent de SMPlayer a la carpeta següent:"
${LangFileString} Reinstall_Msg2 "Seleccioneu com continuar:"
${LangFileString} Reinstall_Overwrite "Sobreescriu ($Inst_Type) la instal·lació existent"
${LangFileString} Reinstall_Uninstall "Desinstal·la (suprimeix) la instal·lació existent"
${LangFileString} Reinstall_Msg3_1 "Feu clic a Comença quan estigueu llest per continuar."
${LangFileString} Reinstall_Msg3_2 "Feu clic a Següent quan estigueu llest per continuar."
${LangFileString} Reinstall_Msg3_3 "Feu clic a Desinstal·la quan estigueu llest per continuar."
${LangFileString} Reinstall_Msg4 "Canvia els ajusts de la instal·lació"
${LangFileString} Reinstall_Msg5 "Restableix la meva configuració de SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Esteu segur que voleu restablir els ajusts de SMPlayer? Aquesta acció no pot ser revertida."

${LangFileString} Type_Reinstall "reinstal·la"
${LangFileString} Type_Downgrade "reverteix"
${LangFileString} Type_Upgrade "actualitza"

${LangFileString} StartBtn "Comença"

; Codecs Section
${LangFileString} Codecs_DL_Msg "S'estan baixant els còdecs de MPlayer..."
${LangFileString} Codecs_DL_Retry "No s'han instal·lat correctament els còdecs de MPlayer. Voleu tornar-ho a intentar?"
${LangFileString} Codecs_DL_Failed "Ha fallat la baixada dels còdecs de MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Ha fallat la instal·lació dels còdecs de MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Aquesta instal·lació només pot ser desinstal·lada per un usuari amb privilegis d'administrador."
${LangFileString} Uninstaller_Aborted "Desinstal·lació avortada per l'usuari."
${LangFileString} Uninstaller_NotInstalled "No sembla que SMPlayer estigui instal·lat al directori '$INSTDIR'.$\r$\nVoleu continuar de totes maneres (no recomanat)"
${LangFileString} Uninstaller_InvalidDirectory "No s'ha trobat la instal·lació de SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Aquesta instal·lació només es pot desinstal·lar des d'una versió de 64 bits de Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer és un frontal complet per a MPlayer, des de les funcions bàsiques com ara reproduir vídeos, DVD, VCD fins a les funcions més avançades com ara la compatibilitat per als filtres de MPlayer, llistes EDL, i molt més."

; Misc
${LangFileString} Info_Codecs_Backup "S'està creant una còpia de seguretat dels còdecs binaris d'una instal·lació anterior..."
${LangFileString} Info_Codecs_Restore "S'estan restaurant els còdecs binaris des d'una instal·lació anterior..."
${LangFileString} Info_Del_Files "S'estan eliminant els fitxers..."
${LangFileString} Info_Del_Registry "S'estan eliminant les claus del registre..."
${LangFileString} Info_Del_Shortcuts "S'estan eliminant les dreceres..."
${LangFileString} Info_Rest_Assoc "S'estan restaurant les associacions dels fitxers..."
${LangFileString} Info_RollBack "S'estan revertint els canvis ..."
${LangFileString} Info_Files_Extract "S'estan extraient els fitxers..."
${LangFileString} Info_SMTube_Backup "S'està creant la còpia de seguretat de SMTube..."
${LangFileString} Info_SMTube_Restore "S'està restaurant SMTube des d'una instal·lació anterior..."
${LangFileString} Info_MPV_Backup "S'està creant la còpia de seguretat de mpv..."
${LangFileString} Info_MPV_Restore "S'està restaurant mpv des d'una instal·lació anterior..."

; MPV
${LangFileString} MPV_DL_Msg "S'està baixant mpv..."
${LangFileString} MPV_DL_Retry "No s'ha instal·lat correctament mpv. Voleu tornar-ho a intentar?"
${LangFileString} MPV_DL_Failed "Ha fallat la baixada de mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Ha fallat la instal·lació de mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "No s'ha instal·lat correctament youtube-dl. Voleu tornar-ho a intentar?"
${LangFileString} YTDL_DL_Failed "Ha fallat la baixada de youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "S'està comprovant si hi ha actualitzacions de youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "S'està netejant la memòria cau de fontconfig..."
${LangFileString} Info_Cleaning_SMPlayer "S'estan netejant els ajusts de SMPlayer..."
