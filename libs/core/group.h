#pragma once

#include "declspec.h"

#include "node.h"

class PolygonalDrawable;
class Program;

class CGSEE_API Group : public Node
{
public:
    Group(const QString & name = "unnamed");
    virtual ~Group();

    virtual Group * asGroup();

    virtual void draw(
        Program * program
    ,   const glm::mat4 & transform);


    // child handling

    const bool contains(Node * node) const;

    void prepend(Group * group);
    void prepend(PolygonalDrawable * drawable);

    void append(Group * group);
    void append(PolygonalDrawable * drawable);

    void insert(
        const t_nodes::iterator & before
    ,   Group * group);
    void insert(
        const t_nodes::iterator & before
    ,   PolygonalDrawable * drawable);


    // both removals delete the node if parents are empty
    void removeFirst();
    void removeLast ();

    const void remove(
        Node * node
    ,   const bool deleteIfParentsEmpty = true);

    const t_nodes & children() const;

    virtual const AxisAlignedBoundingBox boundingBox() const;

protected:
    void prepend(Node * node);
    void append(Node * node);

    void insert(
        const t_nodes::iterator & before
    ,   Node * node);

protected:
    t_nodes m_children;
};
