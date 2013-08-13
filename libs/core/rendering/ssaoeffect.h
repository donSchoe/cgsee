#pragma once

#include <vector>

#include "defaultpass.h"

class ScreenQuad;

class CGSEE_API SSAOEffect : public DefaultPass
{
public:
    SSAOEffect(Camera * camera, ScreenQuad * quad, FileAssociatedShader * quadShader,  FrameBufferObject * normalz);
    virtual ~SSAOEffect(void);
    
    virtual void setUniforms() override;
    virtual void resize(const int width, const int height) override;

protected:
    virtual void render() override;

private:
    FrameBufferObject * m_fboNormalz;

    std::vector<glm::vec3> m_kernel;
    std::vector<glm::vec3> m_noise;

    ScreenQuad * m_quad;
};

