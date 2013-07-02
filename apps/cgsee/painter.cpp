

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/random.hpp>
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
#include <core/grid.h>
#include "core/navigation/arcballnavigation.h"
#include "core/navigation/flightnavigation.h"


//for phong, flat and gouraud
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
,   m_grid(nullptr)
,   m_normals(nullptr)
,   m_normalz(nullptr)
,   m_lightsource(nullptr)
,   m_shadowMapping(nullptr)
,   m_SSAO(nullptr)
,   m_blurv(nullptr)
,   m_blurh(nullptr)
,   m_wireframe(nullptr)
,   m_primitiveWireframe(nullptr)
,   m_solidWireframe(nullptr)
,   m_flat(nullptr)
,   m_gouraud(nullptr)
,   m_phong(nullptr)
,   m_gooch(nullptr)
,   m_useProgram(nullptr)
,   m_fboNormalz(nullptr)
,   m_fboColor(nullptr)
,   m_fboTemp(nullptr)
,   m_fboShadows(nullptr)
,   m_fboSSAO(nullptr)
,   m_fboShadowMap(nullptr)
,   m_fboGrid(nullptr)
,   m_fboActiveBuffer(nullptr)
,   m_flush(nullptr)
,   m_camera(camera)
,   m_useColor(true)
,   m_useShadows(true)
,   m_useSSAO(true)
,   m_blurSSAO(true)
,   m_kernel(128)
,   m_noise(16)
,   m_shadow_samples(128)
{
    m_lightcam = new Camera();
    m_lightcam->setViewport(camera->viewport());
    m_lightcam->setFovy(camera->fovy());
    m_lightcam->setZFar(camera->zFar());
    m_lightcam->setZNear(camera->zNear());
    m_lightcam->setView(glm::lookAt(glm::vec3(5.0,5.0,5.0), glm::vec3(0), glm::vec3(0.0,1.0,0.0)));

    for (int i = 0; i < m_kernel.size(); ++i) {
        m_kernel[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(0.0f, 1.0f)));
            
            float scale = glm::linearRand(0.0f, 1.0f);
            scale = glm::mix(0.1f, 1.0f, scale * scale);
            m_kernel[i] *= scale;
    }

    for (int i = 0; i < m_noise.size(); ++i) {
        m_noise[i] = glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f));
    }

    for (int i = 0; i < m_shadow_samples.size(); ++i) {
        m_shadow_samples[i] = glm::vec2(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f));
    }
}

Painter::~Painter()
{
    delete m_quad;
    delete m_grid;
    delete m_normals;
    delete m_normalz;
    delete m_lightsource;
    delete m_shadowMapping;
    delete m_SSAO;
    delete m_blurv;
    delete m_blurh;
    delete m_flat;
    delete m_gouraud;
    delete m_phong;
    delete m_gooch;
    delete m_wireframe;
    delete m_primitiveWireframe;
    delete m_solidWireframe;
    delete m_fboColor;
    delete m_fboNormalz;
    delete m_fboShadows;
    delete m_fboShadowMap;
    delete m_fboSSAO;
    delete m_fboGrid;
    delete m_flush;
}

