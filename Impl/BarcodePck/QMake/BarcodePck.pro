TARGET = BarcodePck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -libarcode -liipr -liproc -limeas -liinsp

include($(ACFCONFIGDIR)/QMake/AcfQt.pri)
include($(ACFCONFIGDIR)/QMake/AcfStd.pri)

