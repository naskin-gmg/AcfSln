include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

TARGET = HotfolderPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lihotf -lifileproc -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
