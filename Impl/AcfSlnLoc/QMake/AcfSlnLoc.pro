TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)

HEADERS =
SOURCES =
ACF_TRANSLATIONS += ../*.ts
GENERATED_RESOURCES += ../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
