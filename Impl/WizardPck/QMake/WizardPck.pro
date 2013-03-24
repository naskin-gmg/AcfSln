include($(ACFDIR)/Config/QMake/ComponentConfig.pri)

TARGET = WizardPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liwiz -liproc

include($(ACFDIR)/Config/QMake/AcfStd.pri)
