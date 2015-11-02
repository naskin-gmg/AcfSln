greaterThan(QT_MAJOR_VERSION, 4) {
TEMPLATE = subdirs
CONFIG += ordered

# ensure Makefile with debug and release configuration on Mac
CONFIG += x86
CONFIG += debug_and_release

SUBDIRS +=	AcfSlnLocTrans.pro
SUBDIRS +=	AcfSlnLocRcc.pro
}
else {
TARGET = AcfSlnLoc

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)

HEADERS =
SOURCES =
ACF_TRANSLATIONS += $$_PRO_FILE_PWD_/../*.ts
GENERATED_RESOURCES += $$_PRO_FILE_PWD_/../*.qrc

include($(ACFCONFIGDIR)/QMake/CustomBuild.pri)
}