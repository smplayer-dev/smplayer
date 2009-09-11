; Installer script for win32 SMPlayer
; Written by redxii <redxii1234@hotmail.com>

;--------------------------------
;Compressor

  SetCompressor /SOLID lzma
  SetCompressorDictSize 32

;--------------------------------
;Additional plugin folders

  !addplugindir plugins

;--------------------------------
;Misc Defines

!ifdef VER_REVISION
  !define PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
!else
  !define PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
!endif
  !define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMPlayer"

  ; Fallback versions
!ifndef DEFAULT_CODECS_VERSION
  !define DEFAULT_CODECS_VERSION "windows-essential-20071007"
!endif
!ifndef WITH_MPLAYER
!ifndef DEFAULT_MPLAYER_VERSION
  !define DEFAULT_MPLAYER_VERSION "mplayer-svn-28311-2"
!endif
!endif

;--------------------------------
;Include Modern UI and functions

  !include MUI2.nsh
  !include Sections.nsh
  !include Memento.nsh
  !include WinVer.nsh

;--------------------------------
;General

  ;Name and file
  Name "SMPlayer ${PRODUCT_VERSION}"
  BrandingText "SMPlayer for Windows v${PRODUCT_VERSION}"
!ifdef WITH_MPLAYER
  OutFile "smplayer-${PRODUCT_VERSION}-win32.exe"
!else
  OutFile "smplayer-${PRODUCT_VERSION}-win32-webdl.exe"
!endif

  ;Version tab properties
!ifdef VER_REVISION
  VIProductVersion "${PRODUCT_VERSION}"
!else
  VIProductVersion "${PRODUCT_VERSION}.0"
!endif
  VIAddVersionKey "ProductName" "SMPlayer"
  VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
!ifdef WITH_MPLAYER
  VIAddVersionKey "FileDescription" "SMPlayer Installer (w/ MPlayer)"
!else
  VIAddVersionKey "FileDescription" "SMPlayer Installer (MPlayer Web Downloader)"
!endif
  VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"
  VIAddVersionKey "LegalCopyright" ""

  ;Default installation folder
  InstallDir "$PROGRAMFILES\SMPlayer"

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\SMPlayer" "Path"

  ;Show details
  ShowInstDetails show
  ShowUnInstDetails show

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var CODEC_VERSION
  Var IS_ADMIN
!ifndef WITH_MPLAYER
  Var MPLAYER_VERSION
!endif
  Var USERNAME

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_COMPONENTSPAGE_SMALLDESC
  !define MUI_LICENSEPAGE_RADIOBUTTONS
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  !define MUI_FINISHPAGE_RUN $INSTDIR\smplayer.exe
  !define MUI_FINISHPAGE_RUN_NOTCHECKED
  !define MUI_FINISHPAGE_RUN_PARAMETERS http://88.191.30.130:8050
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\Release_notes.txt
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "View Release Notes"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

  ;Installer/Uninstaller icons
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

  ;Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT HKLM
  !define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

  ;Memento Settings
  !define MEMENTO_REGISTRY_ROOT HKLM
  !define MEMENTO_REGISTRY_KEY Software\SMPlayer

;--------------------------------
;Pages

  ;Install pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "smplayer-build\Copying.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  ;Uninstall pages
  !insertmacro MUI_UNPAGE_CONFIRM
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
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Spanish"

;--------------------------------
; Translations for setup

  !include "translations\basque.nsh"
  !include "translations\catalan.nsh"
  !include "translations\czech.nsh"
  !include "translations\danish.nsh"
  !include "translations\dutch.nsh"
  !include "translations\english.nsh"
  !include "translations\finnish.nsh"
  !include "translations\french.nsh"
  !include "translations\german.nsh"
  !include "translations\hebrew.nsh"
  !include "translations\hungarian.nsh"
  !include "translations\italian.nsh"
  !include "translations\norwegian.nsh"
  !include "translations\polish.nsh"
  !include "translations\portuguese.nsh"
  !include "translations\russian.nsh"
  !include "translations\slovak.nsh"
  !include "translations\slovenian.nsh"
  !include "translations\spanish.nsh"

