include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = FileProcessingPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -lfileproc

