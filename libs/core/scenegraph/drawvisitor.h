#pragma once

#ifndef DrawVisitor_H
#define DrawVisitor_H

#include <glm/gtc/matrix_transform.hpp>

#include <core/declspec.h>

#include "ISceneVisitor.h"


class AbstractScenePainter;
class Node;

class CGSEE_API DrawVisitor : public ISceneVisitor
{
public:
    DrawVisitor( AbstractScenePainter &painter, glm::mat4 t );
    virtual bool operator() ( Node & node ) override;

private:
    AbstractScenePainter &m_painter;
    glm::mat4 m_transform;
};


#endif // DrawVisitor_H
