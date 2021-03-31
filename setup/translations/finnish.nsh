;Language: Finnish (1035)
;Finnish language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Finnish" "Suomi"
!else
!insertmacro LANGFILE "Finnish" = "Suomi" =
!endif

; Startup
${LangFileString} Installer_Is_Running "Asennusohjelma on jo käynnissä."
${LangFileString} Installer_No_Admin "Sinun on oltava kirjautuneena järjestelmänvalvojana kun asennat tätä ohjelmaa."
${LangFileString} SMPlayer_Is_Running "SMPlayer on jo käynnissä. Sulje SMPlayer ja yritä uudelleen."

${LangFileString} OS_Not_Supported "Ei tuettu käyttöjärjestelmä.$\nSMPlayer ${SMPLAYER_VERSION} vaatii vähintään Windows XP:n, eikä välttämättä toimi oikein järjestelmässäsi.$\nHaluatko todella jatkaa asennusta?"
${LangFileString} OS_Not_Supported_VistaRequired "Ei tuettu käyttöjärjestelmä.$\nSMPlayer ${SMPLAYER_VERSION} vaatii vähintään Windows Vistan, eikä välttämättä toimi oikein järjestelmässäsi.$\nHaluatko todella jatkaa asennusta?"
${LangFileString} Win64_Required "Tämän ohjelmiston asentaminen edellyttää 64-bittistä käyttöjärjestelmää."
${LangFileString} Existing_32bitInst "SMPlayer 32-bittinen asennus on jo olemassa. Sinun on ensin poistettava 32-bittinen SMPlayer."
${LangFileString} Existing_64bitInst "SMPlayer 64-bittinen asennus on jo olemassa. Sinun on ensin poistettava 64-bittinen SMPlayer."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Asennus"
${LangFileString} WelcomePage_Text "Asennusohjelma opastaa sinua $(^NameDA).$\r$\n$\r$\nOn suositeltavaa sulkea kaikki SMPlayer-osat ennen asennuksen aloittamista. Tämä mahdollistaa ohjelmatiedostojen päivittämisen ilman, että tietokonetta tarvitsee käynnistää uudelleen.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Pikanäppäimet"
${LangFileString} MPlayerGroupTitle "MPlayer-osat"
${LangFileString} MPlayerMPVGroupTitle "Multimedia moottori"

${LangFileString} Section_SMPlayer "SMPlayer (vaaditaan)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, jaetut kirjastot ja dokumentaatio."

${LangFileString} Section_DesktopShortcut "Työpöytä"
${LangFileString} Section_DesktopShortcut_Desc "Luo pikakuvakkeen SMPlayer työpöydälle."

${LangFileString} Section_StartMenu "Käynnistys-valikko"
${LangFileString} Section_StartMenu_Desc "Luo SMPlayer käynnistä-valikkoon."

${LangFileString} Section_MPlayer "MPlayer (vaaditaan)"
${LangFileString} Section_MPlayer_Desc "MPlayer; tarvitaan toistossa."

${LangFileString} Section_MPlayerCodecs "Binaariset koodekit"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binäärisiä koodekkeja ei tueta tässä versiossa."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Valinnaiset koodekit MPlayerille (Internet-yhteys tarvitaan asennukseen)."
!endif

${LangFileString} Section_MPV_Desc "Monipuolinen työkalu ohjelmille MPlayer && MPlayer2"

${LangFileString} Section_YTDL "Youtube support"
${LangFileString} Section_YTDL_Desc "Provides the possibility to play Youtube videos."

${LangFileString} Section_MEncoder_Desc "MPlayer-kumppaniohjelma, jota voidaan käyttää tuettujen ääni- tai videosuoratoiston purkamiseen tai muuntamiseen."

${LangFileString} Section_IconThemes "Kuvaketeemat"
${LangFileString} Section_IconThemes_Desc "Lisää kuvakeiden teemoja SMPlayerille."

${LangFileString} Section_Translations "Kielet"
${LangFileString} Section_Translations_Desc "Muut kuin englanninkieliset tiedostot SMPlayerille."

${LangFileString} Section_ResetSettings_Desc "Poistaa SMPlayer-asetusten jäämät aiemmista asennuksista."

${LangFileString} MPlayer_Codec_Msg "Binääriset koodekkit lisäävät tukea koodekeille, joita ei ole vielä toteutettu alkuperäisessä muodossa, kuten uudemmat RealVideo-variantit ja muita epätavallisia muotoja.$\nHuomaa, että niitä ei tarvitse toistaa yleisissä muodoissa, kuten DVD, MPEG-1/2/4, jne."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Valitse asennustyyppi"
${LangFileString} Reinstall_Header_SubText "Valitse ylikirjoitus tai asennuksen poisto."

