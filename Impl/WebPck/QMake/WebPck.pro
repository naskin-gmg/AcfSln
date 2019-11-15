TARGET = WebPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -liweb

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

