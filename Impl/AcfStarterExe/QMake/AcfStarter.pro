include($(ACFDIR)/Config/QMake/StaticConfig.pri)
include($(ACFDIR)/Config/QMake/QtGuiBaseConfig.pri)

TARGET = AcfStarter

RESOURCES += ../*.qrc

LIBS += -liqtgui
