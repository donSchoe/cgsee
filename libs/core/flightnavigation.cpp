//glm to rotate around axis
#include <glm/gtx/rotate_vector.hpp>

#include "flightnavigation.h"
#include "camera.h"


float speed = 0.003f;
float angle = 0.05f;

FlightNavigation::FlightNavigation(Camera * camera) 
    : AbstractNavigation(camera)
    , m_direction(0)
    , m_yprAngle(0)
{
    setFromMatrix(m_viewmatrix);
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        // Pitch, Yaw, roll
        case Qt::Key_W:
            m_yprAngle.y = 1.0f;
            break;
        case Qt::Key_S:
            m_yprAngle.y = -1.0f;
            break;
        case Qt::Key_A:
            m_yprAngle.x = -1.0f;
            break;
        case Qt::Key_D:
            m_yprAngle.x = 1.0f;
            break;
        case Qt::Key_Q:
            m_yprAngle.z = 1.0f;
            break;
        case Qt::Key_E:
            m_yprAngle.z = -1.0f;
            break;

        //Move forward, backward
        case Qt::Key_Up:
            m_direction.y = 1.0f;
            break;
        case Qt::Key_Down:
            m_direction.y = -1.0f;
            break;
        case Qt::Key_Left:
            m_direction.x = 1.0f;
            break;
        case Qt::Key_Right:
            m_direction.x = -1.0f;
            break;
            
        default:
            return; // prevent startTimer
    }
    startTimer();
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    switch (event->key()) {
        // Pitch, Yaw, roll
        case Qt::Key_W:
            if (m_yprAngle.y == 1.0f) {
                m_yprAngle.y = 0.0f;
            }
            break;
        case Qt::Key_S:
            if (m_yprAngle.y == -1.0f) {
                m_yprAngle.y = 0.0f;
            }
            break;
        case Qt::Key_D:
            if (m_yprAngle.x == 1.0f) {
                m_yprAngle.x = 0.0f;
            }
            break;
        case Qt::Key_A:
            if (m_yprAngle.x == -1.0f) {
                m_yprAngle.x = 0.0f;
            }
            break;
        case Qt::Key_Q:
            if (m_yprAngle.z == 1.0f) {
                m_yprAngle.z = 0.0f;
            }
            break;
        case Qt::Key_E:
            if (m_yprAngle.z == -1.0f) {
                m_yprAngle.z = 0.0f;
            }
            break;
            
        //Move forward, backward
        case Qt::Key_Up:
            if (m_direction.y == 1.0f) {
                m_direction.y = 0.0f;
            }
            break;
        case Qt::Key_Down:
            if (m_direction.y == -1.0f) {
                m_direction.y = 0.0f;
            }
            break;
        case Qt::Key_Left:
            if (m_direction.x == 1.0f) {
                m_direction.x = 0.0f;
            }
            break;
        case Qt::Key_Right:
            if (m_direction.x == -1.0f) {
                m_direction.x = 0.0f;
            }
            break;
            
        default:
            return; // prevent stopTimer
    }
    stopTimer();
}

void FlightNavigation::wheelEvent(QWheelEvent *event){
   
}

void FlightNavigation::setFromMatrix(glm::mat4 view){ 
    
    //Extract Up Vector and Viewing direction from viewmatrix
    m_up.x = view[0][1];
    m_up.y = view[1][1];
    m_up.z = view[2][1];
    
    glm::vec3 direction;
    direction.x = - view [0][2];
    direction.y = - view [1][2];
    direction.z = - view [2][2];
    
    //Get Camera position (from: http://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix )
    
    glm::mat4 modelViewT = glm::transpose(view);
    
    // Get plane normals
    glm::vec3 n1(modelViewT[0]);
    glm::vec3 n2(modelViewT[1]);
    glm::vec3 n3(modelViewT[2]);
    
    // Get plane distances
    float d1(modelViewT[0].w);
    float d2(modelViewT[1].w);
    float d3(modelViewT[2].w);
    
    // Get the intersection of these 3 planes
    // (using math from RealTime Collision Detection by Christer Ericson)
    glm::vec3 n2n3 = glm::cross(n2, n3);
    float denom = glm::dot(n1, n2n3);
    
    m_eye = (n2n3 * d1) + glm::cross(n1, (d3*n2) - (d2*n3));
    m_eye /= -denom;
    
    m_center = m_eye + direction;
    
    updateView();
}

void FlightNavigation::reset() {
    AbstractNavigation::reset();
    setFromMatrix(m_viewmatrix);
}

void FlightNavigation::onCameraChanged(){
    m_yView = m_up;
    m_zView = m_center - m_eye;
    m_xView = glm::cross(m_yView, m_zView);
}

void FlightNavigation::updateView(){
    m_viewmatrix =  glm::lookAt(m_eye, m_center, m_up);
    updateCamera();
}


void FlightNavigation::yaw(float angle){
    glm::vec3 diffLR = glm::rotate( m_zView, -angle, m_yView) - m_zView;
    m_center += diffLR;
    updateView();
}

void FlightNavigation::pitch(float angle){
    glm::vec3 diffCenter = glm::rotate( m_zView, -angle, m_xView) - m_zView;
    glm::vec3 diffUp = glm::rotate( m_yView, -angle, m_xView) - m_yView;
    m_center += diffCenter;
    m_up += diffUp;
    updateView();
}

void FlightNavigation::roll(float angle){
    glm::vec3 diffRotZ = glm::rotate(m_yView, angle, m_zView) - m_yView;
    m_up += diffRotZ;
    updateView();
}

void FlightNavigation::forward(float speed){
    glm::vec3 step = speed * m_zView;
    m_center += step;
    m_eye += step;
    updateView();
}

void FlightNavigation::sideward(float speed){
    glm::vec3 step = speed * m_xView;
    m_center += step;
    m_eye += step;
    updateView();
}

void FlightNavigation::onTimerEvent(){
    if (m_direction.y||m_direction.x) {
        forward(m_direction.y * speed * TIMER_MS);
        sideward(m_direction.x * speed * TIMER_MS);
    }
    if (m_yprAngle.x||m_yprAngle.y||m_yprAngle.z){
        yaw(m_yprAngle.x * angle * TIMER_MS);
        pitch(m_yprAngle.y * angle * TIMER_MS);
        roll(m_yprAngle.z * angle * TIMER_MS);
    }
}


