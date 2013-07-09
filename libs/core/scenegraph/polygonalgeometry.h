#pragma once

#include <GL/glew.h>
#include <QString>

#include <memory>

#include <core/declspec.h>
#include <core/common.h>
#include <core/datacore/vertexindexlist.h>  //includes vertexlist.h

class DataBlockRegistry;
class AxisAlignedBoundingBox;
class BufferObject;
class Program;
class GeometryOptimizer;

class CGSEE_API PolygonalGeometry final
{
public:
    typedef QVector<BufferObject *> t_bufferObjects;
    typedef QMap<QString, BufferObject *> t_bufferObjectsByAttribute;
    
    PolygonalGeometry( std::shared_ptr<DataBlockRegistry> registry );
    ~PolygonalGeometry();

    t_VertexListP vertices() const;
    t_vec3s copyVertices() const; // TODO: Temporary solution.
    
    //t_vec3s & vertices();  
    void setVertex(int i, glm::vec3 const & data);
    t_vec3s normals() const;
    //t_vec3s & normals();
    void setNormal(int i, glm::vec3 const & data);
    t_vec2s texcs() const;
    //t_vec2s & texcs();
    void setTexC(int i, glm::vec2 const & data);
    t_uints indices() const;
    //t_uints & indices();
    void setIndex(int i, unsigned int data);

    void resize(unsigned int size);

    void initialize(const Program & program);
    
    inline GLuint vao() { return m_vao; }
    t_bufferObjects & elementArrayBOs() { return m_elementArrayBOs; } // TODO: perhaps as const.
    
    // Geometry Computation

    void applyOptimizer(GeometryOptimizer *opt);
    
    // TODO:
    // * generate triangle strip (update: can probably not be combined with Forsyths vertexCacheOpt algorythm
    // * per face, per vertex normals
    
    void retrieveNormals();
    
protected:
    void deleteBuffers();
    
protected:
    std::shared_ptr<DataBlockRegistry> m_registry;
    t_VertexListP m_datablock;
    GLuint m_vao;
    QString m_vertListHandle;
    QString m_indicesHandle;

    t_bufferObjects m_elementArrayBOs;
    t_bufferObjectsByAttribute m_arrayBOsByAttribute;
};