;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL
  ReserveFile "${NSISDIR}\Plugins\UserInfo.dll"

;--------------------------------
;Installer Types

  InstType "Recommended"
  InstType "Minimum"
  InstType "Full"

;------------------------------------------------------------------------------------------------
;Installer Sections

;--------------------------------
; Main SMPlayer files
Section SMPlayer SMPlayer

  SectionIn 1 2 3 RO
  SetOutPath "$INSTDIR"
  File "smplayer-build\*"
  File /r "smplayer-build\docs"
  File /r "smplayer-build\imageformats"
  File /r "smplayer-build\shortcuts"

  ;Initialize to 0 if don't exist (based on error flag)
  ClearErrors
  ReadRegDWORD $R0 HKLM Software\SMPlayer Installed_MPlayer
  ${If} ${Errors}
    WriteRegDWORD HKLM Software\SMPlayer Installed_MPlayer 0x0
  ${EndIf}
  ClearErrors
  ReadRegDWORD $R0 HKLM Software\SMPlayer Installed_Codecs
  ${If} ${Errors}
    WriteRegDWORD HKLM Software\SMPlayer Installed_Codecs 0x0
  ${EndIf}

  SetOutPath "$PLUGINSDIR"
  File 7za.exe

SectionEnd

;--------------------------------
; Desktop shortcut
${MementoSection} "Desktop Shortcut" DesktopIcon
  SectionIn 1 3

  SetOutPath "$INSTDIR"
  SetShellVarContext all
  CreateShortCut "$DESKTOP\SMPlayer.lnk" "$INSTDIR\smplayer.exe"

${MementoSectionEnd}

;--------------------------------
; Start menu shortcuts
${MementoSection} "Start Menu Shortcut" StartMenuIcon
  SectionIn 1 3

  SetOutPath "$INSTDIR"
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\SMPlayer"
  CreateShortCut "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" "$INSTDIR\smplayer.exe"
  WriteINIStr    "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url" "InternetShortcut" "URL" "http://smplayer.sf.net"
  CreateShortCut "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk" "$INSTDIR\uninst.exe"

${MementoSectionEnd}

;--------------------------------
; MPlayer Components
SectionGroup /e "MPlayer Components"

;--------------------------------
; MPlayer
!ifdef WITH_MPLAYER
  Section MPlayer MPlayer
    SectionIn 1 2 3 RO

    SetOutPath "$INSTDIR"
    File /r "smplayer-build\mplayer"

    WriteRegDWORD HKLM Software\SMPlayer Installed_MPlayer 0x1

  SectionEnd
!else
  Section MPlayer MPlayer
    SectionIn 1 2 3 RO
    AddSize 15300

    ReadRegDWORD $0 HKLM Software\SMPlayer Installed_MPlayer

    IntCmp $0 1 mplayerInstalled mplayerNotInstalled
      mplayerInstalled:
        MessageBox MB_YESNO $(MPLAYER_IS_INSTALLED) /SD IDNO IDYES mplayerNotInstalled IDNO done
      mplayerNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call getVerInfo
        ${EndIf}

        IfFileExists "$PLUGINSDIR\version-info" 0 noVerInfo
          ClearErrors
          ReadINIStr $MPLAYER_VERSION "$PLUGINSDIR\version-info" smplayer mplayer

          IfErrors 0 done_ver_info
            DetailPrint $(VERINFO_IS_MISSING)
            ;Default Value if version-info exists but version string is missing from version-info
            StrCpy $MPLAYER_VERSION ${DEFAULT_MPLAYER_VERSION}
            Goto done_ver_info

        noVerInfo:
          ;Default Value if version-info doesn't exist
          StrCpy $MPLAYER_VERSION ${DEFAULT_MPLAYER_VERSION}

    done_ver_info:

      DetailPrint $(MPLAYER_IS_DOWNLOADING)
      inetc::get /timeout 30000 /resume "" /caption $(MPLAYER_IS_DOWNLOADING) /banner "Downloading $MPLAYER_VERSION.7z" \
      "http://downloads.sourceforge.net/smplayer/$MPLAYER_VERSION.7z?big_mirror=0" \
      "$PLUGINSDIR\$MPLAYER_VERSION.7z"
      Pop $R0
      StrCmp $R0 OK 0 check_mplayer

      ;Extract
      nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$MPLAYER_VERSION.7z" -y -o"$PLUGINSDIR"'

      ;Copy
      CreateDirectory "$INSTDIR\mplayer"
      CopyFiles /SILENT "$PLUGINSDIR\$MPLAYER_VERSION\*" "$INSTDIR\mplayer"

    check_mplayer:
      ;This label does not necessarily mean there was a download error, so check first
      ${If} $R0 != "OK"
        DetailPrint "$(MPLAYER_DL_FAILED) $R0."
      ${EndIf}

      IfFileExists "$INSTDIR\mplayer\mplayer.exe" mplayerInstSuccess mplayerInstFailed
        mplayerInstSuccess:
          WriteRegDWORD HKLM Software\SMPlayer Installed_MPlayer 0x1
          Goto done
        mplayerInstFailed:
          Abort $(MPLAYER_INST_FAILED)

    done:

  SectionEnd
