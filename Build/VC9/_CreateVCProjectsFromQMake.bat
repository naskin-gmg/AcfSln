@echo off

set COMPILER_EXT=VC9
set QMAKESPEC=%QTDIR%\mkspecs\win32-msvc2008
set path=%path%;%QTDIR%\bin

cd %~dp0\..\..

echo Generating %COMPILER_EXT% projects...
cd Build\QMake
%QTDIR%\bin\qmake -recursive -tp vc
cd %~dp0\..\..

call %ACFDIR%\Config\QMake\CopyVCProjToSubdir.js %COMPILER_EXT% -replace%QTDIR%=$(QTDIR) -replace%ACFDIR%\=$(ACFDIR)\ -replace%ACF_TOOLS_BIN%\=$(ACF_TOOLS_BIN)\ -replace%CD%\=$(RelativePath)\ -qt_verWin32=QT_VC9_32


cd %~dp0\
