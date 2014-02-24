;Language: Portuguese Brazil (1046)
;Portuguese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "PortugueseBrazil" "Português (Brasil)"

; Startup
${LangFileString} Installer_Is_Running "O instalador já está em execução."
${LangFileString} Installer_No_Admin "Você precisa estar logado como administrador quando instalar esse programa."
${LangFileString} SMPlayer_Is_Running "Uma instância do SMPlayer está em execução. Por favor feche o SMPlayer e tente novamente."

${LangFileString} OS_Not_Supported "Sistema operacional não suportado.$\nO SMPlayer ${SMPLAYER_VERSION} requer pelo menos o Windows XP e pode não funcionar corretamente em seu sistema.$\nQuer realmente prosseguir com a instalação?"
${LangFileString} Win64_Required "Um sistema operacional Windows 64-bit é necessário para instalar esse software."
${LangFileString} Existing_32bitInst "Uma instalação do SMPlayer 32-bit já existe. Você deve desinstalá-la primeiro."
${LangFileString} Existing_64bitInst "Uma instalação do SMPlayer 64-bit já existe. Você deve desinstalá-la primeiro."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Configuração"
${LangFileString} WelcomePage_Text "A configuração lhe guiará através da instalação do $(^NameDA).$\r$\n$\r$\nÉ recomendado fechar todas as instâncias do SMPlayer antes de iniciar a configuração. Isso tornará possível atualizar arquivos importantes do programa sem ter que reiniciar seu computador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Atalhos"
${LangFileString} MPlayerGroupTitle "Componentes do MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (necessário)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, bibliotecas compartilhadas e documentação."

${LangFileString} Section_DesktopShortcut "Área de trabalho"
${LangFileString} Section_DesktopShortcut_Desc "Cria um atalho para o SMPlayer na área de trabalho."

${LangFileString} Section_StartMenu "Menu Iniciar"
${LangFileString} Section_StartMenu_Desc "Cria uma entrada do SMPlayer no menu Iniciar."

${LangFileString} Section_MPlayer "MPlayer (necessário)"
${LangFileString} Section_MPlayer_Desc "MPlayer; necessário para reprodução."

${LangFileString} Section_MPlayerCodecs "Codecs binários"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Codecs opcionais para o MPlayer. (Necessita conexão com a Internet para instalação)"
!endif

${LangFileString} Section_MEncoder_Desc "Um programa acompanhante do MPlayer que pode ser usado para codificar ou transformar fluxos de áudio ou vídeo suportados."

${LangFileString} Section_IconThemes "Temas de ícones"
${LangFileString} Section_IconThemes_Desc "Temas de ícones adicionais para o SMPlayer."

${LangFileString} Section_Translations "Idiomas"
${LangFileString} Section_Translations_Desc "Arquivos de outros idiomas além do Inglês para o SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Deletar as preferências de antigas instalações do SMPlayer."

${LangFileString} MPlayer_Codec_Msg "Os pacotes de codecs binários e o suporte a codecs que ainda não estão implementados nativamente, como variantes recentes do RealVideo e vários formatos incomuns.$\nNote que não são necessários para reproduzir a maioria dos formatos comuns, como DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Selecione o tipo de instalação"
${LangFileString} Reinstall_Header_SubText "Selecione Reinstalar ou Desinstalar."

${LangFileString} Reinstall_Msg1 "Você possui uma instalação do SMPlayer na seguinte pasta:"
${LangFileString} Reinstall_Msg2 "Por favor, selecione como continuar:"
${LangFileString} Reinstall_Overwrite "Sobrescrever ($Inst_Type) a instalação existente"
${LangFileString} Reinstall_Uninstall "Desinstalar (remover) a instalação existente"
${LangFileString} Reinstall_Msg3_1 "Clique em Iniciar quando estiver pronto para continuar."
${LangFileString} Reinstall_Msg3_2 "Clique em Próximo quando estiver pronto para continuar."
${LangFileString} Reinstall_Msg3_3 "Clique em Desinstalar quando estiver pronto para continuar."
${LangFileString} Reinstall_Msg4 "Altere as configurações da instalação"
${LangFileString} Reinstall_Msg5 "Reinicie minhas configurações do SMPlayer"

${LangFileString} Type_Reinstall "reinstalar"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "atualizar"

${LangFileString} StartBtn "Iniciar"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Baixando codecs do MPlayer..."
${LangFileString} Codecs_DL_Retry "Os codecs do Mplayer não foram instalados corretamente. Tentar novamente?"
${LangFileString} Codecs_DL_Failed "Falha ao baixar o codecs do MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Falha ao instalar os codecs do MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Essa instalação só pode ser desinstalada por um usuário com privilégios de administrador."
${LangFileString} Uninstaller_Aborted "Desinstalação abortada pelo usuário."
${LangFileString} Uninstaller_NotInstalled "Não parece que o SMPlayer está instalado no diretório '$INSTDIR'.$\r$\nContinuar assim mesmo (não é recomendado)?"
${LangFileString} Uninstaller_InvalidDirectory "Instalação do SMPlayer não encontrada."
${LangFileString} Uninstaller_64bitOnly "Essa instalação só pode ser desinstalada no Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "O SMPlayer é uma interface completa para o MPlayer, com funcionalidades básicas como reprodução de vídeos, DVDs, VCDs, e mais avançadas como o suporte aos filtros do MPlayer, listas edl, etc."

; Misc
${LangFileString} Info_Codecs_Backup "Fazendo backup de codecs da instalação anterior..."
${LangFileString} Info_Codecs_Restore "Restaurando codecs da instalação anterior..."
${LangFileString} Info_Del_Files "Deletando arquivos..."
${LangFileString} Info_Del_Registry "Deletando chaves do registro..."
${LangFileString} Info_Del_Shortcuts "Deletando atalhos..."
${LangFileString} Info_Rest_Assoc "Restaurando associações de arquivos..."
${LangFileString} Info_RollBack "Revertendo alterações..."
${LangFileString} Info_Files_Extract "Extraindo arquivos..."
