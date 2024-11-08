TARGET = JsonArchiveIntergrationTest

include(../../../../Config/QMake/ApplicationConfig.pri)
include(../../../../Config/QMake/QtBaseConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR
LIBS += -listd -liser -liprm -litest -limeas

QT += core gui testlib

CONFIG += console

include(../../../..//Config/QMake/AcfQt.pri)
include(../../../..//Config/QMake/AcfStd.pri)
include(../../../..//Config/QMake/CustomBuild.pri)

