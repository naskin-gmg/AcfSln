TARGET = iqtex

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += ../*.ui

QT += network
