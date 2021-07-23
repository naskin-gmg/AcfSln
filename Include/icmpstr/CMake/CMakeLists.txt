project(icmpstr LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
include(${ACFDIR}/Config/CMake/AcfQt.cmake)
include(${ACFDIR}/Config/CMake/AcfStd.cmake)

find_package("Qt${QT_DEFAULT_MAJOR_VERSION}" COMPONENTS PrintSupport REQUIRED)
qt5_use_modules(${PROJECT_NAME} PrintSupport)
