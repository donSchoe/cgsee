
#include "node.h"
#include "drawvisitor.h"


DrawVisitor::DrawVisitor(Program * p, glm::mat4 t) 
: m_program(p)
, m_transform(t)
{}

bool DrawVisitor::operator() (Node & node)
{
    node.draw(*m_program, m_transform);
    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return true;
}
