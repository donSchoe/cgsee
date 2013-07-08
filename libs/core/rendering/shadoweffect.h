#pragma once


#include "effect.h"
#include <vector>
#include "Effect.h"

class FileAssociatedShader;
class Group;

class CGSEE_API  ShadowEffect : public Effect
{
public:
    ShadowEffect(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~ShadowEffect(void);

    void sceneChanged(Group * scene);

    virtual void resize(const int width, const int height) override;

    virtual FrameBufferObject * output() override;
    FrameBufferObject * shadowMap();
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;
    
private:
    Camera * m_lightcam;

    Program * m_lightsource;
    Program * m_shadowMapping;

    FrameBufferObject * m_fboShadows;
    FrameBufferObject * m_fboShadowMap;

    std::vector<glm::vec2> m_shadow_samples;

    static const glm::mat4 biasMatrix;

    Group * m_scene;
};

