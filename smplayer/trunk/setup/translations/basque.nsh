;Language: Basque (1069)
;Basque language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Basque" "Euskera"

; Startup
${LangFileString} SMPLAYER_INSTALLER_IS_RUNNING "Ezartzailea jadanik lanean dago."
${LangFileString} SMPLAYER_INSTALLER_NO_ADMIN "Administratzaile bezala saioa hasita egon behar duzu programa hau ezartzerakoan."

; Components Page
${LangFileString} SMPLAYER_SECSMPLAYER_TITLE "SMPlayer (beharrezkoa)"
${LangFileString} SMPLAYER_SECSMPLAYER_DESC "SMPlayer, elkarbanatutako liburutegiak, eta agiritza."

${LangFileString} SMPLAYER_SHORTCUTGROUP_TITLE "Lasterbideak"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_TITLE "Mahigaina"
${LangFileString} SMPLAYER_SECDESKTOPSHORTCUT_DESC "Sortu SMPlayer lasterbide bat mahigainean."
${LangFileString} SMPLAYER_SECSTARTMENU_TITLE "Hasiera Menua"
${LangFileString} SMPLAYER_SECSTARTMENU_DESC "Sortu SMPlayer sarrera bat Hasiera Menuan."

${LangFileString} SMPLAYER_MPLAYERGROUP_TITLE "MPlayer Osagaiak"
${LangFileString} SMPLAYER_SECMPLAYER_TITLE "MPlayer (beharrezkoa)"
!ifdef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; beharrezkoa irakurketarako."
!else ifndef WITH_MPLAYER
${LangFileString} SMPLAYER_SECMPLAYER_DESC "MPlayer; beharrezkoa irakurketarako. (Internet Elkarketa beharrezkoa da ezarpenerako)"
!endif
${LangFileString} SMPLAYER_SECCODECS_TITLE "Kodek Binarioak"
${LangFileString} SMPLAYER_SECCODECS_DESC "Aukerazko kodekak MPlayerrentzat. (Internet Elkarketa beharrezkoa da ezarpenerako)"
${LangFileString} SMPLAYER_SECCODECS_DESC_2 "Aukerazko kodekak MPlayerrentzat."

${LangFileString} SMPLAYER_SECMENCODER_DESC "MPlayer laguntzen duen programa bat erabili daiteke kodeatzeko edo eraldatzeko sostengatutako audio edo bideo jarioak"

${LangFileString} SMPLAYER_SECTHEMES_TITLE "Ikono Gaiak"
${LangFileString} SMPLAYER_SECTHEMES_DESC "SMPlayer-entzako ikono gai gehigarriak."

${LangFileString} SMPLAYER_SECTRANSLATIONS_TITLE "Hizkuntzak"
${LangFileString} SMPLAYER_SECTRANSLATIONS_DESC "SMPlayer-entzako Ez Ingelerazko hizkuntza agiriak."

${LangFileString} MPLAYER_CODEC_INFORMATION "Kodek binario paketeek jatorrizkoan ez dauden kodek sostengua gehitzen dute, RealVideo aldaera berrienak eta formato ez arrunt asko bezalakoak.$\nOhartu hauek ez direla beharrezkoak formato arruntenak irakurtzeko, DVD, MPEG-1/2/4, etab."

; Upgrade/Reinstall Page
${LangFileString} REINSTALL_HEADER_TEXT "Jadanik Ezarrita"
${LangFileString} REINSTALL_HEADER_SUBTEXT "Hautatu nola nahi duzun ezartzea SMPlayer."
${LangFileString} REINSTALL_HEADER_SUBTEXT_MAINT "Hautatu mantenimendu aukera egintzarako."

${LangFileString} REINSTALL_CHGSETTINGS "Aldatu ezarpenak (aurreratua)"
${LangFileString} REINSTALL_UNKNOWN_VALUE "Balio ezezaguna PREVIOUS_VERSION_STATE, ezarpena uzten"

