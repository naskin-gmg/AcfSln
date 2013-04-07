include($(ACFDIR)/Config/QMake/ComponentConfig.pri)
include($(ACFDIR)/Config/QMake/QtBaseConfig.pri)

TARGET = IprPck

LIBS += -L../../../Lib/$$COMPILER_DIR -liipr -limeas -liproc

win32-msvc* | *-gcc*{
    LIBS += -lgomp
}


include($(ACFDIR)/Config/QMake/AcfQt.pri)
include($(ACFDIR)/Config/QMake/AcfStd.pri)
