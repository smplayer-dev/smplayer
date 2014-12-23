;Language: Korean (1042)
;Korean language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Korean" "한국어"

; Startup
${LangFileString} Installer_Is_Running "설치 프로그램이 실행 중입니다."
${LangFileString} Installer_No_Admin "이 프로그램을 설치하려면 관리자 권한이 필요합니다."
${LangFileString} SMPlayer_Is_Running "SMPlayer가 실행 중입니다. SMPlayer를 종료 후 다시 시도하세요."

${LangFileString} OS_Not_Supported "미지원 운영 체계.$\nSMPlayer ${SMPLAYER_VERSION} 는 윈도우 XP 이상이 필요하고 당신의 시스템에서 정상적인 동작을 보장할 수 없습니다.$\n설치를 계속 진행할까요?"
${LangFileString} Win64_Required "이 프로그램은 윈도우 64비트 운영 체계에서만 설치할 수 있습니다."
${LangFileString} Existing_32bitInst "32비트 버전의 SMPlayer가 이미 설치되어 있습니다. 32비트 SMPlayer를 먼저 제거하세요."
${LangFileString} Existing_64bitInst "64비트 버전의 SMPlayer가 이미 설치되어 있습니다. 64비트 SMPlayer를 먼저 제거하세요."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "Setup이 $(^NameDA)의 설치를 도와줄 것입니다.$\r$\n$\r$\nSetup을 시작하기 전에 SMPlayer를 모두 종료하세요. 그러면 재부팅 없이 관련 프로그램 파일을 업데이트할 수 있습니다.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} ShortcutGroupTitle "바로가기"
${LangFileString} MPlayerGroupTitle "MPlayer 구성 요소"
${LangFileString} MPlayerMPVGroupTitle "Multimedia Engine"

${LangFileString} Section_SMPlayer "SMPlayer (필수)"
${LangFileString} Section_SMPlayer_Desc "SMPlayer, 공유 라이브러리와 문서."

${LangFileString} Section_DesktopShortcut "바탕화면"
${LangFileString} Section_DesktopShortcut_Desc "바탕화면에  SMPlayer의 바로가기를 만듭니다."

${LangFileString} Section_StartMenu "시작 메뉴"
${LangFileString} Section_StartMenu_Desc "SMPlayer의 시작 메뉴 항목을 만듭니다."

${LangFileString} Section_MPlayer "MPlayer (필수)"
${LangFileString} Section_MPlayer_Desc "MPlayer; 재생에 필요함."

${LangFileString} Section_MPlayerCodecs "바이너리 코덱"
!ifdef WIN64
${LangFileString} Section_MPlayerCodecs_Desc "이 버전에서 바이너리 코덱은 지원하지 않습니다."
!else
${LangFileString} Section_MPlayerCodecs_Desc "MPlayer용 추가 코덱. (설치시 인터넷 연결이 필요함)"
!endif

${LangFileString} Section_MPV_Desc "A feature-rich fork of MPlayer && MPlayer2"

${LangFileString} Section_MEncoder_Desc "지원하는 오디오 비디오 스트림을 인코딩하거나 변환하는데 사용할 수 있는 MPlayer의 추가 프로그램."

${LangFileString} Section_IconThemes "아이콘 테마"
${LangFileString} Section_IconThemes_Desc "SMPlayer의 추가 아이콘 테마."

${LangFileString} Section_Translations "언어"
${LangFileString} Section_Translations_Desc "SMPlayer의 비영어 언어 파일들."

${LangFileString} Section_ResetSettings_Desc "이전 설치된 SMPlayer의 환경 설정을 삭제합니다."

${LangFileString} MPlayer_Codec_Msg "바이너리 코덱 꾸러미는 새로운 RealVideo 변종과 많은 희귀한 형식의 동영상에서, 아직 직접 지원하지 않는 코덱을 지원합니다.$\nDVD나 MPEG-1/2/4와 같은 대부분의 동영상을 재생하는데 필수적인 코덱은 아닙니다."

; Upgrade/Reinstall Page
${LangFileString} Reinstall_Header_Text "설치 형태를 선택하세요"
${LangFileString} Reinstall_Header_SubText "덮어쓸 것인지 제거할 것인지 선택하세요."

