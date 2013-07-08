#include "effect.h"
#include "../camera.h"
#include "../program.h"
#include "../framebufferobject.h"
#include "../scenegraph/scenetraverser.h"
#include "../scenegraph/drawvisitor.h"


Effect::Effect(Camera * camera)
:   m_active(true)
,   m_camera(camera)
{
}

Effect::~Effect(void)
{
}

bool Effect::isActive()
{
    return m_active;
}

void Effect::setActive(bool value)
{
    m_active = value;
    if (!value)
        clearFbos();
}

void Effect::applyIfActive()
{
    if (m_active)
    {
        render();
    }
}

void Effect::drawScene(Camera * camera, Program * program,  FrameBufferObject * fbo)
{
    fbo->bind();
    SceneTraverser traverser;
    DrawVisitor drawVisitor(program, camera->transform());
    traverser.traverse(*camera, drawVisitor);
    fbo->release();
}
