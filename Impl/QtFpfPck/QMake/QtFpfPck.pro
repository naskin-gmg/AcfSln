include($(ACFDIR)/Config/QMake/Component.config)
include($(ACFDIR)/Config/QMake/QtBase.config)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = QtFpfPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -lifpf -liqtfpf
