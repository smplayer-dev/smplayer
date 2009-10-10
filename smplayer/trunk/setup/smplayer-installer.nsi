; Installer script for win32 SMPlayer
; Written by redxii <redxii1234@hotmail.com>

;--------------------------------
;Compressor

  SetCompressor /SOLID lzma
  SetCompressorDictSize 32

;--------------------------------
;Additional plugin folders

  !addplugindir .

;--------------------------------
;Defines

!ifdef VER_REVISION
  !define SMPLAYER_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
  !define SMPLAYER_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
!else ifndef VER_REVISION
  !define SMPLAYER_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
  !define SMPLAYER_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.0"
!endif

  !define SMPLAYER_REG_KEY "Software\SMPlayer"
  !define SMPLAYER_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMPlayer"

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
;General

  ;Name and file
  Name "SMPlayer ${SMPLAYER_VERSION}"
  BrandingText "SMPlayer for Windows v${SMPLAYER_VERSION}"
!ifdef WITH_MPLAYER
  OutFile "smplayer-${SMPLAYER_VERSION}-win32.exe"
!else ifndef WITH_MPLAYER
  OutFile "smplayer-${SMPLAYER_VERSION}-win32-webdl.exe"
!endif

  ;Version tab properties
  VIProductVersion "${SMPLAYER_PRODUCT_VERSION}"
  VIAddVersionKey "ProductName" "SMPlayer"
  VIAddVersionKey "ProductVersion" "${SMPLAYER_VERSION}"
  VIAddVersionKey "FileVersion" "${SMPLAYER_VERSION}"
  VIAddVersionKey "LegalCopyright" ""
!ifdef WITH_MPLAYER
  VIAddVersionKey "FileDescription" "SMPlayer Installer (w/ MPlayer)"
!else ifndef WITH_MPLAYER
  VIAddVersionKey "FileDescription" "SMPlayer Installer (MPlayer Web Downloader)"
!endif

  ;Show details
  ShowInstDetails show
  ShowUnInstDetails show

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel user

;--------------------------------
;Variables

  Var ALL_USERS
  Var CODEC_VERSION
  Var IS_ADMIN
!ifndef WITH_MPLAYER
  Var MPLAYER_VERSION
!endif
  Var PREVIOUS_INSTALLDIR
  Var PREVIOUS_INSTALLMODE
  Var PREVIOUS_VERSION
  Var USERNAME

;--------------------------------
;Interface Settings

  ; License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS

  ; Components page
  !define MUI_COMPONENTSPAGE_SMALLDESC

  ; Finish page
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  !define MUI_FINISHPAGE_RUN $INSTDIR\smplayer.exe
  !define MUI_FINISHPAGE_RUN_NOTCHECKED
  !define MUI_FINISHPAGE_RUN_PARAMETERS http://88.191.30.130:8050
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\Release_notes.txt
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "View Release Notes"

  ; Misc
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
  !define MUI_ABORTWARNING

  ;Installer/Uninstaller icons
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

  ;Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT SHELL_CONTEXT
  !define MUI_LANGDLL_REGISTRY_KEY "${SMPLAYER_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

  ;Memento Settings
  !define MEMENTO_REGISTRY_ROOT SHELL_CONTEXT
  !define MEMENTO_REGISTRY_KEY "${SMPLAYER_REG_KEY}"

  ;Multiuser settings
  !define MULTIUSER_EXECUTIONLEVEL Highest
  !define MULTIUSER_INSTALLMODE_COMMANDLINE
  !define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "${SMPLAYER_REG_KEY}"
  !define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME "Path"
  !define MULTIUSER_INSTALLMODE_INSTDIR "SMPlayer"
  !define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "${SMPLAYER_REG_KEY}"
  !define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "Path"
  !define MULTIUSER_MUI

;--------------------------------
;Include Modern UI and functions

  !include MUI2.nsh
  !include Sections.nsh
  !include Memento.nsh
  !include MultiUser.nsh
  !include WinVer.nsh

;--------------------------------
;Pages

  ;Install pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "smplayer-build\Copying.txt"
  !define MUI_PAGE_CUSTOMFUNCTION_PRE CheckPrevInstallMode
  !insertmacro MULTIUSER_PAGE_INSTALLMODE
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

; Custom translations for setup

  !insertmacro LANGFILE_INCLUDE "translations\basque.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\catalan.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\czech.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\danish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\dutch.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\english.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\finnish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\french.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\german.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hebrew.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hungarian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\italian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\norwegian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\polish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\portuguese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\russian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovak.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovenian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\spanish.nsh"

;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL
  ReserveFile "${NSISDIR}\Plugins\UserInfo.dll"

