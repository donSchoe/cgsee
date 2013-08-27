#pragma once

#include <QStack>
#include <GL/gl.h>

class TextureUnitProvider
{
public:
    static TextureUnitProvider *instance();
   // void reset();
    void push();
    void pop();
    void allocate(GLuint &unit, GLuint &id);
    bool allFree();


protected:
    static TextureUnitProvider *s_instance;
    GLuint m_cursor;
    QStack<GLuint> m_stack;

private:
    TextureUnitProvider();

    // Deleted
    TextureUnitProvider(TextureUnitProvider &);
    TextureUnitProvider &operator=(TextureUnitProvider &);
};
