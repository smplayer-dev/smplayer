; NSIS script created by redxii for RVM's SMPlayer
;--------------------------------
;Compressor

  SetCompressor /SOLID lzma
  SetCompressorDictSize 32

;--------------------------------
;Additional plugin folders

  !addplugindir plugins

;--------------------------------
;Defines & includes

!define PRODUCT_NAME "SMPlayer"
!define PRODUCT_VERSION "0.6.7"
!define PRODUCT_PUBLISHER "RVM"
!define PRODUCT_WEB_SITE "http://smplayer.sf.net"
!define PRODUCT_FORUM "http://smplayer.sourceforge.net/forums"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_GROUP "SMPlayer"

; Memento settings (required for Memento)
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY Software\SMPlayer

!include MUI2.nsh
!include Memento.nsh
!include WinVer.nsh

;--------------------------------
;Configuration

  ;General
  Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
!ifdef WITH_MPLAYER
  OutFile "smplayer_${PRODUCT_VERSION}_setup.exe"
!else
  OutFile "smplayer_${PRODUCT_VERSION}_websetup.exe"
!endif

  ;Version tab properties
  VIProductVersion "${PRODUCT_VERSION}.0"
  VIAddVersionKey "ProductName" "SMPlayer"
  VIAddVersionKey "Comments" "This installation was built with NSIS."
  VIAddVersionKey "LegalTrademarks" "RVM"
  VIAddVersionKey "LegalCopyright" "RVM"
  VIAddVersionKey "CompanyName" "RVM"
!ifdef WITH_MPLAYER
  VIAddVersionKey "FileDescription" "SMPlayer Installer (w/ MPlayer)"
!else
  VIAddVersionKey "FileDescription" "SMPlayer Installer (w/o MPlayer)"
!endif
  VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"

  /* Sets default install dir to $PROGRAMFILES\SMPlayer.
  If InstallDirRegKey exists (from a previous installation,
  it will default to that directory instead. */
  InstallDir "$PROGRAMFILES\SMPlayer"
  InstallDirRegKey HKLM "Software\SMPlayer" "Path"

  ;Put on a show
  ShowInstDetails show
  ShowUnInstDetails show

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

;--------------------------------
;Variables

  # Ignore Var MPLAYER_VERSION when including mplayer
!ifndef WITH_MPLAYER
  Var MPLAYER_VERSION
!endif
  Var CODEC_VERSION

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_COMPONENTSPAGE_SMALLDESC
  !define MUI_LICENSEPAGE_CHECKBOX
  !define MUI_FINISHPAGE_RUN $INSTDIR\smplayer.exe
  !define MUI_FINISHPAGE_RUN_NOTCHECKED
  !define MUI_FINISHPAGE_RUN_PARAMETERS http://88.191.30.130:8050
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\Release_notes.txt
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "View Release Notes"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

  # Installer/Uninstaller icons
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

  # Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
  !define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

;--------------------------------
;Pages
;MUI_PAGE_WELCOME should always be first and MUI_PAGE_FINISH should be last

  # Install Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "smplayer-build\Copying.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  # UnInstall Pages
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "Basque"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SpanishInternational"

;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Types
;First in list is #1, second in list in #2, etc

  InstType "Standard installation"
  InstType "Full installation"

;------------------------------------------------------------------------------------------------
;Installer Sections

