;Language: Basque (1069)
;Basque language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Basque" "Euskera"

; Startup
${LangFileString} Installer_Is_Running "Ezartzailea jadanik lanean dago."
${LangFileString} Installer_No_Admin "Administratzaile bezala saioa hasita egon behar duzu programa hau ezartzerakoan."
${LangFileString} SMPlayer_Is_Running "An instance of SMPlayer is running. Please exit SMPlayer and try again."

${LangFileString} Win64_Required "A 64-bit Windows operating system is required to install this software."
${LangFileString} Existing_32bitInst "An existing 32-bit installation of SMPlayer exists. You must uninstall 32-bit SMPlayer first."
${LangFileString} Existing_64bitInst "An existing 64-bit installation of SMPlayer exists. You must uninstall 64-bit SMPlayer first."

; Components Page
${LangFileString} ShortcutGroupTitle "Lasterbideak"
${LangFileString} MPlayerGroupTitle "MPlayer Osagaiak"

${LangFileString} Section_SMPlayer "SMPlayer (beharrezkoa)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, elkarbanatutako liburutegiak, eta agiritza."

${LangFileString} Section_DesktopShortcut "Mahigaina"
${LangFileString} Section_DesktopShortcut_Desc "Sortu SMPlayer lasterbide bat mahigainean."

${LangFileString} Section_StartMenu "Hasiera Menua"
${LangFileString} Section_StartMenu_Desc "Sortu SMPlayer sarrera bat Hasiera Menuan."

${LangFileString} Section_MPlayer "MPlayer (beharrezkoa)"
!ifdef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; beharrezkoa irakurketarako."
!else ifndef WITH_MPLAYER
${LangFileString} Section_MPlayer_Desc "MPlayer; beharrezkoa irakurketarako. (Internet Elkarketa beharrezkoa da ezarpenerako)"
!endif

${LangFileString} Section_MPlayerCodecs "Kodek Binarioak"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Aukerazko kodekak MPlayerrentzat."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Aukerazko kodekak MPlayerrentzat. (Internet Elkarketa beharrezkoa da ezarpenerako)"
!endif

${LangFileString} Section_MEncoder_Desc "MPlayer laguntzen duen programa bat erabili daiteke kodeatzeko edo eraldatzeko sostengatutako audio edo bideo jarioak."

${LangFileString} Section_IconThemes "Ikono Gaiak"
${LangFileString} Section_IconThemes_Desc "SMPlayer-entzako ikono gai gehigarriak."

${LangFileString} Section_Translations "Hizkuntzak"
${LangFileString} Section_Translations_Desc "SMPlayer-entzako Ez Ingelerazko hizkuntza agiriak."

${LangFileString} MPlayer_Codec_Msg "Kodek binario paketeek jatorrizkoan ez dauden kodek sostengua gehitzen dute, RealVideo aldaera berrienak eta formato ez arrunt asko bezalakoak.$\nOhartu hauek ez direla beharrezkoak formato arruntenak irakurtzeko, DVD, MPEG-1/2/4, etab."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Select Install Type"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Uninstall (remove) the existing installation"
${LangFileString} Reinstall_Msg3_1 "Click Start when ready to proceed."
${LangFileString} Reinstall_Msg3_2 "Click Next when ready to proceed."
${LangFileString} Reinstall_Msg3_3 "Click Uninstall when ready to proceed."
${LangFileString} Reinstall_Msg4 "Change Installation Settings"

${LangFileString} Type_Reinstall "reinstall"
${LangFileString} Type_Downgrade "downgrade"
${LangFileString} Type_Upgrade "upgrade"

${LangFileString} StartBtn "Start"

; MPlayer Section
${LangFileString} MPlayer_DL_Msg "MPlayer jeisten..."
${LangFileString} MPlayer_DL_Retry "MPlayer ez da ongi ezarri. Berriro saiatu?"
${LangFileString} MPlayer_DL_Failed "Hutsegitea MPlayer: '$R0'. jeisterakoan"
${LangFileString} MPlayer_Inst_Failed "Hutsegitea MPlayer ezartzerakoan. MPlayer beharrezkoa da irakurketarako"

; Codecs Section
${LangFileString} Codecs_DL_Msg "MPlayer kodekak jeisten..."
${LangFileString} Codecs_DL_Retry "MPlayer kodekak ez dira ongi ezarri. Berriro saiatu?"
${LangFileString} Codecs_DL_Failed "Hutsegitea MPlayer kodekak: '$R0'. jeisterakoan."
${LangFileString} Codecs_Inst_Failed "Hutsegitea MPlayer kodekak ezartzerakoan."

; Version information
${LangFileString} VerInfo_DL_Msg "Bertsio argibideak jeisten..."
${LangFileString} VerInfo_DL_Failed "Hutsegitea bertsio argibideak jeisterakoan: '$R0'. Berezko bertsioa erabiltzen."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Ezarpen hau administrari eskubidea duen erabiltzaileak bakarrik kendu dezake."
${LangFileString} Uninstaller_Aborted "Kentzea erabiltzaileak utzita."
${LangFileString} Uninstaller_NotInstalled "Ez da agertzen SMPlayer zuzenbidean ezarrita dagoenik '$INSTDIR'.$\r$\nJarraitu horrela ere (ez da gomendagarria)?"

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer aurrealde-amaiera oso bat da MPlayer-entzat, ohinarrizko eginkizunetatik: Bideo, DVD, VCD irakurketatik, eginkizun aurreratuenetarainok: MPlayer iragazkiak, edl zerrenda, eta gehiago."

; Misc
${LangFileString} Info_Del_Files "Agiriak Ezabatzen..."
${LangFileString} Info_Del_Registry "Erresgistro Giltzak Ezabatzen..."
${LangFileString} Info_Del_Shortcuts "Lasterbideak Ezabatzen..."
${LangFileString} Info_Rest_Assoc "Agiri elkarketak birrezartzen..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Agiriak ateratzen..."
