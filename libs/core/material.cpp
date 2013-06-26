#include "material.h"
#include <iostream>

using namespace std;

Material::Material()
{

}

Material::~Material()
{
    for(auto attribute : m_attributes) {
        cerr << "Deleting " << (void*)attribute << " from " <<(void*)this << endl;
        delete attribute;
        cerr << "  Performed" << endl;
    }
}

void Material::addAttribute(MaterialAttribute *attribute)
{
        cerr << "Adding " << (void*)attribute << " to " <<(void*)this << endl;
    m_attributes.push_back(attribute);
}

void Material::bind(const Program &program)
{
    for(auto attribute : m_attributes) {
        attribute->bind(program);
    }
}
