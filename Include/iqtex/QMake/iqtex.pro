TARGET = iqtex

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtex

QT += network
