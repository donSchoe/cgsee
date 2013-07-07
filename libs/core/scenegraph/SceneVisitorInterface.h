#pragma once

#include <core/declspec.h>

class Node;

class CGSEE_API SceneVisitorInterface
{
public:
    virtual ~SceneVisitorInterface() {}
    virtual bool operator() (Node & node) =0;
};
