
#include <core/camera.h>
#include <core/framebufferobject.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>

#include "renderingpass.h"

RenderingPass::RenderingPass(Camera * camera)
:   m_active(true)
,   m_camera(camera)
{
}

RenderingPass::~RenderingPass(void)
{
}

bool RenderingPass::isActive()
{
    return m_active;
}

void RenderingPass::setActive(bool value)
{
    m_active = value;
    if (!value)
        clearFbos();
}

void RenderingPass::apply()
{
    render();
}

void RenderingPass::applyIfActive()
{
    if (m_active)
    {
        render();
    }
}

void RenderingPass::drawScene(Camera * camera, Program * program,  FrameBufferObject * fbo)
{
    fbo->bind();
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, this, camera->transform());
    traverser.traverse(*camera, drawVisitor);
    fbo->release();
}

void RenderingPass::sceneChanged(Group * scene)
{
}

void RenderingPass::preRenderHook(PolygonalDrawable &drawable) {

}

void RenderingPass::postRenderHook(PolygonalDrawable &drawable) {

}
