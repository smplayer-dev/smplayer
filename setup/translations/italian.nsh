;Language: Italian (1040)
;Italian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Italian" "Italiano"

; Startup
${LangFileString} Installer_Is_Running "Il programma di installazione è già in esecuzione."
${LangFileString} Installer_No_Admin "Devi essere autenticato come amministratore per installare questo programma."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "Collegamenti"
${LangFileString} MPlayerGroupTitle "Componenti di MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (essenziale)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, librerie condivise, e documentazione."

${LangFileString} Section_DesktopShortcut "Scrivania"
${LangFileString} Section_DesktopShortcut_Desc "Crea un collegamento a SMPlayer sulla scrivania."

${LangFileString} Section_StartMenu "Menu di avvio"
${LangFileString} Section_StartMenu_Desc "Crea un elemento nel menu di avvio per SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (essenziale)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; essenziale per la riproduzione."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; essenziale per la riproduzione. (Connessione a Internet richiesta per l'installazione)"
!endif

${LangFileString} Section_MPlayerCodecs "Codec binari"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codec aggiuntivi per MPlayer."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codec aggiuntivi per MPlayer. (Connessione a Internet richiesta per l'installazione)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Temi di icone"
${LangFileString} Section_IconThemes_Desc "Temi di icone aggiuntivi per SMPlayer."

${LangFileString} Section_Translations "Localizzazioni"
${LangFileString} Section_Translations_Desc "Localizzazioni non Inglesi."

${LangFileString} MPlayer_Codec_Msg "I pacchetti di codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\nNota che non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Select Install Type"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3_1 "Click Start when ready to proceed."
${LangFileString} Reinstall_Msg3_2 "Click Next when ready to proceed."
${LangFileString} Reinstall_Msg3_3 "Click Uninstall when ready to proceed."
${LangFileString} Reinstall_Msg4 "Change Installation Settings"

${LangFileString} Type_Reinstall "reinstall"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

${LangFileString} StartBtn "Start"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "Sto scaricando MPlayer..."
${LangFileString} MPlayer_DL_Retry "MPlayer non è stato completamente installato. Riprova?"
${LangFileString} MPlayer_DL_Failed "Scaricamento di MPlayer fallito: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Installazione di MPlayer fallita. MPlayer è un componente necessario per la riproduzione multimediale."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Sto scaricando i codec di MPlayer..."
${LangFileString} Codecs_DL_Retry "I codec di MPlayer non sono stati completamente installati. Riprova?"
${LangFileString} Codecs_DL_Failed "Scaricamento dei codec di MPlayer fallito: '$R0'."
${LangFileString} Codecs_Inst_Failed "Installazione dei codec di MPlayer fallita."

; Version information
${LangFileString} VerInfo_DL_Msg "Sto scaricando le informazioni di versione..."
${LangFileString} VerInfo_DL_Failed "Scaricamento delle informazioni di versione fallito: '$R0'. Using a default version."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "La disinstallazione può essere effettuata solo da un utente con permessi amministrativi."
${LangFileString} Uninstaller_Aborted "Disinstallazione annullata dall'utente."
${LangFileString} Uninstaller_NotInstalled "Sembra che SMPlayer non sia installato nella cartella '$INSTDIR'.$\r$\nContinua comunque (non raccomandato)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer è un'interfaccia completa per MPlayer, fornisce funzionalità di base come la riproduzione di video, DVD, VCD e funzionalità più avanzate come il supporto dei filtri di MPlayer, le liste edl e altro ancora."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Extracting files..."
