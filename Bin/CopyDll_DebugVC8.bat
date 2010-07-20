copy /Y %QTDIR%\bin\*.dll DebugVC8
copy /Y %QTDIR%\bin\*.dll.Manifest DebugVC8
xcopy /Y %QTDIR%\plugins\imageformats\*.* DebugVC8\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* DebugVC8\iconengines\*.*

copy /Y %ACFDIR%\Bin\DebugVC8\Acf.exe DebugVC8

copy /Y %AVTDIR%\FireGrab\*.dll DebugVC8
copy /Y %MILDIR%\Dll\*.dll DebugVC8
copy /Y %OPENCVDIR%\Bin\*.dll DebugVC8
copy /Y %FFMPEGDIR%\bin\*.dll DebugVC8
