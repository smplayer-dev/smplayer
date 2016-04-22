;Language: Italian (1040)
;Italian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Italian" "Italiano"
!else
!insertmacro LANGFILE "Italian" = "Italiano" =
!endif

; Startup
${LangFileString} Installer_Is_Running "Il programma di installazione è già in esecuzione."
${LangFileString} Installer_No_Admin "È necessario essere autenticati come amministratore per installare questo programma."
${LangFileString} SMPlayer_Is_Running "Un'istanza di SMPlayer è in esecuzione. Esci da SMPlayer e riprova."

${LangFileString} OS_Not_Supported "Sistema operativo non supportato. $\nSmPlayer ${SMPLAYER_VERSION} richiede almeno Windows XP e potrebbe non funzionare correttamente su questo sistema. $\nSi vuole davvero proseguire con l'installazione?"
${LangFileString} OS_Not_Supported_VistaRequired "Sistema operativo non supportato. $\nSmPlayer ${SMPLAYER_VERSION} richiede almeno Windows Vista e potrebbe non funzionare correttamente su questo sistema. $\nSi vuole davvero proseguire con l'installazione?"
${LangFileString} Win64_Required "È richiesto un sistema operativo Windows a 64-bit per installare questo software."
${LangFileString} Existing_32bitInst "Un'installazione a 32-bit di SMPlayer è già presente. Si deve prima disinstallare SMPlayer a 32-bit."
${LangFileString} Existing_64bitInst "Un'installazione a 64-bit di SMPlayer è già presente. Si deve prima disinstallare SMPlayer a 64-bit."

; Welcome page
${LangFileString} WelcomePage_Title "Installazione guidata di $(^NameDA)"
${LangFileString} WelcomePage_Text "Questa è l'installazione guidata di $(^NameDA).$\r$\n$\r$\nSi raccomanda di chiudere tutte le istanze di SMPlayer prima di cominciare. Questo renderà possibile installare i file del programma senza dover riavviare la macchina.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Collegamenti"
${LangFileString} MPlayerGroupTitle "Componenti di MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Motore Multimediale"

${LangFileString} Section_SMPlayer "SMPlayer (essenziale)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, librerie condivise, e documentazione."

${LangFileString} Section_DesktopShortcut "Scrivania"
${LangFileString} Section_DesktopShortcut_Desc "Crea un collegamento a SMPlayer sulla scrivania."

${LangFileString} Section_StartMenu "Menu di avvio"
${LangFileString} Section_StartMenu_Desc "Crea un elemento nel menu di avvio per SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (essenziale)"
${LangFileString} Section_MPlayer_Desc "MPlayer; essenziale per la riproduzione."

${LangFileString} Section_MPlayerCodecs "Codec binari"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "I codec binari non sono supportati in questa versione."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codec aggiuntivi per MPlayer. (Connessione a Internet richiesta per l'installazione)"
!endif

${LangFileString} Section_MPV_Desc "Una fork di MPlayer && MPlayer2 ricca di funzioni"

${LangFileString} Section_MEncoder_Desc "Un programma integrativo di MPlayer che può essere usato per codificare o trasformare flussi audio o video supportati."

${LangFileString} Section_IconThemes "Temi di icone"
${LangFileString} Section_IconThemes_Desc "Temi di icone aggiuntivi per SMPlayer."

${LangFileString} Section_Translations "Localizzazioni"
${LangFileString} Section_Translations_Desc "Localizzazioni non Inglesi."

${LangFileString} Section_ResetSettings_Desc "Cancella preferenze di SMPlayer lasciate da installazioni precedenti."

${LangFileString} MPlayer_Codec_Msg "I pacchetti di codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\nNota che non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Scegliere tipo di installazione"
${LangFileString} Reinstall_Header_SubText "Scegliere se sovrascrivere o disinstallare."

${LangFileString} Reinstall_Msg1 "Un'installazione già presente di SMPlayer è presente nella cartella seguente:"
${LangFileString} Reinstall_Msg2 "Scegliere come proseguire:"
${LangFileString} Reinstall_Overwrite "Sovrascrivi ($Inst_Type) l'installazione esistente"
${LangFileString} Reinstall_Uninstall "Disinstalla (rimuovi) l'installazione esistente"
${LangFileString} Reinstall_Msg3_1 "Per proseguire, scegliere Installa."
${LangFileString} Reinstall_Msg3_2 "Per proseguire, scegliere Avanti."
${LangFileString} Reinstall_Msg3_3 "Per proseguire, scegli Disinstalla"
${LangFileString} Reinstall_Msg4 "Cambia le impostazioni di installazione"
${LangFileString} Reinstall_Msg5 "Rimuovi configurazione di SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Are you sure you want to reset your SMPlayer settings? This action cannot be reversed."

${LangFileString} Type_Reinstall "reinstalla"
${LangFileString} Type_Downgrade "retrocedi"
${LangFileString} Type_Upgrade "aggiorna"

${LangFileString} StartBtn "Installa"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Sto scaricando i codec di MPlayer..."
${LangFileString} Codecs_DL_Retry "I codec di MPlayer non sono stati installati correttamente. Riprovare?"
${LangFileString} Codecs_DL_Failed "Impossibile scaricare i codec di MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Installazione dei codec di MPlayer non riuscita."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "La disinstallazione può essere effettuata solo da un utente con permessi amministrativi."
${LangFileString} Uninstaller_Aborted "Disinstallazione annullata dall'utente."
${LangFileString} Uninstaller_NotInstalled "Sembra che SMPlayer non sia installato nella cartella '$INSTDIR'.$\r$\nContinua comunque (non raccomandato)?"
${LangFileString} Uninstaller_InvalidDirectory "Installazione di SMPlayer non trovata."
${LangFileString} Uninstaller_64bitOnly "Questa installazione può essere disinstallata solo su Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer è un'interfaccia completa per MPlayer, fornisce funzionalità di base come la riproduzione di video, DVD, VCD e funzionalità più avanzate come il supporto dei filtri di MPlayer, le liste edl e altro ancora."

; Misc
${LangFileString} Info_Codecs_Backup "Copia di sicurezza dei codec dell'installazione precedente..."
${LangFileString} Info_Codecs_Restore "Ripristino codec dell'installazione precedente..."
${LangFileString} Info_Del_Files "Eliminazione file..."
${LangFileString} Info_Del_Registry "Eliminazione chiavi di registro..."
${LangFileString} Info_Del_Shortcuts "Eliminazione collegamenti..."
${LangFileString} Info_Rest_Assoc "Ripristino associazioni file..."
${LangFileString} Info_RollBack "Rollback delle modifiche..."
${LangFileString} Info_Files_Extract "Estrazione file..."
${LangFileString} Info_SMTube_Backup "Copia di sicurezza di SMTube..."
${LangFileString} Info_SMTube_Restore "Ripristino di SMTube dall'installazione precedente..."
${LangFileString} Info_MPV_Backup "Backing up mpv..."
${LangFileString} Info_MPV_Restore "Restoring mpv from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Downloading mpv..."
${LangFileString} MPV_DL_Retry "mpv was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Failed to install mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Checking for youtube-dl updates..."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Cleaning fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "Cleaning SMPlayer settings..."
