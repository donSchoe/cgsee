#pragma once

#include "renderingpass.h"

// provides a program, a fbo and default implementations for resize(), output(), clear() and render()
class CGSEE_API DefaultPass : public RenderingPass
{
public:
    DefaultPass(Camera * camera);
    virtual ~DefaultPass(void);

    virtual void resize(const int width, const int height) override;

    virtual FrameBufferObject * output() override;
    virtual void clearFbos() override;
    virtual void setUniforms() override;

protected:
    virtual void render() override;
    Program * m_program;
    FrameBufferObject * m_fbo;
};

