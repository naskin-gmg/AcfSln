TARGET = iqtmm

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += ../*.ui

QT += xml svg
