#include <glm/gtc/matrix_transform.hpp>

#include <QKeyEvent>
#include <QString>
#include <QPainter>
#include <QImage>

#include "abstractpainter.h"
#include "fileassociatedshader.h"

#include "abstractglparent.h"
#include "camera.h"
#include "program.h"
#include "gpuquery.h"



AbstractPainter::AbstractPainter()
:   m_initialized(false)
{
}

AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::paint()
{
    if(m_initialized)
        return;

    if((m_initialized = initialize()))
        qFatal("Painter initialization failed.");
}

void AbstractPainter::resize(
    const int width
,   const int height)
{
    glViewport(0, 0, width, height);
    glError();

    if(m_initialized)
        return;

    if(!(m_initialized = initialize()))
        qFatal("Painter initialization failed.");
}


Program *AbstractPainter::loadProgram(const QString &pathPrefix, int flags)
{
    Program *program = new Program;

    if(flags & VERTEX_SHADER_BIT) {
        program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, pathPrefix + ".vert"));
    }
    if(flags & GEOMETRY_SHADER_BIT) {
        program->attach(new FileAssociatedShader(GL_GEOMETRY_SHADER, pathPrefix + ".geo"));
    }
    if(flags & FRAGMENT_SHADER_BIT) {
        program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, pathPrefix + ".frag"));
    }

    return program;
}



const QImage AbstractPainter::capture(
    AbstractGLParent & parent
,   const QSize & size
,   const bool aspect
,   const bool alpha)
{
    static const GLuint tileW(1024);
    static const GLuint tileH(1024);

    Camera * cam = camera();

    if(!cam)
    {
        qWarning("No camera for frame capture available.");
        return QImage();
    }

    const GLuint w(cam->viewport().x);
    const GLuint h(cam->viewport().y);

    const GLuint frameW = size.width();
    const GLuint frameH = size.height();

    const glm::mat4 proj(aspect ? glm::perspective(cam->fovy()
        , static_cast<float>(frameW) / static_cast<float>(frameH)
        , cam->zNear(), cam->zFar()) : cam->projection());

    const glm::mat4 view(cam->view());

    const glm::vec4 viewport(0, 0, frameW, frameH);

    QImage frame(frameW, frameH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);
    QImage tile(tileW, tileH, alpha ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    QPainter p(&frame);

    parent.resize(tileW, tileH);
    cam->update();

    for (GLuint y = 0; y < frameH; y += tileH)
    for (GLuint x = 0; x < frameW; x += tileW)
    {
        const glm::mat4 pick = glm::pickMatrix(glm::vec2(x + tileW / 2,  y + tileH / 2),
                                               glm::vec2(tileW, tileH), viewport);

        const glm::mat4 projTile(pick * proj);

        cam->setTransform(projTile * view);

        paint();

        glReadPixels(0, 0, tileW, tileH, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tile.bits());
        p.drawImage(x, y, tile);
    }
    p.end();

    parent.resize(w, h);
    cam->setTransform(proj * view);

    return frame.mirrored(false, true); // flip vertically
}

