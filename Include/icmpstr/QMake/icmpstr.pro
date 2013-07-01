include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = icmpstr

RESOURCES += ../*.qrc

LIBS += -licomp
LIBS += -liqtgui
