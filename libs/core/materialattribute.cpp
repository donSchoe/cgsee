#include "materialattribute.h"

MaterialAttribute::MaterialAttribute(const QString &name):
    m_name(name)
{

}

MaterialAttribute::~MaterialAttribute()
{

}


void MaterialAttribute::setName(const QString &name)
{
    m_name = name;
}

const QString &MaterialAttribute::name() const
{
    return m_name;
}
