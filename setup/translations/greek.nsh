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
${LangFileString} OS_Not_Supported_VistaRequired "Μη υποστηριζόμενο λειτουργικό σύστημα.$\nSMPlayer ${SMPLAYER_VERSION} απαιτεί τουλάχιστον Windows Vista και ίσως να μην λειτουργεί σωστά στο σύστημά σας.$\nΘέλετε πραγματικά να συνεχίσετε την εγκατάσταση;"
${LangFileString} Win64_Required "Απαιτείται ένα λειτουργικό σύστημα Windows 64 bit ούτως ώστε να εγκαταστήσετε αυτό το λογισμικό."
${LangFileString} Existing_32bitInst "Υπάρχει ήδη μια εγκατάσταση 32-bit του SMPlayer. Πρέπει πρώτα να την απεγκαταστήσετε. "
${LangFileString} Existing_64bitInst "Υπάρχει ήδη μια εγκατάσταση 64-bit του SMPlayer. Πρέπει πρώτα να την απεγκαταστήσετε."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Εγκατάσταση"
${LangFileString} WelcomePage_Text "Το πρόγραμμα εγκατάστασης θα σας καθοδηγήσει κατά την εγκατάσταση του $(^NameDA).$\r$\n$\r$\nΣυνιστάται να κλείσετε όλα τα παράθυρα του SMPlayer πριν την εκκίνηση της εγκατάστασης. Αυτό θα σας επιτρέψει να ενημερώσετε τα σχετικά αρχεία του προγράμματος χωρίς την ανάγκη να επανεκκινήσετε τον υπολογιστή.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "Συντομεύσεις"
${LangFileString} MPlayerGroupTitle "Μονάδες MPlayer"
${LangFileString} MPlayerMPVGroupTitle "Μηχανή πολυμέσων"

${LangFileString} Section_SMPlayer "SMPlayer (απαιτείται)"
${LangFileString} Section_SMPlayer_Desc "Ο SMPlayer, κοινές βιβλιοθήκες, και η τεκμηρίωση."

${LangFileString} Section_DesktopShortcut "Επιφάνεια εργασίας"
${LangFileString} Section_DesktopShortcut_Desc "Δημιουργεί μια συντόμευση για τον SMPlayer στην επιφάνεια εργασίας."

${LangFileString} Section_StartMenu "Μενού «Έναρξη»"
${LangFileString} Section_StartMenu_Desc "Δημιουργεί μια καταχώρηση στο Μενού «Έναρξη» για τον SMPlayer."

${LangFileString} Section_MPlayer "MPlayer (απαιτείται)"
${LangFileString} Section_MPlayer_Desc "MPlayer· απαιτείται για αναπαραγωγή."

${LangFileString} Section_MPlayerCodecs "Δυαδικοί κωδικοποιητές"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "Οι δυαδικοί κωδικοποιητές δεν υποστηρίζονται σε αυτήν την έκδοση."
!else
${LangFileString} Section_MPlayerCodecs_Desc "Προαιρετικοί κωδικοποιητές για τον MPlayer. (Απαιτείται σύνδεση στο διαδίκτυο για την εγκατάσταση)"
!endif

${LangFileString} Section_MPV_Desc "Μια πλούσια σε χαρακτηριστικά δικράνωση του MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "Ένα συνοδευτικό πρόγραμμα στο MPlayer το οποίο μπορεί να χρησιμοποιηθεί για την κωδικοποίηση ή την μεταμόρφωση των υποστηριζόμενων ροών ήχου ή βίντεο."

${LangFileString} Section_IconThemes "Θέματα εικονιδίων"
${LangFileString} Section_IconThemes_Desc "Επιπλέον θέματα εικόνων για τον SMPlayer."

${LangFileString} Section_Translations "Γλώσσες"
${LangFileString} Section_Translations_Desc "Αρχεία γλωσσών του SMPlayer για γλώσσες εκτός από τα Αγγλικά."

${LangFileString} Section_ResetSettings_Desc "Διαγράφει προτιμήσεις του SMPlayer που έχουν απομείνει από προηγούμενες εγκαταστάσεις."

${LangFileString} MPlayer_Codec_Msg "Τα δυαδικά πακέτα κωδικοποιητών προσθέτουν υποστήριξη για κωδικοποιητές που δεν έχουν ακόμα ενσωματωθεί εγγενώς, όπως τις παραλλαγές του νεώτερου RealVideo και άλλων ασυνήθιστων μορφών.$\nΣημειώστε ότι δεν είναι απαραίτητοι για την αναπαραγωγή των περισσότερων τυπικών μορφών όπως DVD, MPEG-1/2/4, κλπ."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "Επιλέξτε τον τύπο της εγκατάστασης"
${LangFileString} Reinstall_Header_SubText "Επιλέξτε την λειτουργία αντικατάστασης ή απεγκατάστασης."

${LangFileString} Reinstall_Msg1 "Υπάρχει ήδη μια εγκατάσταση του SMPlayer στον κατωτέρω φάκελο:"
${LangFileString} Reinstall_Msg2 "Παρακαλώ επιλέξτε πώς να προχωρήσετε:"
${LangFileString} Reinstall_Overwrite "Αντικατάσταση ($Inst_Type) της υπαρχούσης εγκατάστασης"
${LangFileString} Reinstall_Uninstall "Απεγκατάσταση (αφαίρεση) της υπάρχουσας εγκατάστασης"
${LangFileString} Reinstall_Msg3_1 "Κάντε κλικ στο Έναρξη όταν είστε έτοιμος-η να συνεχίσετε."
${LangFileString} Reinstall_Msg3_2 "Κάντε κλικ στο Επόμενο όταν είστε έτοιμος-η να συνεχίσετε."
${LangFileString} Reinstall_Msg3_3 "Κάντε κλικ στο Απεγκατάσταση όταν είστε έτοιμος-η να συνεχίσετε."
${LangFileString} Reinstall_Msg4 "Αλλαγή των ρυθμίσεων της εγκατάστασης"
${LangFileString} Reinstall_Msg5 "Επαναφορά της διαμόρφωσης του SMPlayer"

