include($(ACFDIR)/Config/QMake/StaticConfig.pri)
include($(ACFDIR)/Config/QMake/QtGuiBaseConfig.pri)

TARGET = AcfStarter

INCLUDEPATH += ../../../Include

LIBS += -liqtgui
