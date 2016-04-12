;Language: Greek (1032)
;Greek language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Greek" "Ελληνικά"
!else
!insertmacro LANGFILE "Greek" = "Ελληνικά" "Ellinika"
!endif

; Startup
${LangFileString} Installer_Is_Running "Το πρόγραμμα εγκατάστασης εκτελείται ήδη."
${LangFileString} Installer_No_Admin "Θα πρέπει να συνδεθείτε ως διαχειριστής κατά την εγκατάσταση του προγράμματος."
${LangFileString} SMPlayer_Is_Running "Μια υπόσταση του SMPlayer εκτελείται. Παρακαλώ τερματίστε την και προσπαθήστε εκ νέου."

${LangFileString} OS_Not_Supported "Μη υποστηριζόμενο λειτουργικό σύστημα.$\nSMPlayer ${SMPLAYER_VERSION} απαιτεί τουλάχιστον Windows XP και ίσως να μην λειτουργεί σωστά στο σύστημά σας.$\nΘέλετε πραγματικά να συνεχίσετε την εγκατάσταση;"
${LangFileString} Win64_Required "Απαιτείται ένα λειτουργικό σύστημα Windows 64 bit ούτως ώστε να εγκαταστήσετε αυτό το λογισμικό."
${LangFileString} Existing_32bitInst "Υπάρχει ήδη μια εγκατάσταση 32-bit του SMPlayer. Πρέπει πρώτα να την απεγκαταστήσετε. "
${LangFileString} Existing_64bitInst "Υπάρχει ήδη μια εγκατάσταση 64-bit του SMPlayer. Πρέπει πρώτα να την απεγκαταστήσετε."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Εγκατάσταση"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMPlayer before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Συντομεύσεις"
${LangFileString} MPlayerGroupTitle "Μονάδες MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Μηχανή πολυμέσων"

${LangFileString} Section_SMPlayer "SMPlayer (απαιτείται)"
${LangFileString} Section_SMPlayer_Desc "Ο SMPlayer, κοινές βιβλιοθήκες, και η τεκμηρίωση."

${LangFileString} Section_DesktopShortcut "Επιφάνεια Εργασίας"
${LangFileString} Section_DesktopShortcut_Desc "Δημιουργεί μια συντόμευση για τον SMPlayer στην επιφάνεια εργασίας."

${LangFileString} Section_StartMenu "Μενού «Έναρξη»"
${LangFileString} Section_StartMenu_Desc "Δημιουργεί μια καταχώρηση στο Μενού «Έναρξη» για τον SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (απαιτείται)"
${LangFileString} Section_MPlayer_Desc "MPlayer· απαιτείται για αναπαραγωγή."

${LangFileString} Section_MPlayerCodecs "Binary Codecs"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Binary codecs are not supported in this version."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Optional codecs for MPlayer. (Internet Connection required for installation)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "A companion program to MPlayer that can be used to encode or transform supported audio or video streams."

${LangFileString} Section_IconThemes "Θέματα Εικόνων"
${LangFileString} Section_IconThemes_Desc "Επιπλέον θέματα εικόνων για τον SMPlayer."

${LangFileString} Section_Translations "Γλώσσες"
${LangFileString} Section_Translations_Desc "Αρχεία γλωσσών του SMPlayer για γλώσσες εκτός από τα Αγγλικά."

${LangFileString} Section_ResetSettings_Desc "Διαγράφει προτιμήσεις του SMPlayer που έχουν απομείνει από προηγούμενες εγκαταστάσεις."

${LangFileString} MPlayer_Codec_Msg "The binary codec packages add support for codecs that are not yet implemented natively, like newer RealVideo variants and a lot of uncommon formats.$\nNote that they are not necessary to play most common formats like DVDs, MPEG-1/2/4, etc."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Επιλέξτε Τύπο Εγκατάστασης"
${LangFileString} Reinstall_Header_SubText "Select Overwrite or Uninstall mode."

${LangFileString} Reinstall_Msg1 "You have an existing installation of SMPlayer in the following folder:"
${LangFileString} Reinstall_Msg2 "Please select how to proceed:"
${LangFileString} Reinstall_Overwrite "Overwrite ($Inst_Type) the existing installation"
${LangFileString} Reinstall_Uninstall "Απεγκατάσταση (αφαίρεση) της υπάρχουσας εγκατάστασης"
${LangFileString} Reinstall_Msg3_1 "Click Start when ready to proceed."
${LangFileString} Reinstall_Msg3_2 "Click Next when ready to proceed."
${LangFileString} Reinstall_Msg3_3 "Click Uninstall when ready to proceed."
${LangFileString} Reinstall_Msg4 "Change Installation Settings"
${LangFileString} Reinstall_Msg5 "Reset my SMPlayer configuration"

${LangFileString} Remove_Settings_Confirmation "Are you sure you want to reset your SMPlayer settings? This action cannot be reversed."

${LangFileString} Type_Reinstall "επανεγκατάσταση"
${LangFileString} Type_Downgrade "υποβάθμιση"
${LangFileString} Type_Upgrade "αναβάθμιση"

${LangFileString} StartBtn "Έναρξη"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Downloading MPlayer Codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer codecs were not successfully installed. Retry?"
${LangFileString} Codecs_DL_Failed "Failed to download MPlayer codecs: '$R0'."
${LangFileString} Codecs_Inst_Failed "Failed to install MPlayer codecs."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "This installation can only be uninstalled by a user with administrator privileges."
${LangFileString} Uninstaller_Aborted "Uninstall aborted by user."
${LangFileString} Uninstaller_NotInstalled "It does not appear that SMPlayer is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)?"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer installation not found."
${LangFileString} Uninstaller_64bitOnly "This installation can only be uninstalled on 64-bit Windows."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."

; Misc
${LangFileString} Info_Codecs_Backup "Backing up codecs from previous installation..."
${LangFileString} Info_Codecs_Restore "Restoring codecs from previous installation..."
${LangFileString} Info_Del_Files "Διαγραφή αρχείων..."
${LangFileString} Info_Del_Registry "Διαγραφή των κλειδιών του μητρώου..."
${LangFileString} Info_Del_Shortcuts "Διαγραφή συντομεύσεων..."
${LangFileString} Info_Rest_Assoc "Restoring file associations..."
${LangFileString} Info_RollBack "Rolling back changes..."
${LangFileString} Info_Files_Extract "Αποσυμπίεση αρχείων..."
${LangFileString} Info_SMTube_Backup "Backing up SMTube..."
${LangFileString} Info_SMTube_Restore "Restoring SMTube from previous installation..."
${LangFileString} Info_MPV_Backup "Backing up mpv..."
${LangFileString} Info_MPV_Restore "Restoring mpv from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Downloading mpv..."
${LangFileString} MPV_DL_Retry "mpv was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download mpv: '$R0'."
${LangFileString} MPV_Inst_Failed "Failed to install mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "youtube-dl was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Checking for youtube-dl updates..."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Cleaning fontconfig cache..."
${LangFileString} Info_Cleaning_SMPlayer "Cleaning SMPlayer settings..."
