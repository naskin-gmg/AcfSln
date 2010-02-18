echo off

echo Building ACF-Solutions

cd ..\Impl\%COMPILER_EXT%
vcbuild /r AcfSlnAll.sln $All

echo Building ACF-Solutions done

rem echo Building ACF-Solutions Tutorials

rem cd ..\..\Docs\Tutorial\Impl\%COMPILER_EXT%
rem vcbuild /r TutorialAcf.sln $All
rem echo Building ACF-Solutions Tutorials done

rem cd ..\..\..\..\Install

cd ..\..\Install
