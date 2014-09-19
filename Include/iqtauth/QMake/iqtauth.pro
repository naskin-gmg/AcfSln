TARGET = iqtauth

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += ../..

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtauth
