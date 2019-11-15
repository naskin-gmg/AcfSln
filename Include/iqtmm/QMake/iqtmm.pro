TARGET = iqtmm

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += $$_PRO_FILE_PWD_/../*.ui

QT += xml svg

