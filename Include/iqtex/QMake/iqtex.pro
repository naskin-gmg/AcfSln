include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqtex

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtex

QT += network
