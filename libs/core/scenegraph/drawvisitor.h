#pragma once

#ifndef DrawVisitor_H
#define DrawVisitor_H

#include <glm/gtc/matrix_transform.hpp>

#include <core/declspec.h>

#include "ISceneVisitor.h"


class Program;
class Node;

class CGSEE_API DrawVisitor : public ISceneVisitor
{
public:
    DrawVisitor( Program * p, glm::mat4 t );
    virtual bool operator() ( Node & node ) override;

private:
    Program * m_program;
    glm::mat4 m_transform;
};


#endif // DrawVisitor_H
