include($(ACFCONFIGDIR)/QMake/ApplicationConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

TARGET = Compositor

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl

RESOURCES += ../*.qrc

LIBS += -L../../../Lib/$$COMPILER_DIR -licmpstr -lAcfSlnLoc
LIBS += -lAcfLoc

HEADERS -= ../*.h

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

mac{
	ICON += ../Mac/Compositor.icns
	QMAKE_INFO_PLIST = ../Mac/Info.plist
}


# configuration of custom builds

# ARX compiler
ARXC_CONFIG = ../../../Config/Core.xpc
ARXC_FILES += ../*.arx

win*{
	# File transformation
	ACF_CONVERT_FILES = ../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = ../Generated
	ACF_CONVERT_REGISTRY = ../../../Partitura/AcfInfoCopyApp.arx
	ACF_CONVERT_CONFIG = ../../../Config/BaseOnly.xpc

	RC_FILE = ../Generated/Compositor.rc
}


include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
