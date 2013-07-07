
set(target cgsee)
message(STATUS "app ${target}")

set(SOURCES

    cgsee.h
    cgsee.cpp
    fallbackrealisticmaterial.h
    fallbackrealisticmaterial.cpp
    main.cpp
    realisticpainter.h
    realisticpainter.cpp
)

source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.ui$" 
    ${CGSEE_SOURCE_GROUP} ${SOURCES})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${target} ${SOURCES})

qt5_use_modules(${target} Core OpenGL Gui Widgets)

target_link_libraries(${target} 
    core cli gui)

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    COMPILE_FLAGS               "${DEFAULT_COMPILE_FLAGS}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    FOLDER ${CGSEE_APP_FOLDER}
    DEBUG_POSTFIX "d${DEBUG_POSTFIX}")
