
set(target core)
message(STATUS "lib ${target}")

configure_file(meta.template meta.h)

set(SOURCES

    declspec.h

    aabb.h
    aabb.cpp
    abstractnavigation.h
    abstractnavigation.cpp
    arcballnavigation.h
    arcballnavigation.cpp
    abstractapplication.h
    abstractapplication.cpp
    abstractglparent.h
    abstractpainter.h
    abstractpainter.cpp

    datacore/datablock.h
    datacore/datablock.cpp
    datacore/attributeiterator.h
    datacore/attributeiterator_impl.inl
    datacore/vertexlist.h
    datacore/vertexlist.cpp
    datacore/vertexindexlist.h
    datacore/vertexindexlist.cpp
    datacore/vertexindexlist_impl.inl
    datacore/typefunctions.h
    datacore/typefunctions.cpp
    datacore/registertypeshere.h
    datacore/vertexlist_impl.inl
    datacore/datablock_impl.inl
    abstracttimer.h
    autotimer.h
    autotimer.cpp
    bufferobject.h
    bufferobject.cpp
    camera.h
    camera.cpp
    chronotimer.h
    chronotimer.cpp
    common.h
    flightnavigation.h
    flightnavigation.cpp
    fileassociatedshader.h
    fileassociatedshader.cpp
    fpsnavigation.h
    fpsnavigation.cpp
    framebufferobject.h
    framebufferobject.cpp
    glformat.h
    glformat.cpp
    gpuquery.h
    gpuquery.cpp
    group.h
    group.cpp
    materialattribute.h
    mathmacros.h
    messagehandler.h
    messagehandler.cpp
    node.h
    node.cpp
    objio.h
    objio.cpp
    polygonaldrawable.h
    polygonaldrawable.cpp
    polygonalgeometry.h
    polygonalgeometry.cpp
    program.h
    program.cpp
    screenquad.h
    screenquad.cpp
    shader.h
    shader.cpp
    texture2d.h
    texture2d.cpp
    textureloader.h
    textureloader.cpp
    timer.h
    timer.cpp
	vertexcacheoptimizer.h
	vertexcacheoptimizer.cpp
	vertexreuse.h
	vertexreuse.cpp
)

source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$|\\\\.ui$|\\\\.inl$" 
    ${CGSEE_SOURCE_GROUP} ${SOURCES})

# setup target

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR})

add_library(${target} SHARED ${HEADERS} ${SOURCES})

qt5_use_modules(${target} Widgets OpenGL)

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${EXTRA_LIBS}
)


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
add_definitions("-DGLM_SWIZZLE")
