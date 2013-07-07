#pragma once

#include <core/declspec.h>
#include <core/glformat.h>

class QKeyEvent;
class QString;
class Camera;
class Program;
class AbstractGLParent;

class CGSEE_API AbstractPainter
{
public:
    enum e_ShaderBits {
        VERTEX_SHADER_BIT   = 0x0001,
        GEOMETRY_SHADER_BIT = 0x0002,
        FRAGMENT_SHADER_BIT = 0x0004
    };

public:
    AbstractPainter(void);
    virtual ~AbstractPainter();

    // calls initialize if not initialized - so call this when subclassed
    virtual void paint();

    const QImage capture(
        AbstractGLParent & parent
    ,   const QSize & size
    ,   const bool aspect
    ,   const bool alpha);

    virtual void resize(
        const int width
    ,   const int height);

    Program *loadProgram(const QString &pathPrefix, int flags);


protected:
    virtual const bool initialize() = 0;
    virtual Camera * camera() = 0;

protected:
    bool m_initialized;
};
