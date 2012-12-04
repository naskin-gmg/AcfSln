include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = WizardPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liwiz -liproc

