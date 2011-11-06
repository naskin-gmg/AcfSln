include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = QsciPck

QT += xml

INCLUDEPATH += ../../../Include
INCLUDEPATH += "$(QScintilla)/include" 

LIBS += -L../../../Lib/$$COMPILER_DIR -liqsci
LIBS += -L$(QScintilla)/Lib/$$COMPILER_DIR -lQScintilla

CONFIG(debug, debug|release){
	LIBS += QAxContainerd
}
CONFIG(release, debug|release){
	LIBS += QAxContainer
}

