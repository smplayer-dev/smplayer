;Language: Spanish (1034)
;Spanish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Spanish" "Español"

; Startup
${LangFileString} Installer_Is_Running "La instalación ya se está ejecutando."
${LangFileString} Installer_No_Admin "Debes tener permisos de administrador para instalar este programa."

; Components Page
${LangFileString} ShortcutGroupTitle "Shortcuts"
${LangFileString} MPlayerGroupTitle "MPlayer Components"

${LangFileString} Section_SMPlayer "SMPlayer (required)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, shared libraries, and documentation."

${LangFileString} Section_DesktopShortcut "Desktop"
${LangFileString} Section_DesktopShortcut_Desc "Creates a shortcut to SMPlayer on the desktop."

${LangFileString} Section_StartMenu "Start Menu"
${LangFileString} Section_StartMenu_Desc "Create a Start Menu entry for SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (required)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; required for playback."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; required for playback. (Internet Connection required for installation)"
!endif

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Optional codecs for MPlayer."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Optional codecs for MPlayer. (Internet Connection required for installation)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Icon Themes"
${LangFileString} Section_IconThemes_Desc "Additional icon themes for SMPlayer."

${LangFileString} Section_Translations "Languages"
${LangFileString} Section_Translations_Desc "Non-English language files for SMPlayer."

${LangFileString} MPlayer_Codec_Msg "El paquete de códecs binarios añade soporte para códecs que todavía no se han implementado nativamente, como como las nuevas variantes de RealVideo y algunos formatos poco comunes.$\nTen en cuenta de que no son necesarios para reproducir los formatos más comunes como DVDs, MPEG-1/2/4, etc."

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
${LangFileString} MPlayer_DL_Msg "Descargando el MPlayer..."
${LangFileString} MPlayer_DL_Retry "El MPlayer no se ha instalado correctamente. ¿Reintentar?"
${LangFileString} MPlayer_DL_Failed "Ha fallado la descarga del MPlayer: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Ha fallado la instalación del MPlayer. El MPlayer es imprescindible o no se podrá reproducir nada."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Descargando los códecs del MPlayer..."
${LangFileString} Codecs_DL_Retry "Los códecs del MPlayer no se han instalado correctamente. ¿Reintentar?"
${LangFileString} Codecs_DL_Failed "Ha fallado la descarga de los códecs del MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Ha fallado la instalación del los códecs del MPlayer."

; Version information
${LangFileString} VerInfo_DL_Msg "Descargando la información de la versión..."
${LangFileString} VerInfo_DL_Failed "Ha fallado la descarga de la información de la versión: '$R0'. Se usará la versión por defecto."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Este programa sólo se puede desinstalar por un usuario con permisos de administrador."
${LangFileString} Uninstaller_Aborted "Desinstalación cancelada por el usuario."
${LangFileString} Uninstaller_NotInstalled "No parece que el SMPlayer esté instalado en el directorio '$INSTDIR'.$\r$\n¿Continuar de todos modos (no recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer pretende ser un interfaz completo para MPlayer, con opciones para cosas básicas, como reproducir vídeos, DVDs y VCDs hasta opciones más avanzadas como soporte para los filtros del MPlayer y mucho más."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_Files_Extract "Extracting files..."
