echo off

call "%VS90COMNTOOLS%vsvars32.bat" x86

call ..\Tortoise\UpdateVersion.cmd

cd /d %~dp0

msbuild AcfSlnAll.sln /m
msbuild AcfSlnAll.sln /p:Configuration=Release /m

pause
