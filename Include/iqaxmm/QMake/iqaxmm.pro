include($(ACFDIR)/Config/QMake/IncludeConfig.pri)

TARGET = iqaxmm
QT += phonon

INCLUDEPATH += ../..

HEADERS += ../Wrapped/*.h
SOURCES += ../Wrapped/*.cpp

