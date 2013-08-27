#pragma once

#include <list>
#include <string>
#include <QStringList>
#include "core/material/material.h"

class QString;
class Program;

class FallbackRealisticMaterial : public Material
{
public:
    FallbackRealisticMaterial();
    virtual ~FallbackRealisticMaterial();

    void initialize();

    bool bind(Program &program, const QString &name);

protected:
    bool m_initialized;

    static QStringList supportedTextures();
};

