;;
;;  czech.nsh
;;
;;  Default language strings for the Windows SMPlayer NSIS installer.
;;
;;  Replace all text between the string with the translation.
;;
;;  Do not edit this line!
!insertmacro LANGFILE_EXT "Czech"
;;

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "Binární kodeky podporují formáty, které zatím nejsou implementovány nativně, např. novější varianty RealVideo a jiné málo používané formáty.$\nPro většinu běžných formátů nejsou potřeba (DVD, MPEG-1/2/4, apod.)."

; MPlayer Section
${LangFileString} MPLAYER_IS_INSTALLED "MPlayer je již nainstalován. Stáhnout znovu?"
${LangFileString} MPLAYER_IS_DOWNLOADING "Stahuji MPlayer..."
${LangFileString} MPLAYER_DL_FAILED "Nepovedlo se stáhnout MPlayer:"
${LangFileString} MPLAYER_INST_FAILED "Nepovedlo se nainstalovat MPlayer. MPlayer je potřebný pro přehrávání."

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Kodeky MPlayeru jsou již nainstalovány. Stáhnout znovu?"
${LangFileString} CODECS_IS_DOWNLOADING "Instaluji kodeky MPlayeru..."
${LangFileString} CODECS_DL_FAILED "Nepovedlo se stáhnout kodeky MPlayeru:"
${LangFileString} CODECS_INST_FAILED "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Stahuji informace o verzích..."
${LangFileString} VERINFO_DL_FAILED "Nepovedlo se stáhnout informace o verzích:"
${LangFileString} VERINFO_IS_MISSING "Verzovací soubor neobsahuje správné informace. Bude použita výchozí verze."