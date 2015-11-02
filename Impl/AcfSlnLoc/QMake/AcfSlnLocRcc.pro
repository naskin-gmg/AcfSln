TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
GENERATED_RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
