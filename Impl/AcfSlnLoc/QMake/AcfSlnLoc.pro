TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)

HEADERS =
SOURCES =
ACF_TRANSLATIONS += ../*.ts
RESOURCES += ../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
