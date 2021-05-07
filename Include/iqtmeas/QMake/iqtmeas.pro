TARGET = iqtmeas

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, true)

