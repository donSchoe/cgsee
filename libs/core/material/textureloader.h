#pragma once

#include <map>
#include <string>
#include <QString>
#include <GL/glew.h>
#include <GL/gl.h>
#include "../declspec.h"

class Texture2D;

// TODO: Put this into some kind of singleton
class CGSEE_API TextureLoader
{
public:

    static Texture2D *loadTexture2D(const QString &path);


protected:

    static std::map<QString, GLuint> m_loadedTextures;
};

