;Language: Portuguese (2070)
;Portuguese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Portuguese" "Português"

; Startup
${LangFileString} Installer_Is_Running "O instalador já está em execução."
${LangFileString} Installer_No_Admin "Tem que iniciar a sessão como administrador para instalar este programa."
${LangFileString} SMPlayer_Is_Running "Já existe uma instância SMPlayer em execução. Feche o SMPlayer e tente novamente."

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "Atalhos"
${LangFileString} MPlayerGroupTitle "Componentes MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (obrigatório)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, bibliotecas partilhadas e documentação."

${LangFileString} Section_DesktopShortcut "Ambiente de trabalho"
${LangFileString} Section_DesktopShortcut_Desc "Cria um atalho para o SMPlayer no ambiente de trabalho."

${LangFileString} Section_StartMenu "Menu Iniciar"
${LangFileString} Section_StartMenu_Desc "Cria uma entrada no Menu Iniciar para o SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (obrigatório)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; necessário para a reprodução."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; necessário para a reprodução. (Precisa de uma ligação à Internet)"
!endif

${LangFileString} Section_MPlayerCodecs "Codificadores"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codificadores opcionais para o MPlayer."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codificadores opcionais para o MPlayer. (Precisa de uma ligação à Internet)"
!endif

${LangFileString} Section_MEncoder_Desc "Um programa complementar ao MPlayer, utilizado para codificar ou converter emissões de vídeo e áudio."

${LangFileString} Section_IconThemes "Ícones"
${LangFileString} Section_IconThemes_Desc "Ícones adicionais para o SMPlayer."

${LangFileString} Section_Translations "Idiomas"
${LangFileString} Section_Translations_Desc "Idiomas adicionais para o SMPlayer."

${LangFileString} MPlayer_Codec_Msg "O pacote de codificadores adiciona suporte para os formatos que ainda não foram implementados no MPlayer, tais como as novas variantes RealVideo e outros formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Selecione o tipo de instalação"
${LangFileString} Reinstall_Header_SubText "Selecione o modo de substituição ou desinstalação."

${LangFileString} Reinstall_Msg1 "Você possui uma instalação do SMPlayer nesta pasta:"
${LangFileString} Reinstall_Msg2 "Por favor, escolha o método a utilizar:"
${LangFileString} Reinstall_Overwrite "Substituir ($Inst_Type) a instalação existente"
${LangFileString} Reinstall_Uninstall "Desinstalar (remove) a instalação existente"
${LangFileString} Reinstall_Msg3_1 "Clique Iniciar para continuar."
${LangFileString} Reinstall_Msg3_2 "Clique Seguinte para continuar."
${LangFileString} Reinstall_Msg3_3 "Clique Desinstalar para continuar."
${LangFileString} Reinstall_Msg4 "Alterar definições de instalação"

${LangFileString} Type_Reinstall "Reinstalar"
${LangFileString} Type_Downgrade "Reverter"
${LangFileString} Type_Upgrade "Atualizar"

${LangFileString} StartBtn "Iniciar"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "A transferir o MPlayer..."
${LangFileString} MPlayer_DL_Retry "O MPlayer não foi instalado corretamente. Tentar novamente?"
${LangFileString} MPlayer_DL_Failed "Falha ao transferir o MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Falha ao instalar o MPlayer. O MPlayer é necessário para reproduzir ficheiros."

; Codecs Section
${LangFileString} Codecs_DL_Msg "A transferir os codificadores MPlayer..."
${LangFileString} Codecs_DL_Retry "Os codificadores MPlayer não foram instalados corretamente. Tentar novamente?"
${LangFileString} Codecs_DL_Failed "Falha ao transferir os codificadores MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Falha ao instalar os codificadores MPlayer."

; Version information
${LangFileString} VerInfo_DL_Msg "A transferir informações da versão..."
${LangFileString} VerInfo_DL_Failed "Ocorreu um erro ao transferir ao informações da versão: '$R0'. A utilizar versão padrão."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Esta aplicação só pode ser desinstalada no modo de administrador."
${LangFileString} Uninstaller_Aborted "Desinstalação cancelada pelo utilizador."
${LangFileString} Uninstaller_NotInstalled "Parece que o SMPlayer não está instalado no diretório '$INSTDIR'.$\r$\nContinuar (não recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "O SMPlayer é um interface gráfico para o MPlayer, com funções simples como a reprodução de vídeos, DVDs e VCDs bem como outras mais avançadas(suporte a filtros MPlayer, listas e mais)."

; Misc
${LangFileString} Info_Del_Files "A eliminar ficheiros..."
${LangFileString} Info_Del_Registry "A eliminar chaves de registo..."
${LangFileString} Info_Del_Shortcuts "A eliminar atalhos..."
${LangFileString} Info_Rest_Assoc "A restaurar as associações de ficheiros..."
${LangFileString} Info_RollBack "A reverter as alterações..."
${LangFileString} Info_Files_Extract "A extrair ficheiros..."
