#include "assimploader.h"
#include "datacore/datablock.h"
#include "scenegraph/group.h"
#include "scenegraph/polygonalgeometry.h"
#include "material/texture2d.h"
#include "scenegraph/polygonaldrawable.h"
#include "material/materialattribute.h"
#include "material/textureloader.h"
#include <QFileInfo>
#include <iostream>
#include <vector>

using namespace std;

const char *DIFFUSE_TEX_NAME("diffuseTex");
const char *AMBIENT_TEX_NAME("ambientTex");
const char *SPECULAR_TEX_NAME("specularTex");
const char *EMISSION_TEX_NAME("emissionTex");
const char *NORMALS_TEX_NAME("normalsTex");
const char *SHININESS_TEX_NAME("shininessTex");

AssimpLoader::AssimpLoader(std::shared_ptr<DataBlockRegistry> registry)
: AbstractModelLoader()
, m_registry(registry)
, m_importer(new Assimp::Importer())
{
    if( m_registry == nullptr )
        m_registry = std::make_shared<DataBlockRegistry>();
}

AssimpLoader::~AssimpLoader()
{
    delete m_importer;
}

void AssimpLoader::clear() {
    m_modelDir.clear();
    m_materials.clear();
}

QStringList AssimpLoader::namedLoadableTypes() const
{
    QStringList types = AbstractModelLoader::namedLoadableTypes();
    types
        << "Collada (*.dae *.xml)"
        << "Blender (*.blend)"
        << "Biovision BVH (*.bvh)"
        << "3D Studio Max 3DS (*.3ds)"
        << "3D Studio Max ASE (*.ase)"
        << "Wavefront Object (*.obj)"
        << "Stanford Polygon Library (*.ply)"
        << "AutoCAD DXF (*.dxf)"
        << "IFC-STEP, Industry Foundation Classes (*.ifc)"
        << "Neutral File Format (*.nff)"
        << "Sense8 WorldToolkit (*.wtk)"
        << "Valve Model (*.smd *.vta)"
        << "Quake I (*.mdl)"
        << "Quake II (*.md2)"
        << "Quake III (*.md3)"
        << "Quake 3 BSP (*.pk3)"
        << "RtCW (*.mdc)"
        << "Doom 3 (*.md5mesh *.md5anim *.md5camera)"
        << "DirectX X (*.x)"
        << "Quick3D (*.q3o *.q3s)"
        << "Raw Triangles (.raw)"
        << "AC3D (*.ac)"
        << "Stereolithography (*.stl)"
        << "Autodesk DXF (*.dxf)"
        << "Irrlicht Mesh (*.irrmesh *.xml)"
        << "Irrlicht Scene (*.irr *.xml)"
        << "Object File Format (*.off)"
        << "Terragen Terrain (*.ter)"
        << "3D GameStudio Model (*.mdl)"
        << "3D GameStudio Terrain (*.hmp)"
        << "Ogre (*.mesh.xml, *.skeleton.xml, *.material)"
        << "Milkshape 3D (*.ms3d)"
        << "LightWave Model (*.lwo)"
        << "LightWave Scene (*.lws)"
        << "Modo Model (*.lxo)"
        << "CharacterStudio Motion (*.csm)"
        << "Stanford Ply (*.ply)"
        << "TrueSpace (*.cob *.scn)"
        << "XGL (*.xgl *.zgl)";

    return types;
}

QStringList AssimpLoader::loadableExtensions() const
{
    aiString assimp_extensions;
    m_importer->GetExtensionList(assimp_extensions);
    QStringList extensions = QString(assimp_extensions.C_Str()).split(";");
    for (int i = 0; i < extensions.length(); i++)
        extensions[i].remove(0, 2);

    return extensions;
}

Group * AssimpLoader::importFromFile(const QString & filePath)
{
    clear();
    qDebug("Reading geometry with Assimp from \"%s\".", qPrintable(filePath));

    m_modelDir = QFileInfo(filePath).dir().path();

    const aiScene * scene = m_importer->ReadFile(filePath.toStdString(), aiProcess_Triangulate);

    if (!scene) {
        qCritical("Assimp couldn't load %s", qPrintable(filePath));
        return nullptr;
    }

    QList<PolygonalDrawable *> drawables;
    drawables.reserve(scene->mNumMeshes);

    // Materials have to be loaded before the meshes using them for now
    parseMaterials(scene->mMaterials, scene->mNumMaterials);
    parseMeshes(scene->mMeshes, scene->mNumMeshes, drawables);

    Group * group = parseNode(*scene, drawables, *(scene->mRootNode));

//    for(auto material : m_materials) {
//        group->addManagedMaterial(material);
//    }

    m_importer->FreeScene();

    return group;
}

