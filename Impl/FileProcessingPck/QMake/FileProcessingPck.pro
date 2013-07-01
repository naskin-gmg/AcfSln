include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

TARGET = FileProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lifileproc -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
