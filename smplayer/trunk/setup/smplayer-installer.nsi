; Installer script for win32 SMPlayer
; Written by redxii (redxii@users.sourceforge.net)
; Tested/Developed with Unicode NSIS 2.45.1/2.46

!ifndef VER_MAJOR | VER_MINOR | VER_BUILD
  !error "Version information not defined (or incomplete). You must define: VER_MAJOR, VER_MINOR, VER_BUILD."
!endif

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

  !define SMPLAYER_APP_PATHS_KEY "Software\Microsoft\Windows\CurrentVersion\App Paths\smplayer.exe"
  !define SMPLAYER_REG_KEY "Software\SMPlayer"

  !define SMPLAYER_UNINST_EXE "uninst.exe"
  !define SMPLAYER_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMPlayer"

  ;Fallback versions
  ;These can be changed in the compiler, otherwise
  ;if not defined the values shown here will be used.
!ifndef DEFAULT_CODECS_VERSION
  !define DEFAULT_CODECS_VERSION "windows-essential-20071007"
!endif

!ifndef WITH_MPLAYER

  !ifndef DEFAULT_MPLAYER_VERSION
    !define DEFAULT_MPLAYER_VERSION "mplayer-svn-30369"
  !endif

!endif

  ;Version control
!ifndef VERSION_FILE_URL
  !define VERSION_FILE_URL "http://smplayer.sourceforge.net/mplayer-version-info"
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

  ;Default installation folder
  InstallDir "$PROGRAMFILES\SMPlayer"

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "${SMPLAYER_REG_KEY}" "Path"

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

  ShowInstDetails show
  ShowUnInstDetails show

;--------------------------------
;Variables

  Var Codec_Version
  Var Is_Admin
!ifndef WITH_MPLAYER
  Var MPlayer_Version
!endif
  Var Previous_Version
  Var Previous_Version_State
  Var Reinstall_Uninstall
  Var Reinstall_UninstallButton
  Var SMP_StartMenuFolder
  Var UserName

;--------------------------------
;Interface Settings

  ;Installer/Uninstaller icons
  !define MUI_ICON "smplayer-orange-installer.ico"
  !define MUI_UNICON "smplayer-orange-uninstaller.ico"

  ; Misc
  !define MUI_WELCOMEFINISHPAGE_BITMAP "smplayer-orange-wizard.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "smplayer-orange-wizard-un.bmp"
  !define MUI_ABORTWARNING

  ; License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS

  ; Components page
  !define MUI_COMPONENTSPAGE_SMALLDESC

  ; Finish page
  !define MUI_FINISHPAGE_LINK "http://smplayer.sourceforge.net"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://smplayer.sourceforge.net"
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  !define MUI_FINISHPAGE_RUN $INSTDIR\smplayer.exe
  !define MUI_FINISHPAGE_RUN_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME $INSTDIR\Release_notes.txt
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED

  ;Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT HKLM
  !define MUI_LANGDLL_REGISTRY_KEY "${SMPLAYER_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

  ;Memento Settings
  !define MEMENTO_REGISTRY_ROOT HKLM
  !define MEMENTO_REGISTRY_KEY "${SMPLAYER_REG_KEY}"

  ;Start Menu Settings
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "SMPlayer"
  !define MUI_STARTMENUPAGE_NODISABLE
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "${SMPLAYER_UNINST_KEY}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenu"

;--------------------------------
;Include Modern UI and functions

  !include MUI2.nsh
  !include FileFunc.nsh
  !include Memento.nsh
  !include nsDialogs.nsh
  !include Sections.nsh
  !include WinVer.nsh
  !include WordFunc.nsh

;--------------------------------
;Pages

  ;Install pages
  #Welcome
  !insertmacro MUI_PAGE_WELCOME
  #License
  !insertmacro MUI_PAGE_LICENSE "smplayer-build\Copying.txt"
  #Upgrade/Reinstall
  Page custom PageReinstall PageLeaveReinstall
  #Components
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageComponentsPre
  !insertmacro MUI_PAGE_COMPONENTS
  #Install Directory
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageDirectoryPre
  !insertmacro MUI_PAGE_DIRECTORY
  #Start Menu
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageStartMenuPre
  !insertmacro MUI_PAGE_STARTMENU "SMP_SMenu" $SMP_StartMenuFolder
  #Install
  !define MUI_PAGE_CUSTOMFUNCTION_SHOW PageInstfilesShow
  !insertmacro MUI_PAGE_INSTFILES
  #Finish
  !insertmacro MUI_PAGE_FINISH

  ;Uninstall pages
  !define MUI_PAGE_CUSTOMFUNCTION_PRE un.ConfirmPagePre
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !define MUI_PAGE_CUSTOMFUNCTION_PRE un.FinishPagePre
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Basque"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "TradChinese"