${LangFileString} Reinstall_Msg1 "Sinulla on jo asennettu SMPlayer seuraavassa kansiossa:"
${LangFileString} Reinstall_Msg2 "Valitse miten edetä:"
${LangFileString} Reinstall_Overwrite "Korvaa ($Inst_Type) nykyinen asennus"
${LangFileString} Reinstall_Uninstall "Poista olemassa oleva asennus (poista)"
${LangFileString} Reinstall_Msg3_1 "Napsauta Käynnistä, kun olet valmis jatkamaan."
${LangFileString} Reinstall_Msg3_2 "Napsauta Seuraava, kun olet valmis jatkamaan."
${LangFileString} Reinstall_Msg3_3 "Napsauta Poista asennus, kun olet valmis jatkamaan."
${LangFileString} Reinstall_Msg4 "Muuta asennuksen asetuksia"
${LangFileString} Reinstall_Msg5 "Palauta minun SMPlayer-määritykset"

${LangFileString} Remove_Settings_Confirmation "Haluatko varmasti nollata SMPlayer-asetukset? Tätä toimintoa ei voi peruuttaa."

${LangFileString} Type_Reinstall "asenna uudelleen"
${LangFileString} Type_Downgrade "paluu aiempaan"
${LangFileString} Type_Upgrade "päivitys"

${LangFileString} StartBtn "Käynnistä"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Ladataan MPlayer-koodekkeja..."
${LangFileString} Codecs_DL_Retry "MPlayer-koodekkeja ei asennettu. Yritä uudelleen?"
${LangFileString} Codecs_DL_Failed "MPlayer-koodekkien lataus epäonnistui: '$R0'."
${LangFileString} Codecs_Inst_Failed "MPlayer-koodekkien asennus epäonnistui."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Tämän asennuksen voi poistaa vain käyttäjä, jolla on järjestelmänvalvojan oikeudet."
${LangFileString} Uninstaller_Aborted "Poisto keskeytettiin käyttäjän toimesta."
${LangFileString} Uninstaller_NotInstalled "Vaikuttaa siltä, että SMPlayer ei ole asennettu hakemistoon '$INSTDIR'.$\r$\nJatkako tästä huolimatta (ei suositella)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer-asennusta ei löydy."
${LangFileString} Uninstaller_64bitOnly "Tämän asennus voidaan poistaa vain 64-bittisessä Windowsissa."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer on täydellinen käyttöliittymä MPlayerille, perusominaisuudet, kuten toistaa videoita, DVD- ja VCD-levyjen kehittyneempiä ominaisuuksia, kuten tuki MPlayer suodattimille, edl-luettelot ja paljon enemmän."

; Misc
${LangFileString} Info_Codecs_Backup "Varmuuskopioidaan koodekit aiemmasta asennuksesta..."
${LangFileString} Info_Codecs_Restore "Palautetaan koodekit aiemmasta asennuksesta..."
${LangFileString} Info_Del_Files "Poistetaan tiedostoja..."
${LangFileString} Info_Del_Registry "Poistetaan rekisteriavaimia..."
${LangFileString} Info_Del_Shortcuts "Poistetaan pikakuvakkeita..."
${LangFileString} Info_Rest_Assoc "Palautetaan tiedostokytkennät..."
${LangFileString} Info_RollBack "Peruutetaan muutoksia..."
${LangFileString} Info_Files_Extract "Puretaan tiedostoja..."
${LangFileString} Info_SMTube_Backup "Varmuuskopioidaan SMTube..."
${LangFileString} Info_SMTube_Restore "Palautetaan SMTube aiemmasta asennuksesta..."
${LangFileString} Info_MPV_Backup "Varmuuskopioidaan mpv..."
${LangFileString} Info_MPV_Restore "Palautetaan mpv edellisestä asennuksesta..."

; MPV
${LangFileString} MPV_DL_Msg "Ladataan mpv..."
${LangFileString} MPV_DL_Retry "Yritä uudelleen? mpv-asennus ei onnistu."
${LangFileString} MPV_DL_Failed "Lataaminen epäonnistui, mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Mpv:n asentaminen epäonnistui."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "Youtube-dl:n asentaminen ei onnistunut. Yritä uudelleen?"
${LangFileString} YTDL_DL_Failed "YouTube-dl:n lataus epäonnistui: '$R0'."
${LangFileString} YTDL_Update_Check "Tarkistetaan youtube-dl päivityksiä..."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Siivotaan fontconfig-välimuistia..."
${LangFileString} Info_Cleaning_SMPlayer "Siivotaan SMPlayer-asetuksia..."
