#include "material.h"


Material::Material()
{

}

Material::~Material()
{
    for(auto attribute : m_attributes) {
        delete attribute;
    }
}

void Material::addAttribute(MaterialAttribute *attribute)
{
    m_attributes.push_back(attribute);
}

void Material::bind(Program &program)
{
    for(auto attribute : m_attributes) {
        attribute->bind(program);
    }
}
