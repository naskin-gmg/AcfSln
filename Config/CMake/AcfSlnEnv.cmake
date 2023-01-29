# Acf
if(NOT DEFINED ACFDIR)
	set(ACFDIR "$ENV{ACFDIR}")
endif()

include(${ACFDIR}/Config/CMake/AcfEnv.cmake)

# ACF-Solutions
if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()

include_directories("${ACFSLNDIR}/AuxInclude/${TARGETNAME}")
include_directories("${ACFSLNDIR}/Include")
include_directories("${ACFSLNDIR}/Impl")

link_directories(${ACFSLNDIR}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})

