#include "texture2d.h"
#include "gpuquery.h"
#include <GL/gl.h>
#include <iostream>

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
    glActiveTexture(GL_TEXTURE0 + 1);
    glError();

    glBindTexture(GL_TEXTURE_2D, m_gTexId);
    glError();

    program.setUniform(m_name, 1);
}

