echo off

set COMPILER_EXT=VC9
set LICENSE_TYPE=OpenSource
set LICENSE_DIR=Install/LGPL
set LICENSE_INSTALL_PATH=../LGPL/License.txt
set ACF_CONFIG_FILE=$(ACFSLNDIR)/Config/Core.xpc

cd ..

call %ACFDIR%\Install\InternalParts\CreateTempDirs.bat

call %ACFDIR%\Install\InternalParts\CreateScripts.bat

call %ACFDIR%\Install\InternalParts\Create_TechnicalDoc.bat

call %ACFDIR%\Install\InternalParts\CopyTempFiles.bat

pause

