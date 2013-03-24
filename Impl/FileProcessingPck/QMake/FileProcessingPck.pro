include($(ACFDIR)/Config/QMake/ComponentConfig.pri)

TARGET = FileProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lifileproc -liproc

include($(ACFDIR)/Config/QMake/AcfStd.pri)
