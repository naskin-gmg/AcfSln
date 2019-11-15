TARGET = iqtex

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += $$_PRO_FILE_PWD_/../*.ui

QT += network

