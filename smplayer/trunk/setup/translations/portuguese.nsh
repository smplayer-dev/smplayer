;Language: Portuguese (2070)
;Portuguese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Portuguese" "Português"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "O instalador já está em execução."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Tem que iniciar a sessão como administrador para instalar este programa."

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "O pacote de codecs binários adiciona suporte para os codecs que ainda não foram implementados, tais como as novas variantes RealVideo e alguns formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Transferindo MPlayer..."
  ${LangFileString} MPLAYER_DL_RETRY "MPlayer não foi correctamente instalado. Tentar novamente?"
  ${LangFileString} MPLAYER_DL_FAILED "Falha ao transferir MPlayer: '$R0'."
  ${LangFileString} MPLAYER_INST_FAILED "Falha ao instalar MPlayer. O MPlayer é necessário para reproduzir."
!endif

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Transferindo codecs MPlayer..."
${LangFileString} CODECS_DL_RETRY "Os codecs MPlayer não foram correctamente instalados. Tentar novamente?"
${LangFileString} CODECS_DL_FAILED "Falha ao transferir os codecs MPlayer: '$R0'."
${LangFileString} CODECS_INST_FAILED "Falha ao instalar os codecs MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Transferindo informações sobre a versão..."
${LangFileString} VERINFO_DL_FAILED "Falha ao transferir informações sobre a versão: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Esta aplicação apenas pode ser desinstalada no modo de administrador."
${LangFileString} UNINSTALL_ABORTED "Desinstalação cancelada pelo utilizador."
${LangFileString} SMPLAYER_NOT_INSTALLED "Parece que o SMPlayer não está instalado no directório '$INSTDIR'.$\r$\nContinuar mesmo assim (não recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "O SMPlayer é um interface gráfico para o MPlayer, com funcionalidades básicas desde a reprodução de vídeos, DVDs, VCDs bem como outras mais avançadas(suporte a filtros MPlayer, listas e mais)."