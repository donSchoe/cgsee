
#include "node.h"
#include "drawvisitor.h"


DrawVisitor::DrawVisitor(Program * p, RenderingPass *renderingpass, glm::mat4 t)
: m_program(p)
, m_transform(t)
, m_renderingpass(renderingpass)
{}

bool DrawVisitor::operator() (Node & node)
{
    node.draw(*m_program, m_renderingpass, m_transform);
    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return true;
}
