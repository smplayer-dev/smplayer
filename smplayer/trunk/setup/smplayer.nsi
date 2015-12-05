;Installer script for win32/win64 SMPlayer
;Written by redxii (redxii@users.sourceforge.net)
;Tested/Developed with Unicode NSIS 2.46.5

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
  !addincludedir .

;--------------------------------
;Defines

!ifdef VER_REVISION
  !define SMPLAYER_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
  !define SMPLAYER_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
!else ifndef VER_REVISION
  !define SMPLAYER_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
  !define SMPLAYER_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.0"
!endif

!ifdef WIN64
  !define SMPLAYER_BUILD_DIR "smplayer-build64"
!else
  !define SMPLAYER_BUILD_DIR "smplayer-build"
!endif

  !define SMPLAYER_REG_KEY "Software\SMPlayer"
  !define SMPLAYER_APP_PATHS_KEY "Software\Microsoft\Windows\CurrentVersion\App Paths\smplayer.exe"
  !define SMPLAYER_DEF_PROGS_KEY "Software\Clients\Media\SMPlayer"

  !define SMPLAYER_UNINST_EXE "uninst.exe"
  !define SMPLAYER_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMPlayer"

  !define MPV_VERSION "20150923"
!ifdef WIN64
  !define MPV_FILENAME "mpv-x86_64-${MPV_VERSION}.7z"
!else
  !define MPV_FILENAME "mpv-i686-${MPV_VERSION}.7z"
!endif

  !define INSTALLER_VERSION "1"

;--------------------------------
;General

  ;Name and file
  Name "SMPlayer ${SMPLAYER_VERSION}"
  BrandingText "SMPlayer for Windows v${SMPLAYER_VERSION}"
!ifdef WIN64
  OutFile "output\smplayer-${SMPLAYER_VERSION}-x64.exe"
!else
  OutFile "output\smplayer-${SMPLAYER_VERSION}-win32.exe"
!endif

  ;Version tab properties
  VIProductVersion "${SMPLAYER_PRODUCT_VERSION}"
  VIAddVersionKey "ProductName" "SMPlayer"
  VIAddVersionKey "ProductVersion" "${SMPLAYER_VERSION}"
  VIAddVersionKey "FileVersion" "${SMPLAYER_VERSION}"
  VIAddVersionKey "LegalCopyright" ""
!ifdef WIN64
  VIAddVersionKey "FileDescription" "SMPlayer Installer (64-bit)"
!else
  VIAddVersionKey "FileDescription" "SMPlayer Installer (32-bit)"
!endif

  ;Default installation folder
!ifdef WIN64
  InstallDir "$PROGRAMFILES64\SMPlayer"
!else
  InstallDir "$PROGRAMFILES\SMPlayer"
!endif

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "${SMPLAYER_REG_KEY}" "Path"

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

  ShowInstDetails show
  ShowUnInstDetails show

;--------------------------------
;Variables

  Var Dialog_Reinstall
  Var Inst_Type
  Var Previous_Version
  Var Previous_Version_State
  Var ReadReg_Installed_MPV
  Var Reinstall_ChgSettings
  Var Reinstall_ChgSettings_State
  Var Reinstall_Message
  Var Reinstall_OverwriteButton
  Var Reinstall_OverwriteButton_State
  Var Reinstall_RemoveSettings
  Var Reinstall_RemoveSettings_State
  Var Reinstall_Uninstall
  Var Reinstall_UninstallButton
  Var Reinstall_UninstallButton_State
!ifndef WIN64
  Var Restore_Codecs
!endif
  Var Restore_MPV
  Var Restore_SMTube
  Var SecRadioButton
  Var SMPlayer_Path
  Var SMPlayer_UnStrPath
  Var SMPlayer_StartMenuFolder

  Var YTDL_Version_Local
  Var YTDL_Version_Remote
  Var YTDL_Version_Remote_File
  Var YTDL_Previous_Version_State

  Var MPV_Version

;--------------------------------
;Interface Settings

  ;Installer/Uninstaller icons
  !define MUI_ICON "smplayer-orange-installer.ico"
  !define MUI_UNICON "smplayer-orange-uninstaller.ico"

  ;Misc
  !define MUI_WELCOMEFINISHPAGE_BITMAP "smplayer-orange-wizard.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "smplayer-orange-wizard-un.bmp"
  !define MUI_ABORTWARNING

  ;Welcome page
  !define MUI_WELCOMEPAGE_TITLE $(WelcomePage_Title)
  !define MUI_WELCOMEPAGE_TEXT $(WelcomePage_Text)

  ;License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS

  ;Components page
  !define MUI_COMPONENTSPAGE_SMALLDESC

  ;Finish page
  !define MUI_FINISHPAGE_LINK "http://www.smplayer.info"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://www.smplayer.info"
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
  !include x64.nsh

