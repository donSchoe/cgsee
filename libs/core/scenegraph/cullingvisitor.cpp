#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <functional>

#include <core/declspec.h>

#include "cullingvisitor.h"

#include "core/framebufferobject.h"
#include "core/camera.h"
#include "node.h"
#include "core/viewfrustum.h"

#include "SceneVisitorInterface.h"

CullingVisitor::CullingVisitor(Camera *camera, Program *program, glm::mat4 transform) :
    m_camera(camera),
    m_program(program),
    m_transform(transform),
    m_viewFrustum(camera->viewFrustum()),
    m_cull(true)
{
}

CullingVisitor::~CullingVisitor()
{
}

bool CullingVisitor::operator() (Node &node) {
    bool returnValue;
    node.draw(*m_program, m_transform);
    if (m_cull) {
        switch (m_viewFrustum->contains(node.boundingBox(), m_transform)) {
        case ViewFrustum::e_insideFrustum::INSIDE_FRUSTUM:
            m_cull = false;
            returnValue = true;
        case ViewFrustum::e_insideFrustum::OUTSIDE_FRUSTUM:
            //YAY - culled!
            returnValue = false;
        default: //ViewFrustum::e_insideFrustum::INTERSECTS_FRUSTUM:
            m_cull = true;
            returnValue = true;
        }
    }

    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return returnValue;
}
