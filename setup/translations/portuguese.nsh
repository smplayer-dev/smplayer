;;
;;  Portuguese.nsh
;;
;;  Default language strings for the Windows SMPlayer NSIS installer.
;;
;;  Replace all text between the string with the translation.
;;
;;  Do not edit this line!
!insertmacro LANGFILE_EXT "Portuguese"
;;

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "O pacote de codecs binários adiciona suporte para os codecs que ainda não foram implementados, tais como as novas variantes RealVideo e alguns formatos não usuais.$\nNote que estes não serão necessários para os formatos mais comuns como DVDs, MPEG-1/2/4, etc."

; MPlayer Section
${LangFileString} MPLAYER_IS_INSTALLED "O MPlayer já está instalado. Re-Transferir?"
${LangFileString} MPLAYER_IS_DOWNLOADING "Transferindo MPlayer..."
${LangFileString} MPLAYER_DL_FAILED "Falha ao transferir MPlayer:"
${LangFileString} MPLAYER_INST_FAILED "Falha ao instalar MPlayer. O MPlayer é necessário para reproduzir."

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Os codecs MPlayer já estão instalados. Re-Transferir?"
${LangFileString} CODECS_IS_DOWNLOADING "Transferindo codecs MPlayer..."
${LangFileString} CODECS_DL_FAILED "Falha ao transferir os codecs MPlayer:"
${LangFileString} CODECS_INST_FAILED "Falha ao instalar os codecs MPlayer."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Transferindo informações sobre a versão..."
${LangFileString} VERINFO_DL_FAILED "Falha ao transferir informações sobre a versão:"
${LangFileString} VERINFO_IS_MISSING "Informação sobre a versão em falta. A configuração irá utilizar a versão padrão."