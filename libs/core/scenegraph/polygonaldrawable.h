#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <QMap>
#include <QVector>

#include <core/declspec.h>
#include <core/material/material.h>

#include "node.h"


class PolygonalGeometry;
class DataBlockRegistry;
class Program;


class CGSEE_API PolygonalDrawable : public Node
{
public:
    typedef std::shared_ptr<PolygonalGeometry> t_geometryP;

    // TODO: wieder rueckgaengig machen...
//     PolygonalDrawable( DataBlockRegistry & registry, const QString & name );
    PolygonalDrawable(const QString & name);
    virtual ~PolygonalDrawable();

    virtual void draw(const Program & program,RenderingPass * renderingpass, const glm::mat4 & transform) override;

    virtual const AxisAlignedBoundingBox boundingBox() const override;

    void setMaterial(Material *material);
    Material *material() const;

    void setGeometry(t_geometryP geometry);
    t_geometryP geometry() { return m_geometry; }

    void setMode(const GLenum mode) { m_mode = mode; }
    inline const GLenum mode() const { return m_mode; }

protected:
    virtual void invalidateBoundingBox() override;

protected:
    t_geometryP m_geometry;
    Material *m_material;
    GLenum  m_mode;
};
