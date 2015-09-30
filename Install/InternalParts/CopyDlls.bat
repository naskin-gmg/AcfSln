echo off

echo Preparing ACF framework

xcopy /Y %QTDIR%\bin\icu*.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Cored.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Guid.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Widgetsd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Xmld.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Svgd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Scriptd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?PrintSupportd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?XmlPatternsd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\bin\Qt?Networkd.dll Bin\DebugVC12\
xcopy /Y %QTDIR%\plugins\imageformats\qsvgd.dll Bin\DebugVC12\imageformats\
xcopy /Y %QTDIR%\plugins\iconengines\qsvgicond.dll Bin\DebugVC12\iconengines\

xcopy /Y %QTDIR%\bin\icu*.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Core.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Gui.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Widgets.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Xml.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Svg.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Script.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?PrintSupport.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?XmlPatterns.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\bin\Qt?Network.dll Bin\ReleaseVC12\
xcopy /Y %QTDIR%\plugins\imageformats\qsvg.dll Bin\ReleaseVC12\imageformats\
xcopy /Y %QTDIR%\plugins\iconengines\qsvgicon.dll Bin\ReleaseVC12\iconengines\

echo Preparing ACF framework done

