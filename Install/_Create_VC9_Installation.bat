echo off

set COMPILER_EXT=VC9
set LICENSE_TYPE=LGPL
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=%~dp0/../Config/Core.xpc

cd %~dp0/..

call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

call %ACFDIR%\Install\InternalParts\Rebuild.bat

call %ACFDIR%\Install\InternalParts\CopyDlls.bat

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat

call %ACFDIR%\Install\InternalParts\Compile_Setup.bat

echo Installation done

call %ACFDIR%\Install\InternalParts\RemoveTempDirs.bat