;--------------------------------
;Pages

  !define MUI_CUSTOMFUNCTION_GUIINIT newGuiInit

  ;Install pages
  #Welcome
  !insertmacro MUI_PAGE_WELCOME

  #License
  !insertmacro MUI_PAGE_LICENSE "license.txt"

  #Upgrade/Reinstall
  Page custom PageReinstall PageReinstallLeave

  #Components
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageComponentsPre
  !insertmacro MUI_PAGE_COMPONENTS

  #Install Directory
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageDirectoryPre
  !insertmacro MUI_PAGE_DIRECTORY

  #Start Menu
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageStartMenuPre
  !insertmacro MUI_PAGE_STARTMENU "SMP_SMenu" $SMPlayer_StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  ;Uninstall pages
  !define MUI_PAGE_CUSTOMFUNCTION_PRE un.ConfirmPagePre
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Basque"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Croatian"
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
  !insertmacro MUI_LANGUAGE "Malay"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Galician"
  !insertmacro MUI_LANGUAGE "Indonesian"
  !insertmacro MUI_LANGUAGE "Turkish"

;Custom translations for setup

  !insertmacro LANGFILE_INCLUDE "translations\english.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\albanian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\arabic.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\basque.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\bulgarian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\catalan.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\croatian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\czech.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\danish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\dutch.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\finnish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\french.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\german.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hebrew.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hungarian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\italian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\japanese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\korean.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\malay.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\norwegian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\polish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\portuguese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\portuguesebrazil.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\russian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\serbian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\simpchinese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovak.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovenian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\spanish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\thai.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\tradchinese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\ukrainian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\galician.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\indonesian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\turkish.nsh"

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
Section $(Section_SMPlayer) SecSMPlayer

  SectionIn RO

  ${If} $Reinstall_Uninstall == 1

    ${If} $Reinstall_UninstallButton_State == 1
      Exec '"$SMPlayer_UnStrPath" /X'
      Quit
    ${ElseIf} $Reinstall_OverwriteButton_State == 1

!ifndef WIN64
      Call Backup_Codecs
!endif
      Call Backup_MPV
      Call Backup_SMTube

      ${If} "$INSTDIR" == "$SMPlayer_Path"
        ExecWait '"$SMPlayer_UnStrPath" /S /R _?=$SMPlayer_Path'
      ${Else}
        ExecWait '"$SMPlayer_UnStrPath" /S /R'
      ${EndIf}

      Sleep 2500

    ${EndIf}

  ${EndIf}

  SetOutPath "$INSTDIR"
  File "${SMPLAYER_BUILD_DIR}\*"

  ;SMPlayer docs
  SetOutPath "$INSTDIR\docs"
  File /r "${SMPLAYER_BUILD_DIR}\docs\*.*"

  ;Qt imageformats
  SetOutPath "$INSTDIR\imageformats"
  File /r "${SMPLAYER_BUILD_DIR}\imageformats\*.*"

  ;Open fonts
  SetOutPath "$INSTDIR\open-fonts"
  File /r "${SMPLAYER_BUILD_DIR}\open-fonts\*.*"

  ;Qt platforms (Qt 5+)
!ifdef QT5
  SetOutPath "$INSTDIR\platforms"
  File /r "${SMPLAYER_BUILD_DIR}\platforms\*.*"
!endif

  ;SMPlayer key shortcuts
  SetOutPath "$INSTDIR\shortcuts"
  File /r "${SMPLAYER_BUILD_DIR}\shortcuts\*.*"

  SetOutPath "$PLUGINSDIR"
  File 7za.exe

  ;Initialize to 0 if don't exist (based on error flag)
  ReadRegDWORD $R0 HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer
  ${If} ${Errors}
    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x0
  ${EndIf}

  ReadRegDWORD $R0 HKLM "${SMPLAYER_REG_KEY}" Installed_MPV
  ${If} ${Errors}
    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPV 0x0
  ${EndIf}

SectionEnd

;--------------------------------
;Shortcuts
SectionGroup $(ShortcutGroupTitle)

  ${MementoSection} $(Section_DesktopShortcut) SecDesktopShortcut

    SetOutPath "$INSTDIR"
    CreateShortCut "$DESKTOP\SMPlayer.lnk" "$INSTDIR\smplayer.exe"

  ${MementoSectionEnd}

  ${MementoSection} $(Section_StartMenu) SecStartMenuShortcut

    SetOutPath "$INSTDIR"
    !insertmacro MUI_STARTMENU_WRITE_BEGIN SMP_SMenu
      CreateDirectory "$SMPROGRAMS\$SMPlayer_StartMenuFolder"
      CreateShortCut "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMPlayer.lnk" "$INSTDIR\smplayer.exe"
      ${If} $Restore_SMTube == 1
        CreateShortCut "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMTube.lnk" "$INSTDIR\smtube.exe"
      ${EndIf}
      WriteINIStr    "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMPlayer on the Web.url" "InternetShortcut" "URL" "http://www.smplayer.info"
      CreateShortCut "$SMPROGRAMS\$SMPlayer_StartMenuFolder\Uninstall SMPlayer.lnk" "$INSTDIR\${SMPLAYER_UNINST_EXE}"
    !insertmacro MUI_STARTMENU_WRITE_END

  ${MementoSectionEnd}

SectionGroupEnd

