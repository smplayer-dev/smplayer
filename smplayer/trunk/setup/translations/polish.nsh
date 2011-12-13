;Language: Polish (1045)
;Polish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Polish" "Polski"

; Startup
${LangFileString} Installer_Is_Running "Instalator już jest uruchomiony."
${LangFileString} Installer_No_Admin "Do instalacji tego programu wymagane są uprawnienia administratora."

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
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; wymagany do odtwarzania filmów."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; wymagany do odtwarzania filmów. (Wymagane połączenie z internetem)"
!endif

${LangFileString} Section_MPlayerCodecs "Kodeki"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Opcjonalne kodeki do MPlayera."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Opcjonalne kodeki do MPlayera. (Wymagane połączenie z internetem)"
!endif

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Zesatwy Ikon"
${LangFileString} Section_IconThemes_Desc "Dodatkowe zestawy ikon do SMPlayera."

${LangFileString} Section_Translations "Tłumaczenia"
${LangFileString} Section_Translations_Desc "Tłumaczenia na inne języki."

${LangFileString} MPlayer_Codec_Msg "Paczka z kodekami dodaje wsparcie dla kodeków, które nie są jeszcze natywnie wspierane, np. nowsze warianty RealVideo, oraz wiele mniej znanych formatów.$\nProszę mieć na uwadze, że paczka nie jest wymagana do odtworzenia popularnych formatów, takich jak DVD, MPEG-1/2/4, itp."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Select Install Type"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3 "Click Start when ready to proceed."

${LangFileString} Type_Reinstall "reinstall"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

${LangFileString} StartBtn "Start"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "Pobieram MPlayera..."
${LangFileString} MPlayer_DL_Retry "Instalacja MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} MPlayer_DL_Failed "Nie udało się pobrać MPlayera: '$R0'."
${LangFileString} MPlayer_Inst_Failed "Nie udało się zainstalować MPlayera. Bez MPlayera odtwarzanie filmów jest nie możliwe."

; Codecs Section
${LangFileString} Codecs_DL_Msg "Pobieram kodeki do MPlayera..."
${LangFileString} Codecs_DL_Retry "Instalacja Kodeków MPlayera nie powiodła się. Ponowić próbę?"
${LangFileString} Codecs_DL_Failed "Nie udało się pobrać Kodeków MPlayera: '$R0'."
${LangFileString} Codecs_Inst_Failed "Nie udało się zainstalować Kodeków MPlayera."

; Version information
${LangFileString} VerInfo_DL_Msg "Pobieram informacje o wersji..."
${LangFileString} VerInfo_DL_Failed "Nie udało się pobrac informacji o wersji: '$R0'. Użyta zostanie wersja domyślna."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Aplikacja może być usunięta tylko przez osobe z uprawnieniami Administratora."
${LangFileString} Uninstaller_Aborted "Usuwanie anulowane przez użytkownika."
${LangFileString} Uninstaller_NotInstalled "Nie wygląda na to, aby SMPlayer został zainstalowany w katalogu '$INSTDIR'.$\r$\nKontynuować mimo to (nie zalecane)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer jest graficzną nakładką programu MPlayer, z prostymi funkcjami, takimi jak odtwarzanie wideo, DVD i VCD oraz z bardziej zaawansowanymi funkcjami np: obsługa filtrów MPlayera i wiele innych."

; Misc
${LangFileString} Info_Del_Files "Deleting Files..."
${LangFileString} Info_Del_Registry "Deleting Registry Keys..."
${LangFileString} Info_Del_Shortcuts "Deleting Shortcuts..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_Files_Extract "Extracting files..."
