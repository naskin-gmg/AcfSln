include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)

TARGET = ProcessingGuiPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liprocgui -liproc

QT += xml

include($(ACFDIR)/Config/QMake/AcfStd.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)
