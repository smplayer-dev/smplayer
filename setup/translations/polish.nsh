;Language: Polish (1045)
;Polish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Polish" "Polski"

; Startup
${LangFileString} Installer_Is_Running "Instalator już jest uruchomiony."
${LangFileString} Installer_No_Admin "Do instalacji tego programu wymagane są uprawnienia administratora."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} OS_Not_Supported "Unsupported operating system.$\nSMPlayer ${SMPLAYER_VERSION} requires at least Windows XP and may not work correctly on your system.$\nDo you really want to continue with the installation?"
${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Skróty"
${LangFileString} MPlayerGroupTitle "Komponenty MPlayera"

${LangFileString} Section_SMPlayer "SMPlayer (wymagane)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, biblioteki współdzielone, dokumentacja."

${LangFileString} Section_DesktopShortcut "Pulpit"
${LangFileString} Section_DesktopShortcut_Desc "Tworzy skrót do programu na pulpicie."

${LangFileString} Section_StartMenu "Menu Start"
${LangFileString} Section_StartMenu_Desc "Tworzy skrót do programu w Menu Start."

${LangFileString} Section_MPlayer "MPlayer (wymagane)"
${LangFileString} Section_MPlayer_Desc "MPlayer; wymagany do odtwarzania filmów."

${LangFileString} Section_MPlayerCodecs "Kodeki"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Opcjonalne kodeki do MPlayera."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Opcjonalne kodeki do MPlayera. (Wymagane połączenie z internetem)"
!endif

${LangFileString} Section_MEncoder_Desc "Jest to prosty koder (kompresor) filmów, zaprojektowany do kodowania filmów otwieralnych MPlayerem do innych obsługiwanych formatów"
${LangFileString} Section_IconThemes "Zestawy Ikon"
${LangFileString} Section_IconThemes_Desc "Dodatkowe zestawy ikon do SMPlayera."

${LangFileString} Section_Translations "Tłumaczenia"
${LangFileString} Section_Translations_Desc "Tłumaczenia na inne języki."

${LangFileString} MPlayer_Codec_Msg "Paczka z kodekami dodaje wsparcie dla kodeków, które nie są jeszcze natywnie wspierane, np. nowsze warianty RealVideo, oraz wiele mniej znanych formatów.$\nProszę mieć na uwadze, że paczka nie jest wymagana do odtworzenia popularnych formatów, takich jak DVD, MPEG-1/2/4, itp."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Wybierz typ instalacji"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "Posiadasz już zainstalowaną wersję SMPlayera w folderze:"
${LangFileString} Reinstall_Msg2 "Wybierz co mam dalej zrobić:"
${LangFileString} Reinstall_Overwrite "Nadpisz ($Inst_Type) istniejącą instalację"
${LangFileString} Reinstall_Uninstall "Odinstaluj (usuń) istniejącą instalację"
${LangFileString} Reinstall_Msg3_1 "Kliknij Start, gdy będziesz gotów."
${LangFileString} Reinstall_Msg3_2 "Kliknij Dalej, gdy będziesz gotów."
${LangFileString} Reinstall_Msg3_3 "Kliknij Odinstaluj, gdy będziesz gotów."
${LangFileString} Reinstall_Msg4 "Zmiana Ustawień Instalacji"

${LangFileString} Type_Reinstall "zainstaluj ponownie"
${LangFileString} Type_Downgrade "zrób downgrade"
${LangFileString} Type_Upgrade "aktualizuj"

${LangFileString} StartBtn "Start"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Pobieram kodeki do MPlayera..."
${LangFileString} Codecs_DL_Retry "Instalacja Kodeków MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} Codecs_DL_Failed "Nie udało się pobrać Kodeków MPlayera: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nie udało się zainstalować Kodeków MPlayera."

; Version information
${LangFileString} VerInfo_DL_Msg "Pobieram informacje o wersji..."
${LangFileString} VerInfo_DL_Failed "Nie udało się pobrać informacji o wersji: '$R0'. Użyta zostanie wersja domyślna."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Aplikacja może być usunięta tylko przez osobe z uprawnieniami Administratora."
${LangFileString} Uninstaller_Aborted "Usuwanie anulowane przez użytkownika."
${LangFileString} Uninstaller_NotInstalled "Wygląda na to, że SMPlayer nie został zainstalowany w katalogu '$INSTDIR'.$\r$\nKontynuować mimo to (nie zalecane)?"
${LangFileString} Uninstaller_64bitOnly "This installation can only be uninstalled on 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer jest graficzną nakładką programu MPlayer, z prostymi funkcjami, takimi jak odtwarzanie wideo, DVD i VCD oraz z bardziej zaawansowanymi funkcjami np: obsługa filtrów MPlayera i wiele innych."

; Misc
${LangFileString} Info_Del_Files "Usuwanie Plików..."
${LangFileString} Info_Del_Registry "Usuwanie Kluczy Rejestru..."
${LangFileString} Info_Del_Shortcuts "Usuwanie Skrótów..."
${LangFileString} Info_Rest_Assoc "Przywracanie skojarzeń plików..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Wypakowywanie plików..."