!endif

;--------------------------------
; Binary codecs
  Section /o "Optional Codecs" Codecs
    SectionIn 3
    AddSize 22300

    ReadRegDWORD $1 HKLM Software\SMPlayer Installed_Codecs

    IntCmp $1 1 mplayerCodecsInstalled mplayerCodecsNotInstalled
      mplayerCodecsInstalled:
        MessageBox MB_YESNO $(CODECS_IS_INSTALLED) /SD IDNO IDYES mplayerCodecsNotInstalled IDNO done
      mplayerCodecsNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call getVerInfo
        ${EndIf}

        IfFileExists "$PLUGINSDIR\version-info" 0 noVerInfo
          ClearErrors
          ReadINIStr $CODEC_VERSION "$PLUGINSDIR\version-info" smplayer mplayercodecs

          IfErrors 0 done_ver_info
            DetailPrint $(VERINFO_IS_MISSING)
            ;Default Value if version-info exists but version string is missing from version-info
            StrCpy $CODEC_VERSION ${DEFAULT_CODECS_VERSION}
            Goto done_ver_info

        noVerInfo:
          ;Default Value if version-info doesn't exist
          StrCpy $CODEC_VERSION ${DEFAULT_CODECS_VERSION}

    done_ver_info:

      DetailPrint $(CODECS_IS_DOWNLOADING)
      inetc::get /timeout 30000 /resume "" /caption $(CODECS_IS_DOWNLOADING) /banner "Downloading $CODEC_VERSION.zip" \
      "http://www.mplayerhq.hu/MPlayer/releases/codecs/$CODEC_VERSION.zip" \
      "$PLUGINSDIR\$CODEC_VERSION.zip"
      Pop $R0
      StrCmp $R0 OK 0 check_codecs

      ;Extract
      nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$CODEC_VERSION.zip" -y -o"$PLUGINSDIR"'

      ;Copy
      CreateDirectory "$INSTDIR\mplayer\codecs"
      CopyFiles /SILENT "$PLUGINSDIR\$CODEC_VERSION\*" "$INSTDIR\mplayer\codecs"

    check_codecs:
      ;This label does not necessarily mean there was a download error, so check first
      ${If} $R0 != "OK"
        DetailPrint "$(CODECS_DL_FAILED) $R0."
      ${EndIf}

      IfFileExists "$INSTDIR\mplayer\codecs\*.dll" codecsInstSuccess codecsInstFailed
        codecsInstSuccess:
          WriteRegDWORD HKLM Software\SMPlayer Installed_Codecs 0x1
          Goto done
        codecsInstFailed:
          DetailPrint $(CODECS_INST_FAILED)
          WriteRegDWORD HKLM Software\SMPlayer Installed_Codecs 0x0
          Sleep 5000

    done:

	SectionEnd

SectionGroupEnd

;--------------------------------
; Icon Themes
${MementoSection} "Icon Themes" Themes

  SectionIn 1 3
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\themes"

