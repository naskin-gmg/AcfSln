include($(ACFDIR)/Config/QMake/ComponentConfig.pri)

TARGET = HotfolderPck

LIBS += -L../../../Lib/$$COMPILER_DIR -lihotf -lifileproc -liproc

include($(ACFDIR)/Config/QMake/AcfStd.pri)
