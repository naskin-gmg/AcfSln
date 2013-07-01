include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

TARGET = WizardPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liwiz -liproc

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
