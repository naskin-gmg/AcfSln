project(icmpstr LANGUAGES CXX)

include(${ACFDIR}/Config/CMake/StaticConfig.cmake)

find_package("Qt${QT_DEFAULT_MAJOR_VERSION}" COMPONENTS PrintSupport REQUIRED)
qt5_use_modules(${PROJECT_NAME} PrintSupport)
