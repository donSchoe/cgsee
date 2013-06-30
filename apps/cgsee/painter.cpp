
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

#include "painter.h"

#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/group.h>
#include <core/objloader.h>
#include <core/assimploader.h>
#include <core/program.h>
#include <core/screenquad.h>
#include "core/navigation/arcballnavigation.h"
#include "core/navigation/flightnavigation.h"


//for phong, flat and gouraud
static const QString CAMERAPOSITION_UNIFORM ("cameraposition");
static const QString LIGHTDIR_UNIFORM ("lightdir");
static const QString LIGHTDIR_UNIFORM2 ("lightdir2");
static const QString LIGHTAMBIENTGLOBAL_UNIFORM ("lightambientglobal");
static const QString LIGHT_UNIFORM ("light");
static const QString LIGHT_UNIFORM2 ("light2");
static const QString MATERIAL_UNIFORM ("material");
static const QString LIGHTPOSITION_UNIFORM ("lightposition");
//gooch
static const QString WARMCOLDCOLOR_UNIFORM ("warmcoldcolor");

Painter::Painter(Camera * camera)
:   AbstractScenePainter()
,   m_quad(nullptr)
,   m_textured(nullptr)
,   m_normals(nullptr)
,   m_normalz(nullptr)
,   m_wireframe(nullptr)
,   m_primitiveWireframe(nullptr)
,   m_solidWireframe(nullptr)
,   m_flat(nullptr)
,   m_gouraud(nullptr)
,   m_phong(nullptr)
,   m_gooch(nullptr)
,   m_useProgram(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_camera(camera)
{
}

Painter::Painter(Group * scene)
:   AbstractScenePainter(scene)
,   m_quad(nullptr)
,   m_normalz(nullptr)
,   m_fboNormalz(nullptr)
,   m_flush(nullptr)
,   m_camera(nullptr)
{
}

Painter::~Painter()
{
    delete m_quad;

    delete m_textured;
    delete m_normals;
    delete m_normalz;
    delete m_flat;
    delete m_gouraud;
    delete m_phong;
    delete m_gooch;
    delete m_wireframe;
    delete m_primitiveWireframe;
    delete m_solidWireframe;
    delete m_fboNormalz;
    delete m_flush;
}

const bool Painter::initialize()
{
    AutoTimer t("Initialization of Painter");

    if (m_scene) {
        glm::mat4 transform(1.f);

        transform *= glm::scale(glm::mat4(1.f), glm::vec3(0.02f));
        transform *= glm::rotate(glm::mat4(1.f), 180.f, glm::vec3(0.f, 1.f, 0.f));
        transform *= glm::rotate(glm::mat4(1.f), -90.f, glm::vec3(1.f, 0.f, 0.f));
        transform *= glm::rotate(glm::mat4(1.f), 25.f, glm::vec3(0.f, 0.f, 1.f));

        m_scene->setTransform(transform);
        m_camera->append(m_scene);
    }

    m_quad = new ScreenQuad();

    // Textured
    m_textured = new Program();
    m_textured->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/textured.frag"));
    m_textured->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/textured.vert"));


    // NORMALS
    m_normals = new Program();
    m_normals->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normals.frag"));
    m_normals->attach(
        new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/normals.geo"));
    m_normals->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/normals.vert"));

    // NORMALZ
    m_normalz = new Program();
    m_normalz->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_normalz->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));

    FileAssociatedShader *m_wireframeShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/wireframe.vert");
    FileAssociatedShader *m_wireframeShaderGEO = new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/wireframe.geo");

    //Wireframe
    m_wireframe = new Program();
    m_wireframe->attach(m_wireframeShaderGEO);
    m_wireframe->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe.frag"));
    m_wireframe->attach(m_wireframeShader);

    //Primitive Wireframe
    m_primitiveWireframe = new Program();
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/primitiveWireframe.geo"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/primitiveWireframe.frag"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/primitiveWireframe.vert"));

    //Solid Wireframe
    m_solidWireframe = new Program();
    m_solidWireframe->attach(m_wireframeShaderGEO);
    m_solidWireframe->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframeSolid.frag"));
    m_solidWireframe->attach(m_wireframeShader);



    FileAssociatedShader *phongLighting = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/phongLighting.glsl");

    //FLAT
    m_flat = new Program();
    m_flat->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flat.frag"));
    m_flat->attach(phongLighting);
    m_flat->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/flat.vert"));
    m_flat->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/flat.geo"));

    //GOURAUD
    m_gouraud = new Program();
    m_gouraud->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/gouraud.frag"));
    m_gouraud->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/gouraud.vert"));
    m_gouraud->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/phongLighting.vert"));

    //PHONG
    m_phong = new Program();
    m_phong->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/phong.frag"));
    m_phong->attach(phongLighting);
    m_phong->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/phong.vert"));

    //GOOCH
    m_gooch = new Program();
    m_gooch->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/gooch.frag"));
    m_gooch->attach(
         new FileAssociatedShader(GL_VERTEX_SHADER, "data/gooch.vert"));

    //set UNIFORMS for selected shader
    m_useProgram = m_flat;
    setUniforms();

    // Post Processing Shader

    m_flush = new Program();
    m_flush->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
    m_flush->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert"));

    m_fboNormalz = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    return true;
}