; Custom translations for setup

  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\english.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\basque.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\catalan.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\czech.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\danish.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\dutch.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\finnish.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\french.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\german.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\hebrew.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\hungarian.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\italian.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\japanese.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\korean.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\norwegian.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\polish.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\portuguese.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\russian.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\simpchinese.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\slovak.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\slovenian.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\spanish.nsh"
  !insertmacro LANGFILE_INCLUDE "svn\smplayer\setup\translations\tradchinese.nsh"

;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL
  ReserveFile "${NSISDIR}\Plugins\UserInfo.dll"

;--------------------------------
;Installer Sections

;--------------------------------
;Main SMPlayer files
Section SMPlayer SMPlayer

  SectionIn RO

  SetOutPath "$INSTDIR"
  File "smplayer-build\*"

  ;SMPlayer docs
  SetOutPath "$INSTDIR\docs"
  File /r "smplayer-build\docs\*.*"

  ;Qt imageformats
  SetOutPath "$INSTDIR\imageformats"
  File /r "smplayer-build\imageformats\*.*"

  ;SMPlayer key shortcuts
  SetOutPath "$INSTDIR\shortcuts"
  File /r "smplayer-build\shortcuts\*.*"

  SetOutPath "$PLUGINSDIR"
  File 7za.exe

  ;Initialize to 0 if don't exist (based on error flag)
  ClearErrors
  ReadRegDWORD $R0 HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer
  ${If} ${Errors}
    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x0
  ${EndIf}
  ClearErrors
  ReadRegDWORD $R0 HKLM "${SMPLAYER_REG_KEY}" Installed_Codecs
  ${If} ${Errors}
    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_Codecs 0x0
  ${EndIf}

SectionEnd

;--------------------------------
;Desktop shortcut
${MementoSection} "Desktop Shortcut" DesktopIcon

  SetOutPath "$INSTDIR"
  SetShellVarContext all
  CreateShortCut "$DESKTOP\SMPlayer.lnk" "$INSTDIR\smplayer.exe"

${MementoSectionEnd}

;--------------------------------
;Start menu shortcuts
${MementoSection} "Start Menu Shortcut" StartMenuIcon

  SetOutPath "$INSTDIR"
  SetShellVarContext all
  !insertmacro MUI_STARTMENU_WRITE_BEGIN SMP_SMenu
    CreateDirectory "$SMPROGRAMS\$SMP_StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$SMP_StartMenuFolder\SMPlayer.lnk" "$INSTDIR\smplayer.exe"
    WriteINIStr    "$SMPROGRAMS\$SMP_StartMenuFolder\SMPlayer on the Web.url" "InternetShortcut" "URL" "http://smplayer.sf.net"
    CreateShortCut "$SMPROGRAMS\$SMP_StartMenuFolder\Uninstall SMPlayer.lnk" "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  !insertmacro MUI_STARTMENU_WRITE_END

${MementoSectionEnd}

;--------------------------------
;MPlayer & MPlayer Codecs
SectionGroup /e "MPlayer Components"

!ifdef WITH_MPLAYER
  Section MPlayer MPlayer

    SectionIn RO

    SetOutPath "$INSTDIR\mplayer"
    File /r "smplayer-build\mplayer\*.*"

    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x1

  SectionEnd