;--------------------------------
; Main SMPlayer files
Section SMPlayer SMPlayer

  SectionIn 1 2 RO
  SetOutPath "$INSTDIR"
  File "smplayer-build\*"

  # Docs folder
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\docs"

  # Imageformats folder
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\imageformats"

  # Shortcuts folder
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\shortcuts"

  # UnInstall file
  WriteUninstaller "$INSTDIR\uninst.exe"

  # Store installed path
  WriteRegStr HKLM "Software\SMPlayer" "Path" "$INSTDIR"

  # HKEY_CLASSES_ROOT ProgId registration
  WriteRegStr HKCR "MPlayerFileVideo\DefaultIcon" "" '"$INSTDIR\smplayer.exe",1'
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue" "" "Enqueue in SMPlayer"
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue\command" "" '"$INSTDIR\smplayer.exe" -add-to-playlist "%1"'
  WriteRegStr HKCR "MPlayerFileVideo\shell\open" "FriendlyAppName" "SMPlayer Media Player"
  WriteRegStr HKCR "MPlayerFileVideo\shell\open\command" "" '"$INSTDIR\smplayer.exe" "%1"'

  # Windows Vista+ Default Programs registration
  ${If} ${AtLeastWinVista}
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities" "ApplicationDescription" "SMPlayer is a complete front-end for MPlayer, from basic features like playing videos, DVDs, VCDs to more advanced features like support for MPlayer filters, edl lists, and more."
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities" "ApplicationName" "SMPlayer"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".3gp" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ac3" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ape" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".asf" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".avi" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".bin" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".dat" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".divx" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".dv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".dvr-ms" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".flv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".iso" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".m1v" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".m2v" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".m4v" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mkv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mov" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mp3" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mp4" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mpeg" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mpg" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mpv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".mqv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".nsv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ogg" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ogm" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ra" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ram" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".rmvb" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".ts" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".vcd" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".vfw" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".vob" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".wav" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".wma" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities\FileAssociations" ".wmv" "MPlayerFileVideo"
    WriteRegStr HKLM "Software\RegisteredApplications" "SMPlayer" "Software\Clients\Media\SMPlayer\Capabilities"
  ${EndIf}

  # Registry Uninstall information
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\smplayer.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "HelpLink" "${PRODUCT_FORUM}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLUpdateInfo" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

  # Copy 7zip to installer's temp directory
  SetOutPath "$PLUGINSDIR"
  File 7za.exe

SectionEnd

;--------------------------------
; Desktop shortcut
${MementoSection} "Create Desktop shortcut" DesktopIcon
  SectionIn 1 2

  SetOutPath "$INSTDIR"
  # all = global; current = current user
  SetShellVarContext all
  CreateShortCut "$DESKTOP\SMPlayer.lnk" "$INSTDIR\smplayer.exe"

${MementoSectionEnd}

;--------------------------------
; Start menu shortcuts
${MementoSection} "Create Start Menu shortcuts" StartMenuIcon
  SectionIn 1 2

  SetOutPath "$INSTDIR"
  # Start menu shortcut creation
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}\${PRODUCT_NAME}.lnk" "$INSTDIR\smplayer.exe"
  WriteINIStr    "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}\SMPlayer on the Web.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe"

${MementoSectionEnd}

;--------------------------------
; MPlayer Components
SectionGroup /e "MPlayer Components"

;--------------------------------
; MPlayer
!ifdef WITH_MPLAYER
  Section MPlayer MPlayer
    SectionIn 1 2 RO

    SetOutPath "$INSTDIR"
    File /r "smplayer-build\mplayer"

    WriteRegDWORD HKLM Software\SMPlayer Installed_MPlayer 0x1

  SectionEnd
!else
  Section MPlayer MPlayer
    SectionIn 1 2 RO
    AddSize 15300

    ReadRegDWORD $0 HKLM Software\SMPlayer Installed_MPlayer

    IntCmp $0 1 mplayerInstalled mplayerNotInstalled
      mplayerInstalled:
        MessageBox MB_YESNO "MPlayer is already installed. Re-Download?" IDYES mplayerNotInstalled IDNO done
      mplayerNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call getVerInfo
        ${EndIf}

        ${If} ${FileExists} "$PLUGINSDIR\version-info"
          ReadINIStr $MPLAYER_VERSION "$PLUGINSDIR\version-info" smplayer mplayer
        ${Else}
          StrCpy $MPLAYER_VERSION "mplayer-svn-28311"
        ${EndIf}

        DetailPrint "Downloading MPlayer..."
        inetc::get /timeout 30000 /resume "" /caption "Downloading MPlayer..." /banner "Downloading $MPLAYER_VERSION.7z" \
        "http://downloads.sourceforge.net/smplayer/$MPLAYER_VERSION.7z?big_mirror=0" \
        "$PLUGINSDIR\$MPLAYER_VERSION.7z"
        /* inetc::get /caption "Downloading MPlayer..." /banner "Downloading $MPLAYER_VERSION.7z" \
        "ftp://ftp.berlios.de/pub/smplayer/test/$MPLAYER_VERSION.7z" \
        "$PLUGINSDIR\$MPLAYER_VERSION.7z" */
        Pop $R0
        StrCmp $R0 OK mplayerdl1
          MessageBox MB_OK "Failed to download mplayer package: $R0.$\nMPlayer is required for playback!"
          Abort
          mplayerdl1:
            # Extract
            nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$MPLAYER_VERSION.7z" -o"$PLUGINSDIR"'

            # Copy
            CreateDirectory "$INSTDIR\mplayer"
            CopyFiles /SILENT "$PLUGINSDIR\$MPLAYER_VERSION\*" "$INSTDIR\mplayer"

            IfFileExists "$INSTDIR\mplayer\mplayer.exe" mplayerInstSuccess mplayerInstFailed
              mplayerInstFailed:
                MessageBox MB_OK "Failed to install MPlayer. MPlayer is required for playback!"
                Abort
              mplayerInstSuccess:
                WriteRegDWORD HKLM Software\SMPlayer Installed_MPlayer 0x1

    done:

  SectionEnd
