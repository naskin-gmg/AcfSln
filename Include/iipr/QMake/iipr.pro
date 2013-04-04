include($(ACFDIR)/Config/QMake/StaticConfig.pri)


win32-msvc*{
	QMAKE_CXXFLAGS += /openmp
}

*-clang* | *-llvm* | *-gcc*{
	QMAKE_CXXFLAGS += -fopenmp
}

TARGET = iipr