${LangFileString} Remove_Settings_Confirmation "Είστε σίγουρος-η ότι θέλετε να επαναφέρετε τις ρυθμίσεις του SMPlayer; Αυτή η ενέργεια είναι μη αναστρέψιμη."

${LangFileString} Type_Reinstall "επανεγκατάσταση"
${LangFileString} Type_Downgrade "υποβάθμιση"
${LangFileString} Type_Upgrade "αναβάθμιση"

${LangFileString} StartBtn "Έναρξη"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Υποβάθμιση των κωδικοποιητών του MPlayer..."
${LangFileString} Codecs_DL_Retry "Οι κωδικοποιητές του MPlayer δεν εγκαταστάθηκαν επιτυχώς. Προσπάθεια ξανά;"
${LangFileString} Codecs_DL_Failed "Αποτυχία λήψης των κωδικοποιητών του MPlayer: '$R0'."
${LangFileString} Codecs_Inst_Failed "Αποτυχία εγκατάστασης των κωδικοποιητών του MPlayer."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "Αυτή η εγκατάσταση μπορεί να απεγκατασταθεί μόνο με προνόμια διαχειριστή."
${LangFileString} Uninstaller_Aborted "Η απεγκατάσταση εγκαταλείφθηκε από τον χρήστη."
${LangFileString} Uninstaller_NotInstalled "Δεν φαίνεται το SMPlayer να έχει εγκατασταθεί στον κατάλογο '$INSTDIR'.$\r$\nΝα συνεχίσω ούτως ή άλλως (δεν συνιστάται);"
${LangFileString} Uninstaller_InvalidDirectory "Δεν βρέθηκε η εγκατάσταση του SMPlayer."
${LangFileString} Uninstaller_64bitOnly "Αυτή η εγκατάσταση μπορεί να απεγκατασταθεί μόνο σε Windows 64-bit."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "Το SMPlayer είναι ένα πλήρες περιβάλλον του συστήματος υποστήριξης του MPlayer, από τα βασικά χαρακτηριστικά όπως την αναπαραγωγή βίντεο, DVD, VCD σε πιο προηγμένες λειτουργίες όπως την υποστήριξη των φίλτρων του MPlayer, κατάλογους edl, και άλλα πολλά."

; Misc
${LangFileString} Info_Codecs_Backup "Εφεδρικό αντίγραφο των κωδικοποιητών από προηγούμενη εγκατάσταση..."
${LangFileString} Info_Codecs_Restore "Επαναφορά των κωδικοποιητών από προηγούμενη εγκατάσταση..."
${LangFileString} Info_Del_Files "Διαγραφή αρχείων..."
${LangFileString} Info_Del_Registry "Διαγραφή των κλειδιών του μητρώου..."
${LangFileString} Info_Del_Shortcuts "Διαγραφή συντομεύσεων..."
${LangFileString} Info_Rest_Assoc "Επαναφορά των συσχετίσεων των αρχείων..."
${LangFileString} Info_RollBack "Αναίρεση των αλλαγών..."
${LangFileString} Info_Files_Extract "Αποσυμπίεση αρχείων..."
${LangFileString} Info_SMTube_Backup "Εφεδρικό αντίγραφο του SMTube..."
${LangFileString} Info_SMTube_Restore "Επαναφορά του SMTube από προηγούμενη εγκατάσταση..."
${LangFileString} Info_MPV_Backup "Εφεδρικό αντίγραφο του mpv..."
${LangFileString} Info_MPV_Restore "Επαναφορά του mpv από προηγούμενη εγκατάσταση..."

; MPV
${LangFileString} MPV_DL_Msg "Γίνεται λήψη του mpv..."
${LangFileString} MPV_DL_Retry "Η εγκατάσταση του mpv απέτυχε. Να δοκιμάσω πάλι;"
${LangFileString} MPV_DL_Failed "Απέτυχε η λήψη του mpv: «$R0»."
${LangFileString} MPV_Inst_Failed "Απέτυχε η εγκατάσταση του mpv."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "Η εγκατάσταση του youtube-dl απέτυχε. Να δοκιμάσω πάλι;"
${LangFileString} YTDL_DL_Failed "Αποτυχία λήψης του youtube-dl: '$R0'."
${LangFileString} YTDL_Update_Check "Έλεγχος ενημερώσεων του youtube-dl...."
${LangFileString} YTDL_Error_Msg1 "Warning: youtube-dl exited abnormally with exit code: $YTDL_Exit_code"
${LangFileString} YTDL_Error_Msg2 "Visual C++ 2010 Runtime (x86) is required for youtube-dl."

; SMTube
${LangFileString} SMTube_Incompatible_Msg1 "The current SMTube installation is incompatible with this version of SMPlayer."
${LangFileString} SMTube_Incompatible_Msg2 "Please upgrade to a newer version of SMTube."

; Post install
${LangFileString} Info_Cleaning_Fontconfig "Εκκαθάριση της λανθάνουσας μνήμης διαμόρφωσης των γραμματοσειρών..."
${LangFileString} Info_Cleaning_SMPlayer "Εκκαθάριση των ρυθμίσεων του SMPlayer..."
