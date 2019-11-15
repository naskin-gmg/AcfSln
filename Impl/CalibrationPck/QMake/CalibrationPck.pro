TARGET = CalibrationPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -licalib

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

