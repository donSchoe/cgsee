
#include <core/declspec.h>
#include <core/abstractpainter.h>

class Group;

class CGSEE_API AbstractScenePainter : public AbstractPainter
{
public:
    AbstractScenePainter();
    virtual ~AbstractScenePainter();

    virtual void setShading(char shader) = 0;
    virtual void setFrameBuffer(int frameBuffer) = 0;
    virtual void setEffect(int effect, bool active) = 0;
    
    virtual void postShaderRelinked() = 0;

    void assignScene(Group * scene);
    Group * retainScene();
    Group & getScene() const;
protected:
    virtual void sceneChanged(Group * scene);
    Group * m_scene;
};