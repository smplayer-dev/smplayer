;Language: Basque (1069)
;Basque language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Basque" "Euskara"

; Startup
${LangFileString} Installer_Is_Running "Ezartzailea jadanik lanean dago."
${LangFileString} Installer_No_Admin "Administratzaile bezala saioa hasita egon behar duzu programa hau ezartzerakoan."
${LangFileString} SMPlayer_Is_Running "SMPlayerren eskabide bat lanean ari da. Mesedez irten SMPlayerretik eta saiatu berriro."

${LangFileString} OS_Not_Supported "Sistema eragile sostengu gabea.$\nSMPlayer ${SMPLAYER_VERSION} gutxienez Windows XP behar du eta ezin du zuzen lan egin zure sisteman.$\nEgitan nahi duzu ezarpenarekin jarraitzea?"
${LangFileString} Win64_Required "64-biteko Windows sistema eragile bat behar da software hau ezartzeko."
${LangFileString} Existing_32bitInst "32-biteko SMPlayer ezarpen bat dago. Lehenik 32-biteko SMPlayer kendu behar duzu."
${LangFileString} Existing_64bitInst "64-biteko SMPlayer ezarpen bat dago. Lehenik 64-biteko SMPlayer kendu behar duzu."

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
${LangFileString} Section_MPlayer_Desc "MPlayer; beharrezkoa irakurketarako."

${LangFileString} Section_MPlayerCodecs "Kodek Binarioak"
!ifdef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Aukerazko kodekak MPlayerrentzat."
!else ifndef WITH_CODECS
${LangFileString} Section_MPlayerCodecs_Desc "Aukerazko kodekak MPlayerrentzat. (Internet Elkarketa beharrezkoa da ezarpenerako)"
!endif

${LangFileString} Section_MEncoder_Desc "MPlayer laguntzen duen programa bat erabili daiteke kodeatzeko edo eraldatzeko sostengatutako audio edo bideo jarioak."

${LangFileString} Section_IconThemes "Ikur Gaiak"
${LangFileString} Section_IconThemes_Desc "SMPlayer-entzako ikur gai gehigarriak."

${LangFileString} Section_Translations "Hizkuntzak"
${LangFileString} Section_Translations_Desc "SMPlayer-entzako Ez Ingelerazko hizkuntza agiriak."

${LangFileString} MPlayer_Codec_Msg "Kodek binario paketeek jatorrizkoan ez dauden kodek sostengua gehitzen dute, RealVideo aldaera berrienak eta formato ez arrunt asko bezalakoak.$\nOhartu hauek ez direla beharrezkoak heuskarri arruntenak irakurtzeko, DVD, MPEG-1/2/4, etab."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Hautatu Ezarpen Mota"
${LangFileString} Reinstall_Header_SubText "Hautatu Gainidatzi edo Kendu modua."

${LangFileString} Reinstall_Msg1 "Jadanik baduzu SMPlayerren ezarpen bat agiritegi honetan:"
${LangFileString} Reinstall_Msg2 "Mesedez hautatu nola jarraitu:"
${LangFileString} Reinstall_Overwrite "Gainidatzi ($Inst_Type) dagoen ezarpena"
${LangFileString} Reinstall_Uninstall "Kendu (kendu) dagoen ezarpena"
${LangFileString} Reinstall_Msg3_1 "Klikatu Hasi jarraitzeko gertu zaudenean."
${LangFileString} Reinstall_Msg3_2 "Klikatu Hurrengoa jarraitzeko gertu zaudenean."
${LangFileString} Reinstall_Msg3_3 "Klikatu Kendu jarraitzeko gertu zaudenean."
${LangFileString} Reinstall_Msg4 "Aldatu Ezarpenaren Hobespenak"

${LangFileString} Type_Reinstall "berrezarri"
${LangFileString} Type_Downgrade "aurrekoratu"
${LangFileString} Type_Upgrade "eguneratu"

${LangFileString} StartBtn "Hasi"

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
${LangFileString} Uninstaller_No_Admin "Ezarpen hau administratzaile eskubidea duen erabiltzaileak bakarrik kendu dezake."
${LangFileString} Uninstaller_Aborted "Kentzea erabiltzaileak utzita."
${LangFileString} Uninstaller_NotInstalled "Ez da agertzen SMPlayer zuzenbidean ezarrita dagoenik '$INSTDIR'.$\r$\nJarraitu horrela ere (ez da gomendagarria)?"
${LangFileString} Uninstaller_64bitOnly "Ezarpen hau 64-biteko Windowsetik bakarrik kendu daiteke."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer aurrealde-amaiera oso bat da MPlayer-entzat, ohinarrizko eginkizunetatik: Bideo, DVD, VCD irakurketatik, eginkizun aurreratuenetarainok: MPlayer iragazkiak, edl zerrenda, eta gehiago."

; Misc
${LangFileString} Info_Del_Files "Agiriak Ezabatzen..."
${LangFileString} Info_Del_Registry "Erresgistro Giltzak Ezabatzen..."
${LangFileString} Info_Del_Shortcuts "Lasterbideak Ezabatzen..."
${LangFileString} Info_Rest_Assoc "Agiri elkarketak birrezartzen..."
${LangFileString} Info_RollBack "Aldaketak desegiten..."
${LangFileString} Info_Files_Extract "Agiriak ateratzen..."
