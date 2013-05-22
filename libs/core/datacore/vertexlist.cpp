#include <assert.h>

#include "typefunctions.h"
#include "vertexlist.h"

namespace
{
    struct TypeSpec
    {
        unsigned int size;
        std::shared_ptr<AbstractInPlaceTypeFunctions> factory;
        const type_info *typeInfo;
    };

    QMap<QString, TypeSpec> g_registeredTypes;
}

AttributeSpec::AttributeSpec(const QString &name, const QString &type):
    attrName(name)
,   attrType(type)
{
    
}

AttributeStorage::AttributeStorage():
    m_storage(nullptr)
,   m_storageSize(0)
,   m_destroyed(false)
,   m_useCount(new unsigned int(1))
{
}

AttributeStorage::AttributeStorage(const AttributeStorage& rhs):
    m_storage(rhs.m_storage)
,   m_storageSize(rhs.m_storageSize)
,   m_destroyed(rhs.m_destroyed)
{
    if (!m_destroyed && m_storage != nullptr)
    {
        m_useCount = rhs.m_useCount;
        ++(*m_useCount);
    }
    else // m_destroyed || m_storage == nullptr
    {
        m_useCount = new unsigned int(1);
    }
}

AttributeStorage::AttributeStorage(AttributeStorage &&rhs):
    m_storage(rhs.m_storage)
,   m_storageSize(rhs.m_storageSize)
,   m_destroyed(rhs.m_destroyed)
,   m_useCount(rhs.m_useCount)
{
    rhs.m_storage = nullptr;
    rhs.m_storageSize = 0;
    rhs.m_destroyed = true;
    rhs.m_useCount = nullptr;
}

AttributeStorage::AttributeStorage(VertexList &owner):
    m_storage(nullptr)
,   m_storageSize(0)
,   m_destroyed(false)
,   m_useCount(new unsigned int(1))
,   m_owner(&owner)
{
    initialize(owner);
}

AttributeStorage::~AttributeStorage()
{
    if (m_useCount)
    {
        assert(*m_useCount > 0);
        if (--(*m_useCount) == 0)
        {    
            if (m_storage && !m_destroyed)
            {
                if (m_owner)
                {
                    runDestructors(m_owner->getAttrMap());
                    assert(m_destroyed);
                }
                else
                    //something bad has happened
                    qWarning(QObject::tr("Destroying Attribute storage with objects in it intact. Potential leak. In %1, line %2")
                        .arg(__FILE__)
                        .arg(__LINE__)
                        .toLocal8Bit());
            }

            delete [] m_storage;
            delete m_useCount;
        }
        else
        {
            // storage is still in use
        }
    }
    else
    {
        assert (!m_storage || m_destroyed);
        delete [] m_storage;
    }
}

const AttributeStorage & AttributeStorage::operator=(const AttributeStorage &rhs)
{
    if (this == &rhs) // NOTE: perhaps not needed, but it is in some cases
        return *this;

    m_storage = rhs.m_storage;
    m_storageSize = rhs.m_storageSize;
    m_destroyed = rhs.m_destroyed;

    if (!m_destroyed && m_storage != nullptr)
    {
        m_useCount = rhs.m_useCount;
        ++(*m_useCount);
    }
    else // m_destroyed || m_storage == nullptr
    {
        m_useCount = new unsigned int(1);
    }

    return *this;
}

void AttributeStorage::initialize(VertexList &owner)
{
    assert(m_storage == nullptr);
    assert(!m_destroyed);
    assert(m_useCount && *m_useCount == 1);

    delete [] m_storage;
    unsigned int memoryNeeded = 0;
    const t_AttrMap &attrMap = owner.getAttrMap();

    for (const t_AttrDesc &attr: attrMap)
        memoryNeeded = qMax(memoryNeeded, attr.size + attr.location);

    m_storage = new unsigned char[memoryNeeded];
    assert(m_storage != nullptr);
    m_storageSize = memoryNeeded;

    for (const t_AttrDesc &attr: attrMap)
    {
        attr.factory->construct(m_storage + attr.location);
    }
}

void AttributeStorage::runDestructors(const t_AttrMap &attrMap)
{
    if (m_destroyed)
        return;

    if (m_useCount && *m_useCount > 1)
    {
        --(*m_useCount);
        m_useCount = nullptr;
        m_storage = nullptr;
    }
    else for (const t_AttrDesc & attr: attrMap)
        attr.factory->destruct(m_storage + attr.location);

    m_destroyed = true;
}

