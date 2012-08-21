echo Generating Visual Studio 2005 projects...

cd ..
call %ACFDIR%\Config\QMake\CopyProToSubdir.js VC8
cd Build\VC8

set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2005
qmake -recursive -tp vc AcfSlnAll.pro

del /q Static.sln
del /q Component.sln

cd ..\..
call %ACFDIR%\Config\QMake\RemoveProFromSubdir.js VC8
cd Build

pause
