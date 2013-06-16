#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "program.h"
#include "materialattribute.h"

class CGSEE_API Texture2D : public MaterialAttribute
{
public:

    Texture2D(GLuint gTexId, const std::string &name = "texture");
    ~Texture2D();

    void bind(Program &program);

private:

    GLuint m_gTexId;
};