;--------------------------------
;Installer Types

  InstType "Typical"
  InstType "Compact"
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
  ReadRegDWORD $R0 SHCTX "${SMPLAYER_REG_KEY}" Installed_MPlayer
  ${If} ${Errors}
    WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x0
  ${EndIf}
  ClearErrors
  ReadRegDWORD $R0 SHCTX "${SMPLAYER_REG_KEY}" Installed_Codecs
  ${If} ${Errors}
    WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_Codecs 0x0
  ${EndIf}

  SetOutPath "$PLUGINSDIR"
  File 7za.exe

SectionEnd

;--------------------------------
; Desktop shortcut
${MementoSection} "Desktop Shortcut" DesktopIcon
  SectionIn 1 3

  SetOutPath "$INSTDIR"
  CreateShortCut "$DESKTOP\SMPlayer.lnk" "$INSTDIR\smplayer.exe"

${MementoSectionEnd}

;--------------------------------
; Start menu shortcuts
${MementoSection} "Start Menu Shortcut" StartMenuIcon
  SectionIn 1 3

  SetOutPath "$INSTDIR"
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

    WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x1

  SectionEnd
!else ifndef WITH_MPLAYER
  Section MPlayer MPlayer
    SectionIn 1 2 3 RO
    AddSize 15300

    ReadRegDWORD $0 SHCTX "${SMPLAYER_REG_KEY}" Installed_MPlayer

    IntCmp $0 1 mplayerInstalled mplayerNotInstalled
      mplayerInstalled:
        MessageBox MB_YESNO $(MPLAYER_IS_INSTALLED) /SD IDNO IDYES mplayerNotInstalled IDNO done
      mplayerNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call GetVerInfo
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
          WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x1
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

    ReadRegDWORD $1 SHCTX "${SMPLAYER_REG_KEY}" Installed_Codecs

    IntCmp $1 1 mplayerCodecsInstalled mplayerCodecsNotInstalled
      mplayerCodecsInstalled:
        MessageBox MB_YESNO $(CODECS_IS_INSTALLED) /SD IDNO IDYES mplayerCodecsNotInstalled IDNO done
      mplayerCodecsNotInstalled:
        ${IfNot} ${FileExists} "$PLUGINSDIR\version-info"
          Call GetVerInfo
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
          WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_Codecs 0x1
          Goto done
        codecsInstFailed:
          DetailPrint $(CODECS_INST_FAILED)
          WriteRegDWORD SHCTX "${SMPLAYER_REG_KEY}" Installed_Codecs 0x0
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
  WriteRegStr SHCTX "${SMPLAYER_REG_KEY}" "Path" "$INSTDIR"
  WriteRegStr SHCTX "${SMPLAYER_REG_KEY}" "Version" "${SMPLAYER_VERSION}"

  ${If} ${AtLeastWinVista}
  ${AndIf} $MultiUser.InstallMode == "AllUsers"
    Call DefaultProgramsReg
  ${EndIf}

  ;Registry Uninstall information
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "DisplayIcon" "$INSTDIR\smplayer.exe"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "DisplayVersion" "${SMPLAYER_VERSION}"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "HelpLink" "http://smplayer.sourceforge.net/forums"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "Publisher" "RVM"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "URLInfoAbout" "http://smplayer.sf.net"
  WriteRegStr SHCTX "${SMPLAYER_UNINST_KEY}" "URLUpdateInfo" "http://smplayer.sf.net"
  WriteRegDWORD SHCTX "${SMPLAYER_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD SHCTX "${SMPLAYER_UNINST_KEY}" "NoRepair" "1"

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
!else ifndef WITH_MPLAYER
  !insertmacro MUI_DESCRIPTION_TEXT ${MPlayer} "Downloads/installs mplayer; requires an active internet connection. Required for playback."
!endif
  !insertmacro MUI_DESCRIPTION_TEXT ${Codecs} "Downloads/installs optional codecs for mplayer; requires an active internet connection."
  !insertmacro MUI_DESCRIPTION_TEXT ${Themes} "Stylish icon themes for SMPlayer."
  !insertmacro MUI_DESCRIPTION_TEXT ${Translations} "Translations for SMPlayer."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;------------------------------------------------------------------------------------------------
;Installer Functions

Function .onInit

  /* Check if setup is already running */
  System::Call 'kernel32::CreateMutexW(i 0, i 0, t "MPlayerSMPlayer") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION $(SMPLAYER_INSTALLER_IS_RUNNING)
    Abort

  Call GetUserInfo
  Call ReadPreviousVersion

  ${If} $ALL_USERS == 1
    ${If} $IS_ADMIN == 0
      ${If} $PREVIOUS_VERSION != ""
        MessageBox MB_OK|MB_ICONSTOP $(SMPLAYER_INSTALLER_PREV_ALL_USERS) /SD IDOK
        Abort
      ${EndIf}
    ${EndIf}
  ${EndIf}

  !insertmacro MULTIUSER_INIT

  !insertmacro MUI_LANGDLL_DISPLAY

  Call RemovePreviousVersion

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

