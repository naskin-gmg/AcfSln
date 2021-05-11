TARGET = ihotfgui

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

RESOURCES += $$files($$_PRO_FILE_PWD_/../*.qrc, false)
FORMS += $$files($$_PRO_FILE_PWD_/../*.ui, false)

