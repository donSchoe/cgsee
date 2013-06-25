#include "texture2d.h"
#include "gpuquery.h"
#include <GL/gl.h>
#include <iostream>

using namespace std;


Texture2D::Texture2D(GLuint gTexId, const std::string &name):
    MaterialAttribute(name),
    m_gTexId(gTexId)
{

}

Texture2D::~Texture2D()
{

}

void Texture2D::bind(const Program &program)
{
    GLint location;

    glError();

    location = glGetUniformLocation(program.program(), m_name.c_str()); glError();

    glUniform1i(location, GL_TEXTURE1); glError();

    glActiveTexture(GL_TEXTURE1); // TODO - dynamically choose TexUnit?
    glBindTexture(GL_TEXTURE_2D, m_gTexId); glError();
}

