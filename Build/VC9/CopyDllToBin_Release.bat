copy /Y %QTDIR%\bin\*.dll ..\..\Bin\ReleaseVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ..\..\Bin\ReleaseVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ..\..\Bin\ReleaseVC9\iconengines\*.*

copy /Y %ACFDIR%\Bin\ReleaseVC8\Acf.exe ..\..\Bin\ReleaseVC9

copy /Y %AVTDIR%\FireGrab\*.dll ..\..\Bin\ReleaseVC9
copy /Y %MILDIR%\Dll\*.dll ..\..\Bin\ReleaseVC9
copy /Y %OPENCVDIR%\Bin\*.dll ..\..\Bin\ReleaseVC9
copy /Y %FFMPEGDIR%\bin\*.dll ..\..\Bin\ReleaseVC9