;--------------------------------
;MPlayer & MPV
SectionGroup $(MPlayerMPVGroupTitle)

  Section "MPlayer" SecMPlayer

    SetOutPath "$INSTDIR\mplayer"
    File /r /x mplayer.exe /x mencoder.exe /x mplayer64.exe /x mencoder64.exe /x *.exe.debug /x gdb.exe /x gdb64.exe /x vfw2menc.exe /x buildinfo /x buildinfo64 /x buildinfo-mencoder-32 /x buildinfo-mencoder-debug-32 /x buildinfo-mplayer-32 /x buildinfo-mplayer-debug-32 /x buildinfo-mencoder-64 /x buildinfo-mencoder-debug-64 /x buildinfo-mplayer-64 /x buildinfo-mplayer-debug-64 "${SMPLAYER_BUILD_DIR}\mplayer\*.*"
!ifdef WIN64
    File /oname=mplayer.exe "${SMPLAYER_BUILD_DIR}\mplayer\mplayer64.exe"
!else
    File "${SMPLAYER_BUILD_DIR}\mplayer\mplayer.exe"
!endif

    WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPlayer 0x1

  SectionEnd

  Section /o "MPV" SecMPV

    AddSize 30000

    ${If} $Restore_MPV == 1
      DetailPrint $(Info_MPV_Restore)
      CopyFiles /SILENT "$PLUGINSDIR\mpvbak\*" "$INSTDIR\mplayer"
      Goto check_mpv
    ${ElseIf} ${FileExists} "$EXEDIR\${MPV_FILENAME}"
      CopyFiles /SILENT "$EXEDIR\${MPV_FILENAME}" "$PLUGINSDIR\mpv.7z"
      Goto extract_mpv
    ${EndIf}

    retry_mpv_dl:

    DetailPrint $(MPV_DL_Msg)
!ifdef USE_INETC
    inetc::get /CONNECTTIMEOUT 30000 /RESUME "" /BANNER $(MPV_DL_Msg) /CAPTION $(MPV_DL_Msg) \
    "http://downloads.smplayer.info/mpv/${MPV_FILENAME}" \
    "$PLUGINSDIR\mpv.7z" /END
    Pop $R0
    StrCmp $R0 OK +4 0
!else
    NSISdl::download /TIMEOUT=30000 \
    "http://downloads.smplayer.info/mpv/${MPV_FILENAME}" \
    "$PLUGINSDIR\mpv.7z" /END
    Pop $R0
    StrCmp $R0 "success" +4 0
!endif
      DetailPrint $(MPV_DL_Failed)
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION $(MPV_DL_Retry) /SD IDCANCEL IDRETRY retry_mpv_dl
      Goto check_mpv

    extract_mpv:

    DetailPrint $(Info_Files_Extract)
    CreateDirectory "$INSTDIR\mplayer"
    nsExec::Exec '"$PLUGINSDIR\7za.exe" x "$PLUGINSDIR\mpv.7z" -y -o"$INSTDIR\mplayer"'

    check_mpv:

    IfFileExists "$INSTDIR\mplayer\mpv*.exe" 0 mpvInstFailed
        WriteRegDWORD HKLM "${SMPLAYER_REG_KEY}" Installed_MPV 0x1
        WriteRegStr   HKLM "${SMPLAYER_REG_KEY}" "MPV_Version" "${MPV_VERSION}"
        Goto dl_youtube-dl
      mpvInstFailed:
        Abort $(MPV_Inst_Failed)

    dl_youtube-dl:

    NSISdl::download_quiet /TIMEOUT=30000 \
    "http://yt-dl.org/latest/version" \
    "$PLUGINSDIR\version" /END

    ClearErrors
    FileOpen $YTDL_Version_Remote_File "$PLUGINSDIR\version" r
    IfErrors YTDL
    FileRead $YTDL_Version_Remote_File $YTDL_Version_Remote
    FileClose $YTDL_Version_Remote_File

    ClearErrors
    ${GetFileVersion} "$INSTDIR\mplayer\youtube-dl.exe" $YTDL_Version_Local
    IfErrors YTDL

    ${VersionCompare} $YTDL_Version_Remote $YTDL_Version_Local $YTDL_Previous_Version_State
    ${Unless} $YTDL_Previous_Version_State == 1
      Goto skip_ytdl
    ${EndIf}

    YTDL:
    NSISdl::download /TIMEOUT=30000 \
    "http://yt-dl.org/latest/youtube-dl.exe" \
    "$INSTDIR\mplayer\youtube-dl.exe" /END
    Pop $R0
    StrCmp $R0 "success" +3 0
      DetailPrint $(YTDL_DL_Failed)
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION $(YTDL_DL_Retry) /SD IDCANCEL IDRETRY dl_youtube-dl

    skip_ytdl:

  SectionEnd

SectionGroupEnd

;--------------------------------
;Icon themes
${MementoSection} $(Section_IconThemes) SecThemes

  SetOutPath "$INSTDIR\themes"
  File /r "${SMPLAYER_BUILD_DIR}\themes\*.*"

${MementoSectionEnd}

;--------------------------------
;Translations
${MementoSection} $(Section_Translations) SecTranslations

  SetOutPath "$INSTDIR\translations"
  File /r "${SMPLAYER_BUILD_DIR}\translations\*.*"

