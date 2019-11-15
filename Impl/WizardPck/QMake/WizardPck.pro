TARGET = WizardPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -liwiz -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