!else ifndef WITH_MPLAYER
  Section MPlayer MPlayer

    SectionIn RO
    AddSize 16800

    Call GetVerInfo

    /* Read from version-info
    If it was unable to download, set version to that defined in the
    beginning of the script. */
    ${If} ${FileExists} "$PLUGINSDIR\version-info"
      ReadINIStr $MPLAYER_VERSION "$PLUGINSDIR\version-info" smplayer mplayer
    ${Else}
      StrCpy $MPLAYER_VERSION ${DEFAULT_MPLAYER_VERSION}
    ${EndIf}

    retry_mplayer:

    DetailPrint $(MPLAYER_IS_DOWNLOADING)
    inetc::get /timeout 30000 /resume "" /caption $(MPLAYER_IS_DOWNLOADING) /banner "Downloading $MPLAYER_VERSION.7z" \
    "http://downloads.sourceforge.net/smplayer/$MPLAYER_VERSION.7z?big_mirror=0" \
    "$PLUGINSDIR\$MPLAYER_VERSION.7z"
    Pop $R0
    StrCmp $R0 OK 0 check_mplayer

    nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$MPLAYER_VERSION.7z" -y -o"$PLUGINSDIR"'

    CreateDirectory "$INSTDIR\mplayer"
    CopyFiles /SILENT "$PLUGINSDIR\$MPLAYER_VERSION\*" "$INSTDIR\mplayer"

    check_mplayer:
    ;This label does not necessarily mean there was a download error, so check first
    ${If} $R0 != "OK"
      DetailPrint $(MPLAYER_DL_FAILED)
    ${EndIf}

    IfFileExists "$INSTDIR\mplayer\mplayer.exe" mplayerInstSuccess mplayerInstFailed
      mplayerInstSuccess:
        WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x1
        Goto done
      mplayerInstFailed:
        MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION $(MPLAYER_DL_RETRY) /SD IDCANCEL IDRETRY retry_mplayer
        Abort $(MPLAYER_INST_FAILED)

    done:

  SectionEnd
!endif

  Section /o "Binary Codecs" Codecs

    AddSize 22300

    Call GetVerInfo

    /* Read from version-info
    If it was unable to download, set version to that defined in the
    beginning of the script. */
    ${If} ${FileExists} "$PLUGINSDIR\version-info"
      ReadINIStr $Codec_Version "$PLUGINSDIR\version-info" smplayer mplayercodecs
    ${Else}
      StrCpy $Codec_Version ${DEFAULT_CODECS_VERSION}
    ${EndIf}

    retry_codecs:

    DetailPrint $(CODECS_IS_DOWNLOADING)
    inetc::get /timeout 30000 /resume "" /caption $(CODECS_IS_DOWNLOADING) /banner "Downloading $Codec_Version.zip" \
    "http://www.mplayerhq.hu/MPlayer/releases/codecs/$Codec_Version.zip" \
    "$PLUGINSDIR\$Codec_Version.zip" /end
    Pop $R0
    StrCmp $R0 OK 0 check_codecs

    nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\$Codec_Version.zip" -y -o"$PLUGINSDIR"'

    CreateDirectory "$INSTDIR\mplayer\codecs"
    CopyFiles /SILENT "$PLUGINSDIR\$Codec_Version\*" "$INSTDIR\mplayer\codecs"

    check_codecs:
    ;This label does not necessarily mean there was a download error, so check first
    ${If} $R0 != "OK"
      DetailPrint $(CODECS_DL_FAILED)
    ${EndIf}

    IfFileExists "$INSTDIR\mplayer\codecs\*.dll" codecsInstSuccess codecsInstFailed
      codecsInstSuccess:
        WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_Codecs 0x1
        Goto done
      codecsInstFailed:
        MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION $(CODECS_DL_RETRY) /SD IDCANCEL IDRETRY retry_codecs
        DetailPrint $(CODECS_INST_FAILED)
        WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_Codecs 0x0
        Sleep 5000

    done:

	SectionEnd

SectionGroupEnd

;--------------------------------
;Icon themes
${MementoSection} "Icon Themes" Themes

  SetOutPath "$INSTDIR\themes"
  File /r "smplayer-build\themes\*.*"

${MementoSectionEnd}

;--------------------------------
;Translations
${MementoSection} Translations Translations

  SetOutPath "$INSTDIR\translations"
  File /r "smplayer-build\translations\*.*"

${MementoSectionEnd}

;--------------------------------
;Install/Uninstall information
Section -Post

  ;Uninstall file
  WriteUninstaller "$INSTDIR\${SMPLAYER_UNINST_EXE}"

  ;Store installed path & version
  WriteRegStr HKLM "${SMPLAYER_REG_KEY}" "Path" "$INSTDIR"
  WriteRegStr HKLM "${SMPLAYER_REG_KEY}" "Version" "${SMPLAYER_VERSION}"

  ;Allows user to use 'start smplayer.exe'
  WriteRegStr HKLM "${SMPLAYER_APP_PATHS_KEY}" "" "$INSTDIR\smplayer.exe"

  ;Registry entries needed for Default Programs in Vista & later
  ${If} ${AtLeastWinVista}
    Call RegisterDefaultPrograms
  ${EndIf}

  ;Registry Uninstall information
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayIcon" "$INSTDIR\smplayer.exe"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayVersion" "${SMPLAYER_VERSION}"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "HelpLink" "http://smplayer.berlios.de/forum"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "Publisher" "Ricardo Villalba"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString" "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "URLInfoAbout" "http://smplayer.sf.net"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "URLUpdateInfo" "http://smplayer.sf.net"
  WriteRegDWORD HKLM "${SMPLAYER_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD HKLM "${SMPLAYER_UNINST_KEY}" "NoRepair" "1"

