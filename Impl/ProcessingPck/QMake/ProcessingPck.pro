include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

TARGET = ProcessingPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
