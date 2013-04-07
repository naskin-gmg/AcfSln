include($(ACFDIR)/Config/QMake/StaticConfig.pri)


win32-msvc* | *-gcc*{
	QMAKE_CXXFLAGS += /openmp
}


TARGET = iipr
