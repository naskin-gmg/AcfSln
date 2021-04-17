project(Compositor LANGUAGES CXX)

set(ARXC_CONFIG "${ACFSLNDIR}/Config/AcfSlnConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ARXC_OUTDIR "${AUX_INCLUDE_DIR}")

if(WIN32)
	set(CMAKE_WIN32_EXECUTABLE "on")
	set(ACF_CONVERT_FILES "${PROJECT_SOURCE_DIR}/../VC/Compositor.rc.xtracf")
	set(ACF_CONVERT_REGISTRY "${PROJECT_SOURCE_DIR}/../../../Partitura/AcfInfoCopyApp.acc")
	set(ACF_CONVERT_CONFIG  "${PROJECT_SOURCE_DIR}/../../../Config/BaseOnly.awc")
	set(RC_FILE "${AUX_INCLUDE_DIR}/${PROJECT_NAME}/Compositor.rc")	
endif()

if(APPLE)
	set(CMAKE_MACOSX_BUNDLE "on")
endif()


include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
#include(${ACFDIR}/Config/CMake/QtGuiBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

target_link_libraries(${PROJECT_NAME} AcfLoc icmpstr AcfSlnLoc)

find_package("Qt${QT_DEFAULT_MAJOR_VERSION}" COMPONENTS PrintSupport REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::PrintSupport)

if(WIN32)
windeployqt(${PROJECT_NAME})
endif()
