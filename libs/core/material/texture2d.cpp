#include "texture2d.h"
#include "../gpuquery.h"
#include <GL/gl.h>
#include <iostream>
#include <core/textureunitprovider.h>

using namespace std;


Texture2D::Texture2D(GLuint gTexId, Texture2D_Type type, const QString &name):
    MaterialAttribute(name),
    m_gTexId(gTexId),
    m_type(type)
{

}

Texture2D::~Texture2D()
{

}

void Texture2D::bind(const Program &program)
{
    GLuint unit, id;
    //TextureUnitProvider::instance()->allocate(unit, id);

    glActiveTexture(GL_TEXTURE0 + unitOfType(m_type));
    glError();

    glBindTexture(GL_TEXTURE_2D, m_gTexId);
    glError();

    program.setUniform(m_name, unitOfType(m_type));
}

Texture2D_Type Texture2D::type() const {
    return m_type;
}

void Texture2D::setType(Texture2D_Type type) {
    m_type = type;
}

GLuint Texture2D::unitOfType(Texture2D_Type type) {
    return type;
}
