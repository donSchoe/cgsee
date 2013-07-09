#include "viewfrustum.h"

#include "declspec.h"
#include "aabb.h"
#include "scenegraph/node.h"
#include "scenegraph/group.h"
#include "camera.h"
#include "plane.h"

ViewFrustum::ViewFrustum(Camera *camera) :
    m_camera(camera),
    m_planes(),
    m_dirty(true)
{
}

ViewFrustum::e_insideFrustum ViewFrustum::contains(const AxisAlignedBoundingBox &aabb, glm::mat4 transform) const {
    //TODO: what happens if the bounding sphere completely contains the view frustum?

    // test bounding sphere:
    for (auto plane : m_planes) {
        // distance of center to plane =
        //   < point - plane_origin_dist * normal ; normal > = 
        //   < point ; normal > - plane_origin_dist
        float centerDist = glm::dot(glm::vec4(aabb.center(), 1.0), plane.normal()) - plane.distance();
        if (centerDist <= aabb.radius()) {
            return OUTSIDE_FRUSTUM;
        } else if (centerDist >= -aabb.radius()) {
            return INSIDE_FRUSTUM;
        } else {
            //return INTERSECTS_FRUSTUM;
            // we don't do this because we continue with a more precise test 
            // against the bounding box
            // TODO: is this efficient? Maybe we should do only the test against the bounding box 
            // if the corresponding node does not have subnodes or if the quotient
            // volume / radius is small
        }
    }

    // test bounding box:
    bool contains_at_least_1_point = false;
    bool contains_at_most_7_points = false;
    for (auto aabbVertex : aabb.allVertices()) {
        if (this->contains(transform * glm::vec4(aabbVertex, 1.0))) {
            contains_at_least_1_point = true;
            if (contains_at_most_7_points) {
                return INTERSECTS_FRUSTUM;
            }
        } else {
            contains_at_most_7_points = true;
            if (contains_at_least_1_point) {
                return INTERSECTS_FRUSTUM;
            }
        }
    }
    // contains_at_least_1_point and contains_at_most_7_points can't both be true 
    // in this case the function must have returned INTERSECTS_FRUSTUM yet
    if (contains_at_least_1_point) {
        return INSIDE_FRUSTUM;
    } else {
        return OUTSIDE_FRUSTUM;
    }
}

bool ViewFrustum::contains(glm::vec4 point) const {
    for (auto plane : m_planes) {
        // < point - dist * normal ; normal > = < point ; normal > - dist >
        // <= 0: outside
        if (glm::dot(point, plane.normal()) - plane.distance() <= 0 ) {
            return false;
        }
    }
    return true;
}

void ViewFrustum::update() {
    glm::mat4 projection = m_camera->projection();
    m_planes[0] = Plane(glm::vec4(0.0, 0.0,  1.0, 1.0), 1, projection);
    m_planes[1] = Plane(glm::vec4(0.0, 0.0, -1.0, 1.0), 1, projection);
    m_planes[2] = Plane(glm::vec4(0.0,  1.0, 0.0, 1.0), 1, projection);
    m_planes[3] = Plane(glm::vec4(0.0, -1.0, 0.0, 1.0), 1, projection);
    m_planes[4] = Plane(glm::vec4( 1.0, 0.0, 0.0, 1.0), 1, projection);
    m_planes[5] = Plane(glm::vec4(-1.0, 0.0, 0.0, 1.0), 1, projection);
    m_dirty = false;
}

bool ViewFrustum::isDirty() const {
    return m_dirty;
}

void ViewFrustum::dirty() {
    m_dirty = true;
}

ViewFrustum::~ViewFrustum() {
    
}
