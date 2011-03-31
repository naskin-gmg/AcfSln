#define XERCESDIR GetEnv("XERCESDIR")
#define XALANDIR GetEnv("XALANDIR")
#define ZLIBDIR GetEnv("ZLIBDIR")
#define CBIOSDIR GetEnv("CBIOSDIR")
#define FFMPEGDIR GetEnv("FFMPEGDIR")
#define EXTLIBDIR GetEnv("EXTLIBDIR")
#define QTDIR GetEnv("QTDIR")

[Setup]
AppVerName=$AcfProductName$ $AcfVersion:1$
AppVersion=$AcfVersion:1$
AppPublisher=Witold Gantzke & Kirill Lepskiy
AppPublisherURL=http://www.imagingtools.de/
AppName=ACF-Solutions
OutputBaseFilename=Setup_$AcfProductName$_$AcfApplicationType$_v$AcfVersion:1$_$CompilerName$

DefaultDirName={pf}\ImagingTools\$AcfProductName$_v$AcfVersion:1$
WizardImageFile=..\WizardImageFile.bmp
WizardSmallImageFile=..\WizardSmallImageFile.bmp
SetupIconFile=..\..\Docs\Images\Logo\Logo.ico
AlwaysShowDirOnReadyPage=True
DefaultGroupName=ImagingTools\ACF-Solutions
DisableProgramGroupPage=True
UninstallDisplayIcon=..\..\Docs\Images\Logo\Logo.ico
UninstallDisplayName=$AcfProductName$ $AcfVersion:1$
OutputDir=..\Setup
LicenseFile=$LicensePath$
VersionInfoDescription=$AcfProductName$ v. $AcfVersion:1$ ($AcfRawVersion:1$) by $AcfCompanyName$
AppComments=Compiled using $CompilerName$, based on ACF v. $AcfVersion:0$ ($AcfRawVersion:0$), see www.imagingtools.de to find out more about ACF-Solutions
AppCopyright=Copyright (C) 2007-2011 Witold Gantzke and Kirill Lepskiy
VersionInfoVersion=$AcfVersion:1$
ChangesEnvironment=yes

