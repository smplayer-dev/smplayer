;Language: Italian (1040)
;Italian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Italian" "Italiano"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Il programma di installazione è già in esecuzione."
${LangFileString} SMPLAYER_INSTALLER_PREV_ALL_USERS "SMPlayer has been previously installed for all users.$\nPlease restart the installer with Administrator privileges."
${LangFileString} SMPLAYER_INSTALLER_PREV_VERSION "SMPlayer è già stato installato.$\nVuoi rimuovere la versione precedente prima dell'installazione di $(^Name)?"

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "I pacchetti di codec binari forniscono il supporto per i codec che non sono stati ancora implementati nativamente, per esempio per le varianti di RealVideo e anche un sacco di formati poco utilizzati.$\nNota che non sono richiesti per riprodurre i formati più comuni come DVD, MPEG-1/2/4, ecc."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "MPlayer è già stato installato. Ri-scarico?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Sto scaricando MPlayer..."
  ${LangFileString} MPLAYER_DL_FAILED "Scaricamento di MPlayer fallito:"
  ${LangFileString} MPLAYER_INST_FAILED "Installazione di MPlayer fallita. MPlayer è un componente necessario per la riproduzione multimediale."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "I codec di MPlayer sono già stati installati. Ri-scarico?"
${LangFileString} CODECS_IS_DOWNLOADING "Sto scaricando i codec di MPlayer..."
${LangFileString} CODECS_DL_FAILED "Scaricamento dei codec di MPlayer fallito:"
${LangFileString} CODECS_INST_FAILED "Installazione dei codec di MPlayer fallita."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Sto scaricando le informazioni di versione..."
${LangFileString} VERINFO_DL_FAILED "Scaricamento delle informazioni di versione fallito:"
${LangFileString} VERINFO_IS_MISSING "Il file di versione non contiene informazioni di versione. Verrà utilizzata una versione predefinita."

; Uninstaller
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} UNINSTALL_INSTALLED_ALL_USERS "SMPlayer has been installed for all users.$\nPlease restart the uninstaller with Administrator privileges to remove it."
${LangFileString} SMPLAYER_NOT_INSTALLED "Sembra che SMPlayer non sia installato nel direttorio '$INSTDIR'.$\r$\nContinua comunque (non raccomandato)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."