SectionEnd

${MementoSectionDone}

;--------------------------------
;Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SMPlayer} "SMPlayer, shared libraries, and documentation."
  !insertmacro MUI_DESCRIPTION_TEXT ${DesktopIcon} "Creates a shortcut on the desktop."
  !insertmacro MUI_DESCRIPTION_TEXT ${StartMenuIcon} "Creates start menu shortcuts."
!ifdef WITH_MPLAYER
  !insertmacro MUI_DESCRIPTION_TEXT ${MPlayer} "The engine behind SMPlayer, required for playback."
!else ifndef WITH_MPLAYER
  !insertmacro MUI_DESCRIPTION_TEXT ${MPlayer} "Downloads/installs MPlayer; requires an active internet connection. Required for playback."
!endif
  !insertmacro MUI_DESCRIPTION_TEXT ${Codecs} "Downloads/installs optional binary codecs for MPlayer; requires an active internet connection."
  !insertmacro MUI_DESCRIPTION_TEXT ${Themes} "Additional icon themes for SMPlayer."
  !insertmacro MUI_DESCRIPTION_TEXT ${Translations} "Translations for the SMPlayer interface && help into 30+ additional languages."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Macros

!macro MacroAllExtensions _action
  !insertmacro ${_action} ".3gp"
  !insertmacro ${_action} ".ac3"
  !insertmacro ${_action} ".ape"
  !insertmacro ${_action} ".asf"
  !insertmacro ${_action} ".avi"
  !insertmacro ${_action} ".bin"
  !insertmacro ${_action} ".dat"
  !insertmacro ${_action} ".divx"
  !insertmacro ${_action} ".dv"
  !insertmacro ${_action} ".dvr-ms"
  !insertmacro ${_action} ".f4v"
  !insertmacro ${_action} ".flac"
  !insertmacro ${_action} ".flv"
  !insertmacro ${_action} ".hdmov"
  !insertmacro ${_action} ".iso"
  !insertmacro ${_action} ".m1v"
  !insertmacro ${_action} ".m2t"
  !insertmacro ${_action} ".m2ts"
  !insertmacro ${_action} ".m2v"
  !insertmacro ${_action} ".m3u"
  !insertmacro ${_action} ".m3u8"
  !insertmacro ${_action} ".m4v"
  !insertmacro ${_action} ".mkv"
  !insertmacro ${_action} ".mov"
  !insertmacro ${_action} ".mp3"
  !insertmacro ${_action} ".mp4"
  !insertmacro ${_action} ".mpeg"
  !insertmacro ${_action} ".mpg"
  !insertmacro ${_action} ".mpv"
  !insertmacro ${_action} ".mqv"
  !insertmacro ${_action} ".nsv"
  !insertmacro ${_action} ".ogg"
  !insertmacro ${_action} ".ogm"
  !insertmacro ${_action} ".ogv"
  !insertmacro ${_action} ".pls"
  !insertmacro ${_action} ".ra"
  !insertmacro ${_action} ".ram"
  !insertmacro ${_action} ".rec"
  !insertmacro ${_action} ".rm"
  !insertmacro ${_action} ".rmvb"
  !insertmacro ${_action} ".swf"
  !insertmacro ${_action} ".ts"
  !insertmacro ${_action} ".vcd"
  !insertmacro ${_action} ".vfw"
  !insertmacro ${_action} ".vob"
  !insertmacro ${_action} ".wav"
  !insertmacro ${_action} ".wma"
  !insertmacro ${_action} ".wmv"
!macroend

!macro WriteRegStrSupportedTypes EXT
  WriteRegStr HKLM  "${SMPLAYER_REG_KEY}\Capabilities\FileAssociations" ${EXT} "MPlayerFileVideo"
!macroend