${MementoSectionEnd}

Section /o $(Reinstall_Msg5) SecResetSettings

    NsExec::Exec '"$INSTDIR\smplayer.exe" -delete-config'

SectionEnd

Section -RestorePrograms

  ${If} $Restore_SMTube == 1
    DetailPrint $(Info_SMTube_Restore)
    CreateDirectory "$INSTDIR\docs\smtube"
    CreateDirectory "$INSTDIR\translations"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\smtube.exe" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\docs\smtube\*" "$INSTDIR\docs\smtube"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\translations\*" "$INSTDIR\translations"
!ifdef QT5
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5WebKit.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Sql.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Qml.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Quick.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Positioning.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Multimedia.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5Sensors.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5WebChannel.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5WebKitWidgets.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5OpenGL.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5PrintSupport.dll" "$INSTDIR"
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\Qt5MultimediaWidgets.dll" "$INSTDIR"
!else
    CopyFiles /SILENT "$PLUGINSDIR\smtubebak\QtWebKit4.dll" "$INSTDIR"
!endif
  ${EndIf}

!ifndef WIN64
  ${If} $Restore_Codecs == 1
    DetailPrint $(Info_Codecs_Restore)
    CopyFiles /SILENT "$PLUGINSDIR\codecbak\*" "$INSTDIR\mplayer\codecs"
  ${EndIf}
!endif

SectionEnd

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
  WriteRegStr HKLM "${SMPLAYER_APP_PATHS_KEY}" "Path" "$INSTDIR"

  ;Default Programs Registration (Vista & later)
  ${If} ${AtLeastWinVista}
    Call RegisterDefaultPrograms
  ${EndIf}

  ;Registry Uninstall information
!ifdef WIN64
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayName" "$(^Name) (x64)"
!else
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayName" "$(^Name)"
!endif
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayIcon" "$INSTDIR\smplayer.exe"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "DisplayVersion" "${SMPLAYER_VERSION}"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "HelpLink" "http://www.smplayer.info/forum"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "Publisher" "Ricardo Villalba"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString" "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "URLInfoAbout" "http://www.smplayer.info"
  WriteRegStr HKLM "${SMPLAYER_UNINST_KEY}" "URLUpdateInfo" "http://www.smplayer.info"
  WriteRegDWORD HKLM "${SMPLAYER_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD HKLM "${SMPLAYER_UNINST_KEY}" "NoRepair" "1"

  DetailPrint "Cleaning up directories..."
  SetDetailsPrint none
!ifdef WIN64
  RMDir "$INSTDIR\mplayer\codecs"
!endif
  SetDetailsPrint both

  DetailPrint "Cleaning fontconfig cache..."
  SetDetailsPrint none
  Delete "$LOCALAPPDATA\fontconfig\cache\CACHEDIR.TAG"
  Delete "$LOCALAPPDATA\fontconfig\cache\*.cache*"
  RMDir "$LOCALAPPDATA\fontconfig\cache"
  RMDir "$LOCALAPPDATA\fontconfig"
  SetDetailsPrint both

  ;Sleep 5000

  ;SetAutoClose false

SectionEnd

${MementoSectionDone}

;--------------------------------
;Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSMPlayer} $(Section_SMPlayer_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopShortcut} $(Section_DesktopShortcut_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenuShortcut} $(Section_StartMenu_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMPlayer} $(Section_MPlayer_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMPV} $(Section_MPV_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecThemes} $(Section_IconThemes_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecTranslations} $(Section_Translations_Desc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecResetSettings} $(Section_ResetSettings_Desc)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Macros

!macro MacroAllExtensions _action
  !insertmacro ${_action} ".3gp"
  !insertmacro ${_action} ".aac"
  !insertmacro ${_action} ".ac3"
  !insertmacro ${_action} ".ape"
  !insertmacro ${_action} ".asf"
  !insertmacro ${_action} ".avi"
  !insertmacro ${_action} ".bik"
  !insertmacro ${_action} ".bin"
  !insertmacro ${_action} ".dat"
  !insertmacro ${_action} ".divx"
  !insertmacro ${_action} ".dts"
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
  !insertmacro ${_action} ".mts"
  !insertmacro ${_action} ".m2v"
  !insertmacro ${_action} ".m3u"
  !insertmacro ${_action} ".m3u8"
  !insertmacro ${_action} ".m4a"
  !insertmacro ${_action} ".m4v"
  !insertmacro ${_action} ".mka"
  !insertmacro ${_action} ".mkv"
  !insertmacro ${_action} ".mov"
  !insertmacro ${_action} ".mp3"
  !insertmacro ${_action} ".mp4"
  !insertmacro ${_action} ".mpeg"
  !insertmacro ${_action} ".mpg"
  !insertmacro ${_action} ".mpv"
  !insertmacro ${_action} ".mqv"
  !insertmacro ${_action} ".nsv"
  !insertmacro ${_action} ".oga"
  !insertmacro ${_action} ".ogg"
  !insertmacro ${_action} ".ogm"
  !insertmacro ${_action} ".ogv"
  !insertmacro ${_action} ".ogx"
  !insertmacro ${_action} ".pls"
  !insertmacro ${_action} ".ra"
  !insertmacro ${_action} ".ram"
  !insertmacro ${_action} ".rec"
  !insertmacro ${_action} ".rm"
  !insertmacro ${_action} ".rmvb"
  !insertmacro ${_action} ".smk"
  !insertmacro ${_action} ".swf"
  !insertmacro ${_action} ".thd"
  !insertmacro ${_action} ".ts"
  !insertmacro ${_action} ".vcd"
  !insertmacro ${_action} ".vfw"
  !insertmacro ${_action} ".vob"
  !insertmacro ${_action} ".vp8"
  !insertmacro ${_action} ".wav"
  !insertmacro ${_action} ".webm"
  !insertmacro ${_action} ".wma"
  !insertmacro ${_action} ".wmv"
  !insertmacro ${_action} ".wtv"
