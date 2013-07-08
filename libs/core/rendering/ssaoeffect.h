#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "effect.h"
#include "../screenquad.h"

class CGSEE_API SSAOEffect : public Effect
{
public:
    SSAOEffect(Camera * camera, FrameBufferObject * normalz,
               FileAssociatedShader * quadShader, ScreenQuad * quad);
    virtual ~SSAOEffect(void);

    virtual void resize(const int width, const int height) override;

    virtual FrameBufferObject * output() override;
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;


private:
    Program * m_program;
    FrameBufferObject * m_fbo;
    FrameBufferObject * m_fboNormalz;

    std::vector<glm::vec3> m_kernel;
    std::vector<glm::vec3> m_noise;

    ScreenQuad * m_quad;
};

