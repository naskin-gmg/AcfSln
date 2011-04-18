include($(ACFDIR)/Config/QMake/Include.config)

TARGET = iqaxmm
QT += phonon

INCLUDEPATH += ../..

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

