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
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; wymagany do odtwarzania filmów."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; wymagany do odtwarzania filmów. (Wymagane połączenie z internetem)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Kodeki"
${LangFileString} SMPLAYER_SECCODECS_DESC "Opcjonalne kodeki do MPlayera. (Wymagane połączenie z internetem)"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Zesatwy Ikon"
${LangFileString} SMPLAYER_SECTHEMES_DESC "Dodatkowe zestawy ikon do SMPlayera."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Tłumaczenia"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "Tłumaczenia na inne języki."

${LangFileString} MPLAYER_CODEC_INFORMATION "Paczka z kodekami dodaje wsparcie dla kodeków, które nie są jeszcze natywnie wspierane, np. nowsze warianty RealVideo, oraz wiele mniej znanych formatów.$\nProszę mieć na uwadze, że paczka nie jest wymagana do odtworzenia popularnych formatów, takich jak DVD, MPEG-1/2/4, itp."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Już zainstalowany"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Wybierz sposób instalacji SMPlayera."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Wybierz czynność do wykonania."

${LangFileString} REINSTALL_CHGSETTINGS "Zmień ustawienia (zaawansowane)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Nieznana wartość PREVIOUS_VERSION_STATE, anulowanie"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "Starsza wersja SMPlayera jest już zainstalowana w systemie. Wybierz akcję, którą chcesz wykonać i kliknij przycisk Dalej."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Zaktualizuj SMPlayera używając obecnych ustawień (zalecane)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "Zainstalowana jest nowsza wersja SMPlayera! Nie zalecane jest instalowanie starszej wersji. Wybierz akcję, którą chcesz wykonać i kliknij przycisk Dalej."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Zainstaluj starszą wersję SMPlayera używając obecnych ustawień (zalecane)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} jest już zainstalowany. Wybierz akcję, którą chcesz wykonać i kliknij przycisk Dalej."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Dodaj/Usuń/Przeinstaluj komponenty"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Odinstaluj SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "Pobieram MPlayera..."
${LangFileString} MPLAYER_DL_RETRY "Instalacja MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} MPLAYER_DL_FAILED "Nie udało się pobrać MPlayera: '$R0'."
${LangFileString} MPLAYER_INST_FAILED "Nie udało się zainstalować MPlayera. Bez MPlayera odtwarzanie filmów jest nie możliwe."

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "Pobieram kodeki do MPlayera..."
${LangFileString} CODECS_DL_RETRY "Instalacja Kodeków MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} CODECS_DL_FAILED "Nie udało się pobrać Kodeków MPlayera: '$R0'."
${LangFileString} CODECS_INST_FAILED "Nie udało się zainstalować Kodeków MPlayera."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Pobieram informacje o wersji..."
${LangFileString} VERINFO_DL_FAILED "Nie udało się pobrac informacji o wersji: '$R0'. Użyta zostanie wersja domyślna."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Aplikacja może być usunięta tylko przez osobe z uprawnieniami Administratora."
${LangFileString} UNINSTALL_ABORTED "Usuwanie anulowane przez użytkownika."
${LangFileString} SMPLAYER_NOT_INSTALLED "Nie wygląda na to, aby SMPlayer został zainstalowany w katalogu '$INSTDIR'.$\r$\nKontynuować mimo to (nie zalecane)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer jest graficzną nakładką programu MPlayer, z prostymi funkcjami, takimi jak odtwarzanie wideo, DVD i VCD oraz z bardziej zaawansowanymi funkcjami np: obsługa filtrów MPlayera i wiele innych."

; Misc
${LangFileString} INFO_DEL_FILES "Deleting Files..."
${LangFileString} INFO_DEL_REGISTRY "Deleting Registry Keys..."
${LangFileString} INFO_DEL_SHORTCUTS "Deleting Shortcuts..."
${LangFileString} INFO_REST_ASSOC "Restoring file associations..."
${LangFileString} INFO_FILE_EXTRACT "Extracting files..."
