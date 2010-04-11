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

; Upgrade/Reinstall Page
# MUI Headers
${LangFileString} REINSTALL_HEADER_TEXT "Already Installed"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Choose how you want to install SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Choose the maintenance option to perform."
# Misc
${LangFileString} REINSTALL_CHGSETTINGS "Change settings (advanced)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Unknown value of PREVIOUS_VERSION_STATE, aborting"
# Older version detected
${LangFileString} REINSTALL_OLDVER_DESCRIPTION "An older version of SMPlayer is installed on your system. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Upgrade SMPlayer using previous settings (recommended)"
# Newer version detected
${LangFileString} REINSTALL_NEWVER_DESCRIPTION "A newer version of SMPlayer is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Downgrade SMPlayer using previous settings (recommended)"
# Same version detected
${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} is already installed. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Add/Remove/Reinstall components"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Uninstall SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Stahuji MPlayer..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer se nepovedlo nainstalovat. Zkusit znovu?"
${LangFileString} MPLAYER_DL_FAILED "Nepovedlo se stáhnout MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Nepovedlo se nainstalovat MPlayer. MPlayer je potřebný pro přehrávání."

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