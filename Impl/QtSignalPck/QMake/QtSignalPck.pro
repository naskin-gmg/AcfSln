include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = QtSignalPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -lisig -liqtsig -limeas

QT += script
