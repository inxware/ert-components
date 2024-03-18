;inxware ehs nsis script
;
;Tested against NSIS 3.0.4.1 available from:
;https://github.com/electron-userland/electron-builder-binaries/releases/tag/nsis-3.0.4.1
;
;Newer versions of NSIS suffer from false positive detection as trojan by e.g.
;Windows Defender. The apparent workaround is to downgrade to the above version
;as mentioned in the thread below:
;https://github.com/electron-userland/electron-builder/issues/6334

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "inxware eRT"
  OutFile "inxware-ert-installer.exe"
  Unicode True

  ;Default installation folder (inside inxware tools install dir)
  InstallDir "$LOCALAPPDATA\inxware\dist\EHS"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\inxware-eRT" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  ;TODO change license, also this macro is buggy in nsis 3.0.4.1
  ;!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

;This section goes first in order to make the $INSTDIR variable available to
;other sections below.
Section "inxware-eRT" SecInxwareEhs
  
  ;Store installation folder
  WriteRegStr HKCU "Software\inxware-eRT" "" $INSTDIR

  ;Add files here (and subsequently also to the uninstall section)
  SetOutPath "$INSTDIR"

  File /r /x README /x .gitkeep /x .gitignore ..\ehs_env-${ERT_TARGET}

  WriteUninstaller "$INSTDIR\Uninstall-${ERT_NSIS_EXE_NAME}.exe"
  
  ;Create Start Program Shortcuts
  SetOutPath "$INSTDIR\ehs_env-${ERT_TARGET}\bin"
  CreateDirectory "$SMPROGRAMS\inxware"
  CreateShortcut "$SMPROGRAMS\inxware\${ERT_NSIS_EXE_NAME}.lnk" "$INSTDIR\ehs_env-${ERT_TARGET}\bin\${ERT_PACKAGE_NAME}.exe"
  CreateShortcut "$SMPROGRAMS\inxware\Uninstall-${ERT_NSIS_EXE_NAME}.lnk" "$INSTDIR\Uninstall-${ERT_NSIS_EXE_NAME}.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecInxwareEhs ${LANG_ENGLISH} \
  "Install inxware Unity eRT."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecInxwareEhs} $(DESC_SecInxwareEhs)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$SMPROGRAMS\inxware\${ERT_NSIS_EXE_NAME}.lnk"
  Delete "$SMPROGRAMS\inxware\Uninstall-${ERT_NSIS_EXE_NAME}.lnk"
  ;Remove only if empty
  RMDir "$SMPROGRAMS\inxware"

  RMDir /r "$INSTDIR\ehs_env-${ERT_TARGET}"

  ;Uninstaller itself should be the last file to be deleted.
  Delete "$INSTDIR\Uninstall-${ERT_NSIS_EXE_NAME}.exe"

  RMDir "$INSTDIR"
  
  DeleteRegKey /ifempty HKCU "Software\inxware-eRT"

SectionEnd
