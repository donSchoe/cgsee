#include "fallbackrealisticmaterial.h"
#include "core/program.h"
#include "core/material/texture2d.h"
#include "core/material/textureloader.h"
#include "core/assimploader.h"
#include <assert.h>
#include <QString>
#include <QStringList>

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
    QStringList textures = supportedTextures();

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

    assert(Material::bindAttribute(program, name));
    return true;
}

QStringList FallbackRealisticMaterial::supportedTextures()
{
    QStringList textures;

    textures.push_back(DIFFUSE_TEX_NAME);
    textures.push_back(AMBIENT_TEX_NAME);
    textures.push_back(SPECULAR_TEX_NAME);
    textures.push_back(DIFFUSE_TEX_NAME);
    textures.push_back(EMISSION_TEX_NAME);
    textures.push_back(NORMALS_TEX_NAME);
    textures.push_back(SHININESS_TEX_NAME);

    return textures;
}
