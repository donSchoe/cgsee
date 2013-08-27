#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <QString>
#include <QMap>

#include <core/abstractscenepainter.h>

class DataBlockRegistry;
class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;
class RenderingPass;
class LightSourcePass;

class Painter : public AbstractScenePainter
{
public:
    Painter(Camera * camera);
    virtual ~Painter();

    virtual void paint();

    virtual void setShading(char shader);
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect( int effect, bool active );

    virtual void resize(const int width, const int height);
    virtual void postShaderRelinked() override;

protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

protected:

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

//    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);

    static void bindSampler(const t_samplerByName & sampler, const Program & program);

    static void releaseSampler(const t_samplerByName & sampler);
    void sceneChanged(Group * scene);

protected:
    ScreenQuad * m_quad;
    Program * m_normals;
    Program * m_wireframe;
    Program * m_primitiveWireframe;
    Program * m_solidWireframe;
    Program * m_flat;
    Program * m_gouraud;
    Program * m_phong;
    Program * m_gooch;
    Program * m_useProgram;
    Program * m_flush;
    FrameBufferObject * m_fboColor;
    FrameBufferObject * m_fboTemp;
    FrameBufferObject * m_fboActiveBuffer;

    RenderingPass * m_normalz;
    RenderingPass * m_realistic;
    LightSourcePass * m_lightsource;
    RenderingPass * m_shadows;
    RenderingPass * m_shadowBlur;
    RenderingPass * m_ssao;
    RenderingPass * m_ssaoBlur;
    QList<RenderingPass*> m_passes;

    Camera * m_camera;
    bool m_useColor;

};
