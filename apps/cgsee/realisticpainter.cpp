#include <glm/gtx/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

#include "realisticpainter.h"

#include <iostream>
#include <core/autotimer.h>
#include <core/mathmacros.h>
#include <core/glformat.h>
#include <core/camera.h>
#include <core/fileassociatedshader.h>
#include <core/framebufferobject.h>
#include <core/gpuquery.h>
#include <core/textureunitprovider.h>
#include <core/datacore/datablock.h>
#include <core/scenegraph/group.h>
#include <core/scenegraph/scenetraverser.h>
#include <core/scenegraph/drawvisitor.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/objloader.h>
#include <core/assimploader.h>
#include <core/program.h>
#include <core/screenquad.h>
#include "core/navigation/arcballnavigation.h"
#include "core/navigation/flightnavigation.h"

using namespace std;

static const QString TRANSFORM_UNIFORM( "transform" );


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

const glm::mat4 RealisticPainter::biasMatrix (
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
);

QStringList RealisticPainter::supportedTextures()
{
    QStringList textures;

    textures.push_back(DIFFUSE_TEX_NAME);
    textures.push_back(AMBIENT_TEX_NAME);
    textures.push_back(SPECULAR_TEX_NAME);
    textures.push_back(DIFFUSE_TEX_NAME);
    textures.push_back(EMISSION_TEX_NAME);
    textures.push_back(NORMALS_TEX_NAME);
    textures.push_back(SHININESS_TEX_NAME);

    return textures;
}

RealisticPainter::RealisticPainter(Camera * camera):
   m_camera(camera)
, m_lightsource(nullptr)
, m_shadowMapping(nullptr)
, m_SSAO(nullptr)
, m_blurv(nullptr)
, m_blurh(nullptr)
, m_fboColor(nullptr)
, m_fboNormalz(nullptr)
, m_fboTemp(nullptr)
, m_fboShadows(nullptr)
, m_fboSSAO(nullptr)
, m_fboShadowMap(nullptr)
, m_fboActiveBuffer(nullptr)
, m_flush(nullptr)
, m_useColor(true)
, m_useShadows(true)
, m_blurShadows(true)
, m_useSSAO(true)
, m_blurSSAO(true)
, m_kernel(128)
, m_noise(16)
, m_shadow_samples(128)
{
    m_lightcam = new Camera();
    m_lightcam->setViewport(camera->viewport());
    m_lightcam->setFovy(camera->fovy());
    m_lightcam->setZFar(camera->zFar());
    m_lightcam->setZNear(camera->zNear());

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

RealisticPainter::~RealisticPainter()
{
    for(Program *program : m_programs) {
        delete program;
    }

    delete m_lightsource;
    delete m_shadowMapping;
    delete m_SSAO;
    delete m_blurv;
    delete m_blurh;
    delete m_fboColor;
    delete m_fboNormalz;
    delete m_fboShadows;
    delete m_fboShadowMap;
    delete m_fboSSAO;
}

const bool RealisticPainter::initialize()
{
    AutoTimer t("Initialization of RealisticPainter");

    if(m_scene) {
        m_camera->append(m_scene);
    }

    m_quad = new ScreenQuad();

    m_programs[FLAT]    = loadProgram("data/shaders/flat", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT | GEOMETRY_SHADER_BIT);
    m_programs[GOURAUD] = loadProgram("data/shaders/gouraud", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);
    m_programs[PHONG]   = loadProgram("data/shaders/phong", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);
    m_programs[GOOCH]   = loadProgram("data/shaders/gooch", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);

    m_programs[FLAT]->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shaders/phongLighting.glsl"));
    m_programs[GOURAUD]->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shaders/phongLighting.glsl"));
    m_programs[PHONG]->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shaders/phongLighting.glsl"));


    FileAssociatedShader * depth_util = new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/depth_util.frag");

    // SHADOWS
    m_lightsource = loadProgram("data/shaders/shadows/lightsource", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);
    m_lightsource->attach(depth_util);

    m_shadowMapping = loadProgram("data/shaders/shadows/shadowmapping", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);
    m_shadowMapping->attach(depth_util);

    m_normalz = loadProgram("data/shaders/normalz", FRAGMENT_SHADER_BIT | VERTEX_SHADER_BIT);
    m_shadowMapping->attach(depth_util);

    FileAssociatedShader * screenQuadShader = new FileAssociatedShader(GL_VERTEX_SHADER, "data/shaders/screenquad.vert");

    // SSAO
    m_SSAO = loadProgram("data/shaders/shadows/ssao", FRAGMENT_SHADER_BIT);
    m_SSAO->attach(screenQuadShader);

    // Blur
    m_blurv = loadProgram("data/shaders/shadows/gauss_blur_5_v", FRAGMENT_SHADER_BIT);
    m_blurv->attach(screenQuadShader);

    m_blurh = loadProgram("data/shaders/shadows/gauss_blur_5_h", FRAGMENT_SHADER_BIT);
    m_blurh->attach(screenQuadShader);

    m_activeProgram = m_programs[FLAT];
    setUniforms();
    setShaderProperties();

    m_flush = new Program();
    m_flush->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shaders/flush.frag"));
    m_flush->attach(screenQuadShader);

    m_fboNormalz = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboColor = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboTemp = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboShadows = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboShadowMap = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);
    m_fboSSAO = new FrameBufferObject(GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT0, true);

    m_fboActiveBuffer = &m_fboColor;


    return true;
}

