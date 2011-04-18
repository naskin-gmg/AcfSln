include(../../../Config/QMake/Component.config)
include(../../../Config/QMake/QtGuiBase.config)
include(../../AcfStd/QMake/AcfStd.pri)
include(../../AcfQt/QMake/AcfQt.pri)

TARGET = QwtPck

LIBS += -liqwt
LIBS += -L$(QWTDIR)\Lib\$$COMPILER_DIR -lQwt.lib
LIBS += -L$(QWT3DDIR)\Lib\$$COMPILER_DIR -lqwt3dlib.lib
