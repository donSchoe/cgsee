#pragma once

#include <functional>

#include <core/declspec.h>


class Node;

struct CGSEE_API SceneTraverser final
{
public:
    void traverse(Node & node, std::function<bool (Node &)> visitor);
};


struct CGSEE_API ConstSceneTraverser final
{
public:
    void traverse(const Node & node, std::function<bool (const Node &)> visitor);
};
