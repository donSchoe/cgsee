#pragma once

#include <list>
#include "materialattribute.h"


class Material
{
public:

    Material();
    ~Material();

    void addAttribute(MaterialAttribute *attribute);

    void bind(Program &program);

protected:

    std::list<MaterialAttribute*> m_attributes;

};
