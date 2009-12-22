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
${LangFileString} MPLAYER_CODEC_INFORMATION "A bináris kodek csomagok támogatást nyújtanak natívan még nem támogatott kodekekhez, mint pl. az újabb RealVideo variánsok és sok ritka formátum.$\nNem szükségesek a legtöbb gyakori formátum lejátszásához, mint a DVD-k, MPEG-1/2/4, stb."

; MPlayer Section
!ifndef WITH_MPLAYER
  ${LangFileString} MPLAYER_IS_INSTALLED "Az MPlayer már telepítve van. Újra letöltsem?"
  ${LangFileString} MPLAYER_IS_DOWNLOADING "Az MPlayer letöltése..."
  ${LangFileString} MPLAYER_DL_RETRY "Az MPlayer telepítése nem sikerült. Újra próbáljam?"
  ${LangFileString} MPLAYER_DL_FAILED "Az MPlayer letöltése nem sikerült: '$R0'."
  ${LangFileString} MPLAYER_INST_FAILED "Az MPlayer telepítése nem sikerült. Az MPlayerre szükség van a lejátszáshoz."
!endif

; Codecs Section
${LangFileString} CODECS_IS_INSTALLED "Az MPlayer kodekek már telepítve vannak. Újra letöltsem?"
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
