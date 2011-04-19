include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)

TARGET = XlnPck

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(XERCESDIR)/src" "$(XALANDIR)/src" 

LIBS += -L../../../Lib/$$COMPILER_DIR -lixln
LIBS += -L$(XERCESDIR)/lib -lxerces-c_2
LIBS += -L$(XALANDIR)/lib -lXalan-C_1
