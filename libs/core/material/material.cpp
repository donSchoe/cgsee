#include "material.h"
#include <iostream>

using namespace std;

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

void Material::bind(const Program &program)
{
    for(auto attribute : m_attributes) {
        attribute->bind(program);
    }
}