const bool Painter::initialize()
{
    AutoTimer t("Initialization of Painter");

    m_quad = new ScreenQuad();
    
    m_grid = new Grid();

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
    
    // SHADOWS
    m_lightsource = new Program();
    m_lightsource->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/lightsource.frag"));
    m_lightsource->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/lightsource.vert"));

    m_shadowMapping = new Program();
    m_shadowMapping->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/shadowmapping.frag"));
    m_shadowMapping->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/shadowmapping.vert"));

    FileAssociatedShader * screenQuadShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/screenquad.vert");

    // SSAO
    m_SSAO = new Program();
    m_SSAO->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/ssao.frag"));
    m_SSAO->attach(screenQuadShader);

    // Blur
    m_blurv = new Program();
    m_blurv->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/blurv.frag"));
    m_blurv->attach(screenQuadShader);

    m_blurh = new Program();
    m_blurh->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/blurh.frag"));
    m_blurh->attach(screenQuadShader);

    FileAssociatedShader *wireframeShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/wireframe/wireframe.vert");
    FileAssociatedShader *wireframeShaderGEO = new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/wireframe/wireframe.geo");

    //Wireframe
    m_wireframe = new Program();
    m_wireframe->attach(wireframeShaderGEO);
    m_wireframe->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/wireframe.frag"));
    m_wireframe->attach(wireframeShader);

    //Primitive Wireframe
    m_primitiveWireframe = new Program();
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/wireframe/primitiveWireframe.geo"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/primitiveWireframe.frag"));
    m_primitiveWireframe->attach(
        new FileAssociatedShader(GL_VERTEX_SHADER, "data/wireframe/primitiveWireframe.vert"));

    //Solid Wireframe
    m_solidWireframe = new Program();
    m_solidWireframe->attach(wireframeShaderGEO);
    m_solidWireframe->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/wireframe/wireframeSolid.frag"));
    m_solidWireframe->attach(wireframeShader);

    FileAssociatedShader *phongLighting = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phongLighting.frag");

    //FLAT
    m_flat = new Program();
    m_flat->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/flat.frag"));
    m_flat->attach(phongLighting);
    m_flat->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/flat.vert"));
    m_flat->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, "data/shading/flat.geo"));

    //GOURAUD
    m_gouraud = new Program();
    m_gouraud->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/gouraud.frag"));
    m_gouraud->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/gouraud.vert"));
    m_gouraud->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/phongLighting.vert"));

    //PHONG
    m_phong = new Program();
    m_phong->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phong.frag"));
    m_phong->attach(phongLighting);
    m_phong->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/phong.vert"));

    //GOOCH
    m_gooch = new Program();
    m_gooch->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/gooch.frag"));
    m_gooch->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/gooch.vert"));

    //set UNIFORMS for selected shader
    m_useProgram = m_flat;
    setUniforms();

    // Post Processing Shader
    m_flush = new Program();
    m_flush->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/flush.frag"));
    m_flush->attach(screenQuadShader);
    
    m_fboNormalz = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboColor = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboTemp = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboShadows = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboShadowMap = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboSSAO = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboGrid = new FrameBufferObject(
        GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    
    m_fboActiveBuffer = &m_fboGrid;

    return true;
}

void Painter::setUniforms()
{
    if(m_useProgram == m_flat || m_useProgram == m_gouraud || m_useProgram == m_phong)
    {
        m_useProgram->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,6.5,7.5));
        m_useProgram->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,-8.0,7.5));

        m_useProgram->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.0));

        glm::mat4 lightMat;
        lightMat[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat[1] = glm::vec4(0.2,0.2,0.2,1.0);        //diffuse
        lightMat[2] = glm::vec4(0.0,0.0,0.8,1.0);        //specular
        lightMat[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess
        m_useProgram->setUniform(LIGHT_UNIFORM, lightMat, false);

        glm::mat4 lightMat2;
        lightMat2[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat2[1] = glm::vec4(0.1,0.1,0.1,1.0);        //diffuse
        lightMat2[2] = glm::vec4(0.8,0.0,0.0,1.0);        //specular
        lightMat2[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess

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

        warmColdColor[0] = glm::vec4(0.75, 0.75, 0.75, 0.0); //surface color
        warmColdColor[1] = glm::vec4(0.6, 0.6, 0.0, 0.0);    //warm color
        warmColdColor[2] = glm::vec4(0.0, 0.0, 0.6, 0.0);    //cold color
        warmColdColor[3] = glm::vec4(0.45,0.45,0,0);         //Diffuse Warm, DiffuseCool
        m_useProgram->setUniform(WARMCOLDCOLOR_UNIFORM, warmColdColor);
    }

    m_shadowMapping->setUniform("biasMatrix", glm::mat4(), false);
    m_shadowMapping->setUniform("samples", &m_shadow_samples[0], m_shadow_samples.size());
    m_shadowMapping->setUniform("sample_count", 32);

    m_SSAO->setUniform("kernel", &m_kernel[0], m_kernel.size());
    m_SSAO->setUniform("noise", &m_noise[0], m_noise.size());
    m_SSAO->setUniform("sample_count", 64);

}

glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

void Painter::paint()
{
    AbstractPainter::paint();

    t_samplerByName sampler;
    m_camera->draw(*m_normalz, m_fboNormalz);

    if(m_useColor)
        m_camera->draw(*m_useProgram, m_fboColor);
    else
        m_fboColor->clear();

    if(m_useShadows)
        createShadows();
        
    if(m_useSSAO)
        createSSAO();

    if(m_blurSSAO)
        addBlur(m_fboSSAO);
    
    m_grid->draw( *m_flat, m_fboGrid);

    sampler.clear();
    sampler["source"] = *m_fboActiveBuffer;
    if(*m_fboActiveBuffer == m_fboColor) {
        sampler["shadows"] = m_fboShadows;
        sampler["ssao"] = m_fboSSAO;
    } else { // dont render effects
        m_fboTemp->clear();
        sampler["shadows"] = m_fboTemp;
        sampler["ssao"] = m_fboTemp;
    }

    bindSampler(sampler, *m_flush);
    m_quad->draw(*m_flush, nullptr);
    releaseSampler(sampler);


}

void Painter::createShadows()
{
    t_samplerByName sampler;

    m_lightcam->draw(*m_lightsource, m_fboShadowMap);

    sampler["shadowMap"] = m_fboShadowMap;

    bindSampler(sampler, *m_shadowMapping);
    m_shadowMapping->setUniform("invCameraTransform", glm::inverse(m_camera->transform()), false);
    m_shadowMapping->setUniform("LightSourceTransform", biasMatrix * m_lightcam->transform(), false);
    m_camera->draw(*m_shadowMapping, m_fboShadows);
    releaseSampler(sampler);
}

void Painter::createSSAO()
{
    t_samplerByName sampler;

    sampler["normalz"] = m_fboNormalz;

    bindSampler(sampler, *m_SSAO);
    m_SSAO->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_SSAO, m_fboSSAO);
    releaseSampler(sampler);
    
}

void Painter::addBlur(FrameBufferObject * fbo)
{
    t_samplerByName sampler;

    sampler["source"] = fbo;
    bindSampler(sampler, *m_blurv);
    m_blurv->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_blurv, m_fboTemp);
    releaseSampler(sampler);    
    sampler.clear();

    sampler["source"] = m_fboTemp;
    bindSampler(sampler, *m_blurh);
    m_blurh->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_blurh, fbo);
    releaseSampler(sampler);
    sampler.clear();
}


