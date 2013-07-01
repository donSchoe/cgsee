
#include <cassert>

#include <QFile>
#include <QFileInfo>

#include "fileassociatedshader.h"

using namespace std;

FileAssociatedShader::t_shadersByFilePath FileAssociatedShader::shadersByFilePath;

FileAssociatedShader::FileAssociatedShader(
    const GLenum type
,   const QString & filePath)

:   Shader(type)
,   m_filePath(filePath)
{
    if(!QFile::exists(m_filePath))
    {
        qCritical("\"%s\" does not exist: shader is without source and associated file.", qPrintable(m_filePath));
        return;
    }

    registerShader(m_filePath, this);

    if(setSourceFromFile(false))
        update();
}

FileAssociatedShader::~FileAssociatedShader()
{
    unregisterShader(m_filePath, this);
}

const bool FileAssociatedShader::setSourceFromFile(const bool update)
{
    // http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html

    QFile file(m_filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical("Read from \"%s\" failed.",  qPrintable(m_filePath));
        return nullptr;
    }

    QByteArray data = file.readAll();
    return setSource(QString::fromLocal8Bit(data), update);
}

void FileAssociatedShader::registerShader(
    const QString & filePath
,   FileAssociatedShader * shader)
{
    t_shadersByFilePath::iterator f(shadersByFilePath.find(filePath));

    if(shadersByFilePath.end() != f)
    {
        t_shaders * shaders(f.value());

        assert(shaders);
        assert(!shaders->empty());

        assert(shaders->end() == shaders->find(shader));

        shaders->insert(shader);
    }
    else
    {
        t_shaders * shaders(new t_shaders);
        shaders->insert(shader);

        shadersByFilePath[filePath] = shaders;
    }
}

void FileAssociatedShader::unregisterShader(
    const QString & filePath
,   FileAssociatedShader * shader)
{
    t_shadersByFilePath::iterator f(shadersByFilePath.find(filePath));

    assert(shadersByFilePath.end() != f);

    t_shaders * shaders(f.value());

    assert(shaders->end() != shaders->find(shader));
    shaders->remove(shader);

    delete f.value();
    shadersByFilePath.remove(filePath);
}

void FileAssociatedShader::reloadAll()
{
    t_shadersByFilePath::const_iterator f(shadersByFilePath.cbegin());
    const t_shadersByFilePath::const_iterator fEnd(shadersByFilePath.cend());

    for(; f != fEnd; ++f)
    {
        t_shaders::const_iterator s(f.value()->cbegin());
        const t_shaders::const_iterator sEnd(f.value()->cend());

        for(; s != sEnd; ++s)
            (*s)->reload();
    }
}

void FileAssociatedShader::reload()
{
    if(!QFile::exists(m_filePath))
    {
        qCritical("\"%s\" does not exist: shader will not reload from associated file.", qPrintable(m_filePath));
        return;
    }

    if(isCompiled())
    {
        // if current version works, use its source code as
        // backup if new changes lead to uncompilable shader.

        const QString backup(source());
        if(setSourceFromFile(false))
        {
            update();
            return;
        }

        setSource(backup, true);
        assert(isCompiled());
    }
    else
        setSourceFromFile();
}
