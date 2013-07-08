
#include "node.h"
#include "drawvisitor.h"


DrawVisitor::DrawVisitor( AbstractScenePainter &painter, glm::mat4 t )
: m_painter(painter)
, m_transform(t)
{
}

bool DrawVisitor::operator() (Node & node)
{
 //   node.drawDispatch( m_painter, m_transform );
    if( Node::RF_Relative == node.referenceFrame() )
        m_transform *= node.transform();
    return true;
}
