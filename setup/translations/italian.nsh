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
${LangFileString} Installer_No_Admin "Per installare questo programma è necessario essere autenticati come amministratore."
${LangFileString} SMPlayer_Is_Running "È in esecuzione un'istanza di SMPlayer.$\r$\nEsci da SMPlayer e riprova."

${LangFileString} OS_Not_Supported "Sistema operativo non supportato.$\r$\nSmPlayer ${SMPLAYER_VERSION} richiede almeno Windows XP e potrebbe non funzionare correttamente in questo sistema.$\r$\nVuoi proseguire con l'installazione?"
${LangFileString} OS_Not_Supported_VistaRequired "Sistema operativo non supportato.$\r$\nSmPlayer ${SMPLAYER_VERSION} richiede almeno Windows Vista e potrebbe non funzionare correttamente in questo sistema.$\r$\nVuoi proseguire con l'installazione?"
${LangFileString} Win64_Required "Per installare questo softwareè è richiesto un sistema operativo Windows a 64bit."
${LangFileString} Existing_32bitInst "È già presente un'installazione di SMPlayer 32bit.$\r$\nPrima dinistalla SMPlayer 32bit."
${LangFileString} Existing_64bitInst "È già presente un'installazione di SMPlayer 64bit.$\r$\nPrima dinistalla SMPlayer 64bit."

; Welcome page
${LangFileString} WelcomePage_Title "Installazione guidata di $(^NameDA)"
${LangFileString} WelcomePage_Text "Questa è l'installazione guidata di $(^NameDA).$\r$\n$\r$\nPrima di proseguire ti raccomandiamo di chiudere tutte le istanze di SMPlayer.$\r$\nQuesto renderà possibile installare i file del programma senza dover riavviare il computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Collegamenti"
${LangFileString} MPlayerGroupTitle "Componenti MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Motore multimediale"

${LangFileString} Section_SMPlayer "SMPlayer (richiesto)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, librerie condivise, e documentazione."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Crea suld esktop un collegamento a SMPlayer."

${LangFileString} Section_StartMenu "Menu di avvio"
${LangFileString} Section_StartMenu_Desc "Creanel menu di avvio una voce per SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (richiesto)"
${LangFileString} Section_MPlayer_Desc "MPlayer; essenziale per la riproduzione."

