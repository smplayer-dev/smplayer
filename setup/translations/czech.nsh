;Language: Czech (1029)
;Czech language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Czech" "Čeština"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Instalátor již běží."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Instalaci tohoto programu je potřeba provést s právy administrátora."

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "Binární kodeky podporují formáty, které zatím nejsou implementovány nativně, např. novější varianty RealVideo a jiné málo používané formáty.$\nPro většinu běžných formátů nejsou potřeba (DVD, MPEG-1/2/4, apod.)."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Stahuji MPlayer..."
  ${LangFileString} MPLAYER_DL_RETRY "MPlayer se nepovedlo nainstalovat. Zkusit znovu?"
  ${LangFileString} MPLAYER_DL_FAILED "Nepovedlo se stáhnout MPlayer: '$R0'."
  ${LangFileString} MPLAYER_INST_FAILED "Nepovedlo se nainstalovat MPlayer. MPlayer je potřebný pro přehrávání."
!endif

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Instaluji kodeky MPlayeru..."
${LangFileString} CODECS_DL_RETRY "Kodeky MPlayeru se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} CODECS_DL_FAILED "Nepovedlo se stáhnout kodeky MPlayeru: '$R0'."
${LangFileString} CODECS_INST_FAILED "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Stahuji informace o verzích..."
${LangFileString} VERINFO_DL_FAILED "Nepovedlo se stáhnout informace o verzích: '$R0'. Užita výchozí verze."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Odinstalaci je potřeba provést s právy administrátora."
${LangFileString} UNINSTALL_ABORTED "Odinstalace přerušena uživatelem."
${LangFileString} SMPLAYER_NOT_INSTALLED "V adresáři '$INSTDIR' není SMPlayer nainstalován .$\r$\nPokračovat (nedoporučeno)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer je kompletní frontend pro MPlayer, podporuje vše od základních funkcí jako přehrávání videí, DVD, VCD až po pokročilé funkce jako filtry MPlayeru, edl a více."