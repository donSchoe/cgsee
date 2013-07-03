
#pragma once

#include <memory>
#include <core/abstractapplication.h>

class Viewer;
class Painter;
class DataBlockRegistry;
class Camera;
class AbstractNavigation;

class CGSee : public AbstractApplication
{
    Q_OBJECT
public:
    CGSee(int & argc, char ** argv);
    virtual ~CGSee();

protected:
    std::shared_ptr<DataBlockRegistry> m_registry;
    Viewer * m_viewer;
    Painter * m_painter;
};
