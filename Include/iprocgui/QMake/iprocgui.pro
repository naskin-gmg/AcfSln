include($(ACFDIR)/Config/QMake/StaticConfig.pri)
include($(ACFDIR)/Config/QMake/QtGuiBaseConfig.pri)

TARGET = iprocgui

LIBS += -liqt
LIBS += -liproc
