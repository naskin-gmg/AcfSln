echo off

set LICENSE_TYPE=Public
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=%~dp0/../Config/AcfSlnConfig.awc

call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

set COMPILER_EXT=VC12

call %~dp0\..\Build\%COMPILER_EXT%\_CreateVCProjectsFromQMake.bat

cd %~dp0\..

echo Compiling...
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfSlnAll.sln /p:Configuration=Debug
msbuild %~dp0\..\Build\%COMPILER_EXT%\AcfSlnAll.sln /p:Configuration=Release
echo Compiling done

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat

echo Installation done
