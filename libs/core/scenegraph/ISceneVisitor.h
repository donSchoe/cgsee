#pragma once

#ifndef ISceneVisitor_H
#define ISceneVisitor_H

#include <core/declspec.h>


class Node;

class CGSEE_API ISceneVisitor
{
public:
    virtual ~ISceneVisitor() {}
    virtual bool operator() ( Node & node ) =0;
};

#endif // ISceneVisitor_H
