#include <GL/glew.h>

#include "polygonaldrawable.h"
#include "../abstractscenepainter.h"

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "polygonalgeometry.h"

// TODO: wieder rueckgaengig machen...
// PolygonalDrawable::PolygonalDrawable( DataBlockRegistry & registry, const QString & name )
PolygonalDrawable::PolygonalDrawable(const QString & name)
:   Node( name )
,   m_geometry( nullptr )
,   m_mode( GL_TRIANGLES )
{
}

PolygonalDrawable::~PolygonalDrawable()
{
}

void PolygonalDrawable::setGeometry(t_geometryP geometry)
{
    m_geometry = geometry;
}

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox() const
{
    if( m_aabb.valid() )
        return m_aabb;

    if( m_geometry == nullptr )
        return AxisAlignedBoundingBox();

    // TODO: Hier muss was effizienteres her! vertices() legt jedes mal eine Kopie an.
//     for( const auto & pos : m_geometry->vertices() ){
//         m_aabb.extend( pos );
//     }

    t_VertexListP myVList = m_geometry->vertices();
    AxisAlignedBoundingBox & aabb = m_aabb;
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&aabb](int i, const glm::vec3 & pos)
        {
            aabb.extend( pos );
        }
    );

    return m_aabb;
}

void PolygonalDrawable::setMaterial(Material *material)
{
    m_material = material;
}

Material *PolygonalDrawable::material() const
{
    return m_material;
}


void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::draw(const Program & program, const glm::mat4 & transform)
{
    if( !m_geometry || m_geometry->indices().empty() )
        return;

    m_geometry->initialize(program);

    glBindVertexArray(m_geometry->vao());
    glError();

    glEnable(GL_DEPTH_TEST);
//     glEnable(GL_CULL_FACE);
//     glCullFace(GL_BACK);

    for( const auto & bo : m_geometry->elementArrayBOs() )
        bo->draw( mode() );

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);
    glError();
}

void PolygonalDrawable::drawDispatch(AbstractScenePainter &painter, const glm::mat4 & transform)
{
    painter.draw(*this, transform);
}


