#include "cgsee.h"
#include "painter.h"

#include <gui/viewer.h>
#include <core/camera.h>
#include <core/arcballnavigation.h>
#include <core/flightnavigation.h>

CGSee::CGSee(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(nullptr)
,   m_painter(nullptr)
{
    // Create Viewer
    m_viewer = new Viewer();
    m_viewer->setWindowTitle(title());
    m_viewer->initialize(format());

    Camera * camera = new Camera();
    camera->setFovy (45.0f);
    camera->setZNear( 1.0f);
    camera->setZFar (10.0f);
    m_viewer->setCamera(camera);

    m_painter = new Painter(camera);
    m_viewer->setPainter(m_painter);

    AbstractNavigation * navigation = new ArcballNavigation(camera);
    navigation->reset();
    m_viewer->setNavigation(navigation);
    
    // Start
    m_viewer->show();
}



CGSee::~CGSee()
{
    // NOTE: painter must be destroyed before viewer, since viewer deletes context, 
    // and painter deinitializes all opengl handles...

    delete m_painter;
    delete m_viewer;
}