!macro MacroRemoveSMPlayer
  ;Delete desktop and start menu shortcuts
  SetDetailsPrint textonly
  DetailPrint "Deleting Shortcuts..."
  SetDetailsPrint listonly

  SetShellVarContext all
  Delete "$DESKTOP\SMPlayer.lnk"
  Delete "$SMPROGRAMS\$SMP_StartMenuFolder\SMPlayer.lnk" 
  Delete "$SMPROGRAMS\$SMP_StartMenuFolder\SMPlayer on the Web.url"
  Delete "$SMPROGRAMS\$SMP_StartMenuFolder\Uninstall SMPlayer.lnk"
  RMDir "$SMPROGRAMS\$SMP_StartMenuFolder"

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

  ;Delete registry keys
  SetDetailsPrint textonly
  DetailPrint "Deleting Registry Keys..."
  SetDetailsPrint listonly

  DeleteRegKey HKLM "${SMPLAYER_REG_KEY}"
  DeleteRegKey HKLM "${SMPLAYER_APP_PATHS_KEY}"
  DeleteRegKey HKLM "${SMPLAYER_UNINST_KEY}"
  DeleteRegKey HKCR "MPlayerFileVideo"
  DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"

  SetDetailsPrint both
!macroend

;--------------------------------
;Installer functions

Function .onInit

  ;Check if setup is already running
  System::Call 'kernel32::CreateMutexW(i 0, i 0, t "MPlayerSMPlayer") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION $(SMPLAYER_INSTALLER_IS_RUNNING)
    Abort

  ;Check for admin (mimic old Inno Setup behavior)
  Call CheckUserRights

  ${If} $Is_Admin == 0
    MessageBox MB_OK|MB_ICONSTOP $(SMPLAYER_INSTALLER_NO_ADMIN)
    Abort
  ${EndIf}

  ;Setup language selection
  !insertmacro MUI_LANGDLL_DISPLAY

  Call CheckPreviousVersion

  Call LoadPreviousSettings

FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}

FunctionEnd

Function .onInstFailed

  !insertmacro MacroRemoveSMPlayer

  Delete "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  RMDir "$INSTDIR"

FunctionEnd

Function .onSelChange

  SectionGetFlags ${Codecs} $R0
  ${If} $R0 != $R1
    StrCpy $R1 $R0
    IntOp $R0 $R0 & ${SF_SELECTED}
  ${If} $R0 == ${SF_SELECTED}
    MessageBox MB_OK|MB_ICONINFORMATION $(MPLAYER_CODEC_INFORMATION)
  ${EndIf}
  ${EndIf}

FunctionEnd

Function CheckPreviousVersion

  ReadRegStr $Previous_Version HKLM "${SMPLAYER_REG_KEY}" "Version"

  /* $Previous_Version_State Assignments:
  $Previous_Version_State=0  This installer is the same version as the installed copy
  $Previous_Version_State=1  A newer version than this installer is already installed
  $Previous_Version_State=2  An older version than this installer is already installed */
  ${VersionCompare} $Previous_Version ${SMPLAYER_VERSION} $Previous_Version_State

FunctionEnd

Function CheckUserRights

  ClearErrors
  UserInfo::GetName
  ${If} ${Errors}
    StrCpy $Is_Admin 1
    Return
  ${EndIf}

  Pop $UserName
  UserInfo::GetAccountType
  Pop $R0
  ${Switch} $R0
    ${Case} "Admin"
    ${Case} "Power"
      StrCpy $Is_Admin 1
      ${Break}
    ${Default}
      StrCpy $Is_Admin 0
      ${Break}
  ${EndSwitch}

FunctionEnd

Function GetVerInfo

  IfFileExists "$PLUGINSDIR\version-info" end_dl_ver_info 0
    DetailPrint $(VERINFO_IS_DOWNLOADING)
    inetc::get /timeout 30000 /resume "" /silent ${VERSION_FILE_URL} "$PLUGINSDIR\version-info" /end
    Pop $R0
    StrCmp $R0 OK +2
      DetailPrint $(VERINFO_DL_FAILED)

  end_dl_ver_info:

FunctionEnd

Function LoadPreviousSettings

  ;MPlayer codecs section doesn't use Memento so we need to restore it manually
  ReadRegDWORD $R0 HKLM "${SMPLAYER_REG_KEY}" "Installed_Codecs"
  ${If} $R0 == 1
    !insertmacro SelectSection ${Codecs}
  ${EndIf}

  ;Gets start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMP_StartMenuFolder

  ${MementoSectionRestore}

