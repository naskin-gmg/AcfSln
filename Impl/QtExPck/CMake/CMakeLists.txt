project(QtExtPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

if(QT_VERSION_MAJOR EQUAL 5)
    find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS XmlPatterns REQUIRED)
    target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::XmlPatterns)
endif()
target_link_libraries(${PROJECT_NAME} iservice iqtex )
