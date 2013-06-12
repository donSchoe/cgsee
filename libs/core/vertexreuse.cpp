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
    static const size_t numHashElements = 8;

    size_t operator()(const VertexData& v) const 
    {
        size_t hashValues[numHashElements] = {
            std::hash<float>()(v.vertex[0]),
            std::hash<float>()(v.vertex[1]),
            std::hash<float>()(v.vertex[2]),
            std::hash<float>()(v.normal[0]),
            std::hash<float>()(v.normal[1]),
            std::hash<float>()(v.normal[2]),
            std::hash<float>()(v.texCoord[0]),
            std::hash<float>()(v.texCoord[1])};
        size_t resultHash = 0;
        for (int i = 0; i < numHashElements; ++i)
            resultHash ^= (hashValues[i] << i);
        return resultHash;
    }
};

}

typedef std::pair<VertexData, int> VertexPair;
typedef std::unordered_map<VertexData, int, VertexHash<VertexData>> VertexMap;


void VertexReuse::applyOn(t_VertexIndexListP vertexIndices, t_VertexListP vertexData)
{
    t_uints indices = vertexIndices->getIndices();
    t_vec3s vertices, normals;
    t_vec2s texcs;
    uint oldDataSize = vertexData->size(), newDataSize=0;
    vertices.resize(vertexData->size());
    normals.resize(vertexData->size());
    texcs.resize(vertexData->size());

    vertexData->foreachVertexAttribute<glm::vec3>(0, vertexData->size(), "position", nullptr,
        [&vertices](int i, const glm::vec3 & pos)
        {
            vertices[i] = pos;
        });
    vertexData->foreachVertexAttribute<glm::vec3>(0, vertexData->size(), "normal", nullptr,
        [&normals](int i, const glm::vec3 & pos)
        {
            normals[i] = pos;
        });
    vertexData->foreachVertexAttribute<glm::vec2>(0, vertexData->size(), "texcoord", nullptr,
        [&texcs](int i, const glm::vec2 & pos)
        {
            texcs[i] = pos;
        });

    VertexReuse::reuseVertices(vertices, normals, texcs, indices);

    // shrink size of vertexData, as it should be smaller after deduplication
    vertexData->resize(0); // resizing directly to vertices size does not work for some reason (r6010) - seems like a qt bug(?)
    vertexData->resize(vertices.size());

    vertexIndices->setMultipleIndices(0, indices.size(), std::bind(&t_uints::at, &indices, std::placeholders::_1));
    vertexData->setVertexAttributes<glm::vec3>(0, vertices.size(), "position",
        [&vertices](int i, glm::vec3 & pos)
        {
            pos = vertices[i];
        });
    vertexData->setVertexAttributes<glm::vec3>(0, normals.size(), "normal",
        [&normals](int i, glm::vec3 & normal)
        {
            normal = normals[i];
        });
    vertexData->setVertexAttributes<glm::vec2>(0, texcs.size(), "texcoord",
        [&texcs](int i, glm::vec2 & texcoord)
        {
            texcoord = texcs[i];
        });

    newDataSize = vertexData->size();
    assert(newDataSize <= oldDataSize);
}

void VertexReuse::reuseVertices(t_vec3s& vertices, t_vec3s& normals, t_vec2s& texcs, t_uints& indices)
{
    const size_t numVertices = vertices.size();

    bool normalsUsed = true;
    bool texcsUsed = true;
    // fill all unused input vectors with 0 values.
    if (normals.size() < numVertices) {
        normals.resize(numVertices);
        normalsUsed = false;
    }
    if (texcs.size() < numVertices) {
        texcs.resize(numVertices);
        texcsUsed = false;
    }

    std::vector<VertexData> vertexData;
    vertexData.reserve(numVertices);
    size_t vertexDataLastIndex = 0;
    std::vector<int> indexMapping;
    indexMapping.reserve(numVertices);
    VertexMap vertexMap;
    for (uint i = 0; i < numVertices; ++i)
    {
        // fill map with vertex data
        VertexData vd(vertices[i], normals[i], texcs[i]);
        
        std::pair<VertexMap::iterator,bool> pair = vertexMap.insert(VertexPair(vd, vertexDataLastIndex));

        indexMapping.push_back(pair.first->second);
        if (pair.second) {
            vertexData.push_back(vd);
            ++vertexDataLastIndex;
        }
    }
    const size_t vertexDataSize = vertexData.size();

    // build output arrays with optimized data
    // clean up arrays that are not used
    vertices.resize(vertexDataSize);
    for (uint i = 0; i < vertexDataSize; ++i) {
        vertices[i] = vertexData[i].vertex;
    }

    if (normalsUsed) {
        normals.resize(vertexDataSize);
        for (uint i = 0; i < vertexDataSize; ++i) {
            normals[i] = vertexData[i].normal;
        }
    }
    else
        normals.clear();

    if (texcsUsed) {
        texcs.resize(vertexDataSize);
        for (uint i = 0; i < vertexDataSize; ++i) {
            texcs[i] = vertexData[i].texCoord;
        }
    }
    else
        texcs.clear();
    
    // return indices generated by the map
    const size_t numIndices = indices.size();
    for (uint i = 0; i < numIndices; ++i)
    {
        indices[i] = indexMapping[indices[i]];
    }
}
