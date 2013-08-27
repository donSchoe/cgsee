#include "glm/gtc/matrix_transform.hpp"

#include <core/camera.h>
#include <core/program.h>
#include <core/fileassociatedshader.h>
#include <core/scenegraph/group.h>

#include "depth.h"

DepthPass::DepthPass(Camera * camera, FileAssociatedShader * depth_util)
:   DefaultPass(camera)
{
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/shadows/lightsource.frag"));
    m_program->attach(depth_util);
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/shadows/lightsource.vert"));
}

DepthPass::~DepthPass(void)
{
}