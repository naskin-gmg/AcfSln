copy /Y %QTDIR%\bin\*.dll DebugVC9
copy /Y %QTDIR%\bin\*.dll.Manifest DebugVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* DebugVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* DebugVC9\iconengines\*.*

copy /Y %ACFDIR%\Bin\DebugVC8\Acf.exe DebugVC9

copy /Y %AVTDIR%\FireGrab\*.dll DebugVC9
copy /Y %MILDIR%\Dll\*.dll DebugVC9
copy /Y %OPENCVDIR%\Bin\*.dll DebugVC9
copy /Y %FFMPEGDIR%\bin\*.dll DebugVC9
