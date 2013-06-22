#include <vector>

#include "vertexcacheoptimizer.h"

namespace 
{
    const float CACHE_DECAY_POWER = 1.5f;
    const float LAST_TRI_SCORE = 0.75f;
    const float VALENCE_BOOST_SCALE = 2.0f;
    const float VALENCE_BOOST_POWER = 0.5f;
    const unsigned MAX_SIZE_VERTEX_CACHE = 32;
    const unsigned VALENCE_BOOST_TABLE_SIZE = 32;
    const float SCALER = 1.0f / (MAX_SIZE_VERTEX_CACHE - 3);
    float cacheScores[MAX_SIZE_VERTEX_CACHE];
    float valenceScores[VALENCE_BOOST_TABLE_SIZE];
}


inline void swap(int& a, int& b)
{
    a ^= b ^= a ^= b;
}

void VertexCacheOptimizer::initScoreTables() {
    for (uint i = 0; i < VALENCE_BOOST_TABLE_SIZE; ++i) {
        valenceScores[i] =  powf(i, -VALENCE_BOOST_POWER) * VALENCE_BOOST_SCALE;
    }

    cacheScores[0] = cacheScores[1] = cacheScores[2] = LAST_TRI_SCORE;
    for (int i = 3; i < MAX_SIZE_VERTEX_CACHE; ++i) {
        float score = 1.0f - (i - 3) * SCALER;
        score = powf(score, CACHE_DECAY_POWER);
        cacheScores[i] = score;
    }
}

void VertexCacheOptimizer::initLists(const t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles)
{
    for (uint index : indices) {
        ++vertices[index].numTriangles;
        ++vertices[index].numTrianglesToDo;
    }
    const auto indicesBegin = indices.cbegin();
    const auto indicesEnd   = indices.cend();
    uint i = 0;
    for (auto it = indicesBegin; it != indicesEnd; ++it) {
        uint currentTriangleIndex = i / 3;
        vertices[*it].triangles.push_back( currentTriangleIndex );
        triangles[currentTriangleIndex].vertices.push_back(*it);
        ++i;
    }
}

void VertexCacheOptimizer::initScores(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles)
{
    for (Vertex &vertex : vertices) 
        vertex.score = calculateVertexScore(vertex);
    
    for (Triangle &triangle : triangles) {
        triangle.score = vertices[triangle.vertices[0]].score
                       + vertices[triangle.vertices[1]].score
                       + vertices[triangle.vertices[2]].score;
    }
}

float VertexCacheOptimizer::calculateVertexScore(const Vertex &vertex) {
    if (vertex.numTrianglesToDo == 0)
        return -1.0f;

    float score(0.0f);
    if (vertex.cachePosition >= 0)
        score = cacheScores[vertex.cachePosition];

    if (vertex.numTrianglesToDo < VALENCE_BOOST_TABLE_SIZE)
        score += valenceScores[vertex.numTrianglesToDo];
    else 
        score += powf(vertex.numTrianglesToDo, -VALENCE_BOOST_POWER) * VALENCE_BOOST_SCALE;

    return score;
}

void VertexCacheOptimizer::addGreatestTriangle(t_uints &indices, std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, int &greatestTriangleIndex)
{
    triangles[greatestTriangleIndex].addedToDrawList = true;
    // add greates Triangle to the draw list
    // dec trianglesToDo of its vertices and "sort" its triangle list
    for (ushort i = 0; i < 3; ++i) {
        const int currentVertexIndex = triangles[greatestTriangleIndex].vertices[i];
        Vertex& currentVertex = vertices[currentVertexIndex];
        indices.push_back(currentVertexIndex);

        //find the triangle in the vertices triangle list
        for (uint t = 0; t < currentVertex.numTrianglesToDo; ++t) {
            if (currentVertex.triangles[t] == greatestTriangleIndex) {
                if (t == currentVertex.numTrianglesToDo - 1)
                    break;
                swap(currentVertex.triangles[t], currentVertex.triangles[currentVertex.numTrianglesToDo - 1]);
                break;
            }
        }
        --currentVertex.numTrianglesToDo;
    }
}