${MementoSectionEnd}

;--------------------------------
; Translations
${MementoSection} Translations Translations

  SectionIn 1 3
  SetOutPath "$INSTDIR"
  File /r "smplayer-build\translations"

${MementoSectionEnd}

Section -Post

  ;Uninstall file
  WriteUninstaller "$INSTDIR\uninst.exe"

  ;Store installed path
  WriteRegStr HKLM "Software\SMPlayer" "Path" "$INSTDIR"
  WriteRegStr HKLM "Software\SMPlayer" "Version" "${PRODUCT_VERSION}"

  ${If} ${AtLeastWinVista}
    Call defaultProgramsReg
  ${EndIf}

  ;Registry Uninstall information
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\smplayer.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "HelpLink" "http://smplayer.sourceforge.net/forums"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "Publisher" "RVM"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "http://smplayer.sf.net"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLUpdateInfo" "http://smplayer.sf.net"
  WriteRegDWORD HKLM "${PRODUCT_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD HKLM "${PRODUCT_UNINST_KEY}" "NoRepair" "1"

SectionEnd

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
  !insertmacro MUI_DESCRIPTION_TEXT ${Codecs} "Downloads/installs optional codecs for mplayer; requires an active internet connection."
  !insertmacro MUI_DESCRIPTION_TEXT ${Themes} "Stylish icon themes for SMPlayer."
  !insertmacro MUI_DESCRIPTION_TEXT ${Translations} "Translations for SMPlayer."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Installer Functions

Function .onInit

  /* Check if setup is already running */
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "MPlayerSMPlayer") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION $(SMPLAYER_INSTALLER_IS_RUNNING)
    Abort

  /* Check unsupported Windows OSes */
  ${Unless} ${AtLeastWin2000}
    MessageBox MB_YESNO|MB_ICONSTOP $(SMPLAYER_INSTALLER_UNSUPPORTED_OS) /SD IDNO IDYES installonoldwindows
    Abort
    installonoldwindows:
  ${EndUnless}

  /* Privileges Check */
  Call CheckUserRights

  ;Check for admin (mimic old Inno Setup behavior... non-admin installation maybe later..)
  ${If} $IS_ADMIN == 0
    MessageBox MB_OK|MB_ICONSTOP $(SMPLAYER_INSTALLER_NO_ADMIN)
    Abort
  ${EndIf}

  /* Uninstall previous version */
  ReadRegStr $R0 HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
  StrCmp $R0 "" nouninst

  MessageBox MB_YESNO|MB_ICONEXCLAMATION $(SMPLAYER_INSTALLER_PREV_VERSION) IDNO nouninst

  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
  nouninst:

  !insertmacro MUI_LANGDLL_DISPLAY

  ${MementoSectionRestore}

FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}

FunctionEnd

Function .onInstFailed

  Call UninstallSMPlayer

  Delete "$INSTDIR\uninst.exe"
  RMDir "$INSTDIR"

FunctionEnd

Function .onSelChange

  SectionGetFlags ${Codecs} $R0
  ${If} $R0 != $R1
    StrCpy $R1 $R0
    IntOp $R0 $R0 & ${SF_SELECTED}
  ${If} $R0 == ${SF_SELECTED}
    MessageBox MB_OK $(MPLAYER_CODEC_INFORMATION)
  ${EndIf}
  ${EndIf}

FunctionEnd

Function defaultProgramsReg

  ;HKEY_CLASSES_ROOT ProgId registration
  WriteRegStr HKCR "MPlayerFileVideo\DefaultIcon" "" '"$INSTDIR\smplayer.exe",1'
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue" "" "Enqueue in SMPlayer"
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue\command" "" '"$INSTDIR\smplayer.exe" -add-to-playlist "%1"'
  WriteRegStr HKCR "MPlayerFileVideo\shell\open" "FriendlyAppName" "SMPlayer Media Player"
  WriteRegStr HKCR "MPlayerFileVideo\shell\open\command" "" '"$INSTDIR\smplayer.exe" "%1"'

  WriteRegStr HKLM "Software\Clients\Media\SMPlayer\Capabilities" "ApplicationDescription" $(APPLICATION_DESCRIPTION)
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

