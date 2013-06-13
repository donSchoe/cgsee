#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <QMap>
#include <QVector>

#include <core/declspec.h>

#include "node.h"


class PolygonalGeometry;
class DataBlockRegistry;
class BufferObject;
class Program;


class CGSEE_API PolygonalDrawable : public Node
{
public:
    typedef std::shared_ptr<PolygonalGeometry> t_geometryP;
    
    // TODO: wieder rueckgaengig machen...
//     PolygonalDrawable( DataBlockRegistry & registry, const QString & name );
    PolygonalDrawable( const QString & name );
    virtual ~PolygonalDrawable();

    virtual t_nodeIterator begin() override;
    virtual t_nodeIterator end() override;
    
    void setGeometry( t_geometryP geometry );
    t_geometryP geometry() { return m_geometry; }

    void setMode( const GLenum mode ) { m_mode = mode; }
    inline const GLenum mode() const { return m_mode; }

    virtual const AxisAlignedBoundingBox boundingBox() const override;
    
    virtual void draw( const Program & program, const glm::mat4 & transform ) override;

protected:
    // TODO: Kontrolle ueber OpenGL Buffer kann nach PolygonalGeometry.
    void initialize(const Program & program);
    void deleteBuffers();
    virtual void invalidateBoundingBox() override;

protected:
    typedef QVector<BufferObject *> t_bufferObjects;
    typedef QMap<QString, BufferObject *> t_bufferObjectsByAttribute;

    GLuint m_vao;
    t_geometryP m_geometry;
    GLenum  m_mode;
    t_bufferObjects m_elementArrayBOs;
    t_bufferObjectsByAttribute m_arrayBOsByAttribute;    
};