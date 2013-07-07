#pragma once

#include <GL/gl.h>

class TextureUnitProvider
{
public:
    static TextureUnitProvider *instance();
    void reset();
    void allocate(GLuint &unit, GLuint &id);


protected:
    static TextureUnitProvider *s_instance;
    GLuint m_cursor;

private:
    TextureUnitProvider();

    // Deleted
    TextureUnitProvider(TextureUnitProvider &);
    TextureUnitProvider &operator=(TextureUnitProvider &);
};