!macroend

!macro WriteRegStrSupportedTypes EXT
  WriteRegStr HKLM  "${SMPLAYER_DEF_PROGS_KEY}\Capabilities\FileAssociations" ${EXT} "MPlayerFileVideo"
!macroend

!macro MacroRemoveSMPlayer
  ;Delete desktop and start menu shortcuts
  SetDetailsPrint textonly
  DetailPrint $(Info_Del_Shortcuts)
  SetDetailsPrint listonly

  SetShellVarContext all
  Delete "$DESKTOP\SMPlayer.lnk"
  Delete "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMPlayer.lnk"
  Delete "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMTube.lnk"
  Delete "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMPlayer on the Web.url"
  Delete "$SMPROGRAMS\$SMPlayer_StartMenuFolder\Uninstall SMPlayer.lnk"
  RMDir "$SMPROGRAMS\$SMPlayer_StartMenuFolder"

  ;Delete directories recursively except for main directory
  ;Do not recursively delete $INSTDIR
  SetDetailsPrint textonly
  DetailPrint $(Info_Del_Files)
  SetDetailsPrint listonly

  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\mplayer"
  RMDir /r "$INSTDIR\open-fonts"
  RMDir /r "$INSTDIR\platforms"
  RMDir /r "$INSTDIR\shortcuts"
  RMDir /r "$INSTDIR\themes"
  RMDir /r "$INSTDIR\translations"
  Delete "$INSTDIR\*.txt"
  Delete "$INSTDIR\icudt51.dll"
  Delete "$INSTDIR\icuin51.dll"
  Delete "$INSTDIR\icuuc51.dll"
  Delete "$INSTDIR\libgcc_s_*.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libwinpthread-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\Qt*.dll"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\sample.avi"
  Delete "$INSTDIR\smplayer.exe"
  Delete "$INSTDIR\smtube.exe"
  Delete "$INSTDIR\dxlist.exe"

  ;Delete registry keys
  SetDetailsPrint textonly
  DetailPrint $(Info_Del_Registry)
  SetDetailsPrint listonly

  DeleteRegKey HKLM "${SMPLAYER_REG_KEY}"
  DeleteRegKey HKLM "${SMPLAYER_APP_PATHS_KEY}"
  DeleteRegKey HKLM "${SMPLAYER_DEF_PROGS_KEY}"
  DeleteRegKey HKLM "${SMPLAYER_UNINST_KEY}"
  DeleteRegKey HKCR "MPlayerFileVideo"
  DeleteRegValue HKLM "Software\RegisteredApplications" "SMPlayer"

  SetDetailsPrint both
!macroend

;--------------------------------
;Shared functions

!ifdef USE_RUNCHECK
!macro RunCheckMacro UN
Function ${UN}RunCheck

    retry_runcheck:
    FindWindow $0 "QWidget" "SMPlayer"
    StrCmp $0 0 notRunning
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION $(SMPlayer_Is_Running) /SD IDCANCEL IDRETRY retry_runcheck
      Abort
    notrunning:

FunctionEnd
!macroend
!insertmacro RunCheckMacro ""
!insertmacro RunCheckMacro "un."
!endif

;--------------------------------
;Installer functions

Function newGUIInit

  ReadRegDWORD $ReadReg_Installed_MPV HKLM "${SMPLAYER_REG_KEY}" "Installed_MPV"
  ${If} $ReadReg_Installed_MPV == 1
    StrCpy $SecRadioButton ${SecMPV}
    !insertmacro UnSelectSection ${SecMPlayer}
    !insertmacro SelectSection ${SecMPV}
  ${Else}
    StrCpy $SecRadioButton ${SecMPlayer}
  ${EndIf}

FunctionEnd

Function .onInit

  ${Unless} ${AtLeastWinXP}
    MessageBox MB_YESNO|MB_ICONSTOP $(OS_Not_Supported) /SD IDNO IDYES installonoldwindows
    Abort
  installonoldwindows:
  ${EndIf}

!ifdef WIN64
  ${IfNot} ${RunningX64}
    MessageBox MB_OK|MB_ICONSTOP $(Win64_Required)
    Abort
  ${EndIf}

  SetRegView 32
  ClearErrors
  ReadRegStr $R0 HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString"

  IfErrors +3 0
    MessageBox MB_OK|MB_ICONSTOP $(Existing_32bitInst)
    Abort

  SetRegView 64
