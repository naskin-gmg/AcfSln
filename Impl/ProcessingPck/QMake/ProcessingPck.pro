include($(ACFDIR)/Config/QMake/ComponentConfig.pri)

TARGET = ProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liproc

include($(ACFDIR)/Config/QMake/AcfStd.pri)
