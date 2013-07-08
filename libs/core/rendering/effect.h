#pragma once

#include <glm/glm.hpp>
#include "../declspec.h"
#include "../camera.h"

class Program;
class FrameBufferObject;
class FileAssociatedShader;

class CGSEE_API Effect
{
public:
    Effect(Camera * camera);
    virtual ~Effect(void);

    bool isActive();
    void setActive(bool value);

    void applyIfActive();

    virtual void setUniforms() = 0;
    virtual void resize(const int width, const int height) = 0;

    virtual FrameBufferObject * output() = 0;
    virtual void clearFbos() = 0;

protected:
    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);
    
    virtual void render() = 0;

    Camera * m_camera;

private:
    bool m_active;
};

