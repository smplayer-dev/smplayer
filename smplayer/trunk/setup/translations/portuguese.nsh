;Language: Portuguese (2070)
;Portuguese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Portuguese" "Português"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "The installer is already running."
${LangFileString} SMPLAYER_INSTALLER_PREV_ALL_USERS "SMPlayer has been previously installed for all users.$\nPlease restart the installer with Administrator privileges."
${LangFileString} SMPLAYER_INSTALLER_PREV_VERSION "SMPlayer has already been installed.$\nDo you want to remove the previous version before installing $(^Name)?"

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "O pacote de codecs binários adiciona suporte para os codecs que ainda não foram implementados, tais como as novas variantes RealVideo e alguns formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "O MPlayer já está instalado. Re-Transferir?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Transferindo MPlayer..."
  ${LangFileString} MPLAYER_DL_FAILED "Falha ao transferir MPlayer:"
  ${LangFileString} MPLAYER_INST_FAILED "Falha ao instalar MPlayer. O MPlayer é necessário para reproduzir."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Os codecs MPlayer já estão instalados. Re-Transferir?"
${LangFileString} CODECS_IS_DOWNLOADING "Transferindo codecs MPlayer..."
${LangFileString} CODECS_DL_FAILED "Falha ao transferir os codecs MPlayer:"
${LangFileString} CODECS_INST_FAILED "Falha ao instalar os codecs MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Transferindo informações sobre a versão..."
${LangFileString} VERINFO_DL_FAILED "Falha ao transferir informações sobre a versão:"
${LangFileString} VERINFO_IS_MISSING "Informação sobre a versão em falta. A configuração irá utilizar a versão padrão."

; Uninstaller
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} UNINSTALL_INSTALLED_ALL_USERS "SMPlayer has been installed for all users.$\nPlease restart the uninstaller with Administrator privileges to remove it."
${LangFileString} SMPLAYER_NOT_INSTALLED "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."