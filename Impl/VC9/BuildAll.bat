echo off

call "%VS90COMNTOOLS%vsvars32.bat" x86

msbuild AcfSlnAll.sln

pause
