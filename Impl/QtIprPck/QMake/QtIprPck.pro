include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)

TARGET = QtIprPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liqtipr -liipr -limeas

include($(ACFDIR)/Config/QMake/AcfQt.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
