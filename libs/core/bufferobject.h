#pragma once

#include <QVector>

#include "declspec.h"
#include "common.h"
#include "gpuquery.h"


class CGSEE_API BufferObject
{
public:
    BufferObject(
        const GLenum target
    ,   const GLenum usage);

    virtual ~BufferObject();

    const GLuint buffer();

    const GLenum target() const;
    const GLenum usage() const;

    template <typename T>
    void data(
        const QVector<T> & v
    ,   const GLenum type
    ,   const GLint  size);

    template <typename T>
    void data(
        const T * v
    ,   const GLsizei count
    ,   const GLenum type
    ,   const GLint size);

    template <typename T>
    void noData(
        const T def
    ,   const GLsizei count
    ,   const GLenum type
    ,   const GLint size);

    void bind();
    void bind(const GLint attributeLocation);

    void release();
    void release(const GLint attributeLocation);

    void draw(const GLenum mode);

protected:
    inline const bool isBuffer() const;
    void generateBuffer();

protected:
    GLuint m_buffer;

    const GLenum  m_target;
    const GLenum  m_usage;

    GLsizei m_count;
    GLint   m_size;
    GLenum  m_type;
};


template <typename T>
void BufferObject::data(
    const QVector<T> & v
,   const GLenum type
,   const GLint  size)
{
    bind();

    m_count  = static_cast<GLsizei>(v.size());
    m_type   = type;
    m_size   = size;

    glBufferData(m_target, m_count * sizeof(T), v.data(), m_usage);
    glError();
}

template <typename T>
void BufferObject::data(
    const T * v
,   const GLsizei count
,   const GLenum type
,   const GLint size)
{
    bind();

    m_count = count;
    m_type  = type;
    m_size  = size;

    glBufferData(m_target, m_count * sizeof(T), v, m_usage);
    glError();
}

template <typename T>
void BufferObject::noData(
    const T def
,   const GLsizei count
,   const GLenum type
,   const GLint size)
{
    bind();

    T* data = new T[count];
    for(int t = 0; t < count; t++) {
        data[t] = def;
    }

    glBufferData(m_target, m_count * sizeof(T), data, m_usage);
    glError();

    delete data;
}
