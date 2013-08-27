
#pragma once

#include <memory>
#include <QString>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <core/material/texture2d.h>
#include <core/material/material.h>

#include "declspec.h"
#include "common.h"
#include "abstractmodelloader.h"

class DataBlockRegistry;
class Group;
class PolygonalDrawable;
struct aiNode;
struct aiScene;
struct aiMesh;

extern const char *DIFFUSE_TEX_NAME;
extern const char *AMBIENT_TEX_NAME;
extern const char *SPECULAR_TEX_NAME;
extern const char *EMISSION_TEX_NAME;
extern const char *NORMALS_TEX_NAME;
extern const char *SHININESS_TEX_NAME;

class CGSEE_API AssimpLoader : public AbstractModelLoader
{
public:
    AssimpLoader(std::shared_ptr<DataBlockRegistry> registry = nullptr);
    virtual ~AssimpLoader();

    virtual void clear();

    virtual QStringList namedLoadableTypes() const;
    virtual Group * importFromFile(const QString & filePath);

protected:
    virtual QStringList loadableExtensions() const;

protected:
    Group * parseNode(const aiScene & scene,
        const QList<PolygonalDrawable *> &drawables, const aiNode & node) const;

    void parseMeshes(aiMesh ** meshes,
        const unsigned int numMeshes, QList<PolygonalDrawable *> & drawables) const;

    void parseMaterials(aiMaterial **materials, const unsigned int numMaterials);

    PolygonalDrawable * parseMesh(const aiMesh & mesh) const;

    Texture2D *loadTexture(aiMaterial *material, Texture2D_Type cgseeType, aiTextureType type);

protected:
    Assimp::Importer * m_importer;

    QString m_modelDir;
    std::vector<Material*> m_materials;
};
