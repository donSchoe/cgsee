#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H
#pragma once

#include <QtCore>

#include "../declspec.h"

class DataBlockRegistry;

class CGSEE_API DataBlock: public QObject
{
    Q_OBJECT
public:
    typedef DataBlock * t_StandardPointer;

public:
    virtual ~DataBlock();

    template<class DataBlockSubclass>
    static t_StandardPointer createDataItemWithName(QString &name, DataBlockRegistry &registry);

    // attempt to clone the item.
    // it will be given a new name and saved in the registry provided.
    virtual t_StandardPointer clone(QString *newName, DataBlockRegistry & registry);

    QString name() const;
    Q_PROPERTY(QString name READ name)
protected:
    QString m_name;
    
    // Objects are created with static function createDataItemWithName which
    // registers them automatically in the registry provided
    DataBlock();

    // This is already done in QObject, but in case if anyone forgets:
    // QObjects cannot be copied with a constructor
    DataBlock(const DataBlock&);
};

typedef DataBlock::t_StandardPointer t_DataBlockP;

template <class DataItemSubclass>
static t_DataItemP DataItem::createDataItemWithName(QString& name, GlobalDataRegistry &registry)
{
    t_DataItemP result = new DataItemSubclass;
    if (nullptr == result)
        return result;
    result->m_name = registry.registerNewData(name, result);
    return result;
}

class CGSEE_API DataBlockRegistry: public QObject
{
    Q_OBJECT
public:
    virtual ~DataBlockRegistry();

    // This Method registers the new data item using the name given; 
    // if an item with this name already exists, GlobalDataRegistry will
    // choose an unique name on its own; 
    // returns the name under which the object has been saved
    QString registerNewData(QString proposedName, t_DataBlockP data);
    t_DataBlockP getDataItemByName(QString itemName);

public slots:
    // Notifies about destruction of a DataItem.
    void dataItemRemoved(QObject* dataItem);

protected:
    QHash<QString, t_DataBlockP> m_dataMap;
};

#endif //DATAINTERFACE_H