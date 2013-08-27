#pragma once

#include "defaultpass.h"

class CGSEE_API DepthPass : public DefaultPass
{
public:
    DepthPass(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~DepthPass(void);
};

