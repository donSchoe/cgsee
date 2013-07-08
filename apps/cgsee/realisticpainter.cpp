#include <glm/gtx/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

#include "realisticpainter.h"
#include "realisticcomposer.h"

#include <iostream>
#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/textureunitprovider.h>
#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/rendering/renderpass.h>
#include <core/objloader.h>
#include <core/assimploader.h>
#include <core/program.h>
#include <core/screenquad.h>
#include "core/navigation/arcballnavigation.h"
#include "core/navigation/flightnavigation.h"


RealisticPainter::RealisticPainter(Camera * camera):
   m_camera(camera)
{

}

RealisticPainter::~RealisticPainter()
{
    for(RenderPass *pass : m_renderPasses) {
        delete pass;
    }
}

const bool RealisticPainter::initialize()
{
    if(m_scene) {
        m_camera->append(m_scene);
    }

    return true;
}

void RealisticPainter::paint()
{
    m_camera->update();

}

void RealisticPainter::resize( //probably never called anywhere?
    const int width
, const int height)
{
    AbstractPainter::resize(width, height);

    m_camera->setViewport(width, height);

    postShaderRelinked();
}

void RealisticPainter::setShading(char shader)
{

}

void RealisticPainter::setFrameBuffer(int frameBuffer)
{
//    switch(frameBuffer)
//    {
//        case 1: m_fboActiveBuffer = &m_fboColor; std::printf("\nColor Buffer\n"); break;
//        case 2: m_fboActiveBuffer = &m_fboNormalz; std::printf("\nNormal Buffer\n"); break;
//        case 3: m_fboActiveBuffer = &m_fboShadows; std::printf("\nShadows Buffer\n"); break;
//        case 4: m_fboActiveBuffer = &m_fboShadowMap; std::printf("\nShadowMap Buffer\n"); break;
//        case 5: m_fboActiveBuffer = &m_fboSSAO; std::printf("\nSSAO Buffer\n"); break;
//    }
}

void RealisticPainter::setEffect( int effect, bool active )
{
//    switch(effect)
//    {
//        case 1: m_useColor = active; std::printf("\nColor toggled\n"); break;
//        case 2: m_useShadows = active; std::printf("\nShadow toggled\n"); break;
//        case 3: m_blurShadows = active; std::printf("\nShadow blur toggled\n"); break;
//        case 4: m_useSSAO = active; std::printf("\nSSAO toggled\n"); break;
//        case 5: m_blurSSAO = active; std::printf("\nSSAO blur toggled\n"); break;
//    }
//
//    m_fboShadows->clear();
//    m_fboSSAO->clear();
}

void RealisticPainter::postShaderRelinked()
{

}

void RealisticPainter::bindFBOs(QMap<QString, FrameBufferObject *> & fbos, Program & program)
{
//    for(QString fboName : fbos.keys()) {
//        fbos[fboName]->bindTexture2D(program, fboName);
//    }
}

void RealisticPainter::releaseFBOs(QMap<QString, FrameBufferObject *> & fbos)
{
//    for(FrameBufferObject * fbo : fbos) {
//        fbo->releaseTexture2D();
//    }
}

Camera * RealisticPainter::camera()
{
    return m_camera;
}


void RealisticPainter::sceneChanged(Group * scene)
{
}