void RealisticPainter::setUniforms()
{
    if(m_activeProgram == m_programs[FLAT] || m_activeProgram == m_programs[GOURAUD] || m_activeProgram == m_programs[PHONG])
    {
        m_activeProgram->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,0.0,7.5));
        m_activeProgram->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,-8.0,0.0));

        m_activeProgram->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.2));

        glm::mat4 lightMat;
        lightMat[0] = glm::vec4(0.2,0.2,0.2,1.0);    //ambient
        lightMat[1] = glm::vec4(0.4,0.6,0.8,1.0);    //diffuse
        lightMat[2] = glm::vec4(0.0,0.0,0.8,1.0);    //specular
        lightMat[3] = glm::vec4(0.000,0.000,0.0000,1.4);    //attenuation1, attenuation2, attenuation3, shininess
        m_activeProgram->setUniform(LIGHT_UNIFORM, lightMat, false);

        glm::mat4 lightMat2;
        lightMat2[0] = glm::vec4(0.3,0.3,0.3,1.0);    //ambient
        lightMat2[1] = glm::vec4(0.9,0.5,0.5,1.0);    //diffuse
        lightMat2[2] = glm::vec4(0.8,0.0,0.0,1.0);    //specular
        lightMat2[3] = glm::vec4(0.000,0.000,0.0000,1.4);    //attenuation1, attenuation2, attenuation3, shininess

        m_activeProgram->setUniform(LIGHT_UNIFORM2, lightMat2, false);

        glm::mat4 materialCoeff;
        materialCoeff[0] = glm::vec4(0.1,0.1,0.1,1.0);    //ambient
        materialCoeff[1] = glm::vec4(1.0,1.0,1.0,1.0);    //diffuse
        materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);    //specular
        materialCoeff[3] = glm::vec4(0,0,0,0);            //emission

        m_activeProgram->setUniform(MATERIAL_UNIFORM, materialCoeff);
    }

    else if(m_activeProgram == m_programs[GOOCH])
    {
        m_activeProgram->setUniform(LIGHTPOSITION_UNIFORM, glm::vec3(-2.0,0.0,2.0));

        glm::mat4 warmColdColor;

        warmColdColor[0] = glm::vec4(0.75, 0.75, 0.75, 0.0);    //surface color
        warmColdColor[1] = glm::vec4(0.6, 0.6, 0.0, 0.0);    //warm color
        warmColdColor[2] = glm::vec4(0.0, 0.0, 0.6, 0.0);    //cold color
        warmColdColor[3] = glm::vec4(0.45,0.45,0,0);            //Diffuse Warm, DiffuseCool
        m_activeProgram->setUniform(WARMCOLDCOLOR_UNIFORM, warmColdColor);
    }

    m_shadowMapping->setUniform("samples", &m_shadow_samples[0], m_shadow_samples.size());
    m_SSAO->setUniform("kernel", &m_kernel[0], m_kernel.size());
    m_SSAO->setUniform("noise", &m_noise[0], m_noise.size());
}

// some time in the future this may be variable properties
void RealisticPainter::setShaderProperties() {
    m_shadowMapping->setUniform("lightSize", 0.03f);
    m_shadowMapping->setUniform("searchWidth", 0.02f);
    m_shadowMapping->setUniform("zOffset", 0.0015f);
    m_shadowMapping->setUniform("sample_count", 24); // usefull range: 0-128

    m_SSAO->setUniform("sample_count", 32); // usefull range: 0-128
    m_SSAO->setUniform("zOffset", 0.005f);
    m_SSAO->setUniform("filterRadius", 0.05f);
}

void RealisticPainter::paint()
{
    assert(TextureUnitProvider::instance()->allFree());
    AbstractScenePainter::paint();

    t_samplerByName sampler;

    m_camera->update();

    //drawScene(m_camera, m_normalz, m_fboNormalz);

    assert(TextureUnitProvider::instance()->allFree());
    assert(m_useColor);

    if(m_useColor)
        drawScene(m_camera, m_programs[FLAT], m_fboColor);
    else
        m_fboColor->clear();

    if(m_useShadows)
        createShadows();
//
//    if(m_useShadows && m_blurShadows)
//        addBlur(m_fboShadows);
//
//    if(m_useSSAO)
//        createSSAO();
//
//    if(m_useSSAO && m_blurSSAO)
//        addBlur(m_fboSSAO);


    sampler.clear();
    sampler["source"] = *m_fboActiveBuffer;
//    if(*m_fboActiveBuffer == m_fboColor) {
//        sampler["shadows"] = m_fboShadows;
//        sampler["ssao"] = m_fboSSAO;
//    } else { // dont render effects
        m_fboTemp->clear();
        sampler["shadows"] = m_fboTemp;
        sampler["ssao"] = m_fboTemp;
//    }


    bindSampler(sampler, *m_flush);
    m_quad->draw(*m_flush, nullptr);
    releaseSampler(sampler);
    assert(TextureUnitProvider::instance()->allFree());
}

