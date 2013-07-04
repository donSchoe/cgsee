//glm to rotate around axis
#include <glm/gtx/rotate_vector.hpp>

#include "fpsnavigation.h"
#include "../camera.h"



FpsNavigation::FpsNavigation(Camera * camera)
: FlightNavigation(camera)
, m_inGameMode(false)
, m_lastMousePosition(glm::vec2(-1,-1))
, m_direction(0)
, m_pitchAngle(0.0f)
{
    setFromMatrix(m_viewmatrix);
    if (m_up != glm::vec3 (0,1,0)){
        glm::mat4 newview = glm::lookAt(m_eye, m_center, glm::vec3 (0,1,0));
        loadView(newview);
    }
    m_pitchAngle= 180 - glm::degrees( std::acos(glm::dot( m_center, m_yView) / ( glm::length(m_center) * glm::length(m_yView))));
}

FpsNavigation::~FpsNavigation()
{
    
}

void FpsNavigation::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_W:
            m_direction.y = 1;
            break;
        case Qt::Key_S:
            m_direction.y = -1;
            break;
        case Qt::Key_A:
            m_direction.x = 1;
            break;
        case Qt::Key_D:
            m_direction.x = -1;
            break;
        default:
            return; // prevent startTimer
    }
    startTimer();
}

void FpsNavigation::keyReleaseEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_W:
        if (m_direction.y == 1)
            m_direction.y = 0;
        break;
    case Qt::Key_S:
        if (m_direction.y == -1)
            m_direction.y = 0;
        break;
    case Qt::Key_A:
        if (m_direction.x == 1)
            m_direction.x = 0;
        break;
    case Qt::Key_D:
        if (m_direction.x == -1)
            m_direction.x = 0;
        break;
    default:
        return; // prevent stopTimer
    }
    stopTimer();
}

void FpsNavigation::mouseMoveEvent(QMouseEvent * event){
    if (m_inGameMode){
        if (m_lastMousePosition != glm::vec2(-1,-1)){
            float divX = event->x()-m_lastMousePosition.x;
            float divY = event->y()-m_lastMousePosition.y;
            
            divX *= 0.3f;
            divY *= 0.3f;
            pitchYaw(-divY, divX);
        }
        m_lastMousePosition = glm::vec2(event->x(),event->y());
    }
}



void FpsNavigation::mousePressEvent(QMouseEvent * event){
    m_inGameMode = true;
    m_lastMousePosition = glm::vec2(-1,-1);
}

void FpsNavigation::mouseReleaseEvent(QMouseEvent * event){
    
}


void FpsNavigation::pitchYaw(float pitchAngle, float yawAngle){
    pitch(pitchAngle);
    yaw(yawAngle);
}

void FpsNavigation::forward(float speed){
    glm::vec3 direction = (m_zView * speed);
    m_center.xz += direction.xz;
    m_eye.xz += direction.xz;
    updateView();
}

void FpsNavigation::onTimerEvent()
{
    float speed = m_BBRadius*0.01;
    if (m_direction.x && m_direction.y) {
        forward(m_direction.y * speed * TIMER_MS / sqrt(2));
        sideward(m_direction.x * speed * TIMER_MS / sqrt(2));
    }
    else if (m_direction.x || m_direction.y) {
        forward(m_direction.y * speed * TIMER_MS);
        sideward(m_direction.x * speed * TIMER_MS);
    }
    
}

void FpsNavigation::sideward(float speed){
    glm::vec3 direction = (m_xView * speed);
    m_center.xz += direction.xz;
    m_eye.xz += direction.xz;
    updateView();
}

void FpsNavigation::pitch(float angle){
    if( m_pitchAngle + angle <  179.0f && m_pitchAngle + angle > 1.0f){
        m_pitchAngle += angle;
        glm::vec3 diffCenter = glm::rotate( m_center-m_eye, -angle, m_xView) - (m_center-m_eye);
        m_center += diffCenter;
        updateView();
    }
}