!else
  ${If} ${RunningX64}
    SetRegView 64
    ClearErrors
    ReadRegStr $R0 HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString"

    IfErrors +3 0
      MessageBox MB_OK|MB_ICONSTOP $(Existing_64bitInst)
      Abort

    SetRegView 32
  ${EndIf}
!endif

  ;Check if setup is already running
  System::Call 'kernel32::CreateMutexW(i 0, i 0, t "SMPlayerSetup") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION $(Installer_Is_Running)
    Abort

!ifdef USE_RUNCHECK
  ;Check if SMPlayer is running
  ;Allow skipping check using /NORUNCHECK
  ${GetParameters} $R0
  ${GetOptions} $R0 "/NORUNCHECK" $R1
  IfErrors 0 +2
    Call RunCheck
!endif

  ;Check for admin on < Vista
  UserInfo::GetAccountType
  Pop $R0
  ${If} $R0 != "admin"
    MessageBox MB_OK|MB_ICONSTOP $(Installer_No_Admin)
    Abort
  ${EndIf}

  ;Setup language selection
  !insertmacro MUI_LANGDLL_DISPLAY

  Call LoadPreviousSettings

  Call CheckPreviousVersion

  SetShellVarContext all

FunctionEnd

Function .onInstSuccess

  ${MementoSectionSave}

FunctionEnd

Function .onInstFailed

  SetDetailsPrint textonly
  DetailPrint $(Info_RollBack)
  SetDetailsPrint listonly

  !insertmacro MacroRemoveSMPlayer

  Delete "$INSTDIR\${SMPLAYER_UNINST_EXE}"
  RMDir "$INSTDIR"

FunctionEnd

Function .onSelChange

  !insertmacro StartRadioButtons $SecRadioButton
    !insertmacro RadioButton ${SecMPlayer}
    !insertmacro RadioButton ${SecMPV}
  !insertmacro EndRadioButtons

FunctionEnd

Function CheckPreviousVersion

  ClearErrors
  ReadRegStr $Previous_Version HKLM "${SMPLAYER_REG_KEY}" "Version"
  ReadRegStr $SMPlayer_UnStrPath HKLM "${SMPLAYER_UNINST_KEY}" "UninstallString"
  ReadRegStr $SMPlayer_Path HKLM "${SMPLAYER_REG_KEY}" "Path"

  ${IfNot} ${Errors}
    StrCpy $Reinstall_Uninstall 1
    !ifdef WIN64
    ;Workaround for InstallDirRegKey on 64-bit
    StrCpy $INSTDIR $SMPlayer_Path
    !endif

    ;Since we can't get input from a silent install to initialize the variables, prefer upgrading
    ${If} ${Silent}
      StrCpy $Reinstall_UninstallButton_State 0
      StrCpy $Reinstall_OverwriteButton_State 1
    ${EndIf}
  ${EndIf}

  /* $Previous_Version_State Assignments:
  $Previous_Version_State=0  This installer is the same version as the installed copy
  $Previous_Version_State=1  A newer version than this installer is already installed
  $Previous_Version_State=2  An older version than this installer is already installed */
  ${VersionCompare} $Previous_Version ${SMPLAYER_VERSION} $Previous_Version_State

  ${If} $Previous_Version_State == 0
    StrCpy $Inst_Type $(Type_Reinstall)
  ${ElseIf} $Previous_Version_State == 1
    StrCpy $Inst_Type $(Type_Downgrade)
  ${ElseIf} $Previous_Version_State == 2
    StrCpy $Inst_Type $(Type_Upgrade)
  ${EndIf}

FunctionEnd

!ifndef WIN64
Function Backup_Codecs

  ${IfNot} ${SectionIsSelected} ${SecMPlayer}
    Return
  ${EndIf}

  IfFileExists "$SMPlayer_Path\mplayer\codecs\*.dll" 0 NoBackup
    DetailPrint $(Info_Codecs_Backup)
    CreateDirectory "$PLUGINSDIR\codecbak"
    CopyFiles /SILENT "$SMPlayer_Path\mplayer\codecs\*" "$PLUGINSDIR\codecbak"
    StrCpy $Restore_Codecs 1
    Return
  NoBackup:
    StrCpy $Restore_Codecs 0

FunctionEnd
!endif

Function Backup_MPV

  ${IfNot} ${SectionIsSelected} ${SecMPV}
    Return
  ${EndIf}

  ClearErrors
  ReadRegStr $MPV_Version HKLM "${SMPLAYER_REG_KEY}" "MPV_Version"
  IfErrors NoBackup 0
    IntCmp $MPV_Version ${MPV_VERSION} +3 0 +3
      DetailPrint "A newer version of MPV is available and will be downloaded."
      Goto NoBackup

  IfFileExists "$SMPlayer_Path\mplayer\mpv*.exe" 0 NoBackup
    DetailPrint $(Info_MPV_Backup)
    CreateDirectory "$PLUGINSDIR\mpvbak"
    CopyFiles /SILENT "$SMPlayer_Path\mplayer\*" "$PLUGINSDIR\mpvbak"
    StrCpy $Restore_MPV 1
    Return
  NoBackup:
    StrCpy $Restore_MPV 0

