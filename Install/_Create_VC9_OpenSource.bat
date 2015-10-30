echo off

set LICENSE_TYPE=OpenSource
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=%~dp0/../Config/AcfSlnConfig.awc

call %~dp0\..\Build\VC10\_CreateVCProjectsFromQMake.bat
call %~dp0\..\Build\VC12\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

set COMPILER_EXT=VC9
call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat
