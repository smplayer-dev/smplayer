;Language: Portuguese (2070)
;Portuguese language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Portuguese" "Português"

; Startup
${LangFileString} Installer_Is_Running "O instalador já está em execução."
${LangFileString} Installer_No_Admin "Tem que iniciar a sessão como administrador para instalar este programa."

; Components Page
${LangFileString} ShortcutGroupTitle "Atalhos"
${LangFileString} MPlayerGroupTitle "Componentes MPlayer"

${LangFileString} Section_SMPlayer "SMPlayer (obrigatório)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, bibliotecas partilhadas e documentação."

${LangFileString} Section_DesktopShortcut "Ambiente de Trabalho"
${LangFileString} Section_DesktopShortcut_Desc "Cria um atalho para o SMPlayer no ambiente de trabalho."

${LangFileString} Section_StartMenu "Menu Iniciar"
${LangFileString} Section_StartMenu_Desc "Cria uma entrada no Menu Iniciar para o SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (obrigatório)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; necessário para a reprodução."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; necessário para a reprodução. (Precisa de uma ligação à Internet)"
!endif

${LangFileString} Section_MPlayerCodecs "Binários dos Codificadores"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codificadores opcionais para o MPlayer."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Codificadores opcionais para o MPlayer. (Precisa de uma ligação à Internet)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Conjuntos de Ícones"
${LangFileString} Section_IconThemes_Desc "Ícones adicionais para o SMPlayer."

${LangFileString} Section_Translations "Localizações"
${LangFileString} Section_Translations_Desc "Localizações Diferentes de Inglês."

${LangFileString} MPlayer_Codec_Msg "O pacote de codecs binários adiciona suporte para os codecs que ainda não foram implementados, tais como as novas variantes RealVideo e alguns formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

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
${LangFileString} MPlayer_DL_Msg "Transferindo MPlayer..."
${LangFileString} MPlayer_DL_Retry "MPlayer não foi correctamente instalado. Tentar novamente?"
${LangFileString} MPlayer_DL_Failed "Falha ao transferir MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Falha ao instalar MPlayer. O MPlayer é necessário para reproduzir."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Transferindo codecs MPlayer..."
${LangFileString} Codecs_DL_Retry "Os codecs MPlayer não foram correctamente instalados. Tentar novamente?"
${LangFileString} Codecs_DL_Failed "Falha ao transferir os codecs MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Falha ao instalar os codecs MPlayer."

; Version information
${LangFileString} VerInfo_DL_Msg "Transferindo informações sobre a versão..."
${LangFileString} VerInfo_DL_Failed "Falha ao transferir informações sobre a versão: '$R0'. Utilizando versão por omissão."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Esta aplicação apenas pode ser desinstalada no modo de administrador."
${LangFileString} Uninstaller_Aborted "Desinstalação cancelada pelo utilizador."
${LangFileString} Uninstaller_NotInstalled "Parece que o SMPlayer não está instalado no directório '$INSTDIR'.$\r$\nContinuar mesmo assim (não recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "O SMPlayer é um interface gráfico para o MPlayer, com funcionalidades básicas desde a reprodução de vídeos, DVDs, VCDs bem como outras mais avançadas(suporte a filtros MPlayer, listas e outras)."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_Files_Extract "Extracting files..."
