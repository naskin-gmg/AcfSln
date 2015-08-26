TARGET = iqtauth

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

INCLUDEPATH += ../..

FORMS += $$_PRO_FILE_PWD_/../*.ui
