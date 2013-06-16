#pragma once

#include <map>
#include <string>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include "declspec.h"

class Texture2D;

// TODO: But this into some kind of singleton
class CGSEE_API TextureLoader
{
public:

    static Texture2D *loadTexture2D(const std::string &path);


protected:

    static std::map<std::string, GLuint> m_loadedTextures;
};

