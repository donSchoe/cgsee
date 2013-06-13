#pragma once

#include <string>
#include "declspec.h"

class Texture2D;

class CGSEE_API TextureLoader
{
public:

    Texture2D *loadTexture2D(const std::string &path);
};

