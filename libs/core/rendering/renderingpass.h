#pragma once

#include <core/declspec.h>

class Camera;
class Group;
class Program;
class FrameBufferObject;
class PolygonalDrawable;
class FileAssociatedShader;

class CGSEE_API RenderingPass
{
public:
    RenderingPass(Camera * camera);
    virtual ~RenderingPass(void);

    bool isActive();
    void setActive(bool value);

    void apply();
    void applyIfActive();

    virtual void setUniforms() = 0;
    virtual void resize(const int width, const int height) = 0;

    virtual FrameBufferObject * output() = 0;
    virtual void clearFbos() = 0;
    virtual void sceneChanged(Group * scene);

    virtual void preRenderHook(PolygonalDrawable &drawable);
    virtual void postRenderHook(PolygonalDrawable &drawable);

protected:
    virtual void render() = 0;
    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);

protected:
    Camera * m_camera;

private:
    bool m_active;
};

