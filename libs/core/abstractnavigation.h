#pragma once

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "declspec.h"

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QBasicTimer>

class Camera;
class QWidget;

class CGSEE_API AbstractNavigation : QObject {
    
public:

    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    //void assignCoordinateProvider(CoordinateProvider *provider);

    // interaction mapping
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual const glm::mat4 viewMatrix();

    void updateCamera();
    virtual void onCameraChanged();

    void loadView(glm::mat4 viewmatrix);

    void setCanvas(QWidget * canvas);
    void setViewPort(const int width, const int height);

protected:
    void startTimer();
    void stopTimer();
    bool isTimerRunning();

    virtual void onTimerEvent();
    virtual void setFromMatrix(glm::mat4 view);


protected:
    int m_width;
    int m_height;
    glm::mat4 m_viewMatrix;

    static const int TIMER_MS = 1000/60;

private:
    void timerEvent(QTimerEvent* event);

    Camera * m_camera;
    QWidget * m_canvas;
    QBasicTimer m_timer;
    int m_timer_requests;

    float m_animation_progress;
    bool m_animation_active;
    glm::mat4 m_oldMatrix;
    glm::mat4 m_newMatrix;
};