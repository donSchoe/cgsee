
#include <glm/gtx/random.hpp>

#include "ssaoeffect.h"
#include "../framebufferobject.h"
#include "../program.h"
#include "../fileassociatedshader.h"

SSAOEffect::SSAOEffect(Camera * camera, FrameBufferObject * normalz,
                       FileAssociatedShader * quadShader, ScreenQuad * quad)
:   Effect(camera)
,   m_fboNormalz(normalz)
,   m_quad(quad)
,   m_kernel(128)
,   m_noise(16)
{
    for (int i = 0; i < m_kernel.size(); ++i) {
        m_kernel[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(0.0f, 1.0f)));

        float scale = glm::linearRand(0.0f, 1.0f);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        m_kernel[i] *= scale;
    }

    for (int i = 0; i < m_noise.size(); ++i) {
        m_noise[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f));
    }

    m_fbo = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    m_program = new Program();
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/ssao.frag"));
    m_program->attach(quadShader);

}

SSAOEffect::~SSAOEffect(void)
{
    delete m_program;
    delete m_fbo;
}

void SSAOEffect::render()
{
    m_fboNormalz->bindTexture2D(*m_program, "normalz", 0);

    m_program->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_program, m_fbo);

    m_fboNormalz->release();
}

void SSAOEffect::clearFbos()
{
    m_fbo->clear();
}

FrameBufferObject * SSAOEffect::output()
{
    return m_fbo;
}

void SSAOEffect::resize(const int width, const int height)
{
    m_fbo->resize(width, height);
}

void SSAOEffect::setUniforms()
{
    m_program->setUniform("kernel", &m_kernel[0], m_kernel.size());
    m_program->setUniform("noise", &m_noise[0], m_noise.size());

    m_program->setUniform("sample_count", 32); // usefull range: 0-128
    m_program->setUniform("zOffset", 0.005f);
    m_program->setUniform("filterRadius", 25.0f);
}
