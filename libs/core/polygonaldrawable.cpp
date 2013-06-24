#include <GL/glew.h>

#include "polygonaldrawable.h"

#include "aabb.h"
#include "bufferobject.h"
#include "polygonalgeometry.h"
#include "program.h"
#include "vertexcacheoptimizer.h"
#include "vertexreuse.h"


static const QString TRANSFORM_UNIFORM ("transform");


PolygonalDrawable::PolygonalDrawable(const QString & name)
:   Node(name)
,   m_vao(-1)
,   m_geometry(nullptr)
{
}

PolygonalDrawable::~PolygonalDrawable()
{
    setGeometry(nullptr);
    deleteBuffers();
}

const AxisAlignedBoundingBox PolygonalDrawable::boundingBox() const
{
    if(!m_geometry)
        return AxisAlignedBoundingBox();

    return m_geometry->boundingBox();
}

void PolygonalDrawable::setGeometry(PolygonalGeometry * geometry)
{
    if(m_geometry)
    {
        m_geometry->parents().remove(this);
        if(m_geometry->parents().empty())
            delete m_geometry;

        deleteBuffers();
    }

    m_geometry = geometry;

    if(m_geometry)
        m_geometry->parents().insert(this);
}

PolygonalGeometry * PolygonalDrawable::geometry()
{
    return m_geometry;
}

void PolygonalDrawable::invalidateBoundingBox()
{
    return Node::invalidateBoundingBox();
}

void PolygonalDrawable::initialize(const Program & program)
{
    assert(m_geometry);

    deleteBuffers();

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);
    glError();

    // Apply vertex deduplication

    m_geometry->applyOptimizer(new VertexReuse());
    // Apply Vertex Cache Optimization
    m_geometry->applyOptimizer(new VertexCacheOptimizer());

    // setup element array buffers

    BufferObject * indexBO(new BufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW));
    indexBO->data<GLuint>(m_geometry->indices(), GL_UNSIGNED_INT, 1);

    m_elementArrayBOs.push_back(indexBO);

    // setup array buffers

	BufferObject * vertexBO(new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
	vertexBO->data<glm::vec3>(m_geometry->vertices(), GL_FLOAT, 3);

    m_arrayBOsByAttribute["a_vertex"] = vertexBO;

    // TODO: the geometry should provide this information.

    if(!m_geometry->normals().isEmpty())
    {
	    BufferObject * normalBO(new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW));
	    normalBO->data<glm::vec3>(m_geometry->normals(), GL_FLOAT, 3);

        m_arrayBOsByAttribute["a_normal"] = normalBO;
    }

    // bind all buffers to their attributes

    t_bufferObjectsByAttribute::const_iterator i(m_arrayBOsByAttribute.begin());
    const t_bufferObjectsByAttribute::const_iterator iEnd(m_arrayBOsByAttribute.end());

    for(; i != iEnd; ++i)
        i.value()->bind(program.attributeLocation(i.key()));

    glBindVertexArray(0);
    glError();
}

void PolygonalDrawable::draw(
    const Program & program
,   const glm::mat4 & transform)
{
    if(!m_geometry || m_geometry->indices().empty())
        return;

    if(m_arrayBOsByAttribute.empty() || m_elementArrayBOs.empty())
        initialize(program);

    program.use();
    program.setUniform(TRANSFORM_UNIFORM, transform);

    glBindVertexArray(m_vao);
    glError();

    if(m_geometry->material()) {
         m_geometry->material()->bind(program);
    }


    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    for(auto e : m_elementArrayBOs) {
        e->draw(m_geometry->mode());
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);
    glError();

    program.release();
}

void PolygonalDrawable::deleteBuffers()
{
    m_elementArrayBOs.clear();
    m_arrayBOsByAttribute.clear();

    if(-1 != m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
        glError();

        m_vao = -1;
    }
}
