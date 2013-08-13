
#include <core/camera.h>
#include <core/program.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/screenquad.h>

#include "blureffect.h"

BlurEffect::BlurEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader,
                       RenderingPass * target, FrameBufferObject * temp)
:   RenderingPass(camera)
,   m_quad(quad)
,   m_target(target)
,   m_fboTemp(temp)
{
    m_blurv = new Program();
    m_blurv->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_v.frag"));
    m_blurv->attach(quadShader);

    m_blurh = new Program();
    m_blurh->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/gauss_blur_5_h.frag"));
    m_blurh->attach(quadShader);
}


BlurEffect::~BlurEffect(void)
{
    // there needs to be a way to get instances of existing FileAssociatedShader
    //delete m_blurv;
    //delete m_blurh;
}

void BlurEffect::resize( const int width, const int height )
{
    m_blurv->setUniform("viewport", m_camera->viewport());
    m_blurh->setUniform("viewport", m_camera->viewport());
}

FrameBufferObject * BlurEffect::output()
{
    return m_target->output();
}

void BlurEffect::clearFbos()
{
}

void BlurEffect::render()
{
    if (!m_target->isActive())
        return;

    blurPass(m_target->output(), m_fboTemp, m_blurv);

    blurPass(m_fboTemp, m_target->output(), m_blurh);
}

void BlurEffect::setUniforms()
{
}

void BlurEffect::blurPass(FrameBufferObject * in, FrameBufferObject * out, Program * program)
{
    in->bindTexture2D(*program, "source", 0);
    m_quad->draw(*program, out);
    in->releaseTexture2D();
}

