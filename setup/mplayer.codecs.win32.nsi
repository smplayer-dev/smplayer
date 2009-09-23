; Written by redxii <redxii1234@hotmail.com>
; Codecs installer for SMPlayer

;--------------------------------
;Compressor

  SetCompressor /SOLID lzma
  SetCompressorDictSize 32

;--------------------------------
;Defines

  !define PRODUCT_VERSION "20071007"

;--------------------------------
;Include Modern UI and functions

  !include MUI2.nsh

;--------------------------------
;General

  ;Name and file
  Name "SMPlayer Extra Codecs ${PRODUCT_VERSION}"
  OutFile "smplayer_codecs_${PRODUCT_VERSION}.exe"

  ;Show details
  ShowInstDetails show

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\SMPlayer" "Path"

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

;--------------------------------
;Variables

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"

  ;Installer/Uninstaller icons
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"

;--------------------------------
;Pages

  ;Install pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Reserve Files

;------------------------------------------------------------------------------------------------
;Installer Sections

;--------------------------------
; Main SMPlayer files
Section -Codecs

  Delete "$INSTDIR\mplayer\codecs\*"

  SetOutPath "$INSTDIR\mplayer\codecs"
  File "codecs\*"

  WriteRegDWORD HKLM Software\SMPlayer Installed_Codecs 0x1

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

  ClearErrors
  ReadRegStr $0 HKLM Software\SMPlayer "Path"
  IfErrors 0 +2
    MessageBox MB_OK|MB_ICONEXCLAMATION "SMPlayer installation not found. You will have to manually locate your SMPlayer installation."

FunctionEnd

Function .onVerifyInstDir

  IfFileExists $INSTDIR\smplayer.exe PathGood
    Abort ; if $INSTDIR is not a smplayer directory, don't let us install there
  PathGood:

FunctionEnd

;End Installer Sections
;------------------------------------------------------------------------------------------------