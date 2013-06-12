#pragma once

#include <GL/gl.h>
#include "materialattribute.h"

class CGSEE_API Texture2D : public MaterialAttribute
{
public:

    Texture2D();
    ~Texture2D();

    void bindTo(Program & program);

    GLuint glTex() const;
    void setGlTex(GLuint glTex);


private:

    GLuint m_glTex;

};
