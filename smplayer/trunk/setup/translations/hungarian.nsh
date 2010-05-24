;Language: Hungarian (1038)
;Hungarian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Hungarian" "Magyar"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "A telepítő már fut."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Rendszergazdaként kell bejelentkeznie a program telepítéséhez."

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

${LangFileString} MPLAYER_CODEC_INFORMATION "A bináris kodek csomagok támogatást nyújtanak natívan még nem támogatott kodekekhez, mint pl. az újabb RealVideo variánsok és sok ritka formátum.$\nNem szükségesek a legtöbb gyakori formátum lejátszásához, mint a DVD-k, MPEG-1/2/4, stb."

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
${LangFileString} MPLAYER_IS_DOWNLOADING "Az MPlayer letöltése..."
${LangFileString} MPLAYER_DL_RETRY "Az MPlayer telepítése nem sikerült. Újra próbáljam?"
${LangFileString} MPLAYER_DL_FAILED "Az MPlayer letöltése nem sikerült: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Az MPlayer telepítése nem sikerült. Az MPlayerre szükség van a lejátszáshoz."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "MPlayer kodekek letöltése..."
${LangFileString} CODECS_DL_RETRY "Az MPlayer kodekek telepítése nem sikerült. Újra próbáljam?"
${LangFileString} CODECS_DL_FAILED "Az MPlayer kodekek letöltése nem sikerült: '$R0'."
${LangFileString} CODECS_INST_FAILED "Az MPlayer kodekek telepítése nem sikerült."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Verzió információ letöltése..."
${LangFileString} VERINFO_DL_FAILED "Verzió információ letöltése nem sikerült: '$R0'. Alapértelmezett verzió használata."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "A program eltávolításához rendszergazda jogosultság szükséges."
${LangFileString} UNINSTALL_ABORTED "Az eltávolítást a felhasználó megszakította."
${LangFileString} SMPLAYER_NOT_INSTALLED "Nem úgy néz ki, mint ha az SMPlayer ebbe a könyvtárba lett volna telepítve: '$INSTDIR'.$\r$\nMégis folytassam (nem ajánlott)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "Az SMPlayer egy komplett felület az MPlayerhez, mindent támogat az alap funkcióktól kezdve, mint a videók, DVDk, VCDk lejátszása, haladó funkciókig, mint az MPlayer szűrők, edl listák és még sok más."