void AttributeStorage::copyStorage()
{
    assert (m_useCount && *m_useCount > 0);
    if (*m_useCount == 1) 
    {
        return;
    }

    t_StorageType newStorage = new unsigned char [m_storageSize];

    // TODO: differentiate between trivially copyable and not types
    // memcpy(newStorage, m_storage, m_storageSize); // for trivially copyable types
    assert(m_owner);
    for (const t_AttrDesc & attrD: m_owner->getAttrMap())
        attrD.factory->copyTo(m_storage + attrD.location, newStorage + attrD.location);

    --(*m_useCount);
    m_useCount = new unsigned int(1);
    m_storage = newStorage;
}

VertexList::VertexList(QObject* parent):
    DataBlock(parent)
,   m_initialized(false)
{
    
}

VertexList::~VertexList()
{
    if (m_initialized)
    {
        for (AttributeStorage &stor: m_vertices)
            stor.runDestructors(m_attrLayout);
    }
}

void VertexList::initialize(const QList<AttributeSpec> &attrTypes)
{
    if (m_initialized)
        return;

    m_attrLayout.clear();

    unsigned int currentLoc = 0;
    for (AttributeSpec const & attrSpec : attrTypes)
    {
        t_AttrDesc newAttribute;
        newAttribute.location = currentLoc;
        
        // Looking if Qt knows about this type
        int attrTypeId = newAttribute.typeId = QMetaType::type(attrSpec.attrType.toUtf8());
        if (QMetaType::UnknownType == attrTypeId)
        {
            // Apparently it doesn't
            if (false == g_registeredTypes.contains(attrSpec.attrType))
            {
                qWarning(tr("Cannot find type info for type %1, on line %2, in file %3. Ignoring.")
                    .arg(attrSpec.attrType)
                    .arg(__LINE__)
                    .arg(__FILE__)
                    .toLocal8Bit());
                continue;
            }
            const TypeSpec &typeSpec = g_registeredTypes[attrSpec.attrType];
            newAttribute.size = typeSpec.size;
            newAttribute.factory = typeSpec.factory;
            newAttribute.typeInfo = typeSpec.typeInfo;
        }
        else
        {
            newAttribute.size = QMetaType::sizeOf(attrTypeId);
            newAttribute.factory = decltype(t_AttrDesc::factory)
                (new QtTypeFunctions(attrSpec.attrType.toUtf8()));
            newAttribute.typeInfo = nullptr;
        }

        newAttribute.typeName = attrSpec.attrType;
        newAttribute.used = false;
        m_attrLayout.insert(attrSpec.attrName, newAttribute);
        currentLoc += newAttribute.size;
    }
    m_initialized = true;
}

void VertexList::createNewVertices(unsigned int amount)
{
    for (unsigned int i = 0; i < amount; ++i)
    {
        m_vertices.append(AttributeStorage(*this));
    }
}

const t_AttrMap& VertexList::getAttrMap()
{
    return m_attrLayout;
}

t_VertexListP VertexList::createClone()
{
    return new VertexList(*this);
}

unsigned int VertexList::size() const
{
    return m_vertices.size();
}

bool VertexList::isEmpty() const
{
    return m_vertices.isEmpty();
}

bool VertexList::isAttributeUsed(QString attrName) const
{
    if (m_attrLayout.contains(attrName))
        return m_attrLayout[attrName].used;

    return false;
}

VertexIndexList::VertexIndexList(QObject* parent)
{
    if (parent)
    {
        VertexList* myList = qobject_cast<VertexList*>(parent);
        if (myList)
            setVertexList(myList);
    }
}

void VertexIndexList::setVertexList(t_VertexListP associatedList)
{
    if (associatedList)
    {
        connect(associatedList, SIGNAL(updated(QObject*)), SLOT(onVertexListUpdated()));
        connect(associatedList, SIGNAL(destroyed(QObject*)), SLOT(onVertexListDestroyed()));
    }
}

QVector<VertexIndexList::t_indexType> const& VertexIndexList::getIndices() const
{
    return m_indices;
}

void VertexIndexList::setSingleIndex(unsigned int pos, VertexIndexList::t_indexType const vindex)
{
    if (m_indices.size() <= pos)
        m_indices.resize(pos + 1);
    m_indices[pos] = vindex;
}

void VertexIndexList::setMultipleIndices(unsigned int start, unsigned int end, std::function<t_indexType(unsigned int)> initFunc)
{
    for (int i = start; i < end; ++i)
    {
        m_indices[i] = initFunc(i);
    }
}

void VertexIndexList::onVertexListUpdated()
{

}

void VertexIndexList::onVertexListDestroyed()
{
    m_associatedList = nullptr;
}

unsigned int VertexIndexList::size() const
{
    return m_indices.size();
}

bool VertexIndexList::isEmpty() const
{
    return m_indices.isEmpty();
}

t_VertexIndexListP VertexIndexList::createClone()
{
    return new VertexIndexList(*this);
}