include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = QsciPck

QT += xml

LIBS += -L"$(QScintilla)/Lib/$$COMPILER_DIR
LIBS += -lQScintilla

CONFIG(debug, debug|release){
	LIBS += QAxContainerd
}
CONFIG(release, debug|release){
	LIBS += QAxContainer
}

INCLUDEPATH += "$(QScintilla)/include" 

