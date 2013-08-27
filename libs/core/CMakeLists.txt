
set(target core)
message(STATUS "lib ${target}")

configure_file(meta.template meta.h)

set(SOURCES

    declspec.h

    aabb.h
    aabb.cpp
    abstractapplication.h
    abstractapplication.cpp
    abstractglparent.h
    abstractmodelloader.cpp
    abstractmodelloader.h
    abstractpainter.h
    abstractpainter.cpp
    abstractscenepainter.h
    abstractscenepainter.cpp

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

    material/fallbackrealisticmaterial.h
    material/fallbackrealisticmaterial.cpp
    material/material.cpp
    material/material.h
    material/materialattribute.cpp
    material/materialattribute.h
    material/texture2d.h
    material/texture2d.cpp
    material/textureloader.h
    material/textureloader.cpp
    
    scenegraph/node.h
    scenegraph/node.cpp
    scenegraph/group.h
    scenegraph/group.cpp
    scenegraph/polygonaldrawable.h
    scenegraph/polygonaldrawable.cpp
    scenegraph/polygonalgeometry.h
    scenegraph/polygonalgeometry.cpp
    scenegraph/scenetraverser.h
    scenegraph/scenetraverser.cpp
    scenegraph/SceneVisitorInterface.h
    scenegraph/drawvisitor.h
    scenegraph/drawvisitor.cpp

    scenegraph/cullingvisitor.h
    scenegraph/cullingvisitor.cpp
    
    abstracttimer.h
    assimploader.h
    assimploader.cpp
    autotimer.h
    autotimer.cpp
    bufferobject.h
    bufferobject.cpp
    camera.h
    camera.cpp
    chronotimer.h
    chronotimer.cpp
    common.h
    fileassociatedshader.h
    fileassociatedshader.cpp
    framebufferobject.h
    framebufferobject.cpp
    geometryoptimizer.h
    glformat.h
    glformat.cpp
    gpuquery.h
    gpuquery.cpp
    messagehandler.h
    messagehandler.cpp
    mathmacros.h
    
    navigation/abstractnavigation.h
    navigation/abstractnavigation.cpp
    navigation/arcballnavigation.h
    navigation/arcballnavigation.cpp
    navigation/flightnavigation.h
    navigation/flightnavigation.cpp
    navigation/fpsnavigation.h
    navigation/fpsnavigation.cpp
    
    objloader.h
    objloader.cpp

    program.h
    program.cpp
    screenquad.h
    screenquad.cpp
    shader.h
    shader.cpp
    timer.h
    timer.cpp
    textureunitprovider.cpp
    textureunitprovider.h
    plane.h
    plane.cpp
    
    rendering/blureffect.h
    rendering/blureffect.cpp
    rendering/defaultpass.h   
    rendering/defaultpass.cpp
    rendering/lightsource.h
    rendering/lightsource.cpp
    rendering/normalzpass.h   
    rendering/normalzpass.cpp
    rendering/realisticpass.h   
    rendering/realisticpass.cpp
    rendering/renderingpass.h
    rendering/renderingpass.cpp
    rendering/shadowmapping.h
    rendering/shadowmapping.cpp
    rendering/ssaoeffect.h   
    rendering/ssaoeffect.cpp
    
    viewfrustum.h
    viewfrustum.cpp
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
    ${ASSIMP_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${DEVIL_LIBRARY}
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
