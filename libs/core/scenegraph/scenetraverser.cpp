
#include "node.h"
#include "scenetraverser.h"


void SceneTraverser::traverse(Node & node, std::function<bool (Node &)> visitor)
{
    if( !visitor( node ) )
        return;
    
    auto it = node.children().begin();
    auto itEnd = node.children().end();
    for( ; it != itEnd; ++it ){
        traverse( **it, visitor );
    }
}


void ConstSceneTraverser::traverse(const Node & node, std::function<bool (const Node &)> visitor)
{
    if( !visitor( node ) )
        return;
    
    Node::t_children::const_iterator it = node.children().begin();
    Node::t_children::const_iterator itEnd = node.children().end();
    for( ; it != itEnd; ++it ){
        traverse( **it, visitor );
    }
}
