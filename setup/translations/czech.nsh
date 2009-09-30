;;
;;  czech.nsh
;;
;;  Czech language strings for the Windows SMPlayer NSIS installer.
;;  Save file with ANSI encoding
;;
;;  Author: 
;;

; Startup
LangString SMPLAYER_INSTALLER_IS_RUNNING ${LANG_CZECH} "Instal·tor jiû bÏûÌ."
LangString SMPLAYER_INSTALLER_UNSUPPORTED_OS ${LANG_CZECH} "Nepodporovan˝ opreËnÌ systÈm.$\n$(^Name) vyûaduje alespoÚ Windows 2000 a nemusel by dob¯e fungovat na vaöem systÈmu.$\nOpravdu chcete pokraËovat v instalaci?"
LangString SMPLAYER_INSTALLER_PREV_ALL_USERS ${LANG_CZECH} "SMPlayer byl minule nainstalov·n pro vöechny uûivatele.$\nProsÌm spusùte instal·tor znovu s pr·vy Administr·tora."
LangString SMPLAYER_INSTALLER_PREV_VERSION ${LANG_CZECH} "SMPlayer je jiû nainstalov·n.$\nP¯ejete si odstranit p¯edchozÌ verzi p¯ed spuötÏnÌm instalace $(^Name)?"

; Components Page
LangString MPLAYER_CODEC_INFORMATION ${LANG_CZECH} "Bin·rnÌ kodeky podporujÌ form·ty, kterÈ zatÌm nejsou implementov·ny nativne, napr. novejöÌ varianty RealVideo a jinÈ m·lo pouûÌvanÈ form·ty.$\nPro vetöinu beûn˝ch form·tu nejsou potreba (DVD, MPEG-1/2/4, apod.)."

; MPlayer Section
LangString MPLAYER_IS_INSTALLED ${LANG_CZECH} "MPlayer je jiû nainstalov·n. St·hnout znovu?"
LangString MPLAYER_IS_DOWNLOADING ${LANG_CZECH} "Stahuji MPlayer..."
LangString MPLAYER_DL_FAILED ${LANG_CZECH} "Nepovedlo se st·hnout MPlayer:"
LangString MPLAYER_INST_FAILED ${LANG_CZECH} "Nepovedlo se nainstalovat MPlayer. MPlayer je potrebn˝ pro prehr·v·nÌ."

; Codecs Section
LangString CODECS_IS_INSTALLED ${LANG_CZECH} "Kodeky MPlayeru jsou jiû nainstalov·ny. St·hnout znovu?"
LangString CODECS_IS_DOWNLOADING ${LANG_CZECH} "Instaluji kodeky MPlayeru..."
LangString CODECS_DL_FAILED ${LANG_CZECH} "Nepovedlo se st·hnout kodeky MPlayeru:"
LangString CODECS_INST_FAILED ${LANG_CZECH} "Nepovedlo se nainstalovat kodeky MPlayeru."

; Version information
LangString VERINFO_IS_DOWNLOADING ${LANG_CZECH} "Stahuji informace o verzÌch..."
LangString VERINFO_DL_FAILED ${LANG_CZECH} "Nepovedlo se st·hnout informace o verzÌch:"
LangString VERINFO_IS_MISSING ${LANG_CZECH} "VerzovacÌ soubor neobsahuje spr·vnÈ informace. Bude pouûita v˝chozÌ verze."

; Uninstaller
LangString UNINSTALL_ABORTED ${LANG_CZECH} "Uninstall aborted by user."
LangString UNINSTALL_INSTALLED_ALL_USERS ${LANG_CZECH} "SMPlayer byl nainstalov·n pro vöechny uûivatele.$\nProsÌm spusùte odinstal·tor znovu s pr·vy Administr·tora."
LangString SMPLAYER_NOT_INSTALLED ${LANG_CZECH} "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
LangString APPLICATION_DESCRIPTION ${LANG_CZECH} "SMPlayer je kompletnÌ frontend pro MPlayer, podporuje vöe od z·kladnÌch funkcÌ jako p¯ehr·v·nÌ videÌ, DVD, VCD aû po pokroËilÈ funkce jako filtry MPlayeru, edl a vÌce."