FunctionEnd

Function getVerInfo

  DetailPrint $(VERINFO_IS_DOWNLOADING)
  inetc::get /timeout 30000 /resume "" /silent "http://smplayer.sourceforge.net/mplayer-version-info" \
  "$PLUGINSDIR\version-info"
  Pop $R0
  StrCmp $R0 OK +2
    DetailPrint "$(VERINFO_DL_FAILED) $R0."

FunctionEnd

;End Installer Sections
;------------------------------------------------------------------------------------------------

/************************************************************************************************
************************************** Shared Functions *****************************************
************************************************************************************************/
!macro CheckUserRightsMacro un
Function ${un}CheckUserRights

  ClearErrors
  UserInfo::GetName
  ${If} ${Errors}
    StrCpy $IS_ADMIN 1
    Return
  ${EndIf}

  Pop $USERNAME
  UserInfo::GetAccountType
  Pop $R0
  ${Switch} $R0
    ${Case} "Admin"
    ${Case} "Power"
      StrCpy $IS_ADMIN 1
      ${Break}
    ${Default}
      StrCpy $IS_ADMIN 0
      ${Break}
  ${EndSwitch}

FunctionEnd
!macroend
!insertmacro CheckUserRightsMacro ""
!insertmacro CheckUserRightsMacro "un."

!macro UninstallSMPlayerMacro un
Function ${un}UninstallSMPlayer

  ;Delete registry keys
  SetDetailsPrint textonly
  DetailPrint "Deleting Registry Keys..."
  SetDetailsPrint listonly

  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKCR "MPlayerFileVideo"
  DeleteRegKey HKLM "Software\Clients\Media\SMPlayer"
  DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"
  DeleteRegKey HKLM "Software\SMPlayer"

  ;Delete desktop and start menu shortcuts
  SetDetailsPrint textonly
  DetailPrint "Deleting Shortcuts..."
  SetDetailsPrint listonly

  SetShellVarContext all
  Delete "$DESKTOP\SMPlayer.lnk"
  Delete "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" 
  Delete "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url"
  Delete "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk"
  RMDir "$SMPROGRAMS\SMPlayer"

  ;Delete directories recursively except for main directory
  ;Do not recursively delete $INSTDIR
  SetDetailsPrint textonly
  DetailPrint "Deleting Files..."
  SetDetailsPrint listonly

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

  SetDetailsPrint both

FunctionEnd
!macroend
!insertmacro UninstallSMPlayerMacro ""
!insertmacro UninstallSMPlayerMacro "un."

;End Shared Functions
;------------------------------------------------------------------------------------------------

/************************************************************************************************
**************************************** Uninstaller ********************************************
************************************************************************************************/

;--------------------------------
;UnInstaller Sections

Section Uninstall

  ;Make sure SMPlayer is installed from where the uninstaller is being executed.
  IfFileExists $INSTDIR\smplayer.exe smplayer_installed
    MessageBox MB_YESNO $(SMPLAYER_NOT_INSTALLED) IDYES smplayer_installed
    Abort $(UNINSTALL_ABORTED)

  smplayer_installed:

  SetDetailsPrint textonly
  DetailPrint "Restoring file associations..."
  SetDetailsPrint listonly

  ExecWait '"$INSTDIR\smplayer.exe" -uninstall'

  Call un.UninstallSMPlayer

  Delete "$INSTDIR\uninst.exe"
  RMDir "$INSTDIR"

SectionEnd

;--------------------------------
;UnInstaller Functions

Function un.onInit

  Call un.CheckUserRights

  ;Check for admin (mimic old Inno Setup behavior... non-admin installation maybe later..)
  ${If} $IS_ADMIN == 0
    MessageBox MB_OK|MB_ICONSTOP $(UNINSTALL_NO_ADMIN)
    Abort
  ${EndIf}

  ;Get the stored language preference
  !insertmacro MUI_UNGETLANGUAGE

FunctionEnd