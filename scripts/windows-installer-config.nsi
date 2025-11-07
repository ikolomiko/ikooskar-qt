Outfile "../build-win32/ikoOSKAR v4 Yükleyici.exe"
Icon "../assets/multires.ico"
InstallDir $PROGRAMFILES\ikoOSKAR
RequestExecutionLevel admin

!include MUI2.nsh

# MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "../assets/multires.ico"
!define MUI_UNICON "../assets/multires.ico"

# Set the installer name used in Modern UI
Name "ikoOSKAR v4.2.1"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "../assets/nsis-header.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "../assets/nsis-header.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "../assets/nsis-welcome.bmp"

# Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

Section

# Set the output path where the executable will be installed
SetOutPath $INSTDIR

# Install the main executable
File "../build-win32/ikoOSKAR.exe"

# Create a desktop shortcut
CreateShortCut "$DESKTOP\ikoOSKAR.lnk" "$INSTDIR\ikoOSKAR.exe"

# Create a Start Menu shortcut
CreateShortCut "$SMPROGRAMS\ikoOSKAR.lnk" "$INSTDIR\ikoOSKAR.exe"

WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "DisplayName" "ikoOSKAR"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "DisplayVersion" "v4.2.1"
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "NoModify" 1
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "NoRemove" 0
WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "NoRepair" 0
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "Publisher" "İlker Avcı"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "HelpLink" "https://ikooskar.com.tr"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "UninstallString" "$INSTDIR\Uninstall.exe"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "DisplayIcon" "$INSTDIR\ikoOSKAR.exe"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR" "InstallLocation" "$INSTDIR"

SectionEnd

Section "un.Uninstall"

# Delete installed files
Delete $INSTDIR\ikoOSKAR.exe
Delete "$DESKTOP\ikoOSKAR.lnk"
Delete "$SMPROGRAMS\ikoOSKAR.lnk"

# Remove the installation directory if it's empty
RMDir $INSTDIR

DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\ikoOSKAR"

SectionEnd

# Specify Turkish as the language
!insertmacro MUI_LANGUAGE "Turkish"

Function .onInit
    StrCpy $INSTDIR $PROGRAMFILES\ikoOSKAR
    WriteUninstaller $INSTDIR\Uninstall.exe
FunctionEnd

