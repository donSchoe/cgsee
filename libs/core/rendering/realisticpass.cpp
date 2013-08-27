#include <QString>
#include <core/program.h>
#include <core/assimploader.h>
#include <core/scenegraph/polygonaldrawable.h>
#include <core/fileassociatedshader.h>

#include "realisticpass.h"

static const QString LIGHTDIR_UNIFORM ("lightdir");
static const QString LIGHTDIR_UNIFORM2 ("lightdir2");
static const QString LIGHTAMBIENTGLOBAL_UNIFORM ("lightambientglobal");
static const QString LIGHT_UNIFORM ("light");
static const QString LIGHT_UNIFORM2 ("light2");
static const QString MATERIAL_UNIFORM ("material");
static const QString LIGHTPOSITION_UNIFORM ("lightposition");
//gooch
static const QString WARMCOLDCOLOR_UNIFORM ("warmcoldcolor");

RealisticPass::RealisticPass(Camera * camera, FileAssociatedShader * depth_util)
:   DefaultPass(camera)
{
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phong.frag"));
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shading/phong.vert"));
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shading/phongLighting.glsl"));
}

RealisticPass::~RealisticPass()
{

}

void RealisticPass::preRenderHook(PolygonalDrawable &drawable) {
    bindMaterial(drawable, DIFFUSE_TEX_NAME);
    bindMaterial(drawable, SPECULAR_TEX_NAME);
    bindMaterial(drawable, EMISSION_TEX_NAME);
    //bindMaterial(drawable, NORMALS_TEX_NAME);
    bindMaterial(drawable, SHININESS_TEX_NAME);
}

void RealisticPass::postRenderHook(PolygonalDrawable &drawable) {

}

void RealisticPass::bindMaterial(PolygonalDrawable &drawable, const QString &name) {
    if(drawable.material() == nullptr || !drawable.material()->bindAttribute(*m_program, name)) {
        m_fallbackMaterial.bindAttribute(*m_program, name);
    }
}

void RealisticPass::setUniforms() {
//    if(m_useProgram == m_flat || m_useProgram == m_gouraud || m_useProgram == m_phong)
//    {
        m_program->setUniform(LIGHTDIR_UNIFORM, glm::vec3(0.0,6.5,7.5));
        m_program->setUniform(LIGHTDIR_UNIFORM2, glm::vec3(0.0,-8.0,7.5));

        m_program->setUniform(LIGHTAMBIENTGLOBAL_UNIFORM, glm::vec4(0.0));

        glm::mat4 lightMat;
        lightMat[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat[1] = glm::vec4(0.2,0.2,0.2,1.0);        //diffuse
        lightMat[2] = glm::vec4(0.0,0.0,0.8,1.0);        //specular
        lightMat[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess
        m_program->setUniform(LIGHT_UNIFORM, lightMat, false);

        glm::mat4 lightMat2;
        lightMat2[0] = glm::vec4(0.0,0.0,0.0,1.0);        //ambient
        lightMat2[1] = glm::vec4(0.1,0.1,0.1,1.0);        //diffuse
        lightMat2[2] = glm::vec4(0.8,0.0,0.0,1.0);        //specular
        lightMat2[3] = glm::vec4(0.002,0.002,0.0004,1.4); //attenuation1, attenuation2, attenuation3, shininess

        m_program->setUniform(LIGHT_UNIFORM2, lightMat2, false);

        glm::mat4 materialCoeff;
        materialCoeff[0] = glm::vec4(0.1,0.1,0.1,1.0);    //ambient
        materialCoeff[1] = glm::vec4(1.0,1.0,1.0,1.0);    //diffuse
        materialCoeff[2] = glm::vec4(1.0,1.0,1.0,1.0);    //specular
        materialCoeff[3] = glm::vec4(0,0,0,0);            //emission

        m_program->setUniform(MATERIAL_UNIFORM, materialCoeff);
//    }
//
//    else if(m_useProgram == m_gooch)
//    {
//        m_useProgram->setUniform(LIGHTPOSITION_UNIFORM, glm::vec3(-2.0,0.0,2.0));
//
//        glm::mat4 warmColdColor;
//
//        warmColdColor[0] = glm::vec4(0.75, 0.75, 0.75, 0.0); //surface color
//        warmColdColor[1] = glm::vec4(0.6, 0.6, 0.0, 0.0);    //warm color
//        warmColdColor[2] = glm::vec4(0.0, 0.0, 0.6, 0.0);    //cold color
//        warmColdColor[3] = glm::vec4(0.45,0.45,0,0);         //Diffuse Warm, DiffuseCool
//        m_useProgram->setUniform(WARMCOLDCOLOR_UNIFORM, warmColdColor);
//    }



}


