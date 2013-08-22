include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = AcfStarter

INCLUDEPATH += ../../../Include

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/AcfStarter

LIBS += -liqtgui
