#include <GL/glew.h>

#include "bufferobject.h"
#include "gpuquery.h"


BufferObject::BufferObject(
    const GLenum target
,   const GLenum usage)

:   m_buffer(-1)
,   m_target(target)
,   m_usage(usage)

,   m_count(0)
,   m_size(0)

,   m_type(GL_NONE)
{
}

BufferObject::~BufferObject()
{
    if(isBuffer())
    {
        glDeleteBuffers(1, &m_buffer);
        glError();
    }
}

inline const bool BufferObject::isBuffer() const
{
    return m_buffer != -1;
}

const GLuint BufferObject::buffer()
{
    if(!isBuffer())
        generateBuffer();

    return m_buffer;
}

const GLenum BufferObject::target() const
{
    return m_target;
}

const GLenum BufferObject::usage() const
{
    return m_usage;
}

void BufferObject::bind()
{
    if(!isBuffer())
        generateBuffer();

    glBindBuffer(m_target, m_buffer);
    glError();
}

void BufferObject::bind(const GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    assert(m_target == GL_ARRAY_BUFFER);

    bind();

    glEnableVertexAttribArray(attributeLocation);
    glError();
    glVertexAttribPointer(attributeLocation, m_size, m_type, GL_FALSE, 0, 0);
    glError();
}

void BufferObject::draw(const GLenum mode)
{
    switch(m_target)
    {
    case GL_ELEMENT_ARRAY_BUFFER:
        glDrawElements(mode, m_count, m_type, 0); glError();
        break;
    case GL_ARRAY_BUFFER:
        glDrawArrays(mode, 0, m_count); glError();
        break;
    };

}

void BufferObject::release(const GLint attributeLocation)
{
    if(-1 == attributeLocation)
        return;

    if(!isBuffer())
        return;

    glDisableVertexAttribArray(attributeLocation);
    glError();

    glBindBuffer(m_target, 0);
    glError();
}

void BufferObject::release()
{
    if(!isBuffer())
        return;

    glBindBuffer(m_target, 0);
    glError();
}

void BufferObject::generateBuffer()
{
    if(isBuffer())
        return;

    glGenBuffers(1, &m_buffer);
    glError();
}
