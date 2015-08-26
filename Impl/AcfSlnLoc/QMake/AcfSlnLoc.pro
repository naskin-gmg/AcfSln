TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)

HEADERS =
SOURCES =
ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts
GENERATED_RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
