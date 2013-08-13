
#include <core/program.h>
#include <core/fileassociatedshader.h>

#include "normalzpass.h"

NormalzPass::NormalzPass(Camera * camera, FileAssociatedShader * depth_util)
:   DefaultPass(camera)
{
    m_program->attach(new FileAssociatedShader(GL_FRAGMENT_SHADER, "data/normalz.frag"));
    m_program->attach(depth_util);
    m_program->attach(new FileAssociatedShader(GL_VERTEX_SHADER, "data/normalz.vert"));
}

NormalzPass::~NormalzPass( void )
{

}

