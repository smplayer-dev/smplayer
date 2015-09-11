;Language: Galician (1110)
;Galician language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Galician" "Galego"

; Startup
${LangFileString} Installer_Is_Running "Xa se está executando o instalador."
${LangFileString} Installer_No_Admin "Debe acceder como administrador para instalar este programa."
${LangFileString} SMPlayer_Is_Running "Xa se está executando unha instancia do SMPlayer. Peche o programa e ténteo de novo."

${LangFileString} OS_Not_Supported "Sistema operativo non soportado. $\nSMPlayer ${SMPLAYER_VERSION} precisa cando menos de Windows XP, e podería non funcionar correctamente no seu sistema. $\nQueres seguir coa instalación?"
${LangFileString} Win64_Required "Precísase un sistema operativo Windows de 64-bits para instalar esta aplicación."
${LangFileString} Existing_32bitInst "Xa existe unha instalación de 32-bits de SMPlayer. Precisas desinstalar o SMPlayer de 32-bits."
${LangFileString} Existing_64bitInst "Xa existe unha instalación de 64-bits de SMPlayer. Precisas desinstalar o SMPlayer de 64-bits."

; Welcome page
${LangFileString} WelcomePage_Title "Configurar $(^NameDA) "
${LangFileString} WelcomePage_Text "O instalador guiarate na instalación de $(^NameDA).$\r$\n$\r$\n Recoméndase que se pechen todas as instancias de SMPlayer antes de comezar coa instalación. Así pódese actualizar os ficheiros da aplicación que son de relevancia sen logo precisar de reiniciar o ordenador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Atallos"
${LangFileString} MPlayerGroupTitle "Compoñentes de MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Motor multimedia"

${LangFileString} Section_SMPlayer "SMPlayer (requirido)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, bibliotecas compartidas e documentación."

${LangFileString} Section_DesktopShortcut "Escritorio"
${LangFileString} Section_DesktopShortcut_Desc "Crear un atallo de escritorio para SMPlayer."

${LangFileString} Section_StartMenu "Menú de Inicio"
${LangFileString} Section_StartMenu_Desc "Crea unha entrada para SMPlayer no Menú de Inicio."

${LangFileString} Section_MPlayer "MPlayer (requirido)"
${LangFileString} Section_MPlayer_Desc "MPlayer; requirido para a reprodución."

${LangFileString} Section_MPlayerCodecs "Códecs binarios"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Os códecs binarios non son compatíbeis con esta versión."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Códecs opcionais para MPlayer (Precísase unha conexión a internet para a instalación)"
!endif

${LangFileString} Section_MPV_Desc "Unha derivación de MPlayer e MPlayer2 con moitas funcionalidades"

${LangFileString} Section_MEncoder_Desc "Un programa compañeiro do MPlayer que se usa para codificar ou transformar fluxos de vídeo e son compatíbeis."

${LangFileString} Section_IconThemes "Temas de iconas"
${LangFileString} Section_IconThemes_Desc "Temas adicionais de iconas para SMPlayer."

${LangFileString} Section_Translations "Idiomas"
${LangFileString} Section_Translations_Desc "Ficheiros de idiomas diferentes do inglés para SMPlayer."

${LangFileString} Section_ResetSettings_Desc "Elimina as configuracións de SMPlayer das instalacións previas."

${LangFileString} MPlayer_Codec_Msg "Este paquete binario de códecs engade soporte para os códecs que ainda non están implementados de xeito nativo, como é o caso das variantes de RealVideo e moitos formatos infrecuentes.$\nTéñase en conta que non son precisos para reproducir a meirande parte dos formatos habituais como DVDs, MPEG-1/2/4 e demais."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Seleccionar o tipo de instalación"
${LangFileString} Reinstall_Header_SubText "Selecconar modo de sobrescritura ou desinstalación."

