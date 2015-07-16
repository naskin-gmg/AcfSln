TARGET = iservice

include($(ACFCONFIGDIR)/QMake/StaticConfig.pri)
include($(ACFCONFIGDIR)/QMake/QtGuiBaseConfig.pri)

win32-msvc*{
	HEADERS = ../qtservice.h ../qtservice_win.h ../qtservicestarter.h ../CServiceApplicationComp.h
	SOURCES = ../qtservice.cpp ../qtservice_win.cpp ../qtservicestarter.cpp
	SOURCES += ../CServiceApplicationComp.cpp
}

