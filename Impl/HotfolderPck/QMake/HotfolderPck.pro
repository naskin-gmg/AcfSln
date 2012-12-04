include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = HotfolderPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lihotf -lifileproc -liproc

