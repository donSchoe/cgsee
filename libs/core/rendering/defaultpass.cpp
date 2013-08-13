
#include <core/framebufferobject.h>
#include <core/program.h>

#include "defaultpass.h"

DefaultPass::DefaultPass(Camera * camera)
:   RenderingPass(camera)
{
    m_fbo = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_program = new Program();
}

DefaultPass::~DefaultPass(void)
{
    delete m_program;
    delete m_fbo;
}

void DefaultPass::render()
{
    drawScene(m_camera, m_program, m_fbo);
}

void DefaultPass::clearFbos()
{
    m_fbo->clear();
}

FrameBufferObject * DefaultPass::output()
{
    return m_fbo;
}

void DefaultPass::resize(const int width, const int height)
{
    m_fbo->resize(width, height);
}

void DefaultPass::setUniforms()
{
    
}
