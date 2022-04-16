#Acf

if(NOT DEFINED ACFCONFIGDIR)
	set(ACFCONFIGDIR "$ENV{ACFCONFIGDIR}")
endif()

include(${ACFCONFIGDIR}/CMake/AcfEnv.cmake)


#Sln
if(NOT DEFINED ACFSLNDIR)
	set(ACFSLNDIR "$ENV{ACFSLNDIR}")
endif()
if(NOT DEFINED CONFIGSLN_DIR)
	set(CONFIGSLN_DIR "$ENV{ACFSLNDIR}/Config")
endif()


include_directories("${ACFSLNDIR}/AuxInclude/${TARGETNAME}")
include_directories("${ACFSLNDIR}/Include")
include_directories("${ACFSLNDIR}/Impl")

link_directories(${ACFSLNDIR}/Lib/${CMAKE_BUILD_TYPE}_${TARGETNAME})

