project(icmpstr LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS PrintSupport REQUIRED)

target_link_libraries(${PROJECT_NAME}  Qt${QT_VERSION_MAJOR}::PrintSupport)

