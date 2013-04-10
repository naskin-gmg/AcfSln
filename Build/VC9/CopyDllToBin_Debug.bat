copy /Y %QTDIR%\bin\*.dll ..\..\Bin\DebugVC9
xcopy /Y %QTDIR%\plugins\imageformats\*.* ..\..\Bin\DebugVC9\imageformats\*.*
xcopy /Y %QTDIR%\plugins\iconengines\*.* ..\..\Bin\DebugVC9\iconengines\*.*

copy /Y %ACFDIR%\Bin\ReleaseVC8\Acf.exe ..\..\Bin\DebugVC9

