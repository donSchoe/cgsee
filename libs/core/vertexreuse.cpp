#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <functional>

#include "vertexreuse.h"

namespace {

struct VertexData
{
    VertexData(glm::vec3& vertex, glm::vec3& normal, glm::vec2& texCoord) :
        vertex(vertex), normal(normal), texCoord(texCoord)
    { }
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec2 texCoord;
};


bool operator==(const VertexData& vL, const VertexData& vR)
{
    return (vL.vertex == vR.vertex)
        && (vL.normal == vR.normal)
        && (vL.texCoord == vR.texCoord);
}

template<class T> class VertexHash;

template<>
class VertexHash<VertexData> {
public:
    size_t operator()(const VertexData& v) const 
    {
        size_t hashValues[] = {
            std::hash<float>()(v.vertex[0]),
            std::hash<float>()(v.vertex[1]),
            std::hash<float>()(v.vertex[2]),
            std::hash<float>()(v.normal[0]),
            std::hash<float>()(v.normal[1]),
            std::hash<float>()(v.normal[2]),
            std::hash<float>()(v.texCoord[0]),
            std::hash<float>()(v.texCoord[1])};
        size_t resultHash = 0;
        for (int i = 0; i < sizeof(hashValues)/sizeof(hashValues[0]); ++i)
            resultHash ^= (hashValues[i] << i);
        return resultHash;
    }
};

}

typedef std::pair<VertexData, int> VertexPair;
typedef std::unordered_map<VertexData, int, VertexHash<VertexData>> VertexMap;

void VertexReuse::reuseVertices(t_vec3s& vertices, t_vec3s& normals, t_vec2s& texcs, t_uints& indices)
{
    bool normalsUsed = true;
    bool texcsUsed = true;
    // fill all unused input vectors with 0 values.
    if (normals.size() < vertices.size()) {
        normals.resize(vertices.size());
        normalsUsed = false;
    }
    if (texcs.size() < vertices.size()) {
        texcs.resize(vertices.size());
        texcsUsed = false;
    }

    std::vector<VertexData> vertexData;
    vertexData.reserve(vertices.size());
    std::vector<int> indexMapping;
    indexMapping.reserve(vertices.size());
    VertexMap vertexMap;
    for (int i = 0; i < vertices.size(); ++i)
    {
        // fill map with vertex data
        VertexData vd(vertices[i], normals[i], texcs[i]);
        
        std::pair<VertexMap::iterator,bool> pair = vertexMap.insert(VertexPair(vd, vertexData.size()));

        indexMapping.push_back(pair.first->second);
        if (pair.second)
            vertexData.push_back(vd);
    }

    // build output arrays with optimized data
    // clean up arrays that are not used
    vertices.resize(vertexData.size());
    for (int i = 0; i < vertexData.size(); ++i) {
        vertices[i] = vertexData[i].vertex;
    }

    if (normalsUsed) {
        normals.resize(vertexData.size());
        for (int i = 0; i < vertexData.size(); ++i) {
            normals[i] = vertexData[i].normal;
        }
    }
    else
        normals.clear();

    if (texcsUsed) {
        texcs.resize(vertexData.size());
        for (int i = 0; i < vertexData.size(); ++i) {
            texcs[i] = vertexData[i].texCoord;
        }
    }
    else
        texcs.clear();
    
    // return indices generated by the map
    for (int i = 0; i < indices.size(); ++i)
    {
        indices[i] = indexMapping[indices[i]];
    }
}
