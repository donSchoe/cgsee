
#include <QFormLayout>
#include <QCheckBox>
#include <QString>
#include "propertywidgetbuilder.h"
#include <core/painter/boolproperty.h>

PropertyWidgetBuilder::PropertyWidgetBuilder()
:   m_widget(new QWidget())
,   m_layout(new QFormLayout(m_widget))
{
    m_widget->setLayout(m_layout);
}

PropertyWidgetBuilder::~PropertyWidgetBuilder()
{
    delete m_widget;
}

void PropertyWidgetBuilder::buildWidget(const QList<AbstractPainterProperty *> & properties)
{
    delete m_widget;
    m_widget = new QWidget();
    m_layout = new QFormLayout(m_widget);
    m_widget->setLayout(m_layout);

    this->iterateOverProperties(properties);
}

void PropertyWidgetBuilder::visitBool(BoolProperty & boolProperty)
{
    QCheckBox * box = new QCheckBox(boolProperty.description(), m_widget);
    if (boolProperty.enabled())
        box->setCheckState(Qt::Checked);
    else
        box->setCheckState(Qt::Unchecked);
    
    m_layout->addWidget(box);
    QObject::connect(box, &QCheckBox::stateChanged, [&boolProperty] (int state) -> void {
        boolProperty.setEnabled(!!state);
        qDebug("Painter: Set %s = %i", qPrintable(boolProperty.name()), boolProperty.enabled());
    });
}

void PropertyWidgetBuilder::visitList(ListProperty & listProperty)
{
    // do stuff
}

QWidget * PropertyWidgetBuilder::retainWidget()
{
    QWidget * widget = m_widget;
    m_widget = nullptr;
    return widget;
}