
set(target gui)
message(STATUS "lib ${target}")

set(SOURCES
    canvas.h
    canvas.cpp
    canvasexporter.h
    canvasexporter.cpp
    canvasexporter.ui
    canvasexportconfig.h
    canvasexportconfig.cpp
    fileExplorer.h
    fileExplorer.cpp
    fileNavigator.h
    fileNavigator.cpp
    propertywidgetbuilder.h
    propertywidgetbuilder.cpp
    viewer.h
    viewer.cpp
    viewer.ui
)

# group source files

source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.ui$" 
    ${CGSEE_SOURCE_GROUP} ${SOURCES})

# wrap ui files and group wrap headers

list_extract(UIS "\\\\.ui$" ${SOURCES})

qt5_wrap_ui(UIHS ${UIS})

source_group_by_path(${CMAKE_CURRENT_BINARY_DIR} "\\\\.h$" 
    ${CGSEE_UIWRAP_GROUP} ${UIHS})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})

add_library(${target} SHARED ${HEADERS} ${SOURCES} ${UIHS})

qt5_use_modules(${target} Core OpenGL Gui Widgets)

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
    core)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    FOLDER ${CGSEE_LIB_FOLDER}
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")

add_definitions("-DCGSEE_EXPORTS")