${LangFileString} Section_MPlayerCodecs "Codec binari"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "I codec binari non sono supportati in questa versione."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codec aggiuntivi MPlayer (per l'instalalzione è richiesta una connessione a internet)."
!endif

${LangFileString} Section_MPV_Desc "Una fork di MPlayer && MPlayer2 ricca di funzioni"

${LangFileString} Section_YTDL "Supporto YouTube (richiesto download)"
${LangFileString} Section_YTDL_Desc "Fornisce la possibilità di visualizzare video YouTube."

${LangFileString} Section_MEncoder_Desc "Un programma integrativo di MPlayer che può essere usato per codificare o trasformare i flussi audio o video supportati."

${LangFileString} Section_IconThemes "Temi icone"
${LangFileString} Section_IconThemes_Desc "Temi icone aggiuntivi SMPlayer."

${LangFileString} Section_Translations "Lingue interfaccia"
${LangFileString} Section_Translations_Desc "Lingue aggiuntive interfaccia (non inglese)."

${LangFileString} Section_ResetSettings_Desc "Elimina impostazioni SMPlayer lasciate da installazioni precedenti."

${LangFileString} MPlayer_Codec_Msg "I codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\r$\nI codec binari non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Scegli il tipo di installazione"
${LangFileString} Reinstall_Header_SubText "Scegli se sovrascrivere o disinstallare."

${LangFileString} Reinstall_Msg1 "È già presente un'installazione di SMPlayer nella seguente cartella:"
${LangFileString} Reinstall_Msg2 "Sceglie come proseguire:"
${LangFileString} Reinstall_Overwrite "Sovrascrivi ($Inst_Type) l'installazione esistente"
${LangFileString} Reinstall_Uninstall "Disinstalla (rimuovi) l'installazione esistente"
${LangFileString} Reinstall_Msg3_1 "Per proseguire, seleziona 'Installa'."
${LangFileString} Reinstall_Msg3_2 "Per proseguire, seleziona 'Avanti'."
${LangFileString} Reinstall_Msg3_3 "Per proseguire, seleziona 'Disinstalla'"
${LangFileString} Reinstall_Msg4 "Modifica le impostazioni di installazione"
${LangFileString} Reinstall_Msg5 "Ripristina configurazione SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Vuoi ripristinare le impostazioni di SMPlayer?$\r$\n$\r$\nQuesta azione non può essere annullata."

${LangFileString} Type_Reinstall "reinstalla"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "aggiorna"

${LangFileString} StartBtn "Installa"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Download codec MPlayer..."
${LangFileString} Codecs_DL_Retry "I codec di MPlayer non sono stati installati correttamente.$\r$\nVuoi riprovare?"
${LangFileString} Codecs_DL_Failed "Impossibile scaricare i codec MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Installazione codec MPlayer non riuscita."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "La disinstallazione può essere effettuata solo da un utente con permessi amministrativi."
${LangFileString} Uninstaller_Aborted "Disinstallazione annullata dall'utente."
${LangFileString} Uninstaller_NotInstalled "Sembra che SMPlayer non sia installato nella cartella '$INSTDIR'.$\r$\n$\r$\nVuoi continua comunque (non raccomandato)?"
${LangFileString} Uninstaller_InvalidDirectory "Installazione di SMPlayer non trovata."
${LangFileString} Uninstaller_64bitOnly "Questa installazione può essere disinstallata solo in Windows 64bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer è un'interfaccia completa per MPlayer, fornisce funzionalità di base come la riproduzione di video, DVD, VCD e funzionalità più avanzate come il supporto dei filtri di MPlayer, elenchi EDL e altro ancora."

; Misc
${LangFileString} Info_Codecs_Backup "Copia di sicurezza codec dell'installazione precedente..."
${LangFileString} Info_Codecs_Restore "Ripristino codec dell'installazione precedente..."
${LangFileString} Info_Del_Files "Eliminazione file..."
${LangFileString} Info_Del_Registry "Eliminazione chiavi di registro..."
${LangFileString} Info_Del_Shortcuts "Eliminazione collegamenti..."
${LangFileString} Info_Rest_Assoc "Ripristino associazioni file..."
${LangFileString} Info_RollBack "Rollback modifiche..."
${LangFileString} Info_Files_Extract "Estrazione file..."
${LangFileString} Info_SMTube_Backup "Copia di sicurezza di SMTube..."
${LangFileString} Info_SMTube_Restore "Ripristino di SMTube dell'installazione precedente..."
${LangFileString} Info_MPV_Backup "Salvataggio mpv..."
${LangFileString} Info_MPV_Restore "Ripristino mpv della precedente installazione..."

; MPV
${LangFileString} MPV_DL_Msg "Download mpv..."
${LangFileString} MPV_DL_Retry "mpv non è stato installato correttamente.$\r$\n$\r$\nVuoi riprovare?"
${LangFileString} MPV_DL_Failed "Impossibile scaricare mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Errore nell'installazione di mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl non è stato installato correttamente.$\r$\n$\r$\nVuoi riprovare?"
${LangFileString} YTDL_DL_Failed "Impossibile scaricare youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Verifica aggiornamenti youtube-dl..."
${LangFileString} YTDL_Error_Msg1 "Avviso: youtube-dl si è chiuso in modo anomalo con il codice di uscita: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Per usare youtube-dl è necessario Visual C++ 2010 Runtime (x86)."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "L'attuale installazione di SMTube non è compatibile con questa versione di SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Aggiorna SMTube alla versione più recnte disponibile."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Pulizia cache fontconfig..."
${LangFileString} Info_Cleaning_SMPlayer "Pulizia impostazioni SMPlayer..."
