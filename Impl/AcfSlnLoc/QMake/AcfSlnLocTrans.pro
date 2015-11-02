include($(ACFCONFIGDIR)/QMake/GeneralConfig.pri)

TEMPLATE = aux
TARGET = none

OBJECTS_DIR = ./
DLLDESTDIR = ./

HEADERS =
SOURCES =

ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts
ACF_TRANSLATIONS_OUTDIR = $$AUXINCLUDEPATH/GeneratedFiles/AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
