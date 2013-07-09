#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "core/framebufferobject.h"
#include "core/camera.h"
#include "node.h"
#include "core/viewfrustum.h"

#include "SceneVisitorInterface.h"


class CGSEE_API CullingVisitor final : public SceneVisitorInterface
{
public:
    typedef std::vector<std::shared_ptr<Node> >::iterator t_nodeIter;
    
    CullingVisitor(Camera *camera, Program *program, glm::mat4 transform);
    ~CullingVisitor();
    bool operator() (Node &node);

private:
    Camera *m_camera;
    Program *m_program;
    glm::mat4 m_transform;
    ViewFrustum *m_viewFrustum;
    bool m_cull;
};
