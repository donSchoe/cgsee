#pragma once

#include "flightnavigation.h"

class CGSEE_API FpsNavigation : public FlightNavigation
{
public:
    FpsNavigation(Camera * camera);
    ~FpsNavigation();
    

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    
protected:
    virtual void onTimerEvent() override;


private:
    void pitchYaw(float pitchAngle, float yawAngle);
    virtual void sideward(float speed) override;
    virtual void forward(float speed) override;
    
    virtual void pitch(float angle) override;
    
    bool m_inGameMode;
    
    glm::vec2 m_lastMousePosition;
    glm::vec2 m_direction;

    float m_pitchAngle;
};