#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "declspec.h"
#include "group.h"


class Program;
class ViewFrustum;

class CameraImplementation;

class CGSEE_API Camera : public Group
{
public:
    Camera(const QString & name = "unnamed");
    virtual ~Camera();

    virtual Camera * asCamera();

    virtual void draw( const Program & program, const glm::mat4 & transform) override;

    const glm::ivec2 & viewport() const;
    void setViewport(const glm::ivec2 & size);
    void setViewport(
        const int width
    ,   const int height);

    const glm::mat4 & projection();

    const glm::mat4 & view() const;
    void setView(const glm::mat4 & view);

    const float fovy() const;
    void setFovy(const float fovy);

    const float zNear() const;
    void setZNear(const float z);

    const float zFar() const;
    void setZFar(const float z);

    const float aspect() const;

    ViewFrustum *viewFrustum() const;

    // updates camera matrices
    void update();

    //
    glm::vec3 getEye();
    glm::vec3 getUp();
    glm::vec3 getCenter();

public:
    void selectImplementation(QString name);
    QString selectedImplementation();
    CameraImplementation * activeImplementation() const;

    int preferredRefreshTimeMSec() const;

protected:
    QVector<CameraImplementation*> m_implementations;
    CameraImplementation * m_activeCamera;

protected:
    void invalidate();
    virtual void invalidateChildren() override;

    glm::ivec2 m_viewport;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    ViewFrustum *m_viewFrustum;

    float m_fovy;
    float m_zNear;
    float m_zFar;

    bool m_invalidated;

// friend CameraImplementation;
};