[Files]
Source: ..\Temp\*; DestDir: {app}; Flags: recursesubdirs; Components: acfSlnComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Debug$CompilerName$\*; DestDir: {app}\Bin\Debug$CompilerName$; Flags: recursesubdirs; Components: acfSlnComp
Source: {#EXTLIBDIR}\Microsoft\Redist\Release$CompilerName$\*; DestDir: {app}\Bin\Release$CompilerName$; Flags: recursesubdirs; Components: acfSlnComp
Source: {#QTDIR}\plugins\imageformats\*.dll; DestDir: {app}\Bin\Debug$CompilerName$\imageformats; Components: acfSlnComp
Source: {#QTDIR}\plugins\iconengines\*.dll; DestDir: {app}\Bin\Debug$CompilerName$\iconengines; Components: acfSlnComp
Source: {#QTDIR}\plugins\imageformats\*.dll; DestDir: {app}\Bin\Release$CompilerName$\imageformats; Components: acfSlnComp
Source: {#QTDIR}\plugins\iconengines\*.dll; DestDir: {app}\Bin\Release$CompilerName$\iconengines; Components: acfSlnComp

Source: {#XERCESDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xercesComp
Source: {#XERCESDIR}\*; DestDir: {app}\ExtLib\Xerces; Flags: recursesubdirs; Excludes: *.dll; Components: xercesComp

Source: {#XALANDIR}\Lib\*.dll; DestDir: {app}\ExtLib\Bin; Components: xalanComp
Source: {#XALANDIR}\*; DestDir: {app}\ExtLib\Xalan; Flags: recursesubdirs; Excludes: *.dll; Components: xalanComp

Source: {#ZLIBDIR}\*; DestDir: {app}\ExtLib\Zlib; Flags: recursesubdirs; Excludes: *.dll; Components: zlibComp

Source: {#CBIOSDIR}\bin\*.exe; DestDir: {app}\ExtLib\Bin; Components: cbiosComp
Source: {#CBIOSDIR}\*; DestDir: {app}\ExtLib\CBios; Flags: recursesubdirs; Excludes: *.exe; Components: cbiosComp

Source: {#FFMPEGDIR}\bin\*.dll; DestDir: {app}\ExtLib\Bin; Components: ffmpegComp
Source: {#FFMPEGDIR}\*; DestDir: {app}\ExtLib\FFMpeg; Flags: recursesubdirs; Excludes: *.dll; Components: ffmpegComp

[Languages]
Name: en; MessagesFile: compiler:Default.isl
Name: de; MessagesFile: compiler:Languages\German.isl
Name: pl; MessagesFile: compiler:Languages\Polish.isl

[CustomMessages]
en.MinimalType=Minimal installation
de.MinimalType=Minimale Installation
pl.MinimalType=Instalacja minimalna

en.FullType=Full installation
de.FullType=Komplete Installation
pl.FullType=Instalacja kompletna

en.CustomType=Custom installation
de.CustomType=Ausgewählte Komponenten
pl.CustomType=Wybrane komponenty

en.QtBinaries=Qt binaries
de.QtBinaries=Qt binäre Datei
pl.QtBinaries=Pliki binarne Qt

en.AddExtPath=Add external binaries path to the system PATH
de.AddExtPath=Pfad für externe Bibliotheken zum Systempfad hinzufügen
pl.AddExtPath=Dodaj sciezke zewnetrznych bibliotek do sciezki systemowej

en.AddAcfSlnPath=Add ACF-Solutions tools path to the system PATH
de.AddAcfSlnPath=Pfad für ACF-Solutions-Tools zum Systempfad hinzufügen
pl.AddAcfSlnPath=Dodaj sciezke narzedzi ACF-Solutions do sciezki systemowej

[Types]
Name: minimalType; Description: {cm:MinimalType}
Name: fullType; Description: {cm:FullType}
Name: customType; Description: {cm:CustomType}; Flags: iscustom

[Components]
Name: acfSlnComp; Description: ACF-Solutions; Types: minimalType fullType customType
Name: xercesComp; Description: Xerces; Types: fullType customType
Name: xalanComp; Description: Xalan; Types: fullType customType
Name: zlibComp; Description: ZLib; Types: fullType customType
Name: cbiosComp; Description: CBios; Types: fullType customType
Name: ffmpegComp; Description: FFmpeg; Types: fullType customType
Name: qtComp; Description: {cm:QtBinaries}; Types: fullType customType

[Icons]
Name: "{group}\Compositor"; Filename: {app}\Bin\Release$CompilerName$\Compositor.exe; WorkingDir: {app}\Bin\Release$CompilerName$; Components: acfSlnComp
Name: "{group}\API Documentation"; Filename: {app}\Docs\TechnicalDoc\index.html; Components: acfSlnComp

[Tasks]
Name: AddExtPath; Description: {cm:AddExtPath}; Flags: checkedonce; Components: xercesComp xalanComp zlibComp cbiosComp ffmpegComp
Name: AddAcfSlnPath; Description: {cm:AddAcfSlnPath}; Flags: checkedonce; Components: acfSlnComp

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\ExtLib\Bin"; Flags: createvalueifdoesntexist; Tasks: AddExtPath
Root: HKCU; Subkey: "Environment"; ValueType: string; ValueName: "Path"; ValueData: "{olddata};{app}\Bin\Release$CompilerName$"; Flags: createvalueifdoesntexist; Tasks: AddAcfSlnPath
Root: HKCU; Subkey: "Environment"; ValueName: ACFSLNDIR; ValueData: {app}; ValueType: string; Flags: uninsdeletevalue; Components: acfSlnComp
Root: HKCU; Subkey: "Environment"; ValueName: XERCESDIR; ValueData: {app}\ExtLib\Xerces; ValueType: string; Flags: uninsdeletevalue; Components: xercesComp
Root: HKCU; Subkey: "Environment"; ValueName: XALANDIR; ValueData: {app}\ExtLib\Xalan; ValueType: string; Flags: uninsdeletevalue; Components: xalanComp
Root: HKCU; Subkey: "Environment"; ValueName: ZLIBDIR; ValueData: {app}\ExtLib\Zlib; ValueType: string; Flags: uninsdeletevalue; Components: zlibComp
Root: HKCU; Subkey: "Environment"; ValueName: CBIOSDIR; ValueData: {app}\ExtLib\Zlib; ValueType: string; Flags: uninsdeletevalue; Components: cbiosComp
Root: HKCU; Subkey: "Environment"; ValueName: FFMPEGDIR; ValueData: {app}\ExtLib\Zlib; ValueType: string; Flags: uninsdeletevalue; Components: ffmpegComp

Root: HKLM; Subkey: "SOFTWARE\\Classes\\arx_auto_file\\Open\\command"; ValueData: "{app}\\Bin\\Release$CompilerName$\\Compositor.exe %22%251%22"; ValueType: string; Flags: uninsdeletevalue; Components: acfSlnComp
