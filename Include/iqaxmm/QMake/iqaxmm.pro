include($(ACFDIR)/Config/QMake/StaticConfig.pri)

TARGET = iqaxmm
QT += phonon

INCLUDEPATH += ../..

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

