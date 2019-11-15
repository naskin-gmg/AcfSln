TARGET = iservice

include(../../../../Acf/Config/QMake/StaticConfig.pri)
include(../../../../Acf/Config/QMake/QtGuiBaseConfig.pri)

SOURCES = $$_PRO_FILE_PWD_/../CServiceApplicationComp.cpp
HEADERS = $$_PRO_FILE_PWD_/../CServiceApplicationComp.h

win32*{
	HEADERS += $$_PRO_FILE_PWD_/../qtservice.h
	HEADERS += $$_PRO_FILE_PWD_/../qtservice_win.h
	HEADERS += $$_PRO_FILE_PWD_/../qtservicestarter.h

	SOURCES += $$_PRO_FILE_PWD_/../qtservice.cpp
	SOURCES += $$_PRO_FILE_PWD_/../qtservice_win.cpp
	SOURCES += $$_PRO_FILE_PWD_/../qtservicestarter.cpp
}

