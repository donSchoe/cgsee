#pragma once

#include <GL/glew.h>

#include <QString>

#include "declspec.h"


class CGSEE_API GPUQuery
{
public:
    static const GLint   queryi(const GLenum penum);
    static const QString querys(const GLenum penum);
    static const QString glew  (const GLenum penum);

    static const QString vendor();
    static const QString renderer();
    static const QString version();
    static const QString glewVersion();

    static const bool isCoreProfile();

    // Checks for an opengl error and prints to console if one occurred
    static const bool error(
        const char * file
    ,   const int line);

    static const QString glewError(const GLenum penum);

    // Extension Info

    static const bool extensionSupported(const char * extension);
    static const bool extensionSupported(const QString & extension);

    // Memory Info (return -1 if query is not possible or failed)

    static const GLint totalMemory();
    static const GLint dedicatedMemory();
    static const GLint availableMemory();
    static const GLint evictionCount();
    static const GLint evictedMemory();

protected:
    static const GLint memoryInfo(const GLenum penum);
};


#define glError() GPUQuery::error(__FILE__, __LINE__)
#define glIsError() GPUQuery::error(__FILE__, __LINE__)
