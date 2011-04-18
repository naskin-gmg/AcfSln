include($(ACFDIR)/Config/QMake/Application.config)
include($(ACFDIR)/Config/QMake/QtBase.config)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = Compositor

LIBS += -licmpstr

QT += xml 

SOURCES += ../*.cpp
ARXC_CONFIG = ../../../Full.xpc
ARXC_FILES += ../*.arx
RESOURCES += ../*.qrc
