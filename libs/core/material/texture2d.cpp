#include "texture2d.h"
#include "../gpuquery.h"
#include <GL/gl.h>
#include <iostream>
#include <core/textureunitprovider.h>

using namespace std;


Texture2D::Texture2D(GLuint gTexId, const QString &name):
    MaterialAttribute(name),
    m_gTexId(gTexId)
{

}

Texture2D::~Texture2D()
{

}

void Texture2D::bind(const Program &program)
{
    GLuint unit, id;
    TextureUnitProvider::instance()->allocate(unit, id);

    glActiveTexture(unit);
    glError();

    glBindTexture(GL_TEXTURE_2D, m_gTexId);
    glError();

    program.setUniform(m_name, id);
}

