#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "program.h"
#include "materialattribute.h"

class CGSEE_API Texture2D : public MaterialAttribute
{
public:

    Texture2D(GLuint gTexId);
    ~Texture2D();

    void bind(const Program &program, const std::string &samplerName = "texture");

private:

    GLuint m_gTexId;
};