Function DefaultProgramsReg

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

Function CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""

    ; Make sure directory is valid
    Push $R0
    Push $R1
    StrCpy $R0 "$PREVIOUS_INSTALLDIR" "" -1
    ${If} $R0 == '\'
    ${OrIf} $R0 == '/'
      StrCpy $R0 $PREVIOUS_INSTALLDIR*.*
    ${Else}
      StrCpy $R0 $PREVIOUS_INSTALLDIR\*.*
    ${EndIf}
    ${IfNot} ${FileExists} $R0
      StrCpy $PREVIOUS_INSTALLDIR ""
    ${EndIf}
    Pop $R1
    Pop $R0

  ${EndIf}

FunctionEnd

Function CheckPrevInstallMode

  /* Detects previous install mode and hides the selection page.
     Abort skips the page, it does not abort the installer. */

  ReadRegStr $PREVIOUS_INSTALLMODE HKLM "${SMPLAYER_REG_KEY}" "Path"
  ${If} $PREVIOUS_INSTALLMODE != ""
    Abort
  ${EndIf}

  ReadRegStr $PREVIOUS_INSTALLMODE HKCU "${SMPLAYER_REG_KEY}" "Path"
  ${If} $PREVIOUS_INSTALLMODE != ""
    Abort
  ${EndIf}

FunctionEnd

Function GetUserInfo

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

Function GetVerInfo

  DetailPrint $(VERINFO_IS_DOWNLOADING)
  inetc::get /timeout 30000 /resume "" /silent "http://smplayer.sourceforge.net/mplayer-version-info" \
  "$PLUGINSDIR\version-info"
  Pop $R0
  StrCmp $R0 OK +2
    DetailPrint "$(VERINFO_DL_FAILED) $R0."

FunctionEnd

Function ReadPreviousVersion

  ReadRegStr $PREVIOUS_INSTALLDIR HKLM "${SMPLAYER_REG_KEY}" "Path"

  Call CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""
    ;Detect version
    ReadRegStr $PREVIOUS_VERSION HKLM "${SMPLAYER_REG_KEY}" "Version"
    ${If} $PREVIOUS_VERSION != ""
      StrCpy $ALL_USERS 1
      Call MultiUser.InstallMode.AllUsers
      return
    ${EndIf}
  ${EndIf}
  
  ReadRegStr $PREVIOUS_INSTALLDIR HKCU "${SMPLAYER_REG_KEY}" "Path"

  Call CheckPrevInstallDirExists

  ${If} $PREVIOUS_INSTALLDIR != ""
    ;Detect version
    ReadRegStr $PREVIOUS_VERSION HKCU "${SMPLAYER_REG_KEY}" "Version"
    ${If} $PREVIOUS_VERSION != ""
      StrCpy $ALL_USERS 0
      Call MultiUser.InstallMode.CurrentUser
      return
    ${EndIf}
  ${EndIf}
 
FunctionEnd

Function RemovePreviousVersion

  ${If} $MultiUser.InstallMode == "AllUsers"
    ReadRegStr $R0 HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString"
  ${ElseIf} $MultiUser.InstallMode == "CurrentUser"
    ReadRegStr $R0 HKCU "${SMPLAYER_UNINST_KEY}" "UninstallString"
  ${EndIf}
  StrCmp $R0 "" nouninst

  MessageBox MB_YESNO|MB_ICONEXCLAMATION $(SMPLAYER_INSTALLER_PREV_VERSION) IDNO nouninst

  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
  nouninst:

FunctionEnd

Function UninstallSMPlayer

  ;Delete directories recursively except for main directory
  ;Do not recursively delete $INSTDIR
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

  ;Delete registry keys & shortcuts
  ${If} $MultiUser.InstallMode == "AllUsers"
    SetShellVarContext all
    Delete "$DESKTOP\SMPlayer.lnk"

    Delete "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" 
    Delete "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url"
    Delete "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk"
    RMDir "$SMPROGRAMS\SMPlayer"

    DeleteRegKey HKLM "${SMPLAYER_REG_KEY}"
    DeleteRegKey HKLM "${SMPLAYER_UNINST_KEY}"
    DeleteRegKey HKCR "MPlayerFileVideo"
    DeleteRegKey HKLM "Software\Clients\Media\SMPlayer"
    DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"
  ${EndIf}
  ${If} $MultiUser.InstallMode == "CurrentUser"
    SetShellVarContext current
    Delete "$DESKTOP\SMPlayer.lnk"

    Delete "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" 
    Delete "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url"
    Delete "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk"
    RMDir "$SMPROGRAMS\SMPlayer"

    DeleteRegKey HKCU "${SMPLAYER_REG_KEY}"
    DeleteRegKey HKCU "${SMPLAYER_UNINST_KEY}"
  ${EndIf}

