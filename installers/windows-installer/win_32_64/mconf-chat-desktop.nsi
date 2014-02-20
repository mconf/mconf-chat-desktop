; mconf-chat-desktop.nsi
; The NSIS installer script for mconf-chat-desktop
; This script is based on example2.nsi from Nullsoft NSIS


;--------------------------------

; The name of the installer
Name "mconf-chat-desktop"

; The file to write
OutFile "mconf-chat-desktop.exe"

; The default installation directory
InstallDir $PROGRAMFILES\mconf-chat-desktop

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\mconf-chat-desktop" "Install_Dir"

; Request application privileges for Windows Vista/7
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Mconf Chat Desktop (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there  
  File libeay32.dll
  File libgcc_s_dw2-1.dll
  File libz-1.dll
  File mconf-chat.exe
  File mingwm10.dll
  File qca2.dll
  File QtCore4.dll
  File QtGui4.dll
  File QtNetwork4.dll
  File QtXml4.dll
  File ssleay32.dll  
  File /r crypto
  File /r translations
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\mconf-chat-desktop "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\" "DisplayName" "Mconf Chat Desktop"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mconf-chat-desktop" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mconf-chat-desktop" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mconf-chat-desktop" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Mconf Chat Desktop"
  CreateShortCut "$SMPROGRAMS\Mconf Chat Desktop\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Mconf Chat Desktop\Mconf Chat Desktop.lnk" "$INSTDIR\mconf-chat.exe" "" "$INSTDIR\mconf-chat.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\mconf-chat-desktop"
  DeleteRegKey HKLM SOFTWARE\mconf-chat-desktop

  ; Remove files and uninstaller
  RMDir /r "$INSTDIR"
  


  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\mconf-chat-desktop\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\mconf-chat-desktop"
  RMDir "$INSTDIR"

SectionEnd
