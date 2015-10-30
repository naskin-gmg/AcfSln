TARGET = Compositor

include($(ACFCONFIGDIR)/QMake/ApplicationConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

INCLUDEPATH += ../../../Include
INCLUDEPATH += ../../../Impl


RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

LIBS += -L../../../Lib/$$COMPILER_DIR -licmpstr -lAcfSlnLoc
LIBS += -lAcfLoc

HEADERS =

QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

# Set OS-specific build options:
win32-msvc*{
	QMAKE_CXXFLAGS += /wd4264

	# copying all Qt DLLs to destination directory
	greaterThan(QT_MAJOR_VERSION, 4): QMAKE_POST_LINK = set path=$(QTDIR)\bin;%path% && $(QTDIR)\bin\windeployqt $$DESTDIR
}

!macx-ios*{
	mac{
		ICON += $$PWD/../Mac/Compositor.icns
		QMAKE_INFO_PLIST = $$PWD/../Mac/Info.plist
	}
}

# Set configuration of custom builds:
# ARX Compiler:
ARXC_CONFIG = $$PWD/../../../Config/AcfSlnConfig.awc
ARXC_FILES += $$PWD/../*.acc
ARXC_OUTDIR = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET

# Conversion of resource templates:
win*{
	# File transformation
	ACF_CONVERT_FILES = $$PWD/../VC/*.rc.xtracf
	ACF_CONVERT_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/$$TARGET
	ACF_CONVERT_REGISTRY = $$PWD/../../../Partitura/AcfInfoCopyApp.acc
	ACF_CONVERT_CONFIG = $$PWD/../../../Config/BaseOnly.awc

	RC_FILE = $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/$$TARGET/Compositor.rc
	RC_INCLUDEPATH = $$_PRO_FILE_PWD_
}

# Get build output directory of shadow build:
ACFSLNDIRBUILD = $$(ACFSLNDIR_BUILD)
!isEmpty(ACFSLNDIRBUILD){
	INCLUDEPATH += $$(ACFSLNDIR_BUILD)/$$AUXINCLUDEDIR
}

include(../../../../Acf/Config/QMake/AcfQt.pri)
include(../../../../Acf/Config/QMake/AcfStd.pri)
include(../../../../Acf/Config/QMake/CustomBuild.pri)

# For iOS build generates qmake a XCode project without applying of enivronment variables.
# This causes build problems in case of shadow build. Following solution is a workaround for this problem.
# We create the output of the ARX-compiler before the build of the XCode project will be started.
# Need to be removed after fix in Qt.
macx-ios*{
	GenerateCompositorCpp.name = GenerateCompositor
	GenerateCompositorCpp.CONFIG += no_link target_predeps
	GenerateCompositorCpp.commands = $$ARXCBIN $$PWD/../Compositor.acc -o $$OUT_PWD/$$AUXINCLUDEPATH/GeneratedFiles/Compositor/CCompositor.cpp -config $$PWD/../../../Config/AcfSlnConfig.awc -v
	QMAKE_EXTRA_COMPILERS += GenerateCompositorCpp
	QMAKE_EXTRA_TARGETS += GenerateCompositorCpp
	PRE_TARGETDEPS += GenerateCompositorCpp
}


