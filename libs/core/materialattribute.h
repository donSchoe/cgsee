#pragma once

#include "program.h"

class CGSEE_API MaterialAttribute
{
public:

    MaterialAttribute(const std::string &name);
    virtual ~MaterialAttribute();

    virtual void bind(Program &program) = 0;


protected:

    const std::string m_name;
};
