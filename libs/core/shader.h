#pragma once

#include <GL/glew.h>

#include <QString>
#include <QMap>
#include <QSet>

#include "declspec.h"

class Program;

class CGSEE_API Shader
{
public:
    typedef QSet<Program *> t_programs;

public:
    Shader(const GLenum type);
    virtual ~Shader();

    const bool setSource(
        const QString & source
    ,   const bool update = true);
    const QString & source() const;

    void update();

    const GLenum type() const;
    const GLuint shader() const;

    const bool isCompiled() const;

    const t_programs & programs() const;
    t_programs & programs();

protected:
    inline const bool isShader() const;

protected:
    const GLenum m_type;
    GLuint m_shader;

    QString m_source;
    QString m_log;

    bool m_compiled;

    t_programs m_programs;

protected:
    typedef QMap<GLenum, QString> t_typeStrings;
    static const t_typeStrings typeStrings;

    static const t_typeStrings initializeTypeStrings();
};
