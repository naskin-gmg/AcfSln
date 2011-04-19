include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)

TARGET = FpfPck

INCLUDEPATH += ../../../Include

LIBS += -L../../../Lib/$$COMPILER_DIR -lifpf

