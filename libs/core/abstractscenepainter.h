#include <glm/glm.hpp>
#include <core/declspec.h>
#include <core/abstractpainter.h>

class Node;
class Group;
class PolygonalDrawable;

class CGSEE_API AbstractScenePainter : public AbstractPainter
{
public:
    AbstractScenePainter();
    virtual ~AbstractScenePainter();

    void assignScene(Group * scene);
    Group * retainScene();
    Group & getScene() const;
    virtual void draw(Group & group, const glm::mat4 & transform) = 0;
    virtual void draw(PolygonalDrawable & drawable, const glm::mat4 & transform) = 0;
    virtual void draw(Node & node, const glm::mat4 & transform) = 0;

protected:
    Group * m_scene;
};
