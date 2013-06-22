
#include <core/declspec.h>
#include <core/abstractpainter.h>

class Group;

class CGSEE_API AbstractScenePainter : public AbstractPainter
{
public:
    AbstractScenePainter();
    AbstractScenePainter(Group * scene);
    virtual ~AbstractScenePainter();

    void assignScene(Group * scene);
    Group * retainScene();
    Group & getScene() const;
protected:
    Group * m_scene;
};