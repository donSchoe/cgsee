#pragma once

#include <QDebug>
#include <list>
#include "materialattribute.h"

class Program;

class Material
{
public:
    Material();
    virtual ~Material();

    void addAttribute(MaterialAttribute *attribute);

    template<typename T> T *attribute(const QString &name) const;

    bool bind(Program &program, const QString &name);
    void bindAll(Program &program);


protected:
    std::list<MaterialAttribute*> m_attributes;
};

template<typename T>
T *Material::attribute(const QString &name) const
{
    for(auto attribute : m_attributes) {
        if(attribute->name() == name) {
            T *re = dynamic_cast<T*>(attribute);
            if(re == nullptr) {
                qCritical("Couldn't cast to requested MaterialAttribute\n");
            }
            return re;
        }
    }
    return nullptr;
}
