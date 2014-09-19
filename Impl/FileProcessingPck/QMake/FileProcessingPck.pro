TARGET = FileProcessingPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -lifileproc -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
