
#include "grid.h"

#include "bufferobject.h"
#include "framebufferobject.h"
#include "program.h"
#include "shader.h"
#include "gpuquery.h"


Grid::Grid()
:   m_vao(-1)
,   m_vertexBO(nullptr)
,   m_indicesBO(nullptr)
{
}

Grid::~Grid()
{
    if(-1 != m_vao)
    {
        delete m_vertexBO;
        delete m_indicesBO;
        
        glDeleteVertexArrays(1, &m_vao);
        glError();
    }
}

void Grid::initialize(const Program & program) const
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml
    
    static const GLfloat vertices[24] =
    {
        -1.f,-1.f,-1.f,	// 0
	    -1.f,-1.f, 1.f,	// 1
		-1.f, 1.f,-1.f,	// 2
		-1.f, 1.f, 1.f,	// 3
        1.f,-1.f,-1.f,	// 4
        1.f,-1.f, 1.f, // 5
        1.f, 1.f,-1.f, // 6
        1.f, 1.f, 1.f  // 7
    };
    
    static const GLubyte indices[14] = {
        2, 0, 6, 4, 5, 0, 1, 2, 3, 6, 7, 5, 3, 1 };
    
    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);
    glError();
    
    // setup array buffer
    if(!m_indicesBO)
    {
        m_indicesBO = new BufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        m_indicesBO->data<GLubyte>(indices, 14, GL_UNSIGNED_BYTE, 3);
    }
    
    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	    m_vertexBO->data<GLfloat>(vertices, 8, GL_FLOAT, 3);
    }
    
    // bind all buffers to their attributes
    
    m_vertexBO->bind(program.attributeLocation("a_vertex"));
    
    glBindVertexArray(0);
    glError();
}

void Grid::draw(
                      const Program & program
                      ,   FrameBufferObject * target) const
{
    if(-1 == m_vao)
        initialize(program);
    
    if(target)
        target->bind();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.use();
    
    glBindVertexArray(m_vao);
    glError();
    
    m_indicesBO->bind();
    glError();
    m_vertexBO->bind();
    glError();
    
    m_indicesBO->draw(GL_TRIANGLE_STRIP);
    glError();
    
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    
    m_indicesBO->release();
    m_vertexBO->release();
    
    glBindVertexArray(0);
    glError();
    
    if(target)
        target->release();
}