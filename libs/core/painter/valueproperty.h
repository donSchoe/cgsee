
#pragma once

#include "abstractproperty.h"
#include "abstractpropertyvisitor.h"

template <typename Type>
class ValueProperty : public AbstractProperty
{
public:
    ValueProperty(QString name, QString description, Type value = NULL);
    virtual ~ValueProperty();

    virtual void visit(AbstractPropertyVisitor & visitor);

    virtual Type value() const;
    virtual void setValue(Type value, bool silent = true);

protected:
    Type m_value;
};

template <typename Type>
ValueProperty<Type>::ValueProperty(QString name, QString description, Type value)
:   AbstractProperty(name, description)
,   m_value(value)
{
}

template <typename Type>
ValueProperty<Type>::~ValueProperty()
{
}

template <typename Type>
void ValueProperty<Type>::visit(AbstractPropertyVisitor & visitor)
{
    visitor.visitGeneric(*this);
}

template <typename Type>
Type ValueProperty<Type>::value() const
{
    return m_value;
}

template <typename Type>
void ValueProperty<Type>::setValue(Type value, bool silent)
{
    m_value = value;
    if (!silent)
        emit this->changed(*this);
}