!endif

;--------------------------------
; Binary codecs
  Section /o "Optional Codecs" Codecs
    SectionIn 2
    AddSize 22300

    ReadRegDWORD $1 HKLM Software\SMPlayer Installed_Codecs

    IntCmp $1 1 mplayerCodecsInstalled mplayerCodecsNotInstalled
      mplayerCodecsInstalled:
        MessageBox MB_YESNO "MPlayer codecs are already installed. Re-Download?" IDYES mplayerCodecsNotInstalled IDNO done
      mplayerCodecsNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call getVerInfo
        ${EndIf}

        ${If} ${FileExists} "$PLUGINSDIR\version-info"
          ReadINIStr $CODEC_VERSION "$PLUGINSDIR\version-info" smplayer mplayercodecs
        ${Else}
          StrCpy $CODEC_VERSION "windows-essential-20071007"
        ${EndIf}

        DetailPrint "Downloading MPlayer Codecs..."
        inetc::get /timeout 30000 /resume "" /caption "Downloading MPlayer Codecs..." /banner "Downloading $CODEC_VERSION.zip" \
        "http://www.mplayerhq.hu/MPlayer/releases/codecs/$CODEC_VERSION.zip" \
        "$PLUGINSDIR\$CODEC_VERSION.zip"
        /* inetc::get /caption "Downloading MPlayer Codecs..." /banner "Downloading ${CODEC_VERSION}.zip" \
        "ftp://ftp.berlios.de/pub/smplayer/test/${CODEC_VERSION}.zip" \
        "$PLUGINSDIR\${CODEC_VERSION}.zip" */
        Pop $R0
        StrCmp $R0 OK codecdlSuccess
          MessageBox MB_OK "Failed to download codec package: $R0.$\nCodec installation will be skipped."
          Goto done
          codecdlSuccess:
            # Extract
            nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$CODEC_VERSION.zip" -o"$PLUGINSDIR"'

            # Copy
            CreateDirectory "$INSTDIR\mplayer\codecs"
            CopyFiles /SILENT "$PLUGINSDIR\$CODEC_VERSION\*" "$INSTDIR\mplayer\codecs"

            IfFileExists "$INSTDIR\mplayer\codecs\Readme.txt" codecsInstSuccess codecsInstFailed
              codecsInstFailed:
                MessageBox MB_OK "Failed to install MPlayer codecs. Re-run setup and try again."
                Goto done
              codecsInstSuccess:
                WriteRegDWORD HKLM Software\SMPlayer Installed_Codecs 0x1

    done:

	SectionEnd

SectionGroupEnd

;--------------------------------
; Icon Themes
${MementoSection} "Icon Themes" Themes

  SectionIn 1 2
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\themes"

${MementoSectionEnd}

;--------------------------------
; Translations
${MementoSection} Translations Translations

  SectionIn 1 2
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\translations"

${MementoSectionEnd}

${MementoSectionDone}

