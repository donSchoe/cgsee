
#include <glm/gtx/random.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/screenquad.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>

#include "ssaoeffect.h"

SSAOEffect::SSAOEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader,  FrameBufferObject * normalz)
:   DefaultPass(camera)
,   m_fboNormalz(normalz)
,   m_quad(quad)
,   m_kernel(128)
,   m_noise(16)
{
    for (int i = 0; i < m_kernel.size(); ++i) {
        m_kernel[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(0.0f, 1.0f))
        );

        float scale = glm::linearRand(0.0f, 1.0f);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        m_kernel[i] *= scale;
    }

    for (int i = 0; i < m_noise.size(); ++i) {
        m_noise[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f)
        );
    }

    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/ssao.frag"));
    m_program->attach(quadShader);

}

SSAOEffect::~SSAOEffect(void)
{
}

void SSAOEffect::render()
{
    m_fboNormalz->bindTexture2D(*m_program, "normalz", 0);

    m_quad->draw(*m_program, m_fbo);

    m_fboNormalz->release();
}

void SSAOEffect::setUniforms()
{
    m_program->setUniform("kernel", &m_kernel[0], m_kernel.size());
    m_program->setUniform("noise", &m_noise[0], m_noise.size());

    m_program->setUniform("sample_count", 32); // usefull range: 0-128
    m_program->setUniform("zOffset", 0.005f);
    m_program->setUniform("filterRadius", 25.0f);
}

void SSAOEffect::resize( const int width, const int height )
{
    m_program->setUniform("viewport", glm::ivec2(width, height));
    DefaultPass::resize(width, height);
}
