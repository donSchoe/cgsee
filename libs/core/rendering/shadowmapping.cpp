
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <core/camera.h>
#include <core/program.h>
#include <core/framebufferobject.h>
#include <core/fileassociatedshader.h>
#include <core/scenegraph/group.h>

#include "shadowmapping.h"
#include "lightsource.h"

const glm::mat4 ShadowMappingPass::biasMatrix (
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

ShadowMappingPass::ShadowMappingPass(Camera * camera, FileAssociatedShader * depth_util, LightSourcePass * lightsource)
:   DefaultPass(camera)
,   m_lightsource(lightsource)
,   m_shadow_samples(128)
{
    for (int i = 0; i < m_shadow_samples.size(); ++i) {
        m_shadow_samples[i] = glm::vec2(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f));
    }

    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/shadowmapping.frag"));
    m_program->attach(depth_util);
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/shadowmapping.vert"));
}

ShadowMappingPass::~ShadowMappingPass()
{
}

void ShadowMappingPass::render()
{
    m_lightsource->output()->bindTexture2D(*m_program, "shadowMap", 0);

    m_program->setUniform("invCameraTransform", glm::inverse(m_camera->transform()), false);
    m_program->setUniform("LightSourceTransform", biasMatrix * m_lightsource->transform(), false);
    drawScene(m_camera, m_program, m_fbo);

    m_lightsource->output()->releaseTexture2D();
}


void ShadowMappingPass::setUniforms()
{
    m_program->setUniform("samples", &m_shadow_samples[0], m_shadow_samples.size());

    // should be variable properties in the future
    m_program->setUniform("lightSize", 15.0f); 
    m_program->setUniform("searchWidth", 10.0f);
    m_program->setUniform("zOffset",  0.0015f);
    m_program->setUniform("sample_count", 24); // usefull range: 0-128
}
