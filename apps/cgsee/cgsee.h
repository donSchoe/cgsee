
#pragma once

#include <core/abstractapplication.h>

class Viewer;
class Painter;
class DeferredPainter;

class CGSee : public AbstractApplication
{
    Q_OBJECT
public:
    CGSee(int & argc, char ** argv);
    virtual ~CGSee();

protected:
    Viewer * m_viewer;
    //Painter * m_painter;
    DeferredPainter * m_painter;
};
