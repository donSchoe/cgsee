#pragma once

#include <string>

class Texture2D;

class CGSEE_API TextureLoader
{
public:

    Texture2D *loadTexture2D(const std::string &path);
};
