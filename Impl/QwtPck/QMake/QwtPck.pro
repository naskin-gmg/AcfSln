include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Impl/AcfStd/QMake/AcfStd.pri)
include($(ACFDIR)/Impl/AcfQt/QMake/AcfQt.pri)

TARGET = QwtPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liqwt -liqwt3d
LIBS += -L$(QWTDIR)/Lib/$$COMPILER_DIR -lQwt.lib
LIBS += -L$(QWT3DDIR)/Lib/$$COMPILER_DIR -lqwt3dlib.lib

INCLUDEPATH += ../../../Include

