#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <QStringList>
#include <QString>
#include <QList>

#include "realisticcomposer.h"
#include <core/abstractscenepainter.h>


template<typename K, typename V> class QMap;
class Camera;
class Program;
class FrameBufferObject;
class RenderPass;


class RealisticPainter : public AbstractScenePainter
{
public:
    enum e_ShadingMode
    {
        FlatShading,
        GouraudShading,
        PhongShading
    };

public:
    RealisticPainter(Camera * camera);
    virtual ~RealisticPainter();

    virtual void paint();
    virtual void resize(const int width, const int height);
    virtual void postShaderRelinked() override;

    virtual void setShading(char shader);
    virtual void setFrameBuffer(int frameBuffer);
    virtual void setEffect(int effect, bool active);

protected:
    virtual const bool initialize() override;
    virtual Camera * camera() override;


    static void bindFBOs(QMap<QString, FrameBufferObject *> & fbos, Program & program);
    static void releaseFBOs(QMap<QString, FrameBufferObject *> & fbos);

    void sceneChanged(Group * scene);

protected:
    Camera * m_camera;
    RealisticComposer m_composer;
    QList<RenderPass*> m_renderPasses;
};

