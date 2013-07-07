#include "abstractscenepainter.h"
#include "scenegraph/group.h"
#include "camera.h"
#include "scenegraph/node.h"
#include <core/textureunitprovider.h>

AbstractScenePainter::AbstractScenePainter()
: AbstractPainter()
, m_scene(nullptr)
{
}

AbstractScenePainter::~AbstractScenePainter()
{
    delete m_scene;
}

void AbstractScenePainter::assignScene(Group * scene)
{
    this->sceneChanged(scene);
    if (m_scene)
        delete retainScene();

    m_scene = scene;
    if (m_initialized)
        this->camera()->append(m_scene);
}

Group * AbstractScenePainter::retainScene()
{
    this->camera()->remove(m_scene, false);
    Group * scene = m_scene;
    m_scene = nullptr;
    return scene;
}

Group & AbstractScenePainter::getScene() const
{
    return *m_scene;
}

void AbstractScenePainter::draw(PolygonalDrawable & drawable, const glm::mat4 & transform)
{

}

void AbstractScenePainter::draw(Node & node, const glm::mat4 & transform)
{

}

void AbstractScenePainter::sceneChanged(Group * scene)
{
}
