#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <QString>
#include "../program.h"
#include "materialattribute.h"

enum Texture2D_Type {
    Texture2D_Diffuse = 0,
    Texture2D_Ambient,
    Texture2D_Specular,
    Texture2D_Emission,
    Texture2D_Normals,
    Texture2D_Shininess,
    Texture2D_Unknown
};

class CGSEE_API Texture2D : public MaterialAttribute
{
public:

    Texture2D(GLuint gTexId, Texture2D_Type type, const QString &name = "texture");
    ~Texture2D();

    void bind(const Program &program);

    Texture2D_Type type() const;
    void setType(Texture2D_Type type);

protected:
    static GLuint unitOfType(Texture2D_Type type);

protected:

    GLuint m_gTexId;
    Texture2D_Type m_type;
};
