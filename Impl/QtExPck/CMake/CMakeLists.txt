project(QtExPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)
include(${ACFDIR}/Config/CMake/AcfStdGui.cmake)
include(${ACFDIR}/Config/CMake/WindeployQt.cmake)

acf_use_qt_graphics_modules()

if(QT_VERSION_MAJOR EQUAL 5)
    find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS XmlPatterns REQUIRED)
    target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::XmlPatterns)
endif()
target_link_libraries(${PROJECT_NAME} iservice iqtex )

if(WIN32)
	set(listFiles \"${AUX_INCLUDE_DIR}/../../../Bin/${CMAKE_BUILD_TYPE}_${TARGETNAME}/${PROJECT_NAME}.arp\")

	windeploy(${PROJECT_NAME} "" "${listFiles}")
	unset(listOptions)
endif()
