
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
        0, 0, 0, 5,  //0
        5, 0, 0, 0, //1
        0,  0, 5,  0, //2
        -5, 0, 0, 0, //3
        0 , 0, -5,  0, //4
        0, 1, 0 , 0
    };

    
    static const GLubyte indices[12] = {
         0,1,2, 0,2,3, 0,3,4, 0,4,1 };
    
    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);
    glError();
    
    // setup array buffer
    if(!m_indicesBO)
    {
        m_indicesBO = new BufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        m_indicesBO->data<GLubyte>(indices, 12, GL_UNSIGNED_BYTE, 3);
    }
    
    if(!m_vertexBO)
    {
        m_vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	    m_vertexBO->data<GLfloat>(vertices, 24, GL_FLOAT, 4);
    }
    
    // bind all buffers to their attributes

    m_vertexBO->bind(program.attributeLocation("a_vertex"));
    

    glBindVertexArray(0);
    glError();
}

void Grid::draw( const Program & program, FrameBufferObject * target) const
{
    program.use();
    
    if(-1 == m_vao)
        initialize(program);
    
    if(target)
        target->bind();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
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