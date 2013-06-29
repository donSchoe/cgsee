#pragma once

#include <list>
#include "materialattribute.h"


class Material
{
public:
    Material();
    virtual ~Material();

    void addAttribute(MaterialAttribute *attribute);

    void bind(const Program &program);


protected:
    std::list<MaterialAttribute*> m_attributes;
};
