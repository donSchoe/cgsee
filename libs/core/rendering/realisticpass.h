#pragma once

#include "defaultpass.h"
#include <core/material/fallbackrealisticmaterial.h>

class QString;

class CGSEE_API RealisticPass : public DefaultPass
{
public:
    RealisticPass(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~RealisticPass();

    virtual void preRenderHook(PolygonalDrawable &drawable);
    virtual void postRenderHook(PolygonalDrawable &drawable);

    void setUniforms();

protected:
    FallbackRealisticMaterial m_fallbackMaterial;

    void bindMaterial(PolygonalDrawable &drawable, const QString &name);
};

