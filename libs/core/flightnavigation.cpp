
#include "flightnavigation.h"

//glm to rotate around axis
#include "glm/gtx/rotate_vector.hpp"

FlightNavigation::FlightNavigation(void)
{
    reset();
}

FlightNavigation::~FlightNavigation(void)
{
    
}

void FlightNavigation::keyPressEvent(QKeyEvent *event){
  //  float speed = 0.05;
    switch (event->key()) {
        case Qt::Key_W:
            //go forward
            //qDebug("forward");
            //m_eye.z += speed;
            roll(0.2);
            break;
        case Qt::Key_S:
            //go backward
            //m_eye.z -= speed;
            roll(-0.2);
            break;
        case Qt::Key_A:
            //go right
            yaw(-0.2);
            break;
        case Qt::Key_D:
            //m_eye.x -= speed;
            yaw(0.2);
            break;
        case Qt::Key_Space:
            //go up
            //m_eye.y += speed;
            pitch(0.2);
            break;
        case Qt::Key_Shift:
            //go down
            //m_eye.y -= speed;
            pitch(-0.2);
            break;
            
        case Qt::Key_Left:
            //m_center.x += speed;
            break;
        case Qt::Key_Right:
            //m_center.x -= speed;
            break;
        case Qt::Key_Up:
            //m_center.y += speed;
            translate(0.02);
            break;
        case Qt::Key_Down:
            //m_center.y -= speed;
            translate(-0.02);
            break;
            
        case Qt::Key_R:
            reset();
            break;
            
        default:
            break;
      //      qDebug(m_eye.x);
    }
}

void FlightNavigation::keyReleaseEvent(QKeyEvent *event){
    
}

void FlightNavigation::reset(){
    m_xView = glm::vec3(0.0f ,0.0f, 0.0f);
    m_yView = glm::vec3(0.0f ,0.0f ,0.0f);
    m_zView = glm::vec3(0.0f ,0.0f ,0.0f);
    m_eye = glm::vec3(0.0f ,0.0f , -2.0f);
    m_center = glm::vec3(0.0f ,0.0f ,0.0f);
    m_up = glm::vec3( 0.f, 1.f, 0.f);
    m_viewMatrix = glm::lookAt(m_eye, m_center, m_up);
    updateAxis();
}

void FlightNavigation::updateAxis(){
    m_yView = m_up;
    m_zView = m_center - m_eye;
    m_xView = glm::cross(m_yView, m_zView);
}

void FlightNavigation::updateView(){
    m_viewMatrix =  glm::lookAt(m_eye, m_center, m_up);
}


const glm::mat4 FlightNavigation::viewMatrix(){
    return m_viewMatrix;
}

const glm::mat4 FlightNavigation::viewMatrixInverted()
{
    return glm::inverse(viewMatrix()); // TODO
}

void FlightNavigation::yaw(float angle){
    glm::vec3 diffLR = glm::rotate( m_zView, -angle, m_yView) - m_zView;
    m_center += diffLR;
    updateView();
    updateAxis();
}

void FlightNavigation::pitch(float angle){
    glm::vec3 diffCenter = glm::rotate( m_zView, -angle, m_xView) - m_zView;
    glm::vec3 diffUp = glm::rotate( m_yView, -angle, m_xView) - m_yView;
    m_center += diffCenter;
    m_up += diffUp;
    updateView();
    updateAxis();
}

void FlightNavigation::roll(float angle){
    glm::vec3 diffRotZ = glm::rotate(m_yView, angle, m_zView) - m_yView;
    m_up += diffRotZ;
    updateView();
    updateAxis();
}

void FlightNavigation::translate(float speed){
    //Todo: Move the view towards or backwards the center
    
    glm::vec3 step = speed * m_zView;
    m_center += step;
    m_eye += step;
    updateView();
    updateAxis();
}


