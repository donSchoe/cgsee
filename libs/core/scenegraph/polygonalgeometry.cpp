
#include "polygonalgeometry.h"

#include <core/datacore/datablock.h>
#include <core/aabb.h>
#include <core/bufferobject.h>
#include <core/program.h>
#include <core/vertexcacheoptimizer.h>
#include <core/vertexreuse.h>

PolygonalGeometry::PolygonalGeometry(std::shared_ptr<DataBlockRegistry> registry)
:   m_registry( registry )
,   m_datablock( nullptr )
,   m_vao( -1 )
,   m_vertListHandle( "VERTICES" )
,   m_indicesHandle( "INDICES" )
,   m_elementArrayBOs()
,   m_arrayBOsByAttribute()
{
    m_datablock = DataBlock::createDataBlockWithName<VertexList>(m_vertListHandle, *m_registry);
    DataBlock::createDataBlockWithName<VertexIndexList>(m_indicesHandle, *m_registry, m_datablock);

    QList<AttributeSpec> attrSpec;
    attrSpec.append(AttributeSpec("position", "glm::vec3"));
    attrSpec.append(AttributeSpec("normal", "glm::vec3"));
    attrSpec.append(AttributeSpec("texcoord", "glm::vec2"));
    m_datablock->initialize(attrSpec);
}

PolygonalGeometry::~PolygonalGeometry()
{
    DataBlock::destroyDataBlock<VertexList>(m_datablock);
    deleteBuffers();
}

t_VertexListP PolygonalGeometry::vertices() const
{
    return qobject_cast<t_VertexListP>( m_registry->getDataBlockByName(m_vertListHandle) );
}

t_vec3s PolygonalGeometry::copyVertices() const // TODO: Temporary solution.
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        }
    );
    return temp;
}

void PolygonalGeometry::setVertex(int i, const glm::vec3 & data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "position")) = data;
}

t_vec3s PolygonalGeometry::normals() const
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "normal", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        }
    );
    return temp;
}

void PolygonalGeometry::setNormal(int i, const glm::vec3& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "normal")) = data;
}

t_vec2s PolygonalGeometry::texcs() const
{
    t_vec2s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec2>(0, myVList->size(), "texcoord", nullptr,
        [&temp](int i, const glm::vec2 & pos)
        {
            temp[i] = pos;
        }
    );
    return temp;
}

void PolygonalGeometry::setTexC(int i, const glm::vec2& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec2>(i, "texcoord")) = data;
}

t_uints PolygonalGeometry::indices() const
{
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry->getDataBlockByName(m_indicesHandle));
    assert(inds);
    return inds->getIndices();
}

void PolygonalGeometry::setIndex(int i, unsigned int data)
{
    VertexIndexList * inds = qobject_cast<VertexIndexList*>(m_registry->getDataBlockByName(m_indicesHandle));
    assert(inds);
    inds->setSingleIndex(i, data);
}

void PolygonalGeometry::retrieveNormals()
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry->getDataBlockByName(m_indicesHandle));
    assert(myVList);
    assert(inds);
    if(! myVList->isAttributeUsed("position"))
        return;

    if(myVList->isAttributeUsed("normal"))
        qDebug("Normals will be replaced.");

    inds->foreachTriangle<glm::vec3>(0, inds->size(), "position", 
        [&](int i, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
        {
            glm::vec3 a = glm::normalize(v3-v1);
            glm::vec3 b = glm::normalize(v2-v1);
            glm::vec3 n = glm::cross(a, b);
            myVList->setVertexAttributes<glm::vec3>(i, i+3, "normal",
                [&](int i, glm::vec3& oldn)
                {
                    oldn = n;
                }
            );
        }
    );
}

void PolygonalGeometry::resize(unsigned int size)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry->getDataBlockByName(m_indicesHandle));
    assert(myVList);
    assert(inds);

    myVList->resize(size);
    inds->resize(size);
}

void PolygonalGeometry::initialize(const Program & program)
{
    if(!m_arrayBOsByAttribute.empty() && !m_elementArrayBOs.empty())
        return;
    
    deleteBuffers();

    glGenVertexArrays(1, &m_vao);
    glError();
    glBindVertexArray(m_vao);                                                                  
    glError();
    
    // Apply vertex deduplication

    applyOptimizer(new VertexReuse()); // TODO: That's just bad!
    // Apply Vertex Cache Optimization
    applyOptimizer(new VertexCacheOptimizer()); // TODO: That's just bad!

    // setup element array buffers

    BufferObject * indexBO = new BufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    indexBO->data<GLuint>(indices(), GL_UNSIGNED_INT, 1);

    m_elementArrayBOs.push_back(indexBO);

    // setup array buffers

    BufferObject * vertexBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertexBO->data<glm::vec3>(copyVertices(), GL_FLOAT, 3);
    
    m_arrayBOsByAttribute["a_vertex"] = vertexBO;

    // TODO: the geometry should provide this information.

    if( !normals().isEmpty() )
    {
        BufferObject * normalBO = new BufferObject(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        normalBO->data<glm::vec3>(normals(), GL_FLOAT, 3);

        m_arrayBOsByAttribute["a_normal"] = normalBO;
    }

    // bind all buffers to their attributes

    t_bufferObjectsByAttribute::const_iterator i(m_arrayBOsByAttribute.begin());
    const t_bufferObjectsByAttribute::const_iterator iEnd(m_arrayBOsByAttribute.end());

    for( ; i != iEnd; ++i )
        i.value()->bind(program.attributeLocation(i.key()));

    glBindVertexArray(0);
    glError();
}

void PolygonalGeometry::deleteBuffers()
{
    qDeleteAll(m_elementArrayBOs);
    qDeleteAll(m_arrayBOsByAttribute);

    if( -1 != m_vao )
    {
        glDeleteVertexArrays(1, &m_vao);
        glError();

        m_vao = -1;
    }
}

void PolygonalGeometry::applyOptimizer( GeometryOptimizer * opt ) 
{
        t_VertexListP vertexData = qobject_cast<t_VertexListP>(m_registry->getDataBlockByName(m_vertListHandle));
        t_VertexIndexListP indices = qobject_cast<VertexIndexList*>(m_registry->getDataBlockByName(m_indicesHandle));
        assert(indices);
        assert(vertexData);
        opt->applyOn(indices, vertexData);
}
