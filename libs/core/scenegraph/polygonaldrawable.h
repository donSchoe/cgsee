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
class Program;


class CGSEE_API PolygonalDrawable : public Node
{
public:
    typedef std::shared_ptr<PolygonalGeometry> t_geometryP;
    
    // TODO: wieder rueckgaengig machen...
//     PolygonalDrawable( DataBlockRegistry & registry, const QString & name );
    PolygonalDrawable( const QString & name );
    virtual ~PolygonalDrawable();

    virtual void draw( const Program & program, const glm::mat4 & transform ) override;

    virtual const AxisAlignedBoundingBox boundingBox() const override;
    
    void setGeometry( t_geometryP geometry );
    t_geometryP geometry() { return m_geometry; }

    void setMode( const GLenum mode ) { m_mode = mode; }
    inline const GLenum mode() const { return m_mode; }
    
protected:
    virtual void invalidateBoundingBox() override;

    glm::uvec4 colorVectorFromId(const unsigned int & id);

protected:
    t_geometryP m_geometry;
    GLenum  m_mode;
};
