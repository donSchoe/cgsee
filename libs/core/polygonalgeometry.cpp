
#include "polygonalgeometry.h"

#include "aabb.h"
#include "datacore/vertexindexlist.h"  //includes vertexlist.h

PolygonalGeometry::PolygonalGeometry(const QString & name)
:   Node(name)
,   m_registry("MyRegistry")
,   m_mode(GL_TRIANGLES)
,   m_material(nullptr)
{
    m_vertListName = "VERTICES";
    m_indicesName = "INDICES";
    t_VertexListP temp = DataBlock::createDataBlockWithName<VertexList>(m_vertListName, m_registry);
    DataBlock::createDataBlockWithName<VertexIndexList>(m_indicesName, m_registry, temp);

    QList<AttributeSpec> attrSpec;
    attrSpec.append(AttributeSpec("position", "glm::vec3"));
    attrSpec.append(AttributeSpec("normal", "glm::vec3"));
    attrSpec.append(AttributeSpec("texcoord", "glm::vec2"));
    temp->initialize(attrSpec);
}

PolygonalGeometry::~PolygonalGeometry()
{
    if(m_material != nullptr)
        delete m_material;
}

const GLenum PolygonalGeometry::mode() const
{
    return m_mode;
}

void PolygonalGeometry::setMode(const GLenum mode)
{
    m_mode = mode;
}

t_vec3s PolygonalGeometry::vertices() const
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}

//t_vec3s & PolygonalGeometry::vertices()
//{
//    invalidateBoundingBox();
//    return m_vertices;
//}

void PolygonalGeometry::setVertex(int i, const glm::vec3& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "position")) = data;
}

t_vec3s PolygonalGeometry::normals() const
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "normal", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}

//t_vec3s & PolygonalGeometry::normals()
//{
//    return m_normals;
//}

void PolygonalGeometry::setNormal(int i, const glm::vec3& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "normal")) = data;
}

t_vec2s PolygonalGeometry::texcs() const
{
    t_vec2s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec2>(0, myVList->size(), "texcoord", nullptr,
        [&temp](int i, const glm::vec2 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}
//t_vec2s & PolygonalGeometry::texcs()
//{
//    return m_texcs;
//}
void PolygonalGeometry::setTexC(int i, const glm::vec2& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec2>(i, "texcoord")) = data;
}

t_uints PolygonalGeometry::indices() const
{
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesName));
    assert(inds);
    return inds->getIndices();
}

//t_uints & PolygonalGeometry::indices()
//{
//    return m_indices;
//}

void PolygonalGeometry::setIndex(int i, unsigned int data)
{
    VertexIndexList * inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesName));
    assert(inds);
    inds->setSingleIndex(i, data);
}

void PolygonalGeometry::setMaterial(Material *material)
{
    m_material = material;
}

Material *PolygonalGeometry::material() {
    return m_material;
}

const AxisAlignedBoundingBox PolygonalGeometry::boundingBox() const
{
    if(m_aabb.valid())
        return m_aabb;

    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    assert(myVList);
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&](int i, const glm::vec3 & pos)
        {
            m_aabb.extend(pos);
        });

    return m_aabb;
}

void PolygonalGeometry::retrieveNormals()
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    t_VertexIndexListP inds = qobject_cast<t_VertexIndexListP>(m_registry.getDataBlockByName(m_indicesName));
    assert(myVList);
    assert(inds);
    if(! myVList->isAttributeUsed("position"))
        return;

    if(myVList->isAttributeUsed("normal"))
        qDebug("Normals of %s will be replaced.", qPrintable(name()));

    inds->foreachTriangle<glm::vec3>(0, inds->size(), "position",
        [&](int i, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
        {
            glm::vec3 a = glm::normalize(v3-v1);
            glm::vec3 b = glm::normalize(v2-v1);
            glm::vec3 n = glm::cross(a, b);
            inds->setVertexAttributes<glm::vec3>(i, i+3, "normal",
                [&](int i, glm::vec3& oldn)
                {
                    oldn = glm::normalize(n);
                });
        });

    //assert(inds->size() % 3 == 0);

    //unsigned int i0, i1, i2;
    //glm::vec3 v[3], a, b, n;

    //for(int i = 0; i < inds->size(); i += 3)
    //{
    //    i0 = m_indices[i + 0];
    //    i1 = m_indices[i + 1];
    //    i2 = m_indices[i + 2];

    //    a = glm::normalize(m_vertices[i2] - m_vertices[i0]);
    //    b = glm::normalize(m_vertices[i1] - m_vertices[i0]);

    //    n = glm::cross(a, b);

    //    m_normals[i0] = n;
    //    m_normals[i1] = n;
    //    m_normals[i2] = n;
    //}
}

void PolygonalGeometry::draw(
    const Program & program
,   const glm::mat4 & transform)
{
}

void PolygonalGeometry::resize(unsigned int size)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesName));
    assert(myVList);
    assert(inds);

    myVList->resize(size);
    inds->resize(size);
}

void PolygonalGeometry::applyOptimizer(GeometryOptimizer *opt) {
        t_VertexIndexListP indices = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesName));
        t_VertexListP vertexData   = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListName));
        assert(indices);
        assert(vertexData);
        opt->applyOn(indices, vertexData);
}
