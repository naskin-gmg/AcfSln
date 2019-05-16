include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)

TARGET = WebPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liweb

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
