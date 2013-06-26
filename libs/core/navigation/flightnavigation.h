#pragma once

#include "abstractnavigation.h"

class CGSEE_API FlightNavigation : public AbstractNavigation
{
public:
    FlightNavigation(Camera * camera);
    ~FlightNavigation();
    
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    
    virtual void reset() override;
    
    
protected:
    virtual void onTimerEvent() override;

    //Defines the view
    glm::vec3 m_eye;
    glm::vec3 m_center; 
    glm::vec3 m_up; 
    
    void updateView();
    
    //Coordinate system of the view
    glm::vec3 m_xView;
    glm::vec3 m_yView;
    glm::vec3 m_zView;
    
    virtual void onCameraChanged() override;
    
    //Yaw, pitch, roll: http://upload.wikimedia.org/wikipedia/commons/7/7e/Rollpitchyawplain.png
    //Rotate center around y-axis
    virtual void yaw(float angle);
    
    //Rotate up - vector and center around x-axis
   virtual void pitch(float angle);
    
    //Rotate up-vector around z axis
    void roll(float angle);
    
    //Move forward or backward (negative speed) to the center of the view
    virtual void forward(float speed);
    virtual void sideward(float speed);
    
    virtual void setFromMatrix(const glm::mat4 & view) override;
    
    glm::vec2 m_direction;
    glm::vec3 m_yprAngle;
};

