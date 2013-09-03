#include "cameraimplementation.h"

#include "camera.h"


QMap<QString, CameraImplementation::CamConstructor> CameraImplementation::implementations;

const QString CameraImplementation::VIEWPORT_UNIFORM   ("viewport");
const QString CameraImplementation::VIEW_UNIFORM       ("view");
const QString CameraImplementation::PROJECTION_UNIFORM ("projection");
const QString CameraImplementation::CAMERAPOSITION_UNIFORM ("cameraposition");

bool CameraImplementation::registerImplementation(QString name, CamConstructor conststructor)
{
    if (implementations.contains(name))
        return false;
    implementations.insert(name, conststructor);
    return true;
}

CameraImplementation * CameraImplementation::newCameraByName(Camera & abstraction, QString name)
{
    if (! implementations.contains(name))
        return nullptr;
    // call constructor of selected type, with given camera as abstraction
    return implementations[name](&abstraction);
}

QVector<CameraImplementation*> CameraImplementation::newImplementations(Camera & abstraction)
{
    QVector<CameraImplementation*> cameras;
    // call constructor of each registered type, add new object to result list
    for (CamConstructor constructor : implementations.values())
        cameras.append(constructor(&abstraction));
    return cameras;
}

CameraImplementation::CameraImplementation(Camera & abstraction)
    : m_abstraction(abstraction)
{
}

CameraImplementation::~CameraImplementation()
{
}

void CameraImplementation::draw(const Program & program, const glm::mat4 & transform)
{
}

void CameraImplementation::onInvalidatedView()
{
}

void CameraImplementation::onInvalidatedViewport(const int height, const int width)
{
}

void CameraImplementation::onInvalidatedChildren()
{
}