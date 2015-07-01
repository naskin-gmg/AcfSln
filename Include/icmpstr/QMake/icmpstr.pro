TARGET = icmpstr

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

RESOURCES += ../*.qrc
FORMS += ../*.ui

LIBS += -licomp
LIBS += -liqtgui
