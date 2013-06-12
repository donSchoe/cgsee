#pragma once

#include "program.h"

class CGSEE_API MaterialAttribute
{
public:

    MaterialAttribute() = 0;
    virtual ~MaterialAttribute() = 0;

    virtual void bindTo(Program & program) = 0;


private:

};
