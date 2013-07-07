#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include <QSet>
#include <QString>

#include "declspec.h"


class Shader;

class CGSEE_API Program
{
public:
    typedef QSet<Shader *> t_shaders;

public:
    Program();
    virtual ~Program();

    const bool use() const;
    const bool release() const;

    const GLuint program() const;

    const bool attach(Shader * shader);
    const bool detach(Shader * shader);

    const t_shaders & shaders();

    const bool link() const;
    const bool isLinked() const;
    const bool isUsed() const;

    void invalidate();

    // location access

    // TODO: location cache

    const GLint attributeLocation(const QString & name) const;
    const GLint uniformLocation(const QString & name) const;

    // value access

    // TODO: use templates?

    void setUniform(
        const QString & name
    ,   const int value) const;

    void setUniform(
        const QString & name
    ,   const unsigned int value) const;

    void setUniform(
        const QString & name
    ,   const float value) const;

    void setUniform(
        const QString & name
    ,   const glm::vec2 & vec) const;

    void setUniform(
        const QString & name
    ,   const glm::vec2 * vec_array
    ,   const unsigned int & count) const;

    void setUniform(
        const QString & name
    ,   const glm::ivec2 & vec) const;

    void setUniform(
        const QString & name
    ,   const glm::vec3 & vec) const;
    
    void setUniform(
        const QString & name
    ,   const glm::vec3 * vec_array
    ,   const unsigned int & count) const;

    void setUniform(
        const QString & name
    ,   const glm::ivec3 & vec) const;

    void setUniform(
        const QString & name
    ,   const glm::vec4 & vec) const;

    void setUniform(
        const QString & name
    ,   const glm::ivec4 & vec) const;

    void setUniform(
        const QString & name
    ,   const glm::mat3 & mat
    ,   const bool transpose = false) const;

    void setUniform(
        const QString & name
    ,   const glm::mat4 & mat
    ,   const bool transpose = false) const;

protected:
    inline const bool isProgram() const;

protected:
    GLuint m_program;

    mutable QString m_log;

    mutable bool m_linked;
    mutable bool m_dirty;

    t_shaders m_shaders;
};
