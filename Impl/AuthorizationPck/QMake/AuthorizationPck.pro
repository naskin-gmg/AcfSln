include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)

TARGET = AuthorizationPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -liauth
