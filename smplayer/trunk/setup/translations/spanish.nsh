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
${LangFileString} MPLAYER_CODEC_INFORMATION "El paquete de códecs binarios añade soporte para códecs que todavía no se han implementado nativamente, como como las nuevas variantes de RealVideo y algunos formatos poco comunes.$\nTen en cuenta de que no son necesarios para reproducir los formatos más comunes como DVDs, MPEG-1/2/4, etc."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "El MPlayer ya está instalado. ¿Descargar de nuevo?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Descargando el MPlayer..."
  ${LangFileString} MPLAYER_DL_RETRY "El MPlayer no se ha instalado correctamente. ¿Reintentar?"
  ${LangFileString} MPLAYER_DL_FAILED "Ha fallado la descarga del MPlayer: '$R0'."
  ${LangFileString} MPLAYER_INST_FAILED "Ha fallado la instalación del MPlayer. El MPlayer es imprescindible o no se podrá reproducir nada."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Los códecs del MPlayer ya están instalados. ¿Descargar de nuevo?"
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
