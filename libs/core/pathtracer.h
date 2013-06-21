#pragma once

#include <GL/glew.h>

#include "declspec.h"
#include "camera.h"


class BufferObject;
class Program;
class FrameBufferObject;

class CGSEE_API PathTracer : public Camera
{
public:
    PathTracer(const QString & name = "unnamed");
    virtual ~PathTracer();

    virtual void draw(
        const Program & program
    ,   FrameBufferObject * target = nullptr) override;
    virtual void draw(
        const Program & program
        ,   const glm::mat4 & transform) override;

protected:
    void initialize(const Program & program) const;

    mutable GLuint m_vao;
    mutable BufferObject * m_vertexBO;
};