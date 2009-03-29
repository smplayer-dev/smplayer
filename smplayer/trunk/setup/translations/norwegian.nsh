;;
;;  norwegian.nsh
;;
;;  Default language strings for the Windows SMPlayer NSIS installer.
;;
;;  Replace all text between the string with the translation.
;;

!insertmacro LANGFILE_EXT "Norwegian"

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; MPlayer Section
${LangFileString} MPLAYER_IS_INSTALLED "MPlayer is already installed. Re-Download?"
${LangFileString} MPLAYER_IS_DOWNLOADING "Downloading MPlayer..."
${LangFileString} MPLAYER_DL_FAILED "Failed to download MPlayer:"
${LangFileString} MPLAYER_INST_FAILED "Failed to install MPlayer. MPlayer is required for playback."

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "MPlayer codecs are already installed. Re-Download?"
${LangFileString} CODECS_IS_DOWNLOADING "Downloading MPlayer codecs..."
${LangFileString} CODECS_DL_FAILED "Failed to download MPlayer codecs:"
${LangFileString} CODECS_INST_FAILED "Failed to install MPlayer codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Downloading version information..."
${LangFileString} VERINFO_DL_FAILED "Failed to download version info:"
${LangFileString} VERINFO_IS_MISSING "Version file missing version information. Setup will use a default version."