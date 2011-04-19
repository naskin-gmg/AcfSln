include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)

TARGET = ZlibPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -lizlib

