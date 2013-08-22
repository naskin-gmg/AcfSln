include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = icmpstr

RESOURCES += ../*.qrc
FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/icmpstr

LIBS += -licomp
LIBS += -liqtgui
