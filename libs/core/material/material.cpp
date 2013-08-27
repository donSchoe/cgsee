#include "material.h"
#include "core/program.h"
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

bool Material::bindAttribute(Program &program, const QString &name)
{
    for(MaterialAttribute *attribute : m_attributes) {
        if(attribute->name() == name) {
            attribute->bind(program);
            return true;
        }
    }
    return false;
}

void Material::bindAll(Program &program)
{
    for(MaterialAttribute *attribute : m_attributes) {
        attribute->bind(program);
    }
}
