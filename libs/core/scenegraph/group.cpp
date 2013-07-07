#include <core/program.h>
#include "../material/material.h"

#include "../abstractscenepainter.h"
#include "polygonaldrawable.h"
#include "group.h"

Group::Group(const QString & name)
:   Node(name)
{
}

Group::~Group()
{
    while(!m_children.empty())
        removeLast();

    for(auto material : m_managedMaterials) {
        delete material;
    }
}

void Group::draw(const Program & program, const glm::mat4 & transform)
{

}

void Group::drawDispatch(AbstractScenePainter &painter, const glm::mat4 & transform)
{

}

const bool Group::contains(Node * node) const
{
    return m_children.contains(node);
}

void Group::insert(const Group::t_children::iterator & before, Group * group)
{
    return insert(before, dynamic_cast<Node *>(group));
}

void Group::insert(const Group::t_children::iterator & before, PolygonalDrawable * drawable)
{
    return insert(before, dynamic_cast<Node *>(drawable));
}

void Group::insert(const Group::t_children::iterator & before, Node * node)
{
    if(!node)
        return;

    if(!contains(node))
        node->parents().insert(this);

    m_children.insert(before, node);
}

void Group::addManagedMaterial(Material *material) {
    m_managedMaterials.push_back(material);
}

void Group::prepend(Group * group)
{
    return prepend(dynamic_cast<Node *>(group));
}

void Group::prepend(PolygonalDrawable * drawable)
{
    return prepend(dynamic_cast<Node *>(drawable));
}

void Group::prepend(Node * node)
{
    if(!node)
        return;

    if(!contains(node))
        node->parents().insert(this);

    m_children.push_front(node);
}

void Group::append(Group * group)
{
    return append(dynamic_cast<Node *>(group));
}

void Group::append(PolygonalDrawable * drawable)
{
    return append(dynamic_cast<Node *>(drawable));
}

void Group::append(Node * node)
{
    if(!node)
        return;

    if(!contains(node))
        node->parents().insert(this);

    m_children.push_back(node);
}

void Group::removeFirst()
{
    if(m_children.empty())
        return;

    Node * node(m_children.front());

    node->parents().remove(this);
    m_children.pop_front();

    if(node->parents().empty())
        delete node;
}

void Group::removeLast()
{
    if(m_children.empty())
        return;

    Node * node(m_children.back());

    node->parents().remove(this);
    m_children.pop_back();

    if(node->parents().empty())
        delete node;
}

const void Group::remove(Node * node, const bool deleteIfParentsEmpty)
{
    if(!contains(node))
        node->parents().remove(this);

    m_children.removeAll(node);

    if(deleteIfParentsEmpty && node->parents().empty())
        delete node;
}

const AxisAlignedBoundingBox Group::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    t_children::const_iterator i(m_children.begin());
    const t_children::const_iterator iEnd(m_children.end());

    if(RF_Relative == m_rf)
        for(; i != iEnd; ++i)
            m_aabb.extend((*i)->boundingBox() * transform());
    else
        for(; i != iEnd; ++i)
            m_aabb.extend((*i)->boundingBox());

    return m_aabb;
}

Group * Group::asGroup()
{
    return this;
}
