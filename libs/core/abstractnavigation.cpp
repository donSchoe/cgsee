#include "abstractnavigation.h"
#include "camera.h"

AbstractNavigation::AbstractNavigation(Camera *camera) : m_camera(camera)
{
    
}


AbstractNavigation::~AbstractNavigation(void) 
{
}

void AbstractNavigation::keyPressEvent( QKeyEvent *event ) { }

void AbstractNavigation::keyReleaseEvent( QKeyEvent *event ) { }

void AbstractNavigation::mouseMoveEvent(QMouseEvent * event) { }
void AbstractNavigation::mousePressEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseReleaseEvent(QMouseEvent * event) { }
void AbstractNavigation::mouseDoubleClickEvent(QMouseEvent * event) { }

void AbstractNavigation::wheelEvent(QWheelEvent *event) { }

void AbstractNavigation::setViewPort( const int width, const int height )
{
    m_width = width;
    m_height = height;
    m_viewMatrix = m_camera->view();
    m_viewMatrixInverted = glm::inverse(m_viewMatrix);
}

void AbstractNavigation::setFovy(float fovy) { }
float AbstractNavigation::getFovy(){
    //Return Standard FOV if Navigation hasn't implemented a FOV manipulator
    return 45.0f;
}

