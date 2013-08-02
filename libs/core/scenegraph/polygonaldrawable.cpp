#include <GL/glew.h>

#include "polygonaldrawable.h"

#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>

#include "polygonalgeometry.h"

static const QString TRANSFORM_UNIFORM( "transform" );
static const QString ID_UNIFORM( "u_id" );

// TODO: wieder rueckgaengig machen...
// PolygonalDrawable::PolygonalDrawable( DataBlockRegistry & registry, const QString & name )
PolygonalDrawable::PolygonalDrawable( const QString & name )
:   Node( name )
,   m_geometry( nullptr )
,   m_mode( GL_TRIANGLES )
{
}

PolygonalDrawable::~PolygonalDrawable()
{
}

void PolygonalDrawable::setGeometry( t_geometryP geometry )
{
    m_geometry = geometry;
}

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    if(m_geometry == nullptr)
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

void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    if(!m_geometry || m_geometry->indices().empty())
        return;

    m_geometry->initialize( program );

    program.use();
    program.setUniform(TRANSFORM_UNIFORM, transform);

    glm::vec4 id_vec = colorVectorFromId(m_id);
    program.setUniform(ID_UNIFORM, id_vec);

    glBindVertexArray( m_geometry->vao() );
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

    program.release();
}

glm::vec4 PolygonalDrawable::colorVectorFromId(const unsigned int & id)
{
    glm::vec4 id_vec;
    id_vec.r = id % 256;
    id_vec.g = (id >> 8) % 256;
    id_vec.b = (id >> 16) % 256;
    id_vec.a = (id >> 24) % 256; 

    return id_vec;
}