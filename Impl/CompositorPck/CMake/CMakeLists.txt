project(CompositorPck LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/ComponentConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

find_package("Qt${QT_VERSION_MAJOR}" COMPONENTS PrintSupport REQUIRED)

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(${PROJECT_NAME} icmpstr)

