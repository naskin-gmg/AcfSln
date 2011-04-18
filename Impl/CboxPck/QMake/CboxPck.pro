include(../../../Config/QMake/Component.config)
include(../../AcfStd/QMake/AcfStd.pri)

TARGET = CboxPck

QT -= gui core

LIBS += -L"$(CBIOSDIR)/lib/$$COMPILER_NAME" -lcbios -lwsock32

INCLUDEPATH += "$(CBIOSDIR)/include" 


