#pragma once

#include "common.h"
#include "geometryoptimizer.h"

class VertexReuse : public GeometryOptimizer
{
public:
    void applyOn(t_VertexIndexListP vertexIndices, t_VertexListP vertexData) override;

protected:
    static void reuseVertices(t_vec3s& vertices, t_vec3s& normals, t_vec2s& texcs, t_uints& indices);
};