;--------------------------------
; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SMPlayer} "SMPlayer, shared libraries, and documentation."
  !insertmacro MUI_DESCRIPTION_TEXT ${DesktopIcon} "Creates a shortcut on the desktop."
  !insertmacro MUI_DESCRIPTION_TEXT ${StartMenuIcon} "Creates start menu shortcuts."
!ifdef WITH_MPLAYER
  !insertmacro MUI_DESCRIPTION_TEXT ${MPlayer} "The engine behind SMPlayer, required for playback."
!else
  !insertmacro MUI_DESCRIPTION_TEXT ${MPlayer} "Downloads/installs mplayer; requires an active internet connection. Required for playback."
!endif
  !insertmacro MUI_DESCRIPTION_TEXT ${Codecs} "Downloads optional codecs that aren't yet implemented in mplayer; e.g. RealVideo and uncommon formats."
  !insertmacro MUI_DESCRIPTION_TEXT ${Themes} "Stylish icon themes for SMPlayer."
  !insertmacro MUI_DESCRIPTION_TEXT ${Translations} "Translations for SMPlayer."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Installer Functions

Function .onInit

  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "$(^Name)") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
    Abort

  !insertmacro MUI_LANGDLL_DISPLAY

  ${MementoSectionRestore}

FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}

FunctionEnd

Function .onInstFailed

  # Delete desktop and start menu shortcuts
  SetShellVarContext all
  Delete "$DESKTOP\SMPlayer.lnk"
  RMDir /r "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}"

  # Delete directories recursively except for main directory
  # Nullsoft says it is unsafe to recursively delete $INSTDIR 
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\mplayer"
  RMDir /r "$INSTDIR\shortcuts"
  RMDir /r "$INSTDIR\themes"
  RMDir /r "$INSTDIR\translations"
  Delete "$INSTDIR\*.txt"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\Q*.dll"
  Delete "$INSTDIR\smplayer.exe"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\dxlist.exe"
  RMDir "$INSTDIR"

  # Delete keys pertaining to SMPlayer
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKCR "MPlayerFileVideo"
  DeleteRegKey HKLM "Software\Clients\Media\SMPlayer"
  DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"
  DeleteRegKey HKLM "Software\SMPlayer"

FunctionEnd

Function getVerInfo

  DetailPrint "Gathering version information..."
  inetc::get /timeout 30000 /resume "" /silent "http://smplayer.sourceforge.net/mplayer-version-info" \
  "$PLUGINSDIR\version-info"
  Pop $R0
  StrCmp $R0 OK +2
    DetailPrint "Error retrieving version info: $R0. Setup will use a default version."

FunctionEnd

;End Installer Sections
;------------------------------------------------------------------------------------------------

;------------------------------------------------------------------------------------------------
;UnInstaller Sections

Section Uninstall

  # Restore all file associations...
  ExecWait '"$INSTDIR\smplayer.exe" -uninstall'

  # Delete desktop and start menu shortcuts
  SetShellVarContext all
  Delete "$DESKTOP\SMPlayer.lnk"
  RMDir /r "$SMPROGRAMS\${PRODUCT_STARTMENU_GROUP}"

  # Delete directories recursively except for main directory
  # Nullsoft says it is unsafe to recursively delete $INSTDIR 
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\mplayer"
  RMDir /r "$INSTDIR\shortcuts"
  RMDir /r "$INSTDIR\themes"
  RMDir /r "$INSTDIR\translations"
  Delete "$INSTDIR\*.txt"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\Q*.dll"
  Delete "$INSTDIR\smplayer.exe"
  Delete "$INSTDIR\dxlist.exe"
  Delete "$INSTDIR\uninst.exe"
  RMDir "$INSTDIR"

  # Delete keys pertaining to SMPlayer
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKCR "MPlayerFileVideo"
  DeleteRegKey HKLM "Software\Clients\Media\SMPlayer"
  DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"
  DeleteRegKey HKLM "Software\SMPlayer"

  SetAutoClose true

SectionEnd

;--------------------------------
;UnInstaller Functions
Function un.onInit

  # Get the stored language preference
  !insertmacro MUI_UNGETLANGUAGE

  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort

FunctionEnd

Function un.onUninstSuccess

  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."

FunctionEnd