FunctionEnd

Function PageReinstall

  ${If} $Previous_Version == ""
    Abort
  ${EndIf}

  nsDialogs::Create /NOUNLOAD 1018
  Pop $0

  ${If} $Previous_Version_State == 2

    !insertmacro MUI_HEADER_TEXT $(REINSTALL_HEADER_TEXT) $(REINSTALL_HEADER_SUBTEXT)
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 $(REINSTALL_OLDVER_DESCRIPTION)
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 $(REINSTALL_OLDVER_UPGRADE)
    Pop $Reinstall_UninstallButton
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 $(REINSTALL_CHGSETTINGS)
    Pop $R0

    ${If} $Reinstall_Uninstall == ""
      StrCpy $Reinstall_Uninstall 1
    ${EndIf}

  ${ElseIf} $Previous_Version_State == 1

    !insertmacro MUI_HEADER_TEXT $(REINSTALL_HEADER_TEXT) $(REINSTALL_HEADER_SUBTEXT)
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 $(REINSTALL_NEWVER_DESCRIPTION)
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 $(REINSTALL_NEWVER_DOWNGRADE)
    Pop $Reinstall_UninstallButton
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 $(REINSTALL_CHGSETTINGS)
    Pop $R0

    ${If} $Reinstall_Uninstall == ""
      StrCpy $Reinstall_Uninstall 1
    ${EndIf}

  ${ElseIf} $Previous_Version_State == 0

    !insertmacro MUI_HEADER_TEXT $(REINSTALL_HEADER_TEXT) $(REINSTALL_HEADER_SUBTEXT_MAINT)
    nsDialogs::CreateItem /NOUNLOAD STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 0 100% 40 $(REINSTALL_SAMEVER_DESCRIPTION)
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_VCENTER}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${WS_GROUP}|${WS_TABSTOP} 0 10 55 100% 30 $(REINSTALL_SAMEVER_ADDREMREINST)
    Pop $R0
    nsDialogs::CreateItem /NOUNLOAD BUTTON ${BS_AUTORADIOBUTTON}|${BS_TOP}|${BS_MULTILINE}|${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 10 85 100% 50 $(REINSTALL_SAMEVER_UNINSTSMP)
    Pop $Reinstall_UninstallButton

    ${If} $Reinstall_Uninstall == ""
      StrCpy $Reinstall_Uninstall 2
    ${EndIf}

  ${Else}

    MessageBox MB_ICONSTOP $(REINSTALL_UNKNOWN_VALUE) /SD IDOK
    Abort

  ${EndIf}

  ${If} $Reinstall_Uninstall == 1
    SendMessage $Reinstall_UninstallButton ${BM_SETCHECK} 1 0
  ${Else}
    SendMessage $R0 ${BM_SETCHECK} 1 0
  ${EndIf}

  nsDialogs::Show

FunctionEnd

Function PageComponentsPre

  ${If} $Reinstall_Uninstall == 1
    Abort
  ${EndIf}

FunctionEnd

Function PageDirectoryPre

  ${If} $Reinstall_Uninstall == 1
    Abort
  ${EndIf}

FunctionEnd

Function PageStartMenuPre

  ${If} $Reinstall_Uninstall == 1
    Abort
  ${EndIf}

  ${IfNot} ${SectionIsSelected} ${StartMenuIcon}
    Abort
  ${EndIf}

FunctionEnd

Function PageInstfilesShow

  ${If} $Reinstall_Uninstall != ""
    Call RunUninstaller
    BringToFront
  ${EndIf}

FunctionEnd

Function PageLeaveReinstall

  SendMessage $Reinstall_UninstallButton ${BM_GETCHECK} 0 0 $R0
  ${If} $R0 == 1
    ; Option to uninstall old version selected
    StrCpy $Reinstall_Uninstall 1
  ${Else}
    ; Custom up/downgrade or add/remove/reinstall
    StrCpy $Reinstall_Uninstall 2
  ${EndIf}

  ${If} $Reinstall_Uninstall == 1
    ${If} $Previous_Version_State == 0
      Call RunUninstaller
      Quit
    ${Else}
  ${EndIf}

  ${EndIf}

FunctionEnd

Function RegisterDefaultPrograms

  WriteRegStr HKCR "MPlayerFileVideo\DefaultIcon" "" '"$INSTDIR\smplayer.exe",1'
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue" "" "Enqueue in SMPlayer"
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue\command" "" '"$INSTDIR\smplayer.exe" -add-to-playlist "%1"'
  WriteRegStr HKCR "MPlayerFileVideo\shell\open" "FriendlyAppName" "SMPlayer Media Player"
  WriteRegStr HKCR "MPlayerFileVideo\shell\open\command" "" '"$INSTDIR\smplayer.exe" "%1"'

  ;Modify the list of extensions added in the MacroAllExtensions macro
  WriteRegStr HKLM "${SMPLAYER_REG_KEY}\Capabilities" "ApplicationDescription" $(APPLICATION_DESCRIPTION)
  WriteRegStr HKLM "${SMPLAYER_REG_KEY}\Capabilities" "ApplicationName" "SMPlayer"
  WriteRegStr HKLM "Software\RegisteredApplications" "SMPlayer" "${SMPLAYER_REG_KEY}\Capabilities"
  !insertmacro MacroAllExtensions WriteRegStrSupportedTypes

FunctionEnd

Function RunUninstaller

  ReadRegStr $R1 HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString"

  ${If} $R1 == ""
    Return
  ${EndIf}

  ;Run uninstaller
  HideWindow

  ClearErrors

  ${If} $Previous_Version_State == 0
  ${AndIf} $Reinstall_Uninstall == 1
    ExecWait '$R1 _?=$INSTDIR'
  ${Else}
    ExecWait '$R1 /frominstall _?=$INSTDIR'
  ${EndIf}

  IfErrors no_remove_uninstaller

  IfFileExists "$INSTDIR\${SMPLAYER_UNINST_EXE}" 0 no_remove_uninstaller

    Delete "$R1"
    RMDir $INSTDIR

 no_remove_uninstaller:

FunctionEnd

/*************************************** Uninstaller *******************************************/

Section Uninstall

  ;Make sure SMPlayer is installed from where the uninstaller is being executed.
  IfFileExists $INSTDIR\smplayer.exe smplayer_installed
    MessageBox MB_YESNO $(SMPLAYER_NOT_INSTALLED) /SD IDNO IDYES smplayer_installed
    Abort $(UNINSTALL_ABORTED)

  smplayer_installed:

  SetDetailsPrint textonly
  DetailPrint "Restoring file associations..."
  SetDetailsPrint listonly

  ;Don't restore file associations if reinstalling
  ${un.GetParameters} $R0
  ${un.GetOptions} $R0 "/frominstall" $R1

  IfErrors 0 +2
  ExecWait '"$INSTDIR\smplayer.exe" -uninstall'

  !insertmacro MacroRemoveSMPlayer

  Delete "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  RMDir "$INSTDIR"

SectionEnd

;--------------------------------
;Required functions

!insertmacro un.GetParameters
!insertmacro un.GetOptions

;--------------------------------
;Uninstaller functions

Function un.onInit

  ;Check for admin (mimic old Inno Setup behavior)
  Call un.CheckUserRights

  ${If} $Is_Admin == 0
    MessageBox MB_OK|MB_ICONSTOP $(UNINSTALL_NO_ADMIN)
    Abort
  ${EndIf}

  ;Gets start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMP_StartMenuFolder

  ;Get the stored language preference
  !insertmacro MUI_UNGETLANGUAGE

FunctionEnd

Function un.CheckUserRights

  ClearErrors
  UserInfo::GetName
  ${If} ${Errors}
    StrCpy $Is_Admin 1
    Return
  ${EndIf}

  Pop $UserName
  UserInfo::GetAccountType
  Pop $R0
  ${Switch} $R0
    ${Case} "Admin"
    ${Case} "Power"
      StrCpy $Is_Admin 1
      ${Break}
    ${Default}
      StrCpy $Is_Admin 0
      ${Break}
  ${EndSwitch}

FunctionEnd

Function un.ConfirmPagePre

  ${un.GetParameters} $R0

  ${un.GetOptions} $R0 "/frominstall" $R1
  ${Unless} ${Errors}
    Abort
  ${EndUnless}

FunctionEnd

Function un.FinishPagePre

  ${un.GetParameters} $R0

  ${un.GetOptions} $R0 "/frominstall" $R1
  ${Unless} ${Errors}
    Abort
  ${EndUnless}

FunctionEnd