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
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (obrigatório)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, bibliotecas partilhadas e documentação."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Atalhos"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Ambiente de Trabalho"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Cria um atalho para o SMPlayer no ambiente de trabalho."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Menu Iniciar"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Cria uma entrada no Menu Iniciar para o SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "Componentes MPlayer"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (obrigatório)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; necessário para a reprodução."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; necessário para a reprodução. (Precisa de uma ligação à Internet)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binários dos Codificadores"
${LangFileString} SMPLAYER_SECCODECS_DESC "Codificadores opcionais para o MPlayer. (Precisa de uma ligação à Internet)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Conjuntos de Ícones"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Ícones adicionais para o SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Localizações"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Localizações Diferentes de Inglês."

${LangFileString} MPLAYER_CODEC_INFORMATION "O pacote de codecs binários adiciona suporte para os codecs que ainda não foram implementados, tais como as novas variantes RealVideo e alguns formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Já Instalado"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Escolha o método de instalação do SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Escolha o tipo de manutenção a efectuar."

${LangFileString} REINSTALL_CHGSETTINGS "Alterer definições (avançado)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Valor desconhecido para PREVIOUS_VERSION_STATE, abortando"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Uma versão mais antiga do SMPlayer está instalada. Seleccione a operação que pretende efectuar e clique em Seguinte para continuar."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Actualizar SMPlayer mantendo as definições anteriores (recomendado)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Uma versão mais recente do SMPlayer está instalada! Não é recomendada a sua substituição por uma versão mais antiga. Seleccione a operação que quer efectuar e clique Seguinte para continuar."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Substituir SMPlayer mantendo as definições anteriores (recomendado)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} já está instalado. Seleccione a operação a efectuar e clique Seguinte para continuar."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Adicionar/Remover/Reinstalar componentes"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Desinstalar SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Transferindo MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer não foi correctamente instalado. Tentar novamente?"
${LangFileString} MPLAYER_DL_FAILED "Falha ao transferir MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Falha ao instalar MPlayer. O MPlayer é necessário para reproduzir."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Transferindo codecs MPlayer..."
${LangFileString} CODECS_DL_RETRY "Os codecs MPlayer não foram correctamente instalados. Tentar novamente?"
${LangFileString} CODECS_DL_FAILED "Falha ao transferir os codecs MPlayer: '$R0'."
${LangFileString} CODECS_INST_FAILED "Falha ao instalar os codecs MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Transferindo informações sobre a versão..."
${LangFileString} VERINFO_DL_FAILED "Falha ao transferir informações sobre a versão: '$R0'. Utilizando versão por omissão."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Esta aplicação apenas pode ser desinstalada no modo de administrador."
${LangFileString} UNINSTALL_ABORTED "Desinstalação cancelada pelo utilizador."
${LangFileString} SMPLAYER_NOT_INSTALLED "Parece que o SMPlayer não está instalado no directório '$INSTDIR'.$\r$\nContinuar mesmo assim (não recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "O SMPlayer é um interface gráfico para o MPlayer, com funcionalidades básicas desde a reprodução de vídeos, DVDs, VCDs bem como outras mais avançadas(suporte a filtros MPlayer, listas e outras)."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
