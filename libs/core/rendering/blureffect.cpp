#include "blureffect.h"
#include "../program.h"
#include "../fileassociatedshader.h"
#include "../framebufferobject.h"
#include "../screenquad.h"



BlurEffect::BlurEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader,
                       Effect * target, FrameBufferObject * temp)
:   Effect(camera)
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

    m_target->output()->bindTexture2D(*m_blurv, "source", 0);
    m_blurv->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_blurv, m_fboTemp);
    m_target->output()->releaseTexture2D();

    m_fboTemp->bindTexture2D(*m_blurh, "source", 0);
    m_blurh->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_blurh, m_target->output());
    m_fboTemp->releaseTexture2D();
}

void BlurEffect::setUniforms()
{
}
