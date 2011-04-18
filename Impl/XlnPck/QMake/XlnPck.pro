include(../../../Config/QMake/Component.config)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = XlnPck

INCLUDEPATH += "$(XERCESDIR)/src" "$(XALANDIR)/src" 

LIBS += -lixln

