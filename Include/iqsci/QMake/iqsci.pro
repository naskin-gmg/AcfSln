include($(ACFDIR)/Config/QMake/Include.config)
include($(ACFDIR)/Config/QMake/QtGuiBase.config)

TARGET = iqsci

QT += xml

INCLUDEPATH += ../..
INCLUDEPATH += "$(QScintilla)/include" 

