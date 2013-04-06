include($(ACFDIR)/Config/QMake/ApplicationConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)

TARGET = XpcEditor

RESOURCES += ../*.qrc

LIBS += -licmpstr
LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml

ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

mac{
	ICON += ../Mac/XpcEditor.icns
	QMAKE_INFO_PLIST = ../Mac/Info.plist
}


include($(ACFDIR)/Config/QMake/CustomBuild.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
