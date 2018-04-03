TARGET = iqtmeas

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)
include($(ACFSLNDIR)/Config/QMake/QtCharts.pri)

FORMS += $$_PRO_FILE_PWD_/../*.ui