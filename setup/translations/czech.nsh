;Language: Czech (1029)
;Czech language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Czech" "Čeština"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Instalátor již běží."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "You must be logged in as an administrator when installing this program."
${LangFileString} SMPLAYER_INSTALLER_PREV_VERSION "SMPlayer je již nainstalován.$\nPřejete si odstranit předchozí verzi před spuštěním instalace $(^Name)?"

; Components Page
${LangFileString} MPLAYER_CODEC_INFORMATION "Binární kodeky podporují formáty, které zatím nejsou implementovány nativne, napr. novejší varianty RealVideo a jiné málo používané formáty.$\nPro vetšinu bežných formátu nejsou potreba (DVD, MPEG-1/2/4, apod.)."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "MPlayer je již nainstalován. Stáhnout znovu?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Stahuji MPlayer..."
  ${LangFileString} MPLAYER_DL_FAILED "Nepovedlo se stáhnout MPlayer:"
  ${LangFileString} MPLAYER_INST_FAILED "Nepovedlo se nainstalovat MPlayer. MPlayer je potrebný pro prehrávání."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Kodeky MPlayeru jsou již nainstalovány. Stáhnout znovu?"
${LangFileString} CODECS_IS_DOWNLOADING "Instaluji kodeky MPlayeru..."
${LangFileString} CODECS_DL_FAILED "Nepovedlo se stáhnout kodeky MPlayeru:"
${LangFileString} CODECS_INST_FAILED "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Stahuji informace o verzích..."
${LangFileString} VERINFO_DL_FAILED "Nepovedlo se stáhnout informace o verzích:"
${LangFileString} VERINFO_IS_MISSING "Verzovací soubor neobsahuje správné informace. Bude použita výchozí verze."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} UNINSTALL_ABORTED "Odinstalace přerušena uživatelem."
${LangFileString} SMPLAYER_NOT_INSTALLED "V adresáři '$INSTDIR' není SMPlayer nainstalován .$\r$\nPokračovat (nedoporučeno)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer je kompletní frontend pro MPlayer, podporuje vše od základních funkcí jako přehrávání videí, DVD, VCD až po pokročilé funkce jako filtry MPlayeru, edl a více."