${LangFileString} REINSTALL_OLDVER_DESCRIPTION "SMPlayer bertsio zaharrago bat dago ezarrita zure sisteman. Hautatu egitea nahi duzun eragiketa eta klikatu Hurrengoa jarraitzeko."
${LangFileString} REINSTALL_OLDVER_UPGRADE "Eguneratu SMPlayer aurreko ezarpenak erabiliz (gomendatua)"

${LangFileString} REINSTALL_NEWVER_DESCRIPTION "SMPlayer bertsio berriena jadanink ezarrita dago! Ez da gomendagarria bertsio zaharrago batera eguneratzea. Hautatu egitea nahi duzun eragiketa eta klikatu Hurrengoa jarraitzeko."
${LangFileString} REINSTALL_NEWVER_DOWNGRADE "Eguneratu SMPlayer aurreko ezarpenak erabiliz (gomendatua)"

${LangFileString} REINSTALL_SAMEVER_DESCRIPTION "SMPlayer ${SMPLAYER_VERSION} jadanik ezarrita dago. Hautatu egitea nahi duzun eragiketa eta klikatu Hurrengoa jarraitzeko."
${LangFileString} REINSTALL_SAMEVER_ADDREMREINST "Gehitu/Kendu/Birrezarri osagaiak"
${LangFileString} REINSTALL_SAMEVER_UNINSTSMP "Kendu SMPlayer"

; MPlayer Section
${LangFileString} MPLAYER_IS_DOWNLOADING "MPlayer jeisten..."
${LangFileString} MPLAYER_DL_RETRY "MPlayer ez da ongi ezarri. Berriro saiatu?"
${LangFileString} MPLAYER_DL_FAILED "Hutsegitea MPlayer: '$R0'. jeisterakoan"
${LangFileString} MPLAYER_INST_FAILED "Hutsegitea MPlayer ezartzerakoan. MPlayer beharrezkoa da irakurketarako"

; Codecs Section
${LangFileString} CODECS_IS_DOWNLOADING "MPlayer kodekak jeisten..."
${LangFileString} CODECS_DL_RETRY "MPlayer kodekak ez dira ongi ezarri. Berriro saiatu?"
${LangFileString} CODECS_DL_FAILED "Hutsegitea MPlayer kodekak: '$R0'. jeisterakoan."
${LangFileString} CODECS_INST_FAILED "Hutsegitea MPlayer kodekak ezartzerakoan."

; Version information
${LangFileString} VERINFO_IS_DOWNLOADING "Bertsio argibideak jeisten..."
${LangFileString} VERINFO_DL_FAILED "Hutsegitea bertsio argibideak jeisterakoan: '$R0'. Berezko bertsioa erabiltzen."

; Uninstaller
${LangFileString} UNINSTALL_NO_ADMIN "Ezarpen hau administrari eskubidea duen erabiltzaileak bakarrik kendu dezake."
${LangFileString} UNINSTALL_ABORTED "Kentzea erabiltzaileak utzita."
${LangFileString} SMPLAYER_NOT_INSTALLED "Ez da agertzen SMPlayer zuzenbidean ezarrita dagoenik '$INSTDIR'.$\r$\nJarraitu horrela ere (ez da gomendagarria)?"

; Vista & Later Default Programs Registration
${LangFileString} APPLICATION_DESCRIPTION "SMPlayer aurrealde-amaiera oso bat da MPlayer-entzat, ohinarrizko eginkizunetatik: Bideo, DVD, VCD irakurketatik, eginkizun aurreratuenetarainok: MPlayer iragazkiak, edl zerrenda, eta gehiago."

; Misc
${LangFileString} INFO_DEL_FILES "Agiriak Ezabatzen..."
${LangFileString} INFO_DEL_REGISTRY "Erresgistro Giltzak Ezabatzen..."
${LangFileString} INFO_DEL_SHORTCUTS "Lasterbideak Ezabatzen..."
${LangFileString} INFO_REST_ASSOC "Agiri elkarketak birrezartzen..."
${LangFileString} INFO_FILE_EXTRACT "Agiriak ateratzen..."
