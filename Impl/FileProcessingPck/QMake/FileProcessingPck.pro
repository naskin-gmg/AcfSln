include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = FileProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lifileproc -iproc

