#pragma once

#include "program.h"

class CGSEE_API MaterialAttribute
{
public:

    virtual ~MaterialAttribute() = 0;

    virtual void bind(const Program &program, const std::string &matName) = 0;

private:

};
