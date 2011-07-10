;Language: Spanish (1034)
;Spanish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Spanish" "Español"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "La instalación ya se está ejecutando."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Debes tener permisos de administrador para instalar este programa."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (required)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, shared libraries, and documentation."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Shortcuts"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Desktop"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Creates a shortcut to SMPlayer on the desktop."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Start Menu"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Create a Start Menu entry for SMPlayer."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "MPlayer Components"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (required)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; required for playback."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; required for playback. (Internet Connection required for installation)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Binary Codecs"
${LangFileString} SMPLAYER_SECCODECS_DESC "Optional codecs for MPlayer. (Internet Connection required for installation)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Icon Themes"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Additional icon themes for SMPlayer."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Localizations"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Non-English localizations."

${LangFileString} MPLAYER_CODEC_INFORMATION "El paquete de códecs binarios añade soporte para códecs que todavía no se han implementado nativamente, como como las nuevas variantes de RealVideo y algunos formatos poco comunes.$\nTen en cuenta de que no son necesarios para reproducir los formatos más comunes como DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Already Installed"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Choose how you want to install SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Choose the maintenance option to perform."

${LangFileString} REINSTALL_CHGSETTINGS "Change settings (advanced)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unknown value of PREVIOUS_VERSION_STATE, aborting"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "An older version of SMPlayer is installed on your system. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Upgrade SMPlayer using previous settings (recommended)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "A newer version of SMPlayer is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Downgrade SMPlayer using previous settings (recommended)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} is already installed. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Add/Remove/Reinstall components"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Uninstall SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Descargando el MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "El MPlayer no se ha instalado correctamente. ¿Reintentar?"
${LangFileString} MPLAYER_DL_FAILED "Ha fallado la descarga del MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Ha fallado la instalación del MPlayer. El MPlayer es imprescindible o no se podrá reproducir nada."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Descargando los códecs del MPlayer..."
${LangFileString} CODECS_DL_RETRY "Los códecs del MPlayer no se han instalado correctamente. ¿Reintentar?"
${LangFileString} CODECS_DL_FAILED "Ha fallado la descarga de los códecs del MPlayer: '$R0'."
${LangFileString} CODECS_INST_FAILED "Ha fallado la instalación del los códecs del MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Descargando la información de la versión..."
${LangFileString} VERINFO_DL_FAILED "Ha fallado la descarga de la información de la versión: '$R0'. Se usará la versión por defecto."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Este programa sólo se puede desinstalar por un usuario con permisos de administrador."
${LangFileString} UNINSTALL_ABORTED "Desinstalación cancelada por el usuario."
${LangFileString} SMPLAYER_NOT_INSTALLED "No parece que el SMPlayer esté instalado en el directorio '$INSTDIR'.$\r$\n¿Continuar de todos modos (no recomendado)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer pretende ser un interfaz completo para MPlayer, con opciones para cosas básicas, como reproducir vídeos, DVDs y VCDs hasta opciones más avanzadas como soporte para los filtros del MPlayer y mucho más."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
