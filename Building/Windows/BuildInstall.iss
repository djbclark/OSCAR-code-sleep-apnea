; Script originally generated by the Inno Script Studio Wizard, then manually modified.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!
;
; See DEPLOY.BAT for documentation on how this is used to build OSCAR installer 

#include "buildinfo.iss"

#define MyAppVersion MyMajorVersion+"."+MyMinorVersion+"."+MyRevision+"-"+MyReleaseStatus
#if MyReleaseStatus == "r"
  #define MyAppVersion MyAppVersion+MyBuildNumber
#else
  #define MyAppVersion MyAppVersion+"-"+MyBuildNumber
#endif

#define MyAppPublisher "The OSCAR Team"
#define MyAppExeName "OSCAR.exe"
#define MyAppName "OSCAR"

[Setup]
SetupLogging=yes
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
; Now using separate AppID for Win32 and Win64 and for test builds -- GTS 4/6/2019
#if MyPlatform == "Win64"
    ArchitecturesAllowed=x64
    ArchitecturesInstallIn64BitMode=x64
    #if MyReleaseStatus == "r" || MyReleaseStatus == "rc"
      AppId={{FC6F08E6-69BF-4469-ADE3-78199288D305}
      #define MyGroupName "OSCAR"
      #define MyDirName "OSCAR"
    #else
      AppId={{C5E23210-4BC5-499D-A0E4-81192748D322}
      #define MyGroupName "OSCAR (test)"
      #define MyDirName "OSCAR-test"
    #endif
;    DefaultDirName={%PROGRAMFILES|{pf}}\OSCAR
; above doesn't work.  Always returns x86 directory
#else  // 32-bit
    #if MyReleaseStatus == "r" || MyReleaseStatus == "rc"
      AppId={{4F3EB81B-1866-4124-B388-5FB5DA34FFDD}
      #define MyGroupName "OSCAR 32-bit"
      #define MyDirName "OSCAR"
    #else
      AppId={{B0382AB3-ECB4-4F9D-ABB1-F6EF73D4E3DB}
      #define MyGroupName "OSCAR 32-bit (test)"
      #define MyDirName "OSCAR-test"
    #endif
;    DefaultDirName={%PROGRAMFILES(X86)|{pf}}\OSCAR
#endif
AppName={#MyAppName}
AppVersion={#MyAppVersion}-{#MyPlatform}-{#MyGitRevision}{#MySuffix}
AppVerName={#MyAppName} {#MyAppVersion}-{#MyPlatform}-{#MyGitRevision}{#MySuffix}
AppPublisher={#MyAppPublisher}
AppCopyright=Copyright 2019 {#MyAppPublisher}
; **** AppCopyright=Copyright {GetDateTimeString('yyyy', #0, #0)} {%MyAppPublisher}
DefaultDirName={pf}\{#MyDirName}
DefaultGroupName={#MyGroupName}
OutputDir=.\Installer
#if MyReleaseStatus == "r"
  OutputBaseFilename={#MyAppName}-{#MyAppVersion}-{#MyPlatform}{#MySuffix}
#else
  OutputBaseFilename={#MyAppName}-{#MyAppVersion}-{#MyPlatform}-{#MyGitRevision}{#MySuffix}
#endif
SetupIconFile=setup.ico
Compression=lzma
SolidCompression=yes
VersionInfoCompany={#MyAppPublisher}
VersionInfoProductName={#MyAppName}
UninstallDisplayName={#MyGroupName}
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "armenian"; MessagesFile: "compiler:Languages\Armenian.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "icelandic"; MessagesFile: "compiler:Languages\Icelandic.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: ".\Release\OSCAR.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyGroupName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyGroupName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyGroupName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Messages]
ConfirmUninstall=Are you sure you want to completely remove %1 and all of its components?  (Data folder and settings will not be removed.)

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
