#pragma once

#include "defaultpass.h"

class CGSEE_API  LightSourcePass : public DefaultPass
{
public:
    LightSourcePass(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~LightSourcePass(void);

    virtual void sceneChanged(Group * scene) override;
    virtual void resize(const int width, const int height) override;
//
    glm::mat4 transform();
//
protected:
    virtual void render() override;

private:
    Camera * m_lightcam;
    Group * m_scene;
};

