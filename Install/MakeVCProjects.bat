cd Temp

echo Generating VC8 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2005
cd Build\QMake
qmake -recursive -tp vc AcfAll.pro
cd ..\..

call %ACFDIR%\Config\QMake\CopyVCProjToSubdir.js VC8

echo Generating VC9 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2008
cd Build\QMake
qmake -recursive -tp vc AcfSlnAll.pro
cd ..\..

call %ACFDIR%\Config\QMake\CopyVCProjToSubdir.js VC9

cd ..
