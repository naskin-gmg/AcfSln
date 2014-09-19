TARGET = HotfolderPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -lihotf -lifileproc -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
