TARGET = ihotfgui

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

RESOURCES += $$_PRO_FILE_PWD_/../*.qrc
FORMS += $$_PRO_FILE_PWD_/../*.ui

