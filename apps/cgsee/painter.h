#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <QString>
#include <QMap>

#include <core/painter/abstractscenepainter.h>




class Camera;
class Group;
class ScreenQuad;
class Program;
class FrameBufferObject;

class Painter : public AbstractScenePainter
{
public:
    Painter(Camera * camera);
    virtual ~Painter();

    virtual void paint();

    void helloWord();

    virtual void setShading(char shader);

    virtual void resize(
        const int width
    ,   const int height);
protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

protected:
    void postShaderRelinked();
    void setUniforms();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    static void bindSampler(
        const t_samplerByName & sampler
    ,   const Program & program);

    static void releaseSampler(
        const t_samplerByName & sampler);
   
protected:
    ScreenQuad * m_quad;

    Program * m_normalz;
    Program * m_normals;
    Program * m_wireframe;
    Program * m_primitiveWireframe;
    Program * m_solidWireframe;
    Program * m_flat;
    Program * m_gouraud;
    Program * m_phong;
    Program * m_gooch;
    Program * m_useProgram;
    FrameBufferObject * m_fboNormalz;


    glm::vec3 camPos;

    Program * m_flush;

    Camera * m_camera;
};
