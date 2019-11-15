TARGET = ProcessingPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

