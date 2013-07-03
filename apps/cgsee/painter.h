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

class Painter : public AbstractScenePainter
{
public:
    Painter(Camera * camera);
    Painter(Group * scene);
    virtual ~Painter();

    virtual void paint();

    virtual void setShading(char shader);

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
// <<<<<<< HEAD
//     std::shared_ptr<DataBlockRegistry> m_registry;
//     Group * m_group;
// =======
// >>>>>>> master
    ScreenQuad * m_quad;

    QMap<QString, Program *> programs;

    Program * m_textured;
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

private:
    Painter(Group * scene, Camera * camera);
};

