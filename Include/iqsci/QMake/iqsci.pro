include($(ACFDIR)/Config/QMake/StaticConfig.pri)
include($(ACFDIR)/Config/QMake/QtGuiBaseConfig.pri)

TARGET = iqsci

QT += xml

INCLUDEPATH += ../..
INCLUDEPATH += "$(QScintilla)/include" 