Group * AssimpLoader::parseNode(const aiScene & scene,
    const QList<PolygonalDrawable *> &drawables, const aiNode & node) const
{
    Group * group = new Group(node.mName.C_Str());

    const aiMatrix4x4 & mat = node.mTransformation;
    glm::mat4 transform(
        mat.a1, mat.a2, mat.a3, mat.a4,
        mat.b1, mat.b2, mat.b3, mat.b4,
        mat.c1, mat.c2, mat.c3, mat.c4,
        mat.d1, mat.d2, mat.d3, mat.d4
    );
    group->setTransform(transform);

    for (int i = 0; i < node.mNumChildren; i++) {
        group->append(parseNode(scene, drawables, *(node.mChildren[i])));
    }

    for (int i = 0; i < node.mNumMeshes; i++) {
        group->append(drawables[node.mMeshes[i]]);
    }

    return group;
}

void AssimpLoader::parseMeshes(aiMesh **meshes,
    const unsigned int numMeshes, QList<PolygonalDrawable *> &drawables) const
{
    for (unsigned int i = 0; i < numMeshes; i++)
        drawables.insert(i, parseMesh(*meshes[i]));
}

void AssimpLoader::parseMaterials(aiMaterial **materials, const unsigned int numMaterials)
{
    const int numTextureTypes = 6;
    const aiTextureType TEXTURE_TYPES[] = {aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_AMBIENT,  aiTextureType_EMISSIVE, aiTextureType_NORMALS, aiTextureType_SHININESS};
    const char *TEXTURE_NAMES[] = {DIFFUSE_TEX_NAME, SPECULAR_TEX_NAME, AMBIENT_TEX_NAME, EMISSION_TEX_NAME, NORMALS_TEX_NAME, SHININESS_TEX_NAME};

    for(int m = 0; m < numMaterials; m++) {
        aiMaterial *aMaterial = materials[m];
        Material *material = new Material;
        cout << "Loading Material " << m << endl;
        // Try to load textures
        for(int t = 0; t < numTextureTypes; t++) {
            if(aMaterial->GetTextureCount(TEXTURE_TYPES[t]) > 0) {
                cout << "Material has " << TEXTURE_NAMES[t] << endl;
                Texture2D *tex = loadTexture(aMaterial, TEXTURE_TYPES[t]);
                if(tex == nullptr) {
                    qCritical("  Failed to load %s\n", TEXTURE_NAMES[t]);
                }
                else {
                    tex->setName(QString(TEXTURE_NAMES[t]));
                    material->addAttribute(tex);
                }
            }
            else {
                cout << "Material has no " << TEXTURE_NAMES[t] << endl;
            }
        }

        // If certain textures are not available, set defaults
        if(material->attribute<Texture2D>(DIFFUSE_TEX_NAME) == NULL && material->attribute<Texture2D>(AMBIENT_TEX_NAME) != NULL) {
            Texture2D *diffuseTex = loadTexture(aMaterial, aiTextureType_AMBIENT);
            diffuseTex->setName(DIFFUSE_TEX_NAME);
            material->addAttribute(diffuseTex);
        }
        if(material->attribute<Texture2D>(AMBIENT_TEX_NAME) == NULL && material->attribute<Texture2D>(DIFFUSE_TEX_NAME) != NULL) {
            Texture2D *ambientTex = loadTexture(aMaterial, aiTextureType_DIFFUSE);
            ambientTex->setName(AMBIENT_TEX_NAME);
            material->addAttribute(ambientTex);
        }


        m_materials.push_back(material);
    }
}

Texture2D *AssimpLoader::loadTexture(aiMaterial *material, aiTextureType type)
{
    Texture2D *tex;
    aiString aTexPath;
    QString texPath;

    material->GetTexture(type, 0, &aTexPath);
    texPath = m_modelDir + "/" + aTexPath.C_Str();
    tex = TextureLoader::loadTexture2D(texPath);

    if(tex == nullptr) {
        qCritical("Couldn't load texture %s", texPath.toStdString().c_str());
    }

    return tex;
}

PolygonalDrawable * AssimpLoader::parseMesh(const aiMesh & mesh) const
{
    auto geometry = std::make_shared<PolygonalGeometry>(m_registry);

    for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
        glm::vec3 vector(
                         mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z
                         );
        geometry->setVertex(i, vector);
    }

    if (mesh.HasNormals()) {
        for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
            glm::vec3 vector(
                             mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z
                             );
            geometry->setNormal(i, vector);
        }
    }

    if (mesh.HasTextureCoords(0)) {
        for (int i = 0; i < mesh.mNumVertices; i++) {
            glm::vec2 vector(
                             mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y
                             );
            geometry->setTexC(i, vector);
        }
    }

    unsigned int currentIndex = 0;
    for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
        if (mesh.mFaces[i].mNumIndices != 3)
            qCritical("Ignore polygon with num vertices != 3 (only triangles are supported).");
        else
            for (unsigned int j = 0; j < mesh.mFaces[i].mNumIndices; j++)
                geometry->setIndex(currentIndex++, mesh.mFaces[i].mIndices[j]);
    }

    if (!mesh.HasNormals())
        geometry->retrieveNormals();

    assert(mesh.mMaterialIndex < m_materials.size());
//    geometry->setMaterial(m_materials[mesh.mMaterialIndex]);

    PolygonalDrawable * drawable = new PolygonalDrawable(mesh.mName.C_Str());
    drawable->setGeometry(geometry);
    drawable->setMode(GL_TRIANGLES);
    return drawable;
}
