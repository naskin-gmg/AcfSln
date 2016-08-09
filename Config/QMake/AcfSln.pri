# Base configuration should be included for all projects used ACF-Solutions

INCLUDEPATH += $$PWD/../../Include
INCLUDEPATH += $$PWD/../../Impl
INCLUDEPATH += $$PWD/../../$$AUXINCLUDEDIR

# get build output directory of shadow build and add include path
ACFDIRBUILD = $$(ACFDIR_BUILD)
!isEmpty(ACFDIRBUILD){
	INCLUDEPATH += $$(ACFDIR_BUILD)/../AcfSln/$$AUXINCLUDEDIR
	LIBS += -L$$(ACFSLNDIR_BUILD)/Lib/$$COMPILER_DIR
}
else{
	LIBS += -L$$PWD/../../Lib/$$COMPILER_DIR
}

