TARGET = iservice

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

win32-msvc*{
	HEADERS = $$_PRO_FILE_PWD_/../qtservice.h $$_PRO_FILE_PWD_/../qtservice_win.h $$_PRO_FILE_PWD_/../qtservicestarter.h $$_PRO_FILE_PWD_/../CServiceApplicationComp.h
	SOURCES = $$_PRO_FILE_PWD_/../qtservice.cpp $$_PRO_FILE_PWD_/../qtservice_win.cpp $$_PRO_FILE_PWD_/../qtservicestarter.cpp
	SOURCES += $$_PRO_FILE_PWD_/../CServiceApplicationComp.cpp
}
else
{
	SOURCES = $$_PRO_FILE_PWD_/../CServiceApplicationComp.cpp
	HEADERS = $$_PRO_FILE_PWD_/../CServiceApplicationComp.h
}

