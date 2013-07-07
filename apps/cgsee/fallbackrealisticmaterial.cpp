#include "fallbackrealisticmaterial.h"
#include "realisticpainter.h"
#include "core/program.h"
#include "core/material/texture2d.h"
#include "core/material/textureloader.h"
#include <assert.h>
#include <QString>

FallbackRealisticMaterial::FallbackRealisticMaterial():
    m_initialized(false)
{

}

FallbackRealisticMaterial::~FallbackRealisticMaterial()
{

}

void FallbackRealisticMaterial::initialize()
{
    QString root = "data/defaults/RealisticPainter/";
    QStringList textures = RealisticPainter::supportedTextures();

    for(QString texture : textures) {
        Texture2D *tex = TextureLoader::loadTexture2D(root + texture + ".tga");
        tex->setName(texture);
        addAttribute(tex);
    }

    m_initialized = true;
}

bool FallbackRealisticMaterial::bind(Program &program, const QString &name)
{
    if(!m_initialized)
        initialize();

    assert(Material::bind(program, name));
    return true;
}

