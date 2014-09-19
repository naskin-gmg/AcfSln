TARGET = AcfStarter

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += ../../../Include

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/AcfStarter

LIBS += -liqtgui