FunctionEnd

Function Backup_SMTube

  IfFileExists "$SMPlayer_Path\smtube.exe" 0 NoBackup
    DetailPrint $(Info_SMTube_Backup)
    CreateDirectory "$PLUGINSDIR\smtubebak\translations"
    CreateDirectory "$PLUGINSDIR\smtubebak\docs\smtube"
    CopyFiles /SILENT "$SMPlayer_Path\smtube.exe" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\docs\smtube\*" "$PLUGINSDIR\smtubebak\docs\smtube"
    CopyFiles /SILENT "$SMPlayer_Path\translations\smtube*.qm" "$PLUGINSDIR\smtubebak\translations"
!ifdef QT5
    CopyFiles /SILENT "$SMPlayer_Path\Qt5WebKit.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Sql.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Qml.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Quick.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Positioning.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Multimedia.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5Sensors.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5WebChannel.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5WebKitWidgets.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5OpenGL.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5PrintSupport.dll" "$PLUGINSDIR\smtubebak"
    CopyFiles /SILENT "$SMPlayer_Path\Qt5MultimediaWidgets.dll" "$PLUGINSDIR\smtubebak"
!else
    CopyFiles /SILENT "$SMPlayer_Path\QtWebKit4.dll" "$PLUGINSDIR\smtubebak"
!endif

    StrCpy $Restore_SMTube 1
    Return
  NoBackup:
    StrCpy $Restore_SMTube 0

FunctionEnd

Function LoadPreviousSettings

  ;Gets previous start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMPlayer_StartMenuFolder

  ${MementoSectionRestore}

FunctionEnd

Function PageReinstall

  ${If} $Reinstall_Uninstall != 1
    Abort
  ${EndIf}

  nsDialogs::Create /NOUNLOAD 1018
  Pop $Dialog_Reinstall

  nsDialogs::SetRTL $(^RTL)

  !insertmacro MUI_HEADER_TEXT $(Reinstall_Header_Text) $(Reinstall_Header_SubText)

  ${NSD_CreateLabel} 0 0 225u 8u $(Reinstall_Msg1)

  ${NSD_CreateText} 10u 15u 290u 14u "$SMPlayer_Path"
  Pop $R0

  ${NSD_CreateLabel} 0 40u 100u 8u $(Reinstall_Msg2)

  ${NSD_CreateRadioButton} 10u 58u 200u 8u $(Reinstall_Overwrite)
  Pop $Reinstall_OverwriteButton
  ${NSD_CreateRadioButton} 10u 73u 200u 8u $(Reinstall_Uninstall)
  Pop $Reinstall_UninstallButton

  ${NSD_CreateCheckBox} 0 90u 100% 8u $(Reinstall_Msg4)
  Pop $Reinstall_ChgSettings

  ${NSD_CreateCheckBox} 0 102u 100% 8u $(Reinstall_Msg5)
  Pop $Reinstall_RemoveSettings

  ${NSD_CreateLabel} 0 121u 100% 16u 
  Pop $Reinstall_Message

  SendMessage $Reinstall_OverwriteButton ${BM_SETCHECK} 1 0
  EnableWindow $R0 0

  ${If} $Reinstall_ChgSettings_State == 1
    SendMessage $Reinstall_ChgSettings ${BM_SETCHECK} 1 0
  ${Endif}

  ${If} $Reinstall_RemoveSettings_State == 1
    SendMessage $Reinstall_RemoveSettings ${BM_SETCHECK} 1 0
  ${Endif}

  ${NSD_OnClick} $Reinstall_OverwriteButton PageReinstallUpdate
  ${NSD_OnClick} $Reinstall_UninstallButton PageReinstallUpdate
  ${NSD_OnClick} $Reinstall_ChgSettings PageReinstallUpdate

  Call PageReinstallUpdate

  nsDialogs::Show

FunctionEnd

Function PageReinstallLeave

  ${NSD_GetState} $Reinstall_OverwriteButton $Reinstall_OverwriteButton_State
  ${NSD_GetState} $Reinstall_UninstallButton $Reinstall_UninstallButton_State
  ${NSD_GetState} $Reinstall_ChgSettings $Reinstall_ChgSettings_State
  ${NSD_GetState} $Reinstall_RemoveSettings $Reinstall_RemoveSettings_State

  ${If} $Reinstall_RemoveSettings_State == 1
    !insertmacro SelectSection ${SecResetSettings}
  ${EndIf}

FunctionEnd

