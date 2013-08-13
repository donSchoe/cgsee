#pragma once

#include <vector>
#include "defaultpass.h"

class FileAssociatedShader;
class LightSourcePass;

class CGSEE_API  ShadowMappingPass : public DefaultPass
{
public:
    ShadowMappingPass(Camera * camera, FileAssociatedShader * depth_util, LightSourcePass * lightsource);
    virtual ~ShadowMappingPass(void);
    virtual void render() override;
    virtual void setUniforms() override;

private:
    std::vector<glm::vec2> m_shadow_samples;
    LightSourcePass * m_lightsource;
    static const glm::mat4 biasMatrix;
};