void Painter::setUniforms()
{
    if(m_useProgram == m_flat || m_useProgram == m_gouraud || m_useProgram == m_phong)
    {
        m_useProgram->setUniform(CAMERAPOSITION_UNIFORM, camPos);
        m_useProgram->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,6.5,7.5));
        m_useProgram->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,-8.0,7.5));

        m_useProgram->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.4));

        glm::mat4 lightMat;
        lightMat[0] = glm::vec4(0.0,0.0,0.0,1.0);    //ambient
        lightMat[1] = glm::vec4(0.4,0.6,0.8,1.0);    //diffuse
        lightMat[2] = glm::vec4(0.0,0.0,0.8,1.0);    //specular
        lightMat[3] = glm::vec4(0.000,0.000,0.0000,1.4);    //attenuation1, attenuation2, attenuation3, shininess
        m_useProgram->setUniform(LIGHT_UNIFORM, lightMat, false);

        glm::mat4 lightMat2;
        lightMat2[0] = glm::vec4(0.0,0.0,0.0,1.0);    //ambient
        lightMat2[1] = glm::vec4(0.9,0.5,0.5,1.0);    //diffuse
        lightMat2[2] = glm::vec4(0.8,0.0,0.0,1.0);    //specular
        lightMat2[3] = glm::vec4(0.000,0.000,0.0000,1.4);    //attenuation1, attenuation2, attenuation3, shininess

        m_useProgram->setUniform(LIGHT_UNIFORM2, lightMat2, false);

        glm::mat4 materialCoeff;
        materialCoeff[0] = glm::vec4(0.1,0.1,0.1,1.0);    //ambient
        materialCoeff[1] = glm::vec4(1.0,1.0,1.0,1.0);    //diffuse
        materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);    //specular
        materialCoeff[3] = glm::vec4(0,0,0,0);            //emission

        m_useProgram->setUniform(MATERIAL_UNIFORM, materialCoeff);
    }

    else if(m_useProgram == m_gooch)
    {
        m_useProgram->setUniform(LIGHTPOSITION_UNIFORM, glm::vec3(-2.0,0.0,2.0));

        glm::mat4 warmColdColor;

        warmColdColor[0] = glm::vec4(0.75, 0.75, 0.75, 0.0);    //surface color
        warmColdColor[1] = glm::vec4(0.6, 0.6, 0.0, 0.0);    //warm color
        warmColdColor[2] = glm::vec4(0.0, 0.0, 0.6, 0.0);    //cold color
        warmColdColor[3] = glm::vec4(0.45,0.45,0,0);            //Diffuse Warm, DiffuseCool
        m_useProgram->setUniform(WARMCOLDCOLOR_UNIFORM, warmColdColor);
    }
}

void Painter::paint()
{
    AbstractPainter::paint();

    t_samplerByName sampler;

    m_camera->draw(*m_useProgram, m_fboNormalz);

    sampler.clear();
    sampler["source"] = m_fboNormalz;

    bindSampler(sampler, *m_flush);
    m_quad->draw(*m_flush, nullptr);
    releaseSampler(sampler);

}

void Painter::resize(  //probably never called anywhere?
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);

    m_camera->setViewport(width, height);

    m_fboNormalz->resize(width, height);

    postShaderRelinked();
}

void Painter::setShading(char shader)
{
    switch(shader)
    {
        case 'p': m_useProgram = m_phong; std::printf("\nPhong Shading\n"); break;
        case 'g': m_useProgram = m_gouraud; std::printf("\nGouraud Shading\n"); break;
        case 'f': m_useProgram = m_flat; std::printf("\nFlat Shading\n"); break;
        case 'o': m_useProgram = m_gooch; std::printf("\nGooch Shading\n\n"); break;
        case 'w': m_useProgram = m_wireframe; std::printf("\nWireframe Shading\n\n"); break;
        case 's': m_useProgram = m_solidWireframe; std::printf("\nWireframeSolid Shading\n\n"); break;
        case 'r': m_useProgram = m_primitiveWireframe; std::printf("\nprimitive Wireframe Shading\n\n"); break;
        case 'n': m_useProgram = m_normals; std::printf("\nNormals\n\n"); break;
    }
    setUniforms();
    //repaint missing

}

void Painter::postShaderRelinked()
{
}

void Painter::bindSampler(

    const t_samplerByName & sampler
,   const Program & program)
{
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(glm::uint slot(0); i != iEnd; ++i, ++slot)
        i.value()->bindTexture2D(program, i.key(), slot);
}

void Painter::releaseSampler(
    const t_samplerByName & sampler)
{
    t_samplerByName::const_iterator i(sampler.begin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();

}

Camera * Painter::camera()
{
    return m_camera;
}