${LangFileString} Reinstall_Msg1 "Xa existe unha instalación do SMPlayer no cartafol seguinte:"
${LangFileString} Reinstall_Msg2 "Seleccione como proceder:"
${LangFileString} Reinstall_Overwrite "Sobrescribir ($Inst_Type) a instalación existente"
${LangFileString} Reinstall_Uninstall "Desinstalar (eliminar) a instalación existente"
${LangFileString} Reinstall_Msg3_1 "Prema Iniciar cando estea listo."
${LangFileString} Reinstall_Msg3_2 "Prema Seguinte cando estea listo."
${LangFileString} Reinstall_Msg3_3 "Prema Desinstalar cando estea listo."
${LangFileString} Reinstall_Msg4 "Cambiar os axustes da instalación"
${LangFileString} Reinstall_Msg5 "Restabelecer a configuración do SMPlayer"

${LangFileString} Type_Reinstall "reinstalar"
${LangFileString} Type_Downgrade "desanovar"
${LangFileString} Type_Upgrade "anovar"

${LangFileString} StartBtn "Iniciar"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Descargando os códecs de MPlayer..."
${LangFileString} Codecs_DL_Retry "Os códecs do MPlayer non se instalaron correctamente. Desexar tentalo de novo?"
${LangFileString} Codecs_DL_Failed "Produciuse un fallo ao descargar os códecs do MPlayer: «$R0»."
${LangFileString} Codecs_Inst_Failed "Produciuse un fallo ao instalar os códecs de MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Esta desinstalación precisa ter permisos de administrador."
${LangFileString} Uninstaller_Aborted "Desinstalación cancelada polo usuario."
${LangFileString} Uninstaller_NotInstalled "Non semella que SMPlayer estea instalado no cartafol '$INSTDIR'.$\r$\n. Continuar de calquera xeito (aínda que non se aconsella)?"
${LangFileString} Uninstaller_InvalidDirectory "Non se atopou ningunha instalación do SMPlayer"
${LangFileString} Uninstaller_64bitOnly "Só se pode facer a desinstalación en Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer é unha interface completa para MPlayer. Dispón desde funcionalidades básicas como reproducir vídeos, DVD, VCD ata algunhas máis avanzadas como a compatibilidade con filtros de MPlayer, listas edl e máis."

; Misc
${LangFileString} Info_Codecs_Backup "Facendo copia de seguranza dos códecs da instalación anterior..."
${LangFileString} Info_Codecs_Restore "Restabelecendo os códecs desde a instalación anterior..."
${LangFileString} Info_Del_Files "Eliminando ficheiros..."
${LangFileString} Info_Del_Registry "Eliminando chaves de rexistro..."
${LangFileString} Info_Del_Shortcuts "Eliminando atallos..."
${LangFileString} Info_Rest_Assoc "Restabelecendo as asociacións do ficheiro..."
${LangFileString} Info_RollBack "Desfacendo os cambios..."
${LangFileString} Info_Files_Extract "Extraendo ficheiros..."
${LangFileString} Info_SMTube_Backup "Facendo copia de seguranza de SMTube..."
${LangFileString} Info_SMTube_Restore "Restabelecendo SMTube desde a instalación anterior..."
${LangFileString} Info_MPV_Backup "Facendo copia de seguranza de MPV..."
${LangFileString} Info_MPV_Restore "Restabelecendo MPV desde a instalación anterior..."

; MPV
${LangFileString} MPV_DL_Msg "Descargando MPV..."
${LangFileString} MPV_DL_Retry "Os códecs de MPV non se instalaron correctamente. Desexa tentalo de novo?"
${LangFileString} MPV_DL_Failed "A descarga de MPV fallou: «$R0»."
${LangFileString} MPV_Inst_Failed "Produciuse un fallo ao instalar MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "Non se instalou correctamente YouTube-DL. Desexa tentalo de novo?"
${LangFileString} YTDL_DL_Failed "A descarga de Youtube-DL fallou: «$R0»."
