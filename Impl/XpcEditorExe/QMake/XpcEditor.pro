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
	ICON += $$PWD/../Mac/XpcEditor.icns
	QMAKE_INFO_PLIST = $$PWD/../Mac/Info.plist
}


# configuration of custom builds

# ARX compiler
ARXC_CONFIG = $$PWD/../../../Config/AcfSlnConfig.awc
ARXC_FILES += $$PWD/../*.acc
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

win*{
	# File transformation
	ACF_CONVERT_FILES = $$PWD/../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET
	ACF_CONVERT_REGISTRY = $$PWD/../../../Partitura/AcfInfoCopyApp.acc
	ACF_CONVERT_CONFIG = $$PWD/../../../Config/BaseOnly.awc

	RC_FILE = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET/XpcEditor.rc
	RC_INCLUDEPATH = $$_PRO_FILE_PWD_
}


# get build output directory of shadow build
ACFSLNDIRBUILD = $$(ACFSLNDIR_BUILD)
!isEmpty(ACFSLNDIRBUILD){
	INCLUDEPATH += $$(ACFSLNDIR_BUILD)/$$AUXINCLUDEDIR
}

include(../../../../Acf/Config/QMake/CustomBuild.pri)
include(../../../../Acf/Config/QMake/AcfQt.pri)
include(../../../../Acf/Config/QMake/AcfStd.pri)


# For iOS build generates qmake a XCode project without applying of enivronment variables.
# This causes build problems in case of shadow build. Following solution is a workaround for this problem.
# We create the output of the ARX-compiler before the build of the XCode project will be started.
# Need to be removed after fix in Qt.

macx-ios*{
	GenerateXpcEditorCpp.name = GenerateXpcEditorCpp
	GenerateXpcEditorCpp.CONFIG += no_link target_predeps
	GenerateXpcEditorCpp.commands = $$ARXCBIN $$PWD/../XpcEditor.acc -o $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/XpcEditor/CXpcEditor.cpp -config $$PWD/../../../Config/AcfSlnConfig.awc -v
	QMAKE_EXTRA_COMPILERS += GenerateXpcEditorCpp
	QMAKE_EXTRA_TARGETS += GenerateXpcEditorCpp
	PRE_TARGETDEPS += GenerateXpcEditorCpp
}


