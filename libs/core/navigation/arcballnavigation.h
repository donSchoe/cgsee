#pragma once

#include "abstractnavigation.h"

class CGSEE_API ArcballNavigation : public AbstractNavigation
{
public:
    ArcballNavigation(Camera * camera);
    ~ArcballNavigation();
    
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;

private: 
    glm::vec3 getArcballVector(glm::vec2 v);
    void updateArcball(); 
    void updateZoom();
    void updatePanning();

private:
    glm::vec2 m_mouse_last;
    glm::vec2 m_mouse_cur;
    bool m_arcball_on;
    bool m_zoom_on;
    bool m_panning_on;
};

