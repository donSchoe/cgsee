
#pragma once

#include <QStringList>

#include <core/declspec.h>
#include <core/glformat.h>


class QKeyEvent;

class CGSEE_API AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    // calls initialize if not initialized - so call this when subclassed
    virtual void paint();

    virtual void resize(
        const int width
    ,   const int height);

    virtual void keyPress(QKeyEvent * event);
    virtual void keyRelease(QKeyEvent * event);

    // TODO: more to come...

protected:
    virtual const bool initialize() = 0;

protected:
    bool m_initialized;
};