FunctionEnd

/************************************************************************************************
**************************************** Uninstaller ********************************************
************************************************************************************************/

;--------------------------------
;Uninstaller Variables

  Var un.REMOVE_ALL_USERS
  Var un.REMOVE_CURRENT_USER

;------------------------------------------------------------------------------------------------
;UnInstaller Sections

Section Uninstall

  ;Make sure SMPlayer is installed from where the uninstaller is being executed.
  IfFileExists $INSTDIR\smplayer.exe smplayer_installed
    MessageBox MB_YESNO $(SMPLAYER_NOT_INSTALLED) IDYES smplayer_installed
    Abort $(UNINSTALL_ABORTED)

  smplayer_installed:

  ExecWait '"$INSTDIR\smplayer.exe" -uninstall'

  ;Delete directories recursively except for main directory
  ;Do not recursively delete $INSTDIR
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

  ;Delete registry keys & shortcuts
  ${If} $un.REMOVE_ALL_USERS == 1
    SetShellVarContext all
    Delete "$DESKTOP\SMPlayer.lnk"

    Delete "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" 
    Delete "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url"
    Delete "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk"
    RMDir "$SMPROGRAMS\SMPlayer"

    DeleteRegKey HKLM "${SMPLAYER_REG_KEY}"
    DeleteRegKey HKLM "${SMPLAYER_UNINST_KEY}"
    DeleteRegKey HKCR "MPlayerFileVideo"
    DeleteRegKey HKLM "Software\Clients\Media\SMPlayer"
    DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"
  ${EndIf}
  ${If} $un.REMOVE_CURRENT_USER == 1
    SetShellVarContext current
    Delete "$DESKTOP\SMPlayer.lnk"

    Delete "$SMPROGRAMS\SMPlayer\SMPlayer.lnk" 
    Delete "$SMPROGRAMS\SMPlayer\SMPlayer on the Web.url"
    Delete "$SMPROGRAMS\SMPlayer\Uninstall SMPlayer.lnk"
    RMDir "$SMPROGRAMS\SMPlayer"

    DeleteRegKey HKCU "${SMPLAYER_REG_KEY}"
    DeleteRegKey HKCU "${SMPLAYER_UNINST_KEY}"
  ${EndIf}

  Delete "$INSTDIR\uninst.exe"
  RMDir "$INSTDIR"

SectionEnd

;------------------------------------------------------------------------------------------------
;UnInstaller Functions

Function un.onInit

  Call un.GetUserInfo
  Call un.ReadPreviousVersion

  ${If} $un.REMOVE_ALL_USERS == 1
  ${AndIf} $IS_ADMIN == 0
    MessageBox MB_OK|MB_ICONSTOP $(UNINSTALL_INSTALLED_ALL_USERS) /SD IDOK
    Abort
  ${EndIf}

  !insertmacro MULTIUSER_UNINIT

  !insertmacro MUI_UNGETLANGUAGE

FunctionEnd

Function un.GetUserInfo

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

Function un.ReadPreviousVersion

  ReadRegStr $R0 HKLM "${SMPLAYER_REG_KEY}" "Path"

  ${If} $R0 != ""
    ;Detect version
    ReadRegStr $R2 HKLM "${SMPLAYER_REG_KEY}" "Version"
    ${If} $R2 == ""
      StrCpy $R0 ""
    ${EndIf}
  ${EndIf}

  ReadRegStr $R1 HKCU "${SMPLAYER_REG_KEY}" "Path"
  
  ${If} $R1 != ""
    ;Detect version
    ReadRegStr $R2 HKCU "${SMPLAYER_REG_KEY}" "Version"
    ${If} $R2 == ""
      StrCpy $R1 ""
    ${EndIf}
  ${EndIf}

  ${If} $R1 == $INSTDIR
    Strcpy $un.REMOVE_CURRENT_USER 1
  ${EndIf}
  ${If} $R0 == $INSTDIR
    Strcpy $un.REMOVE_ALL_USERS 1
  ${EndIf}
  ${If} $un.REMOVE_CURRENT_USER != 1
  ${AndIf} $un.REMOVE_ALL_USERS != 1
    ${If} $R1 != ""
      Strcpy $un.REMOVE_CURRENT_USER 1
      ${If} $R0 == $R1
        Strcpy $un.REMOVE_ALL_USERS 1
      ${EndIf}
    ${Else}
      StrCpy $un.REMOVE_ALL_USERS = 1
    ${EndIf}
  ${EndIf}

FunctionEnd