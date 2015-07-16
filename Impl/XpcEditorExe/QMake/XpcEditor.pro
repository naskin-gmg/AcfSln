TARGET = XpcEditor

include($(ACFCONFIGDIR)/QMake/ApplicationConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl

INCLUDEPATH += $$AUXINCLUDEPATH

RESOURCES += ../*.qrc

LIBS += -L../../../Lib/$$COMPILER_DIR -licmpstr -lAcfSlnLoc
LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx
ARXC_OUTDIR = ../../GeneratedFiles/XpcEditor

mac{
	ICON += ../Mac/XpcEditor.icns
	QMAKE_INFO_PLIST = ../Mac/Info.plist
}


include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