void RealisticPainter::draw(Node & node, const glm::mat4 & transform)
{
    node.draw(*m_activeProgram, transform);
}

void RealisticPainter::draw(PolygonalDrawable & drawable, const glm::mat4 & transform)
{
    TextureUnitProvider::instance()->push();

    assert(m_activeProgram);

    m_activeProgram->use();
    m_activeProgram->setUniform(TRANSFORM_UNIFORM, transform);

    if(drawable.material() != nullptr) {
        QStringList textures = supportedTextures();
        for(QString texture : textures) {
            if(!drawable.material()->bind(*m_activeProgram, texture)) {
                m_fallbackMaterial.bind(*m_activeProgram, texture);
            }
        }
    }
    else {
        m_fallbackMaterial.bindAll(*m_activeProgram);
    }

    drawable.draw(*m_activeProgram, transform);

    m_activeProgram->release();
    TextureUnitProvider::instance()->pop();
}


void RealisticPainter::drawScene(Camera * camera, Program * program, FrameBufferObject * fbo)
{
    m_activeProgram = program;
    fbo->bind();
    SceneTraverser traverser;
    DrawVisitor drawVisitor(*this, camera->transform());
    traverser.traverse(*camera, drawVisitor);
    fbo->release();
}

void RealisticPainter::createShadows()
{
    t_samplerByName sampler;

    drawScene(m_lightcam, m_lightsource, m_fboShadowMap);

    sampler["shadowMap"] = m_fboShadowMap;

    bindSampler(sampler, *m_shadowMapping);
    m_shadowMapping->setUniform("invCameraTransform", glm::inverse(m_camera->transform()), false);
    m_shadowMapping->setUniform("LightSourceTransform", biasMatrix * m_lightcam->transform(), false);
    drawScene(m_camera, m_shadowMapping, m_fboShadows);
    releaseSampler(sampler);
}

void RealisticPainter::createSSAO()
{
    t_samplerByName sampler;

    sampler["normalz"] = m_fboNormalz;

    bindSampler(sampler, *m_SSAO);
    m_SSAO->setUniform("viewport", m_camera->viewport());
    m_quad->draw(*m_SSAO, m_fboSSAO);
    releaseSampler(sampler);

}

void RealisticPainter::addBlur(FrameBufferObject * fbo)
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
}


void RealisticPainter::resize( //probably never called anywhere?
    const int width
, const int height)
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

    postShaderRelinked();

}

void RealisticPainter::setFrameBuffer(int frameBuffer)
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

void RealisticPainter::setEffect( int effect, bool active )
{
    switch(effect)
    {
        case 1: m_useColor = active; std::printf("\nColor toggled\n"); break;
        case 2: m_useShadows = active; std::printf("\nShadow toggled\n"); break;
        case 3: m_blurShadows = active; std::printf("\nShadow blur toggled\n"); break;
        case 4: m_useSSAO = active; std::printf("\nSSAO toggled\n"); break;
        case 5: m_blurSSAO = active; std::printf("\nSSAO blur toggled\n"); break;
    }

    m_fboShadows->clear();
    m_fboSSAO->clear();
}

void RealisticPainter::postShaderRelinked()
{
}

void RealisticPainter::bindSampler(
    const t_samplerByName & samplers
,   const Program & program)
{
    TextureUnitProvider::instance()->push();
    glm::uint slot = 0;
    for(auto samplerKey : samplers.keys()) {
        samplers[samplerKey]->bindTexture2D(program, samplerKey, slot);
        slot++;
    }
}

void RealisticPainter::releaseSampler(
    const t_samplerByName & samplers)
{
    for(auto sampler : samplers) {
        sampler->releaseTexture2D();
    }
    TextureUnitProvider::instance()->pop();
}

Camera * RealisticPainter::camera()
{
    return m_camera;
}


void RealisticPainter::sceneChanged(Group * scene)
{
    if(m_scene)
        m_lightcam->remove(m_scene);
    m_lightcam->append(scene);

    AxisAlignedBoundingBox bb = scene->boundingBox();
    m_lightcam->setView(glm::lookAt(glm::vec3(3.5, 5.0, 5.5)+bb.center(), bb.center(), glm::vec3(0.0,1.0,0.0)));

}
