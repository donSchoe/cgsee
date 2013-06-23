
#pragma once

#include <QString>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "declspec.h"
#include "common.h"
#include "abstractmodelloader.h"

class Group;
class PolygonalDrawable;
struct aiNode;
struct aiScene;
struct aiMesh;

class CGSEE_API AssimpLoader : public AbstractModelLoader
{
public:
    AssimpLoader();
    virtual ~AssimpLoader();

    virtual QStringList namedLoadableTypes() const;
    virtual Group * importFromFile(const QString & filePath) const;

protected:
    virtual QStringList loadableExtensions() const;

protected:
    Group * parseNode(const aiScene & scene,
        const QList<PolygonalDrawable *> &drawables, const aiNode & node) const;

    void parseMeshes(aiMesh ** meshes,
        const unsigned int numMeshes, QList<PolygonalDrawable *> & drawables) const;

    void parseMaterials(aiMaterial **materials, const unsigned int numMaterials) const;

    PolygonalDrawable * parseMesh(const aiMesh & mesh) const;

protected:
    Assimp::Importer * m_importer;
};
