#pragma once

#include <QString>
#include "../program.h"

class CGSEE_API MaterialAttribute
{
public:

    MaterialAttribute(const QString &name);
    virtual ~MaterialAttribute();

    virtual void bind(const Program &program) = 0;

    void setName(const QString &name);
    const QString &name() const;


protected:

    QString m_name;
};
