;Language: Polish (1045)
;Polish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Polish" "Polski"

; Startup
${LangFileString} Installer_Is_Running "Instalator już jest uruchomiony."
${LangFileString} Installer_No_Admin "Do instalacji tego programu wymagane są uprawnienia administratora."
${LangFileString} SMPlayer_Is_Running "Instancja SMPlayera jest już uruchomiona. Proszę zamknij SMPlayera i spróbuj ponownie."

${LangFileString} OS_Not_Supported "Nieobsługiwany system operacyjny.$\nSMPlayer ${SMPLAYER_VERSION} wymaga co najmniej Windows XP i może nie działać poprawnie na twoim systemie.$\nCzy naprawdę chcesz kontynuować instalację?"
${LangFileString} Win64_Required "Do zainstalowania tego programu wymagany jest system operacyjny Windows w wersji 64-bitowej."
${LangFileString} Existing_32bitInst "W systemie jest już zainstalowany SMPlayer w wersji 32-bitowej. Aby kontynuować należy najpierw odinstalować 32-bitową wersję SMPlayera."
${LangFileString} Existing_64bitInst "W systemie jest już zainstalowany SMPlayer w wersji 64-bitowej. Aby kontynuować należy najpierw odinstalować 64-bitową wersję SMPlayera."

; Welcome page
${LangFileString} WelcomePage_Title "Instalator $(^NameDA)"
${LangFileString} WelcomePage_Text "Instalator poprowadzi Cię przez proces instalacji $(^NameDA).$\r$\n$\r$\nZalecane jest zamknięcie SMPlayera przed rozpoczęciem instalacji. Dzięki temu możliwe będzie zaktualizowanie odpowiednich plików programu, bez konieczności ponownego uruchamiania komputera.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Skróty"
${LangFileString} MPlayerGroupTitle "Komponenty MPlayera"

${LangFileString} Section_SMPlayer "SMPlayer (wymagany)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, biblioteki współdzielone, dokumentacja."

${LangFileString} Section_DesktopShortcut "Pulpit"
${LangFileString} Section_DesktopShortcut_Desc "Tworzy skrót do programu na pulpicie."

${LangFileString} Section_StartMenu "Menu Start"
${LangFileString} Section_StartMenu_Desc "Tworzy skrót do programu w Menu Start."

${LangFileString} Section_MPlayer "MPlayer (wymagany)"
${LangFileString} Section_MPlayer_Desc "MPlayer; wymagany do odtwarzania filmów."

${LangFileString} Section_MPlayerCodecs "Kodeki"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Kodeki binarne nie są obsługiwane w tej wersji."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Opcjonalne kodeki do MPlayera. (Wymagane połączenie z internetem)"
!endif

${LangFileString} Section_MEncoder_Desc "Jest to prosty koder (kompresor) filmów, zaprojektowany do kodowania filmów otwieralnych MPlayerem do innych obsługiwanych formatów"

${LangFileString} Section_IconThemes "Zestawy Ikon"
${LangFileString} Section_IconThemes_Desc "Dodatkowe zestawy ikon do SMPlayera."

${LangFileString} Section_Translations "Tłumaczenia"
${LangFileString} Section_Translations_Desc "Tłumaczenia na inne języki."

${LangFileString} Section_ResetSettings_Desc "Usuwa preferencje SMPlayera pozostawione z poprzednich instalacji."

${LangFileString} MPlayer_Codec_Msg "Paczka z kodekami dodaje wsparcie dla formatów, które nie są jeszcze natywnie wspierane, np. nowsze warianty RealVideo, oraz dla wielu mniej znanych formatów.$\nMiej na uwadze, że dodatkowe kodeki nie są wymagane do odtwarzania najpopularniejszych formatów, takich jak DVD, MPEG-1/2/4, itp."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Wybierz typ instalacji"
${LangFileString} Reinstall_Header_SubText "Wybierz tryb nadpisywania lub odinstalowywania."

${LangFileString} Reinstall_Msg1 "Posiadasz już zainstalowanego SMPlayera w folderze:"
${LangFileString} Reinstall_Msg2 "Wybierz, co następnie zrobić:"
${LangFileString} Reinstall_Overwrite "Nadpisz ($Inst_Type) istniejącą instalację"
${LangFileString} Reinstall_Uninstall "Odinstaluj (usuń) istniejącą instalację"
${LangFileString} Reinstall_Msg3_1 "Kliknij Start, gdy będziesz gotów."
${LangFileString} Reinstall_Msg3_2 "Kliknij Dalej, gdy będziesz gotów."
${LangFileString} Reinstall_Msg3_3 "Kliknij Odinstaluj, gdy będziesz gotów."
${LangFileString} Reinstall_Msg4 "Zmiana Ustawień Instalacji"
${LangFileString} Reinstall_Msg5 "Zresetuj moją konfigurację SMPlayera"

${LangFileString} Type_Reinstall "zainstaluj ponownie"
${LangFileString} Type_Downgrade "aktualizuj do starszej wersji"
${LangFileString} Type_Upgrade "aktualizuj"

${LangFileString} StartBtn "Uruchom"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Pobieram kodeki do MPlayera..."
${LangFileString} Codecs_DL_Retry "Instalacja Kodeków MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} Codecs_DL_Failed "Nie udało się pobrać Kodeków MPlayera: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nie udało się zainstalować Kodeków MPlayera."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Aplikacja może być usunięta tylko przez osobę z uprawnieniami Administratora."
${LangFileString} Uninstaller_Aborted "Usuwanie anulowane przez użytkownika."
${LangFileString} Uninstaller_NotInstalled "Wygląda na to, że SMPlayer nie został zainstalowany w katalogu '$INSTDIR'.$\r$\nKontynuować mimo to (nie zalecane)?"
${LangFileString} Uninstaller_InvalidDirectory "Nie znaleziono śladów instalacji SMPlayera."
${LangFileString} Uninstaller_64bitOnly "Aplikacja może być usunięta tylko na 64-bitowym systemie Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer jest graficzną nakładką programu MPlayer, z prostymi funkcjami, takimi jak odtwarzanie wideo, DVD i VCD oraz z bardziej zaawansowanymi funkcjami np: obsługa filtrów MPlayera i wiele innych."

; Misc
${LangFileString} Info_Codecs_Backup "Tworzenie kopii zapasowej kodeków z poprzedniej instalacji..."
${LangFileString} Info_Codecs_Restore "Przywracanie kodeków z poprzedniej instalacji..."
${LangFileString} Info_Del_Files "Usuwanie Plików..."
${LangFileString} Info_Del_Registry "Usuwanie Kluczy Rejestru..."
${LangFileString} Info_Del_Shortcuts "Usuwanie Skrótów..."
${LangFileString} Info_Rest_Assoc "Przywracanie skojarzeń plików..."
${LangFileString} Info_RollBack "Cofanie zmian..."
${LangFileString} Info_Files_Extract "Wypakowywanie plików..."
