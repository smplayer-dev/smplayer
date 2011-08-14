;Language: Italian (1040)
;Italian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Italian" "Italiano"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Il programma di installazione è già in esecuzione."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Devi essere autenticato come amministratore per installare questo programma."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (essenziale)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, librerie condivise, e documentazione."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Collegamenti"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Scrivania"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Crea un collegamento a SMPlayer sulla scrivania."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Menu di avvio"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Crea un elemento nel menu di avvio per SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "Componenti di MPlayer"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (essenziale)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; essenziale per la riproduzione."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; essenziale per la riproduzione. (Connessione a Internet richiesta per l'installazione)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Codec binari"
${LangFileString} SMPLAYER_SECCODECS_DESC "Codec aggiuntivi per MPlayer. (Connessione a Internet richiesta per l'installazione)"
${LangFileString} SMPLAYER_SECCODECS_DESC_2 "Codec aggiuntivi per MPlayer."

${LangFileString} SMPLAYER_SECMENCODER_DESC "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Temi di icone"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Temi di icone aggiuntivi per SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Localizzazioni"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Localizzazioni non Inglesi."

${LangFileString} MPLAYER_CODEC_INFORMATION "I pacchetti di codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\nNota che non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Già installato"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Scegli come vuoi installare SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Scegli l'operazione di manutenzione da effettuare."

${LangFileString} REINSTALL_CHGSETTINGS "Cambia la configurazione (avanzato)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Valore di PREVIOUS_VERSION_STATE sconosciuto, interrotto"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Una versione precedente di SMPlayer è installata sul tuo sistema. Seleziona l'operazione da effettuare e premi Successivo per continuare."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Aggiorna SMPlayer utilizzando le configurazioni esistenti (raccomandato)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Una versione recente di SMPlayer è già installata! Non è raccomandato l'aggiornamento ad una versione più vecchia. Seleziona l'operazione da effettuare e premi Successivo per continuare."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Aggiorna SMPlayer alla versione più vecchia utilizzando le configurazioni esistenti (raccomandato)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} è già installato. Seleziona l'operazione da effettuare e premi Successivo per continuare."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Aggiungi/Rimuovi/Reinstalla i componenti"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Rimuovi SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Sto scaricando MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer non è stato completamente installato. Riprova?"
${LangFileString} MPLAYER_DL_FAILED "Scaricamento di MPlayer fallito: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Installazione di MPlayer fallita. MPlayer è un componente necessario per la riproduzione multimediale."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Sto scaricando i codec di MPlayer..."
${LangFileString} CODECS_DL_RETRY "I codec di MPlayer non sono stati completamente installati. Riprova?"
${LangFileString} CODECS_DL_FAILED "Scaricamento dei codec di MPlayer fallito: '$R0'."
${LangFileString} CODECS_INST_FAILED "Installazione dei codec di MPlayer fallita."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Sto scaricando le informazioni di versione..."
${LangFileString} VERINFO_DL_FAILED "Scaricamento delle informazioni di versione fallito: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "La disinstallazione può essere effettuata solo da un utente con permessi amministrativi."
${LangFileString} UNINSTALL_ABORTED "Disinstallazione annullata dall'utente."
${LangFileString} SMPLAYER_NOT_INSTALLED "Sembra che SMPlayer non sia installato nella cartella '$INSTDIR'.$\r$\nContinua comunque (non raccomandato)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer è un'interfaccia completa per MPlayer, fornisce funzionalità di base come la riproduzione di video, DVD, VCD e funzionalità più avanzate come il supporto dei filtri di MPlayer, le liste edl e altro ancora."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
