TARGET = ProcessingGuiPck

include($(ACFCONFIGDIR)/QMake/ComponentConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtBaseConfig.pri)

QT += xml

include($(ACFCONFIGDIR)/QMake/AcfStd.pri)
include($(ACFCONFIGDIR)/QMake/AcfQt.pri)

LIBS += -L../../../Lib/$$COMPILER_DIR -liprocgui -liproc

