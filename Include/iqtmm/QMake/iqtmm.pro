include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqtmm

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtmm

QT += xml svg
