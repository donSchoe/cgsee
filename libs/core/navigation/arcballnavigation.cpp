#include <glm/gtx/transform.hpp>

#include "arcballnavigation.h"
#include "../camera.h"

ArcballNavigation::ArcballNavigation(Camera * camera) 
    : AbstractNavigation(camera)
    , m_mouse_last(0)
    , m_mouse_cur(0)
    , m_arcball_on(false)
    , m_zoom_on(false)
    , m_panning_on(false)
{
}

ArcballNavigation::~ArcballNavigation()
{
}


void ArcballNavigation::updateArcball()
{
    glm::vec3 va = getArcballVector(m_mouse_last);
    glm::vec3 vb = getArcballVector(m_mouse_cur);

    float angle = glm::acos(std::min(1.0f, glm::dot(va, vb)));
    glm::vec3 axis_in_camera_coord = glm::cross(va, vb);

    glm::mat3 camera2world = glm::mat3(glm::inverse(m_viewmatrix));
    glm::vec3 axis_in_world_coord = camera2world * axis_in_camera_coord;
    m_viewmatrix = m_viewmatrix * glm::rotate(glm::degrees(angle), axis_in_world_coord);
}

/**
 * Get a normalized vector from the center of the virtual ball O to a
 * point P on the virtual ball surface, such that P is aligned on
 * screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 * sphere, return the nearest point on the virtual ball surface.
 * source: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
 */
glm::vec3 ArcballNavigation::getArcballVector(glm::vec2 v)
{
    // normalize mouse coordinates to [-1:1]
    glm::vec3 P = glm::vec3(v.x/m_width * 2.0 - 1.0,
                            v.y/m_height * 2.0 - 1.0,
                            0);
    P.y = -P.y;
    float P_squared = P.x * P.x + P.y * P.y;
    if (P_squared <= 1*1) {                     // P is inside ball
        P.z = sqrt(1*1 - P_squared);            // calculate height in ball
    } else {
        P = glm::normalize(P);                  // nearest point to the ball
    }
    return P;
}

void ArcballNavigation::updatePanning()
{
    glm::vec2 delta = (m_mouse_cur - m_mouse_last);
    glm::mat4 translate = glm::translate(glm::vec3(delta.x/500, -delta.y/500, 0));
    m_viewmatrix = translate * m_viewmatrix;
}

void ArcballNavigation::updateZoom()
{
    float delta = (m_mouse_cur - m_mouse_last).y / m_height; // normalized height difference
    glm::vec3 zoom_in_camera = glm::vec3(0, 0, -delta*5) * glm::mat3(m_viewmatrix) ; // TODO relative to z-buffer
    
    glm::mat4 translate = glm::translate(glm::mat4(), zoom_in_camera);
    m_viewmatrix = m_viewmatrix * translate;
}


void ArcballNavigation::mouseMoveEvent(QMouseEvent * event)
{
    m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
    if (m_mouse_cur != m_mouse_last) { // avoid div/0
        if (m_arcball_on) {
            updateArcball();
        } else if (m_panning_on) {
            updatePanning();
        } else if (m_zoom_on) {
            updateZoom();
        }
        m_mouse_last = m_mouse_cur;
        updateCamera();
    }
}

void ArcballNavigation::mousePressEvent(QMouseEvent * event)
{
    m_arcball_on = event->button() == Qt::LeftButton;
    m_panning_on = event->button() == Qt::RightButton;
    m_zoom_on = event->button() == Qt::MiddleButton;
    m_mouse_last = m_mouse_cur = glm::vec2(event->pos().x(), event->pos().y());
}

void ArcballNavigation::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) {
        m_arcball_on = false;
    } else if (event->button() == Qt::RightButton) {
        m_panning_on = false;
    } else if (event->button() == Qt::MiddleButton) {
        m_zoom_on = false;
    }
}