void Painter::resize(  //probably never called anywhere?
    const int width
,   const int height)
{
    AbstractPainter::resize(width, height);

    m_camera->setViewport(width, height);
    m_lightcam->setViewport(width, height);
    m_lightcam->update();

    m_fboNormalz->resize(width, height);
    m_fboColor->resize(width, height);
    m_fboTemp->resize(width, height);
    m_fboShadows->resize(width, height);
    m_fboShadowMap->resize(width, height);
    m_fboSSAO->resize(width, height);
    m_fboGrid->resize(width, height);

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

void Painter::setFrameBuffer(int frameBuffer)
{
    switch(frameBuffer) 
    {
        case 1: m_fboActiveBuffer = &m_fboColor; std::printf("\nColor Buffer\n"); break;
        case 2: m_fboActiveBuffer = &m_fboNormalz; std::printf("\nNormal Buffer\n"); break;
        case 3: m_fboActiveBuffer = &m_fboShadows; std::printf("\nShadows Buffer\n"); break;
        case 4: m_fboActiveBuffer = &m_fboShadowMap; std::printf("\nShadowMap Buffer\n"); break;
        case 5: m_fboActiveBuffer = &m_fboSSAO; std::printf("\nSSAO Buffer\n"); break;
    }
}

void Painter::setEffect( int effect, bool active )
{
    switch(effect) 
    {
        case 1: m_useColor = active; std::printf("\nColor toggled\n"); break;
        case 2: m_useShadows = active; std::printf("\nShadow toggled\n"); break;
        case 3: m_useSSAO = active; std::printf("\nSSAO toggled\n"); break;
        case 4: m_blurSSAO = active; std::printf("\nSSAO blur toggled\n"); break;
    }

    m_fboShadows->clear();
    m_fboSSAO->clear();
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
    t_samplerByName::const_iterator i(sampler.cbegin());
    const t_samplerByName::const_iterator iEnd(sampler.cend());

    for(; i != iEnd; ++i)
        i.value()->releaseTexture2D();

}

Camera * Painter::camera()
{
    return m_camera;
}

void Painter::sceneChanged(Group * scene)
{
    if(m_scene)
        m_lightcam->remove(m_scene);
    m_lightcam->append(scene);
}
