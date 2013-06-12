#include "textureloader.h"
#include "texture2d.h"
#include <assert.h>

Texture2D *TextureLoader::loadTexture2D(const std::string &path)
{
    Ilboolean re;
    ILuint ilTexture;
    GLuint glTexture;
    Texture2D *texture;

    ilGenImages(1, &ilTexture);
    ilBindImage(ilTexture);

    re = ilLoadImage(path.c_str());
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
        assert(0);
        switch(ilGetError) {
            case IL_ILLEGAL_OPERATION: break;
            case IL_INVALID_CONVERSIONv
            case IL_OUT_OF_MEMORY: break;
        }
    }

    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(   GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
                    ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
                    0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,  ilGetData());

    ilBindImage(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
