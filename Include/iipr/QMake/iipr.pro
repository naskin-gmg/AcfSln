include($(ACFDIR)/Config/QMake/StaticConfig.pri)


win32-msvc*{
	QMAKE_CXXFLAGS += /openmp
}
else{
	QMAKE_CXXFLAGS += -fopenmp
}

TARGET = iipr
