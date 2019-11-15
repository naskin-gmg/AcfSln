TARGET = ProductionPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -liprod

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

