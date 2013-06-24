#include "textureloader.h"
#include "texture2d.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <IL/il.h>

using namespace std;


std::map<QString, GLuint> TextureLoader::m_loadedTextures;

Texture2D *TextureLoader::loadTexture2D(const QString &path)
{
    ILboolean re;
    ILuint iTexture;
    GLuint gTexture;
    Texture2D *texture;

    cout << "Loading texture from " << path.toStdString() << endl;

    auto i = m_loadedTextures.find(path);

    if(i == m_loadedTextures.end()) {
        ilGenImages(1, &iTexture);
        ilBindImage(iTexture);

        re = ilLoadImage(path.toStdString().c_str());
        if(!re) {
            assert(0);
            switch(ilGetError()) {
                case IL_COULD_NOT_OPEN_FILE: break;
                case IL_ILLEGAL_OPERATION: break;
                case IL_INVALID_EXTENSION:break;
                case IL_INVALID_PARAM: break;
            }
        }

        re = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
        if(!re) {
            switch(ilGetError()) {
                case IL_ILLEGAL_OPERATION: assert(0); break;
                case IL_INVALID_CONVERSION: assert(0); break;
                case IL_OUT_OF_MEMORY: assert(0); break;
            }
            assert(0);
        }

        glGenTextures(1, &gTexture);
        glBindTexture(GL_TEXTURE_2D, gTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(   GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
                        ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                        0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,  ilGetData());
        ilBindImage(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_loadedTextures[path] = gTexture;
    }
    else {
        gTexture = i->second;
    }

    texture = new Texture2D(gTexture);

    return texture;
}
