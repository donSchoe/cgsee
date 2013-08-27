#pragma once

#include "defaultpass.h"

class CGSEE_API NormalzPass : public DefaultPass
{
public:
    NormalzPass(Camera * camera, FileAssociatedShader * depth_util);
    virtual ~NormalzPass(void);

};

