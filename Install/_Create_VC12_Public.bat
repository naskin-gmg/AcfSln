echo off

set LICENSE_TYPE=Public
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=%~dp0/../Config/AcfSlnConfig.awc

call %~dp0\..\Build\VC12\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

set COMPILER_EXT=VC12
call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

call Install\InternalParts\CopyDlls.bat

echo Compiling...
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfSlnAll.sln /p:Configuration=Debug
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfSlnAll.sln /p:Configuration=Release
echo Compiling done

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat

echo Installation done
