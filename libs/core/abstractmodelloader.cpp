
#include "abstractpainter.h"
#include "group.h"
#include "abstractmodelloader.h"

AbstractModelLoader::AbstractModelLoader()
{
}

AbstractModelLoader::~AbstractModelLoader()
{
}

bool AbstractModelLoader::canLoad(const QString & extension) const
{
    return loadableExtensions().contains(extension);
}

bool AbstractModelLoader::canSave(const QString & extension) const
{
    return saveableExtensions().contains(extension);
}

QStringList AbstractModelLoader::namedLoadableTypes() const
{
    QString allTypes = "All Supported Files (" + allLoadableTypes().join(" ") + ")";
    return QStringList(allTypes);
}

QStringList AbstractModelLoader::allLoadableTypes() const
{
    QStringList extensions = loadableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions;
}

QStringList AbstractModelLoader::namedSaveableTypes() const
{
    QString allTypes = "All Supported Files (" + allSaveableTypes().join(" ") + ")";
    return QStringList(allTypes);
}

QStringList AbstractModelLoader::allSaveableTypes() const
{
    QStringList extensions = saveableExtensions();
    for (int i = 0; i < extensions.length(); ++i) {
        extensions[i].prepend("*.");
    }
    return extensions;
}

Group * AbstractModelLoader::importFromFile(const QString & filePath)
{
    return nullptr;
}

bool AbstractModelLoader::exportToFile(const Group & group,
    const QString & filePath, const QString & extension) const
{
    return false;
}

QStringList AbstractModelLoader::loadableExtensions() const
{
    return QStringList();
}

QStringList AbstractModelLoader::saveableExtensions() const
{
    return QStringList();
}

