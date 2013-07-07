#include "glm/glm.hpp"

#include "plane.h"

Plane::Plane() :
    m_distance(1.0),
    m_normal(glm::vec4(1.0, 0.0, 0.0, 1.0))
{
}

Plane::Plane(glm::vec4 normal, float distance) :
    m_distance(distance),
    m_normal(glm::normalize(normal))
{
}

Plane::Plane(glm::vec4 normal, glm::vec4 supportVector) :
    m_distance(0.0f),
    m_normal(glm::normalize(normal))
{
    m_distance = glm::dot(supportVector, m_normal);
}

Plane::Plane(glm::vec4 normal, float distance, glm::mat4 transform) :
    m_distance(distance), // value of m_distance is not yet the valid
    m_normal(glm::normalize(transform * normal)) // value of m_normal is valid
{
    glm::vec4 supportVector = m_normal * distance;
    supportVector = transform * supportVector;

    m_distance = glm::dot(supportVector, m_normal);
}

    
Plane::~Plane() {
}

glm::vec4 Plane::normal() {
    return m_normal;
}

float Plane::distance() {
    return m_distance;
}

Plane Plane::transformed(const glm::mat4 transform) {
    glm::vec4 newNormal = m_normal;
    newNormal = newNormal * transform;
    glm::normalize(newNormal);

    glm::vec4 newSupportVector = newNormal * m_distance;
    newSupportVector = newSupportVector * transform;

    float newDistance = glm::dot(newSupportVector, newNormal);
    return Plane(newNormal, newDistance);
}