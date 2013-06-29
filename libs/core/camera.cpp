#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "camera.h"

#include "program.h"
#include "gpuquery.h"
#include "framebufferobject.h"

const QString Camera::VIEWPORT_UNIFORM   ("viewport");
const QString Camera::VIEW_UNIFORM       ("view");
const QString Camera::PROJECTION_UNIFORM ("projection");

const QString Camera::CAMERAPOSITION_UNIFORM ("cameraposition");
const QString Camera::ZNEAR_UNIFORM      ("znear");
const QString Camera::ZFAR_UNIFORM       ("zfar");


Camera::Camera(const QString & name)
:   Group(name)
,   m_fovy(0.f)
,   m_zNear(0.f)
,   m_zFar (0.f)
,   m_invalidated(true)
{
    m_rf = RF_Absolute;
}

Camera::~Camera()
{
}

void Camera::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    return draw(program);
}

void Camera::draw(
    const Program & program
,   FrameBufferObject * target)
{
    if(m_invalidated)
        update();

    if(target)
        target->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_viewport.x, m_viewport.y);
    glError();

    program.setUniform(VIEWPORT_UNIFORM, m_viewport);
    program.setUniform(VIEW_UNIFORM, m_view);
    program.setUniform(PROJECTION_UNIFORM, m_projection);
    program.setUniform(CAMERAPOSITION_UNIFORM, getEye());
        
    program.setUniform(ZNEAR_UNIFORM, m_zNear);
    program.setUniform(ZFAR_UNIFORM, m_zFar);
    
    Group::draw(program, glm::mat4());

    if(target)
        target->release();
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

glm::vec3 Camera::getEye(){
    //Get Camera position (from: http://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix )

    glm::mat4 modelViewT = glm::transpose(m_view);
    
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
    
    glm::vec3 eye = (n2n3 * d1) + glm::cross(n1, (d3*n2) - (d2*n3));
    eye /= -denom;
    
    return eye;
}

glm::vec3 Camera::getCenter(){
    glm::vec3 lookat = glm::row(m_view, 2).xyz;
    glm::vec3 eye = getEye();
    
    return eye - lookat;
    
}

glm::vec3 Camera::getUp(){
    return glm::row(m_view, 1).xyz;
}