Function PageReinstallUpdate

  ${NSD_GetState} $Reinstall_OverwriteButton $Reinstall_OverwriteButton_State
  ${NSD_GetState} $Reinstall_UninstallButton $Reinstall_UninstallButton_State
  ${NSD_GetState} $Reinstall_ChgSettings $Reinstall_ChgSettings_State

  ${If} $Reinstall_OverwriteButton_State == 1

    EnableWindow $Reinstall_ChgSettings 1
    EnableWindow $Reinstall_RemoveSettings 1

    GetDlgItem $R0 $HWNDPARENT 1
    ${If} $Reinstall_ChgSettings_State != 1
      SendMessage $R0 ${WM_SETTEXT} 0 "STR:$(StartBtn)"
      ${NSD_SetText} $Reinstall_Message $(Reinstall_Msg3_1)
    ${ElseIf} $Reinstall_ChgSettings_State == 1
      SendMessage $R0 ${WM_SETTEXT} 0 "STR:$(^NextBtn)"
      ${NSD_SetText} $Reinstall_Message $(Reinstall_Msg3_2)
    ${EndIf}

  ${ElseIf} $Reinstall_UninstallButton_State == 1

    EnableWindow $Reinstall_ChgSettings 0
    ${NSD_SetState} $Reinstall_ChgSettings 0

    EnableWindow $Reinstall_RemoveSettings 0
    ${NSD_SetState} $Reinstall_RemoveSettings 0

    GetDlgItem $R0 $HWNDPARENT 1
    SendMessage $R0 ${WM_SETTEXT} 0 "STR:$(^UninstallBtn)"

    ${NSD_SetText} $Reinstall_Message $(Reinstall_Msg3_3)

  ${EndIf}

FunctionEnd

Function PageComponentsPre

  ${If} $Reinstall_Uninstall == 1
  ${AndIf} $Reinstall_ChgSettings_State != 1
    Abort
  ${EndIf}

FunctionEnd

Function PageDirectoryPre

  ${If} $Reinstall_Uninstall == 1
  ${AndIf} $Reinstall_ChgSettings_State != 1
    Abort
  ${EndIf}

FunctionEnd

Function PageStartMenuPre

  ${If} $Reinstall_Uninstall == 1
  ${AndIf} $Reinstall_ChgSettings_State != 1
    Abort
  ${EndIf}

  ${IfNot} ${SectionIsSelected} ${SecStartMenuShortcut}
    Abort
  ${EndIf}

FunctionEnd

Function RegisterDefaultPrograms

  WriteRegStr HKCR "MPlayerFileVideo\DefaultIcon" "" '"$INSTDIR\smplayer.exe",1'
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue" "" "Enqueue in SMPlayer"
  WriteRegStr HKCR "MPlayerFileVideo\shell\enqueue\command" "" '"$INSTDIR\smplayer.exe" -add-to-playlist "%1"'
  WriteRegStr HKCR "MPlayerFileVideo\shell\open" "FriendlyAppName" "SMPlayer Media Player"
  WriteRegStr HKCR "MPlayerFileVideo\shell\open\command" "" '"$INSTDIR\smplayer.exe" "%1"'

  ;Modify the list of extensions added in the MacroAllExtensions macro
  WriteRegStr HKLM "${SMPLAYER_DEF_PROGS_KEY}" "" "SMPlayer"
  WriteRegStr HKLM "${SMPLAYER_DEF_PROGS_KEY}\Capabilities" "ApplicationDescription" $(Application_Description)
  WriteRegStr HKLM "${SMPLAYER_DEF_PROGS_KEY}\Capabilities" "ApplicationName" "SMPlayer"
  WriteRegStr HKLM "Software\RegisteredApplications" "SMPlayer" "${SMPLAYER_DEF_PROGS_KEY}\Capabilities"
  !insertmacro MacroAllExtensions WriteRegStrSupportedTypes

FunctionEnd

/*************************************** Uninstaller *******************************************/

Section Uninstall

  ;Make sure SMPlayer is installed from where the uninstaller is being executed.
  IfFileExists "$INSTDIR\smplayer.exe" +2
    Abort $(Uninstaller_InvalidDirectory)

  SetDetailsPrint textonly
  DetailPrint $(Info_Rest_Assoc)
  SetDetailsPrint listonly

  ;Don't restore file associations if reinstalling
  ${un.GetParameters} $R0
  ${un.GetOptionsS} $R0 "/R" $R1

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

!ifdef WIN64
  ${IfNot} ${RunningX64}
    MessageBox MB_OK|MB_ICONSTOP $(Uninstaller_64bitOnly)
    Abort
  ${EndIf}

  SetRegView 64
!endif

  ;Check for admin on < Vista
  UserInfo::GetAccountType
  Pop $R0
  ${If} $R0 != "admin"
    MessageBox MB_OK|MB_ICONSTOP $(Uninstaller_No_Admin)
    Abort
  ${EndIf}

!ifdef USE_RUNCHECK
  ;Check if SMPlayer is running
  ;Allow skipping check using /NORUNCHECK
  ${un.GetParameters} $R0
  ${un.GetOptions} $R0 "/NORUNCHECK" $R1
  IfErrors 0 +2
    Call un.RunCheck
!endif

  ;Gets start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMPlayer_StartMenuFolder

  ;Get the stored language preference
  !insertmacro MUI_UNGETLANGUAGE

FunctionEnd

Function un.ConfirmPagePre

  ${un.GetParameters} $R0

  ${un.GetOptionsS} $R0 "/X" $R1
  ${Unless} ${Errors}
    Abort
  ${EndUnless}

FunctionEnd
