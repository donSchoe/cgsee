
#pragma once

#include <GL/glew.h>

#include "declspec.h"


class BufferObject;
class Program;
class FrameBufferObject;
class Camera;

class CGSEE_API Grid
{
public:
    Grid();
    virtual ~Grid();
    
    void draw(
              const Program & program
              ,   FrameBufferObject * target = nullptr
              , Camera * camera = nullptr);
    
protected:
    void initialize(const Program & program) const;
    
protected:
    mutable GLuint m_vao;
    mutable BufferObject * m_vertexBO;
    mutable BufferObject * m_indicesBO;
    mutable BufferObject * m_normalBO;
    
};