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
${LangFileString} MPLAYER_CODEC_INFORMATION "I pacchetti di codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\nNota che non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; Upgrade/Reinstall Page
# MUI Headers
${LangFileString} REINSTALL_HEADER_TEXT "Already Installed"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Choose how you want to install SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Choose the maintenance option to perform."
# Misc
${LangFileString} REINSTALL_CHGSETTINGS "Change settings (advanced)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unknown value of PREVIOUS_VERSION_STATE, aborting"
# Older version detected
${LangFileString} REINSTALL_OLDVER_DESCRIPTION "An older version of SMPlayer is installed on your system. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Upgrade SMPlayer using previous settings (recommended)"
# Newer version detected
${LangFileString} REINSTALL_NEWVER_DESCRIPTION "A newer version of SMPlayer is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Downgrade SMPlayer using previous settings (recommended)"
# Same version detected
${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} is already installed. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Add/Remove/Reinstall components"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Uninstall SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Sto scaricando MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer was not successfully installed. Retry?"
${LangFileString} MPLAYER_DL_FAILED "Scaricamento di MPlayer fallito: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Installazione di MPlayer fallita. MPlayer è un componente necessario per la riproduzione multimediale."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Sto scaricando i codec di MPlayer..."
${LangFileString} CODECS_DL_RETRY "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} CODECS_DL_FAILED "Scaricamento dei codec di MPlayer fallito: '$R0'."
${LangFileString} CODECS_INST_FAILED "Installazione dei codec di MPlayer fallita."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Sto scaricando le informazioni di versione..."
${LangFileString} VERINFO_DL_FAILED "Scaricamento delle informazioni di versione fallito: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "La disinstallazione può essere effettuata solo da un utente con permessi amministrativi."
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} SMPLAYER_NOT_INSTALLED "Sembra che SMPlayer non sia installato nel direttorio '$INSTDIR'.$\r$\nContinua comunque (non raccomandato)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."