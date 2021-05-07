TARGET = ihotfgui

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

RESOURCES += $$files($$_PRO_FILE_PWD_/../*.qrc, true)
FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, true)

