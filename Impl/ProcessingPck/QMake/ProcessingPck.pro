include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = ProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liproc

