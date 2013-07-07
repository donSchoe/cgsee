#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <core/declspec.h>

#include "SceneVisitorInterface.h"


class AbstractScenePainter;
class Node;

class CGSEE_API DrawVisitor : public SceneVisitorInterface
{
public:
    DrawVisitor( AbstractScenePainter &painter, glm::mat4 t );
    virtual bool operator() (Node & node) override;

private:
    AbstractScenePainter &m_painter;
    glm::mat4 m_transform;
};
