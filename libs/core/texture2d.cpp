#include "texture2d.h"


Texture2D::Texture2D(GLuint gTexId, const std::string &name):
    MaterialAttribute(name),
    m_gTexId(gTexId)
{

}

Texture2D::~Texture2D()
{

}

void Texture2D::bind(Program &program)
{
    GLint location;

    glActiveTexture(GL_TEXTURE0); // TODO - dynamically choose TexUnit?

    location = glGetUniformLocation(program.program(), m_name.c_str());
    glUniform1i(location, GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexId);
}

