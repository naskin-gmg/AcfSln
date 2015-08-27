TARGET = XpcEditor

include($(ACFCONFIGDIR)/QMake/ApplicationConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl

RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

LIBS += -L../../../Lib/$$COMPILER_DIR -licmpstr -lAcfSlnLoc
LIBS += -lAcfLoc

HEADERS =

win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264
}

QT += xml

mac{
	ICON += $$PWD/../Mac/$$TARGET.icns
	QMAKE_INFO_PLIST = $$PWD/../Mac/Info.plist
}


# configuration of custom builds

# ARX compiler
ARXC_CONFIG = $$PWD/../../../Config/Core.xpc
ARXC_FILES += $$PWD/../*.arx
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

win*{
	# File transformation
	ACF_CONVERT_FILES = $$PWD/../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET
	ACF_CONVERT_REGISTRY = $$PWD/../../../Partitura/AcfInfoCopyApp.arx
	ACF_CONVERT_CONFIG = $$PWD/../../../Config/BaseOnly.xpc

	RC_FILE = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET/XpcEditor.rc
	RC_INCLUDEPATH = $$_PRO_FILE_PWD_
}


include(../../../../Acf/Config/QMake/CustomBuild.pri)
include(../../../../Acf/Config/QMake/AcfQt.pri)
include(../../../../Acf/Config/QMake/AcfStd.pri)
