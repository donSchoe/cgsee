
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "shadoweffect.h"

#include "../program.h"
#include "../framebufferobject.h"
#include "../fileassociatedshader.h"
#include "../scenegraph/group.h"

const glm::mat4 ShadowEffect::biasMatrix (
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
    );

ShadowEffect::ShadowEffect(Camera * camera, FileAssociatedShader * depth_util)
:   Effect(camera)
,   m_scene(nullptr)
,   m_shadow_samples(128)
{
    m_lightcam = new Camera();
    m_lightcam->setViewport(camera->viewport());
    m_lightcam->setFovy(camera->fovy());
    m_lightcam->setZFar(camera->zFar());
    m_lightcam->setZNear(camera->zNear());

    for (int i = 0; i < m_shadow_samples.size(); ++i) {
        m_shadow_samples[i] = glm::vec2(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f));
    }

    m_lightsource = new Program();
    m_lightsource->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/lightsource.frag"));
    m_lightsource->attach(depth_util);
    m_lightsource->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/lightsource.vert"));

    m_shadowMapping = new Program();
    m_shadowMapping->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/shadowmapping.frag"));
    m_shadowMapping->attach(depth_util);
    m_shadowMapping->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/shadowmapping.vert"));

    m_fboShadows = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboShadowMap = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

}


ShadowEffect::~ShadowEffect(void)
{
    delete m_lightsource;
    delete m_shadowMapping;
    delete m_fboShadows;
    delete m_fboShadowMap;
}

void ShadowEffect::render()
{
    drawScene(m_lightcam, m_lightsource, m_fboShadowMap);

    m_fboShadowMap->bindTexture2D(*m_shadowMapping, "shadowMap", 0);

    m_shadowMapping->setUniform("invCameraTransform", glm::inverse(m_camera->transform()), false);
    m_shadowMapping->setUniform("LightSourceTransform", biasMatrix * m_lightcam->transform(), false);
    drawScene(m_camera, m_shadowMapping, m_fboShadows);

    m_fboShadows->releaseTexture2D();
}

void ShadowEffect::sceneChanged(Group * scene)
{
    if(m_scene)
        m_lightcam->remove(m_scene);
    m_lightcam->append(scene);

    AxisAlignedBoundingBox bb = scene->boundingBox();
    m_lightcam->setView(glm::lookAt(glm::vec3(3.5, 5.0, 5.5)+bb.center(),
                                    bb.center(), glm::vec3(0.0,1.0,0.0)));
    m_scene = scene;
}

void ShadowEffect::resize( const int width, const int height )
{
    m_lightcam->setViewport(width, height);
    m_lightcam->update();

    m_fboShadows->resize(width, height);
    m_fboShadowMap->resize(width, height);
}

FrameBufferObject * ShadowEffect::output()
{
    return m_fboShadows;
}

FrameBufferObject * ShadowEffect::shadowMap()
{
    return m_fboShadowMap;
}

void ShadowEffect::clearFbos()
{
    m_fboShadowMap->clear();
    m_fboShadows->clear();
}

void ShadowEffect::setUniforms()
{
    m_shadowMapping->setUniform("samples", &m_shadow_samples[0], m_shadow_samples.size());

    // should be variable properties in the future
    m_shadowMapping->setUniform("lightSize", 15.0f); 
    m_shadowMapping->setUniform("searchWidth", 10.0f);
    m_shadowMapping->setUniform("zOffset",  0.0015f);
    m_shadowMapping->setUniform("sample_count", 24); // usefull range: 0-128
}
