;;
;;  hungarian.nsh
;;
;;  Hungarian language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_HUNGARIAN} "A telepíto már fut."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_HUNGARIAN} "Nem támogatott operációs rendszer.$\n$(^Name) legalább Windows 2000-et igényel és lehet hogy nem fog megfeleloen muködni a rendszerén.$\nBiztosan folytatni akarja a telepítést?"
LangString SMPLAYER_INSTALLER_NO_ADMIN ${LANG_HUNGARIAN} "Rendszergazdaként kell bejelentkeznie a program telepítéséhez."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_HUNGARIAN} "Az SMPlayer már telepítve van.$\nEl akarja távolítani az elozo verziót $(^Name) telepítése elott?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_HUNGARIAN} "A bináris kodek csomagok támogatást nyújtanak natívan még nem támogatott kodekekhez, mint pl. az újabb RealVideo variánsok és sok ritka formátum.$\nNem szükségesek a legtöbb gyakori formátum lejátszásához, mint a DVD-k, MPEG-1/2/4, stb."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_HUNGARIAN} "Az MPlayer már telepítve van. Újra letöltsem?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_HUNGARIAN} "Az MPlayer letöltése..."
LangString MPLAYER_DL_FAILED ${LANG_HUNGARIAN} "Az MPlayer letöltése nem sikerült:"
LangString MPLAYER_INST_FAILED ${LANG_HUNGARIAN} "Az MPlayer telepítése nem sikerült. Az MPlayerre szükség van a lejátszáshoz."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_HUNGARIAN} "Az MPlayer kodekek már telepítve vannak. Újra letöltsem?"
LangString CODECS_IS_DOWNLOADING ${LANG_HUNGARIAN} "MPlayer kodekek letöltése..."
LangString CODECS_DL_FAILED ${LANG_HUNGARIAN} "Az MPlayer kodekek letöltése nem sikerült:"
LangString CODECS_INST_FAILED ${LANG_HUNGARIAN} "Az MPlayer kodekek telepítése nem sikerült."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_HUNGARIAN} "Verzió információ letöltése..."
LangString VERINFO_DL_FAILED ${LANG_HUNGARIAN} "Verzió információ letöltése nem sikerült:"
LangString VERINFO_IS_MISSING ${LANG_HUNGARIAN} "A verzió fájlból hiányzik a verzió információ. A telepítõ egy alapértelmezett verziót fog használni."

; Uninstaller
LangString UNINSTALL_NO_ADMIN ${LANG_HUNGARIAN} "A telepítés eltávolításhoz rendszergazdai jogosultságok szükségesek."
LangString UNINSTALL_ABORTED ${LANG_HUNGARIAN} "Uninstall aborted by user."
LangString SMPLAYER_NOT_INSTALLED ${LANG_HUNGARIAN} "Nem úgy néz ki, mintha az SMPlayer ebbe a könyvtárba lett volna telepítve: '$INSTDIR'.$\r$\nMégis folytassam (nem ajánlott)?"

; Vista & Later Default Programs Registration
LangString APPLICATION_DESCRIPTION ${LANG_HUNGARIAN} "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."