${LangFileString} Reinstall_Msg1 "다음 폴더에 SMPlayer가 설치되어 있습니다:"
${LangFileString} Reinstall_Msg2 "어떻게 처리할 것인지 선택하세요:"
${LangFileString} Reinstall_Overwrite "기존의 설치 위에 덮어 씁니다 ($Inst_Type)"
${LangFileString} Reinstall_Uninstall "기존의 설치를 제거합니다"
${LangFileString} Reinstall_Msg3_1 "준비가 되었으면 시작을 누르세요."
${LangFileString} Reinstall_Msg3_2 "준비가 되었으면 다음을 누르세요."
${LangFileString} Reinstall_Msg3_3 "준비가 되었으면 제거를 누르세요."
${LangFileString} Reinstall_Msg4 "설치 설정을 변경합니다"
${LangFileString} Reinstall_Msg5 "내 SMPlayer 설정을 초기화합니다"

${LangFileString} Type_Reinstall "재설치"
${LangFileString} Type_Downgrade "다운그레이드"
${LangFileString} Type_Upgrade "업그레이드"

${LangFileString} StartBtn "시작"

; Codecs Section
${LangFileString} Codecs_DL_Msg "Downloading MPlayer codecs..."
${LangFileString} Codecs_DL_Retry "MPlayer 코덱 설치에 실패했습니다. 다시 할까요?"
${LangFileString} Codecs_DL_Failed "MPlayer 코덱 다운로드에 실패했습니다: '$R0'."
${LangFileString} Codecs_Inst_Failed "MPlayer 코덱 설치에 실패했습니다."

; Uninstaller
${LangFileString} Uninstaller_No_Admin "이 설치는 관리자 권한을 지닌 사용자만 제거할 수 있습니다."
${LangFileString} Uninstaller_Aborted "사용자가 제거를 취소함."
${LangFileString} Uninstaller_NotInstalled "SMPlayer가 '$INSTDIR' 디렉토리에 설치된 것 같지 않습니다.$\r$\n어쨌든 계속할까요? (비권장)"
${LangFileString} Uninstaller_InvalidDirectory "SMPlayer  설치를 찾을 수 없습니다."
${LangFileString} Uninstaller_64bitOnly "이 설치는 64비트 윈도우에서만 제거할 수 있습니다."

; Vista & Later Default Programs Registration
${LangFileString} Application_Description "SMPlayer는 비디오, DVD, VCD 재생과 같은 기본적인 기능들로부터 MPlayer 필터와 edl 목록 등 고급 기능들까지 지원하는 MPlayer의 완전한 프론트엔트입니다."

; Misc
${LangFileString} Info_Codecs_Backup "이전 설치에서 코덱 백업하는 중..."
${LangFileString} Info_Codecs_Restore "이전 설치에서 코덱 복원하는 중..."
${LangFileString} Info_Del_Files "파일 삭제하는 중..."
${LangFileString} Info_Del_Registry "레지스트리 키 삭제하는 중..."
${LangFileString} Info_Del_Shortcuts "바로가기 삭제하는 중..."
${LangFileString} Info_Rest_Assoc "파일 연결 복원하는 중..."
${LangFileString} Info_RollBack "변경점을 원상 복원하는 중..."
${LangFileString} Info_Files_Extract "파일 압축을 해제하는 중..."
${LangFileString} Info_SMTube_Backup "Backing up SMTube..."
${LangFileString} Info_SMTube_Restore "Restoring SMTube from previous installation..."
${LangFileString} Info_MPV_Backup "Backing up MPV..."
${LangFileString} Info_MPV_Restore "Restoring MPV from previous installation..."

; MPV
${LangFileString} MPV_DL_Msg "Downloading MPV..."
${LangFileString} MPV_DL_Retry "MPV was not successfully installed. Retry?"
${LangFileString} MPV_DL_Failed "Failed to download MPV: '$R0'."
${LangFileString} MPV_Inst_Failed "Failed to install MPV."

; YouTube-DL
${LangFileString} YTDL_DL_Retry "YouTube-DL was not successfully installed. Retry?"
${LangFileString} YTDL_DL_Failed "Failed to download Youtube-DL: '$R0'."
