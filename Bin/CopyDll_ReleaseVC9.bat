copy /Y %QTDIR%\bin\*.dll ReleaseVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ReleaseVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ReleaseVC9\iconengines\*.*

copy /Y %ACFDIR%\Bin\ReleaseVC8\Acf.exe ReleaseVC9

copy /Y %AVTDIR%\FireGrab\*.dll ReleaseVC9
copy /Y %MILDIR%\Dll\*.dll ReleaseVC9
copy /Y %OPENCVDIR%\Bin\*.dll ReleaseVC9
copy /Y %FFMPEGDIR%\bin\*.dll ReleaseVC9
