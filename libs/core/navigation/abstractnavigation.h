#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QBasicTimer>

#include "../declspec.h"

class Camera;
class QWidget;
class Group;

class CGSEE_API AbstractNavigation : QObject {
    
public:

    AbstractNavigation(Camera *camera);
    virtual ~ AbstractNavigation();

    virtual void reset();

    // ADD to implement scene dependent navigation
    // void assignCoordinateProvider(CoordinateProvider *provider);

    // interaction mapping
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent *event);

    virtual const glm::mat4 & viewMatrix();
    void loadView(const glm::mat4 & viewmatrix);

    void setCanvas(QWidget * canvas);
    void setViewPort(const int width, const int height);
    void sceneChanged(Group * scene);


    glm::mat4 defaultView();
    glm::mat4 frontview();
    glm::mat4 rightview();
    glm::mat4 backview();
    glm::mat4 leftview();
    glm::mat4 topview();
    glm::mat4 bottomview();
    glm::mat4 topRightView();


protected:
    void startTimer();
    void stopTimer();
    bool isTimerRunning();
    virtual void onTimerEvent();

    virtual void setFromMatrix(const glm::mat4 & view);

    void updateCamera();
    virtual void onCameraChanged(); // override to get notified for camera changes

protected:
    int m_width;
    int m_height;
    float m_fovy;
    glm::mat4 m_viewmatrix;

    static const float TIMER_MS;

private:
    void timerEvent(QTimerEvent * event);

    void finishTransition();
    void updateTransition();
    Camera * m_camera;
    QWidget * m_canvas;
    QBasicTimer m_timer;
    int m_timer_requests;

    float m_animation_progress;
    bool m_animation_active;
    glm::vec3 m_old_pos;
    glm::vec3 m_new_pos;
    glm::quat m_old_rotation;
    glm::quat m_new_rotation;
    glm::mat4 m_frontView;
};
