include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)

TARGET = CompositorPck

LIBS += -L../../../Lib/$$COMPILER_DIR -licmpstr -lipackage

QT += xml

include($(ACFDIR)/Config/QMake/AcfQt.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
