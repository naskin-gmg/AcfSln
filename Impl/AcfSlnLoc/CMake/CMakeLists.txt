project(AcfSlnLoc LANGUAGES CXX)

set(ARXC_CONFIG "${ACFDIR}/Config/AcfConfig.awc")
set(ARXC_FILES "${PROJECT_SOURCE_DIR}/../${PROJECT_NAME}.acc")
set(ACF_CONVERT_FILES "")

include(${ACFDIR}/Config/CMake/CustomBuild.cmake)
include(${ACFDIR}/Config/CMake/StaticConfig.cmake)