void VertexCacheOptimizer::pushVerticesToStack(std::vector<Vertex> &vertices, const std::vector<Triangle> &triangles, const int greatestTriangleIndex, std::vector<int> &cache)
{
    // add/move the three vertices to the top of the LRU-Stack
    for (ushort vertexIndex = 0;  vertexIndex < 3; ++vertexIndex) {
        uint currentVertexIndex = triangles[greatestTriangleIndex].vertices[vertexIndex];
        int cachePosition = vertices[currentVertexIndex].cachePosition;
        //if we add a new element to the cache we need to move all elements in it
        if (cachePosition == -1)
            cachePosition = MAX_SIZE_VERTEX_CACHE + 3 - 1;
        //move all elements from index 1 to cachePosition
        for (int i = cachePosition; i > 0; --i)   {
            cache[i] = cache[i - 1];
            if (cache[i] != -1)
                ++vertices[cache[i]].cachePosition;
        }
        //move the new element to the top of the cache
        cache[0] = currentVertexIndex;
        vertices[currentVertexIndex].cachePosition = 0;
    }
}

//update scores, remove overhang and find new greatest triangle
void VertexCacheOptimizer::updateCacheAndFindGreatestTriangle(std::vector<Vertex> &vertices, std::vector<Triangle> &triangles, 
                                                              int &greatestTriangleIndex, std::vector<int> &cache)
{
    greatestTriangleIndex = -1;
    float greatestTriangleScore = -1.0f;
    const size_t cacheSize = cache.size();
    for (uint v = 0; v < cacheSize; ++v) {
        if (cache[v] == -1)
            break;
        Vertex& currentVertex = vertices[cache[v]];
        //remove overhang
        if(v >= MAX_SIZE_VERTEX_CACHE) {
            currentVertex.cachePosition = -1;
            cache[v] = -1;
        }

        const float oldScore = currentVertex.score;
        currentVertex.score = calculateVertexScore(currentVertex);
        //update the scores of this vertices triangles
        for (uint t = 0; t < currentVertex.numTrianglesToDo; ++t)  {
            const int currentTriangleIndex = currentVertex.triangles[t];
            triangles[currentTriangleIndex].score += currentVertex.score - oldScore;
            if (triangles[currentTriangleIndex].score > greatestTriangleScore) {
                greatestTriangleIndex = currentTriangleIndex;
                greatestTriangleScore = triangles[greatestTriangleIndex].score;
            }
        }
    }
}

//scan the complete triangle list for the best triangle
int VertexCacheOptimizer::findGreatestTriangle(const std::vector<Triangle> &triangles)
{
    float greatestTriangleScore(-1.0);
    int greatestTriangleIndex(-1);
    const size_t maxIndex = triangles.size()-1;
    for (int i = 0; i != maxIndex; ++i) {
        if (triangles[i].addedToDrawList)
            continue;
        if (triangles[i].score > greatestTriangleScore) {
            greatestTriangleIndex = i;
            greatestTriangleScore = triangles[i].score;
        }
    }
    return greatestTriangleIndex;
}


void VertexCacheOptimizer::applyOn(t_VertexIndexListP vertexIndices, t_VertexListP vertexData)
{
    t_uints indices = vertexIndices->getIndices();
    const uint numVertices = vertexData->size();

    VertexCacheOptimizer::applyOptimization(indices, numVertices);

    vertexIndices->setMultipleIndices(0, indices.size(), std::bind(&t_uints::at, &indices, std::placeholders::_1));
}

void VertexCacheOptimizer::applyOptimization(t_uints &indices, const uint numVertices)
{
    int scanPos = 0;
    std::vector<Vertex> vertices(numVertices);
    std::vector<Triangle> triangles(indices.size() / 3);
    std::vector<int> cache(MAX_SIZE_VERTEX_CACHE + 3, -1);

    initScoreTables();

    initLists(indices, vertices, triangles);

    initScores(vertices, triangles);

    int greatestTriangleIndex = findGreatestTriangle(triangles);

    indices.clear();
    indices.reserve(triangles.size() * 3);

    // repeat until all triangles are added to the draw list
    while (greatestTriangleIndex != -1) {
        addGreatestTriangle(indices, vertices, triangles, greatestTriangleIndex);

        pushVerticesToStack(vertices, triangles, greatestTriangleIndex, cache);
        
        // this will also give as a new greatestTriangle, if there is any reusable in our cache
        updateCacheAndFindGreatestTriangle(vertices, triangles, greatestTriangleIndex, cache);

        //if no greatestTriangle was found, iterate over all triangles
        if (greatestTriangleIndex == -1) {
            for (;scanPos < triangles.size(); scanPos++) {
                if (!triangles[scanPos].addedToDrawList) {
                    greatestTriangleIndex = scanPos;
                    break;
                }
            }
        }
    }
}
