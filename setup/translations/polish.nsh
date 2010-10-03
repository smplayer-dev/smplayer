;Language: Polish (1045)
;Polish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Polish" "Polski"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Instalator już jest uruchomiony."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Do instalacji tego programu wymagane są uprawnienia administratora."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (wymagane)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, biblioteki współdzielone, dokumentacja."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Skróty"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Pulpit"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Tworzy skrót do programu na pulpicie."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Menu Start"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Tworzy skrót do programu w Menu Start."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "Komponenty MPlayera"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (wymagane)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; wymagany doo odtwarzania filmów."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; wymagany doo odtwarzania filmów. (Do instalacji wymagane połączenie z internetem)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Kodeki"
${LangFileString} SMPLAYER_SECCODECS_DESC "Opcjonalne kodeki do MPlayera. (Do instalacji wymagane połączenie z internetem)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Zesatwy Ikon"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Dodatkowe zestawy ikon do SMPlayera."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Tłumaczenia"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Nie-angielskie tłumaczenia."

${LangFileString} MPLAYER_CODEC_INFORMATION "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Już zainstalowany"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Jak chcesz zainstalować SMPlayera."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Choose the maintenance option to perform."

${LangFileString} REINSTALL_CHGSETTINGS "Zmień ustawienia (zaawansowane)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Nieznana wartość PREVIOUS_VERSION_STATE, anulowanie"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "An older version of SMPlayer is installed on your system. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Upgrade SMPlayer using previous settings (recommended)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "A newer version of SMPlayer is already installed! It is not recommended that you downgrade to an older version. Select the operation you want to perform and click Next to continue."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Downgrade SMPlayer using previous settings (recommended)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} jest juz zainstalowany. Wybierz akcję, którą chcesz wykonac i kliknij Dalej."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Dodaj/Usuń/Przeinstaluj komponenty"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Odinstaluj SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Pobieram MPlayera..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer was not successfully installed. Retry?"
${LangFileString} MPLAYER_DL_FAILED "Failed to download MPlayer: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Failed to install MPlayer. MPlayer is required for playback."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Pobieram kodeki do MPlayera..."
${LangFileString} CODECS_DL_RETRY "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} CODECS_DL_FAILED "Failed to download MPlayer codecs: '$R0'."
${LangFileString} CODECS_INST_FAILED "Failed to install MPlayer codecs."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Pobieram informacje o wersji..."
${LangFileString} VERINFO_DL_FAILED "Failed to download version info: '$R0'. Using a default version."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} UNINSTALL_ABORTED "Uninstall aborted by user."
${LangFileString} SMPLAYER_NOT_INSTALLED "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."
