project(iservice LANGUAGES CXX)


#include(${ACFDIR}/Config/CMake/QtGuiBaseConfig.cmake)
include(${ACFDIR}/Config/CMake/GeneralConfig.cmake)


file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/../*.h")
file(GLOB SOURCES_FILES "${PROJECT_SOURCE_DIR}/../*.cpp")
file(GLOB QRC_FILES "${PROJECT_SOURCE_DIR}/../*.qrc")




if (WIN32)
    add_compile_options(/utf-8)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
	file(GLOB HEADER_FILES
		"${PROJECT_SOURCE_DIR}/../CServiceApplicationComp.h"
		"${PROJECT_SOURCE_DIR}/../qtservice.h"
		"${PROJECT_SOURCE_DIR}/../qtservice_win.h"
		"${PROJECT_SOURCE_DIR}/../qtservicestarter.h"
		)
	file(GLOB SOURCES_FILES
		"${PROJECT_SOURCE_DIR}/../CServiceApplicationComp.cpp"
		"${PROJECT_SOURCE_DIR}/../qtservice.cpp"
		"${PROJECT_SOURCE_DIR}/../qtservice_win.cpp"
		"${PROJECT_SOURCE_DIR}/../qtservicestarter.cpp"
		)
else()
	file(GLOB HEADER_FILES
		"${PROJECT_SOURCE_DIR}/../CServiceApplicationComp.h"
	)
    file(GLOB SOURCES_FILES
		"${PROJECT_SOURCE_DIR}/../CServiceApplicationComp.cpp"
	)
endif()


file(GLOB PROJECT_SRC
	    ${HEADER_FILES}
		${SOURCES_FILES}
)

#set(TARGET_EXT ".lib")

add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})

acf_create_moc(MOC_SOURCES "${HEADER_FILES}")

target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES})
#set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ${TARGET_EXT})
set_property(
    TARGET ${PROJECT_NAME}
	PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${ACFSLNDIR}/Lib/${TARGETNAME})

set_property(
    TARGET ${PROJECT_NAME}
	PROPERTY LIBRARY_OUTPUT_DIRECTORY ${ACFSLNDIR}/Lib/${TARGETNAME})

qt5_use_modules(${PROJECT_NAME} Core Widgets Gui Xml Network XmlPatterns Svg Concurrent)


