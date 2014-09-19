TARGET = iqtsig

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtsig

QT += script
