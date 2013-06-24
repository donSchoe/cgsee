#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <QMap>
#include <QVector>

#include "declspec.h"

#include "node.h"


class PolygonalGeometry;
class BufferObject;
class Program;


class CGSEE_API PolygonalDrawable : public Node
{
public:
    PolygonalDrawable(const QString & name = "unnamed");
    virtual ~PolygonalDrawable();

    void setGeometry(PolygonalGeometry * geometry);
    PolygonalGeometry * geometry();

    virtual void draw(
        const Program & program
    ,   const glm::mat4 & transform);

    virtual const AxisAlignedBoundingBox boundingBox() const;

protected:

    const glm::mat4 & transform() const;

    void initialize(const Program & program);
    void deleteBuffers();

    virtual void invalidateBoundingBox();

protected:
    GLuint m_vao;

    PolygonalGeometry * m_geometry;

    typedef QVector<BufferObject *> t_bufferObjects;
    t_bufferObjects m_elementArrayBOs;

    typedef QMap<QString, BufferObject *> t_bufferObjectsByAttribute;
    t_bufferObjectsByAttribute m_arrayBOsByAttribute;
};
