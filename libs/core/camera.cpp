#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"

static const QString VIEWPORT_UNIFORM   ("viewport");
static const QString VIEW_UNIFORM       ("view");
static const QString PROJECTION_UNIFORM ("projection");

static const QString ZNEAR_UNIFORM      ("znear");
static const QString ZFAR_UNIFORM       ("zfar");


Camera::Camera(const QString & name)
:   Group(name)
,   m_fovy(0.f)
,   m_zNear(0.f)
,   m_zFar (0.f)
,   m_invalidated(true)
{
    // TODO: Sinn und Unsinn der Camera als Node (d.h. als Teil des Graphen)...
    m_rf = RF_Absolute;
//     m_rf = RF_Relative;
}

Camera::~Camera()
{
}

void Camera::draw( const Program & program, const glm::mat4 & transform )
{
    if(m_invalidated)
        update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x, m_viewport.y);
    glError();

    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
}

void Camera::invalidate()
{
    if(m_invalidated)
        return;

    m_invalidated = true;
}

const float Camera::aspect() const
{
    return static_cast<float>(m_viewport.x) / static_cast<float>(m_viewport.y);
}

void Camera::update()
{
    m_projection = glm::perspective(m_fovy, aspect(), m_zNear, m_zFar);
   
    setTransform(m_projection * m_view);

    m_invalidated = false;
}

const glm::ivec2 & Camera::viewport() const
{
    return m_viewport;
}

void Camera::setViewport(const glm::ivec2 & size)
{
    m_viewport = size;
}

void Camera::setViewport(
    const int width
,   const int height)
{
    m_viewport = glm::ivec2(width, height);
    invalidate();
}

const glm::mat4 & Camera::projection()
{
    if(m_invalidated)
        update();

    return m_projection;
}

const glm::mat4 & Camera::view() const
{
    return m_view;
}

void Camera::setView(const glm::mat4 & view)
{
    m_view = view;
    invalidate();
}

const float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if(fovy == m_fovy)
        return;

    m_fovy = fovy;
    invalidate();
}

const float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float z)
{
    if(z == m_zNear)
        return;

    m_zNear = z;
    invalidate();
}

const float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float z)
{
    if(z == m_zNear)
        return;

    m_zFar = z;
    invalidate();
}

Camera * Camera::asCamera()
{
    return this;
}