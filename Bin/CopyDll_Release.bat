copy /Y %QTDIR%\lib\*.dll ReleaseVC8
copy /Y %QTDIR%\lib\*.dll.Manifest ReleaseVC8
xcopy /Y %QTDIR%\plugins\imageformats\*.* ReleaseVC8\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ReleaseVC8\iconengines\*.*

copy /Y %ACFDIR%\Bin\ReleaseVC8\Acf.exe ReleaseVC8

copy /Y %AVTDIR%\FireGrab\*.dll ReleaseVC8
copy /Y %MILDIR%\Dll\*.dll ReleaseVC8
copy /Y %OPENCVDIR%\Bin\*.dll ReleaseVC8
copy /Y %FFMPEGDIR%\lib\*.dll ReleaseVC8
