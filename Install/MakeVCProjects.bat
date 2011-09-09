cd Temp

echo Generating VC8 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2005
cd Impl\QMake
qmake -recursive -tp vc AcfAll.pro
cd ..\..

call %ACFDIR%\Config\QMake\CopyQMakeToSubdir.js VC8

echo Generating VC9 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2008
cd Impl\QMake
qmake -recursive -tp vc AcfSlnAll.pro
cd ..\..

call %ACFDIR%\Config\QMake\CopyQMakeToSubdir.js VC9

cd ..
