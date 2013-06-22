
#pragma once

#include <QGLWidget>

#include <core/abstractglparent.h>
#include <core/declspec.h>
#include <core/glformat.h>

class QBasicTimer;
class QTimerEvent;
class Timer;

class AbstractScenePainter;
class AbstractNavigation;
class CGSEE_API Canvas : public QGLWidget, public AbstractGLParent
{
public:
    Canvas(
        const GLFormat & format,
        QWidget * parent = nullptr);
    virtual ~Canvas();

    void setPainter(AbstractScenePainter * painter);
    AbstractScenePainter * painter();
    void setNavigation( AbstractNavigation * navigation );
    AbstractNavigation * navigation();

    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    
    virtual void wheelEvent(QWheelEvent *event);

    const QImage capture(
        const bool alpha = false);
    const QImage capture(
        const QSize & size
    ,   const bool aspect = true
    ,   const bool alpha = false);

    virtual void resize(int width, int height);

protected:

    // QGLWidget Interface
    virtual void initializeGL();
    virtual void resizeGL(
        int width
    ,   int height);

    // http://doc.qt.digia.com/qt/opengl-overpainting.html
    // "Instead of implementing paintGL() to handle updates to the
    //  widget, we implement a normal QWidget::paintEvent(). This
    //  allows us to mix OpenGL calls and QPainter operations in a
    //  controlled way."
    
    //virtual void paintEvent(QPaintEvent * event); // Does not work for 3.2 anyway. :(
    //void paintOverlay(QPainter & painter);

    virtual void paintGL();

    // For Rendering Loop
    void timerEvent(QTimerEvent *event);

protected:
    AbstractScenePainter * m_painter;
    AbstractNavigation * m_navigation;

    QBasicTimer * m_timer;
    float m_lastEvent;
    const GLFormat m_format;
};
