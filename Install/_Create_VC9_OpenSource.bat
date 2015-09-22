echo off

set COMPILER_EXT=VC9
set LICENSE_TYPE=OpenSource
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=%~dp0/../Config/Core.xpc

call %~dp0\..\Build\VC10\_CreateVCProjectsFromQMake.bat
call %~dp0\..\Build\VC12\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat
