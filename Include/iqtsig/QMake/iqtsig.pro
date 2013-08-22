include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

TARGET = iqtsig

FORMS += ../*.ui
UI_DIR = ../../GeneratedFiles/iqtsig

QT += script
