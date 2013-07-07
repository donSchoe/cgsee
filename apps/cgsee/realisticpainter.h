#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <QStringList>
#include <QString>
#include <QMap>

#include <core/abstractscenepainter.h>
#include "fallbackrealisticmaterial.h"

class DataBlockRegistry;
class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;


class RealisticPainter : public AbstractScenePainter
{
public:
    enum e_Program
    {
        FLAT, GOURAUD, PHONG, GOOCH
    };

    static QStringList supportedTextures();

public:
    RealisticPainter(Camera * camera);
    virtual ~RealisticPainter();

    virtual void paint();
    virtual void draw(Group & group, const glm::mat4 & transform);
    virtual void draw(PolygonalDrawable & drawable, const glm::mat4 & transform);
    virtual void draw(Node & node, const glm::mat4 & transform);

    void setFrameBuffer(int frameBuffer);
    void setEffect( int effect, bool active );

    virtual void resize(const int width, const int height);


protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

    void postShaderRelinked();
    void setUniforms();

    void drawScene(Camera * camera, Program * program, FrameBufferObject * fbo);
    void createShadows();
    void createSSAO();
    void addBlur(FrameBufferObject * fbo);

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    static void bindSampler(const t_samplerByName & samplers, const Program & program);
    static void releaseSampler(const t_samplerByName & samplers);

    void sceneChanged(Group * scene);
    void setShaderProperties();

protected:
    ScreenQuad * m_quad;

    QMap<e_Program, Program *> m_programs;
    FallbackRealisticMaterial m_fallbackMaterial;
    Program * m_flush;

    Program * m_activeProgram;
    Program * m_lightsource;
    Program * m_shadowMapping;
    Program * m_SSAO;
    Program * m_blurv;
    Program * m_blurh;
    FrameBufferObject * m_fboColor;
    FrameBufferObject * m_fboTemp;
    FrameBufferObject * m_fboSSAO;
    FrameBufferObject * m_fboShadows;
    FrameBufferObject * m_fboNormalz;
    FrameBufferObject * m_fboShadowMap;
    FrameBufferObject ** m_fboActiveBuffer;

    std::vector<glm::vec3> m_kernel;
    std::vector<glm::vec3> m_noise;
    std::vector<glm::vec2> m_shadow_samples;

    Camera *m_camera;
    Camera * m_lightcam;
    bool m_useColor;
    bool m_useShadows;
    bool m_blurShadows;
    bool m_useSSAO;
    bool m_blurSSAO;

    static const glm::mat4 biasMatrix;
};

