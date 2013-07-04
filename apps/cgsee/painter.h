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


class RealisticPainter : public AbstractScenePainter
{
public:
    enum e_Program
    {
        FLAT, GROURAUD, PHONG, GOOCH
    }

public:
    RealisticPainter(Camera * camera);
    virtual ~RealisticPainter();

    virtual void paint();

    virtual void resize(const int width, const int height);


protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;

    void postShaderRelinked();
    void setUniforms();

    typedef QMap<QString, FrameBufferObject *> t_samplerByName;

    static void bindSampler(const t_samplerByName & samplers, const Program & program);
    static void releaseSampler(const t_samplerByName & samplers);


protected:
    ScreenQuad * m_quad;

    QMap<e_Program, Program *> m_programs;
    Program * m_flush;

    Program * m_useProgram;

    FrameBufferObject * m_fboNormalz;

    Camera * m_camera;
};

