include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)
include($(ACFDIR)/Config/QMake/AcfQt.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)

TARGET = ProcessingGuiPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liproc -liprocgui

QT += xml
