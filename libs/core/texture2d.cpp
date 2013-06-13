#include "texture2d.h"


Texture2D::Texture2D(GLuint gTexId)
{
    m_gTexId = gTexId;
}

Texture2D::~Texture2D()
{

}

void Texture2D::bind(const Program &program, const std::string &samplerName)
{
    GLint location;

    glActiveTexture(GL_TEXTURE0); // TODO - dynamically choose TexUnit?

    location = glGetUniformLocation(program.program(), samplerName.c_str());
    glUniform1i(location, GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexId);
}

