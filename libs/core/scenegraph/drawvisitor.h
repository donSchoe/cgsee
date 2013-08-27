#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <core/declspec.h>
#include <core/rendering/renderingpass.h>

#include "SceneVisitorInterface.h"


class Program;
class Node;

class CGSEE_API DrawVisitor : public SceneVisitorInterface
{
public:
    DrawVisitor(Program * p, RenderingPass *renderingpass, glm::mat4 t);
    virtual bool operator() (Node & node) override;

private:
    Program * m_program;
    RenderingPass *m_renderingpass;
    glm::mat4 m_transform;
};
