TARGET = iqtmm

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtmm

QT += xml svg
