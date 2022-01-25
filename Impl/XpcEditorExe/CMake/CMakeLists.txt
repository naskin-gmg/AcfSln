project(XpcEditor LANGUAGES CXX)

set(ARXC_CONFIG "${ACFSLNDIR}/Config/AcfSlnConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")

if(WIN32)
	set(CMAKE_WIN32_EXECUTABLE "on")
	set(ACF_CONVERT_FILES "${PROJECT_SOURCE_DIR}/../VC/XpcEditor.rc.xtracf")
	set(ACF_CONVERT_REGISTRY "${PROJECT_SOURCE_DIR}/../../../Partitura/AcfInfoCopyApp.acc")
	set(ACF_CONVERT_CONFIG  "${PROJECT_SOURCE_DIR}/../../../Config/BaseOnly.awc")
endif()

if(APPLE)
	set(CMAKE_MACOSX_BUNDLE "on")
endif()

set(BUILD_ARGS "dupbuild=warn")
include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
include(${ACFDIR}/Config/CMake/ApplicationConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

target_link_libraries(${PROJECT_NAME} AcfLoc icmpstr AcfSlnLoc)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS PrintSupport REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::PrintSupport)


