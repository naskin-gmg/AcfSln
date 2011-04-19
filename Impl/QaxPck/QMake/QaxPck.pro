include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = QaxPck

QT += phonon

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liqaxmm

CONFIG(debug, debug|release){
	LIBS += QAxContainerd
}
CONFIG(release, debug|release){
	LIBS += QAxContainer
}

