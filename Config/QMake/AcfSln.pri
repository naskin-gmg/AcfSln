# Base configuration should be included for all projects used ACF-Solutions

INCLUDEPATH += $$PWD/../../Include
INCLUDEPATH += $$PWD/../../$$AUXINCLUDEDIR

# get build output directory of shadow build and add include path
ACFDIRBUILD = $$(ACFDIR_BUILD)
!isEmpty(ACFDIRBUILD){
	INCLUDEPATH += $$(ACFDIR_BUILD)/../AcfSln/$$AUXINCLUDEDIR
}

