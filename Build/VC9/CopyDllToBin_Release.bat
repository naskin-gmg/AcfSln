copy /Y %QTDIR%\bin\*.dll ..\..\Bin\ReleaseVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ..\..\Bin\ReleaseVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ..\..\Bin\ReleaseVC9\iconengines\*.*

copy /Y %ACFDIR%\Bin\ReleaseVC8\Acf.exe ..\..\Bin\ReleaseVC9

