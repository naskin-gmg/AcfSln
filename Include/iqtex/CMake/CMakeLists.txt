project(iqtex LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

if(QT_VERSION_MAJOR EQUAL 5)
    find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS XmlPatterns REQUIRED)
    target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::XmlPatterns)
endif()
