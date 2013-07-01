#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <QString>
#include "../program.h"
#include "materialattribute.h"

class CGSEE_API Texture2D : public MaterialAttribute
{
public:

    Texture2D(GLuint gTexId, const QString &name = "texture");
    ~Texture2D();

    void bind(const Program &program);

private:

    GLuint m_gTexId;
};
