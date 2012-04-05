echo Generating VC10 projects...

cd ..
call %ACFDIR%\Config\QMake\CopyProToSubdir.js VC10
cd Build\VC10

set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2010
qmake -recursive -tp vc AcfSlnAll.pro

del /q Static.sln
del /q Component.sln
del /q Tool.sln

cd ..\..
call %ACFDIR%\Config\QMake\RemoveProFromSubdir.js VC10
cd Build

pause
