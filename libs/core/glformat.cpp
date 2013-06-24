#include <GL/glew.h>

#include <cassert>

#include <QGLFormat>

#include "glformat.h"
#include "gpuquery.h"

#ifndef WIN32
#include <GL/glx.h>
#endif


const GLFormat::t_minorsByMajor GLFormat::m_validGLVersions
    = GLFormat::validGLVersions();

const GLFormat::t_profileIdentifier GLFormat::m_validProfileIdentifier
    = GLFormat::validProfileIdentifier();


GLFormat::GLFormat()

:   m_majorVersion(3)
,   m_minorVersion(1)

,   m_profile(NoProfile)

,   m_redBufferSize(8)
,   m_greenBufferSize(8)
,   m_blueBufferSize(8)
,   m_alphaBufferSize(8)

,   m_depthBufferSize(24)
,   m_stencilBufferSize(0)

,   m_doubleBuffer(true)
,   m_stereo(false)

,   m_sampleBuffers(false)
,   m_samples(0)

,   m_swapInterval(0)
{
}

GLFormat::GLFormat(const QGLFormat & format)

:   m_majorVersion(format.majorVersion())
,   m_minorVersion(format.minorVersion())

,   m_redBufferSize  (format.redBufferSize())
,   m_greenBufferSize(format.greenBufferSize())
,   m_blueBufferSize (format.blueBufferSize())
,   m_alphaBufferSize(format.alpha() ? format.alphaBufferSize() : 0)

,   m_depthBufferSize  (format.depth()   ? format.depthBufferSize()   : 0)
,   m_stencilBufferSize(format.stencil() ? format.stencilBufferSize() : 0)

,   m_doubleBuffer(format.doubleBuffer())
,   m_stereo(format.stereo())

,   m_sampleBuffers(format.sampleBuffers())
,   m_samples(format.samples())

,   m_swapInterval(format.swapInterval())
{
  switch(format.profile())
    {
    default:
    case QGLFormat::NoProfile:
        m_profile = NoProfile;
        break;
    case QGLFormat::CoreProfile:
        m_profile = CoreProfile;
        break;
    case QGLFormat::CompatibilityProfile:
        m_profile = CompatibilityProfile;
        break;
    };
}

GLFormat::~GLFormat()
{
}

const QGLFormat GLFormat::asQGLFormat() const
{
    QGLFormat format;

    format.setVersion(m_majorVersion, m_minorVersion);

    switch(m_profile)
    {
    default:
    case NoProfile:
        format.setProfile(QGLFormat::NoProfile);
        break;
    case CoreProfile:
        format.setProfile(QGLFormat::CoreProfile);
        break;
    case CompatibilityProfile:
        format.setProfile(QGLFormat::CompatibilityProfile);
        break;
    };

    format.setRedBufferSize    (m_redBufferSize);
    format.setGreenBufferSize  (m_greenBufferSize);
    format.setBlueBufferSize   (m_blueBufferSize);
    format.setAlphaBufferSize  (m_alphaBufferSize);

    format.setDepthBufferSize  (m_depthBufferSize);
    format.setStencilBufferSize(m_stencilBufferSize);
    format.setDoubleBuffer     (m_doubleBuffer);
    format.setStereo           (m_stereo);
    format.setSampleBuffers    (m_sampleBuffers);
    format.setSamples          (m_samples);
    format.setSwapInterval     (m_swapInterval);

    return format;
}

const unsigned int GLFormat::majorVersion() const
{
    return m_majorVersion;
}

const unsigned int GLFormat::minorVersion() const
{
    return m_minorVersion;
}

const GLFormat::t_minorsByMajor GLFormat::validGLVersions()
{
    t_minorsByMajor minorsByMajor;

    // valid pairs are taken from:
    // http://qt-project.org/doc/qt-5.0/qtopengl/qglformat.html

    minorsByMajor.insertMulti(1, 0);
    minorsByMajor.insertMulti(1, 1);
    minorsByMajor.insertMulti(1, 2);
    minorsByMajor.insertMulti(1, 3);
    minorsByMajor.insertMulti(1, 4);
    minorsByMajor.insertMulti(1, 5);

    minorsByMajor.insertMulti(2, 0);
    minorsByMajor.insertMulti(2, 1);

    minorsByMajor.insertMulti(3, 0);
    minorsByMajor.insertMulti(3, 1);
    minorsByMajor.insertMulti(3, 2);
    minorsByMajor.insertMulti(3, 3);

    minorsByMajor.insertMulti(4, 0);
    minorsByMajor.insertMulti(4, 1);
    minorsByMajor.insertMulti(4, 2);
    minorsByMajor.insertMulti(4, 3);

    return minorsByMajor;
}

void GLFormat::setVersion(
    const unsigned int major
,   const unsigned int minor)
{
    typedef QList<unsigned int> t_minors;

    // retrieve nearest valid major
    m_majorVersion = m_validGLVersions.lowerBound(major).key();
    const t_minors minors(m_validGLVersions.values(m_majorVersion));

    assert(!minors.isEmpty());

    if(major == m_majorVersion)
    {
        // get nearest valid minor
        m_minorVersion = 0;
        foreach(unsigned int m, minors)
            if(m_minorVersion <= m && m <= minor)
                m_minorVersion = m;
    }
    else
        // get highest valid minor
        m_minorVersion = minors.back();

    if(minor != m_minorVersion
    || major != m_majorVersion)
    qDebug("Unknown OpenGL Version %i.%i was adjusted to %i.%i."
        , major, minor, m_majorVersion, m_minorVersion);
}

const GLFormat::e_Profile GLFormat::profile() const
{
    return m_profile;
}

void GLFormat::setProfile(const GLFormat::e_Profile profile)
{
    m_profile = profile;
}

const QStringList GLFormat::extensions() const
{
    return m_extensions;
}

void GLFormat::setExtensions(const QStringList & extensions)
{
    m_extensions = extensions;
}

const unsigned int GLFormat::redBufferSize() const
{
    return m_redBufferSize;
}

void GLFormat::setRedBufferSize(const unsigned int size)
{
    m_redBufferSize = size;
}

const unsigned int GLFormat::greenBufferSize() const
{
    return m_greenBufferSize;
}

void GLFormat::setGreenBufferSize(const unsigned int size)
{
    m_greenBufferSize = size;
}

const unsigned int  GLFormat::blueBufferSize() const
{
    return m_blueBufferSize;
}

void GLFormat::setBlueBufferSize(const unsigned int size)
{
    m_blueBufferSize = size;
}

const unsigned int  GLFormat::alphaBufferSize() const
{
    return m_alphaBufferSize;
}

void GLFormat::setAlphaBufferSize(const unsigned int size)
{
    m_alphaBufferSize = size;
}

const bool GLFormat::depth() const
{
    return m_depthBufferSize > 0;
}

const unsigned int GLFormat::depthBufferSize() const
{
    return m_depthBufferSize;
}

void GLFormat::setDepthBufferSize(const unsigned int size)
{
    m_depthBufferSize = size;
}

const bool GLFormat::stencil() const
{
    return m_stencilBufferSize > 0;
}

const unsigned int GLFormat::stencilBufferSize() const
{
    return m_stencilBufferSize;
}

void GLFormat::setStencilBufferSize(const unsigned int size)
{
    m_stencilBufferSize = size;
}

const bool GLFormat::doubleBuffer() const
{
    return m_doubleBuffer;
}

void GLFormat::setDoubleBuffer(const bool enable)
{
    m_doubleBuffer = enable;
}

const bool GLFormat::stereo() const // quadBuffer
{
    return m_stereo;
}

void GLFormat::setStereo(const bool enable)
{
    m_stereo = enable;
}

const bool GLFormat::sampleBuffers() const
{
    return m_sampleBuffers;
}

void GLFormat::setSampleBuffers(const bool enable)
{
    m_sampleBuffers = enable;
}

const unsigned int GLFormat::samples() const
{
    return m_samples;
}

void GLFormat::setSamples(const unsigned int samples)
{
    m_samples = samples;
}

const int GLFormat::swapInterval() const
{
    return m_swapInterval;
}

void GLFormat::setSwapInterval(const int interval)
{
    m_swapInterval = interval;
}

const bool GLFormat::vsync() const
{
    return m_swapInterval > 0;
}

void GLFormat::setVSync(const bool enable)
{
    m_swapInterval = enable ? 1 : 0;
}

const GLFormat::t_profileIdentifier GLFormat::validProfileIdentifier()
{
    t_profileIdentifier profileIdentifier;

    profileIdentifier[NoProfile]            = "NoProfile";
    profileIdentifier[CompatibilityProfile] = "CompatibilityProfile";
    profileIdentifier[CoreProfile]          = "CoreProfile";

    return profileIdentifier;
}


// VERIFICATION

const bool GLFormat::verify(const QGLFormat & format) const
{
    GLFormat current(format);

    bool valid(true);

    valid &= verifyVersionAndProfile(*this, current);
    valid &= verifyExtensions       (*this);
    valid &= verifySwapInterval     (*this, current);
    valid &= verifyPixelFormat      (*this, current);

    return valid;
}

const bool GLFormat::verifyVersionAndProfile(
    const GLFormat & requested
,   const GLFormat & current)
{
    const bool sameProfiles(requested.profile() == current.profile());

    if(!sameProfiles)
    {
        qWarning("A context with a different profile as requested was created:");
        const QString info(QString("%1 requested, %2 created.\n")
            .arg(m_validProfileIdentifier[requested.profile()])
            .arg(m_validProfileIdentifier[current.profile()]));
        qWarning("%s", qPrintable(info));
    }

    if(requested.majorVersion() != current.majorVersion()
    || requested.minorVersion() != current.minorVersion())
    {
        qWarning("A context with a different OpenGL Version as requested was created:");
        const QString info(QString("%1.%2 requested, %3.%4 created.\n")
            .arg(requested.majorVersion()).arg(requested.minorVersion())
            .arg(current.majorVersion()).arg(current.minorVersion()));
        qWarning("%s", qPrintable(info));

        if(requested.profile() == CoreProfile)
            return false;
    }
    else
    {
        const QString info(QString("Context Version: %1.%2\n")
            .arg(current.majorVersion()).arg(current.minorVersion()));
        qDebug("%s", qPrintable(info));
    }

    return sameProfiles;
}

const bool GLFormat::verifyExtensions(const GLFormat & requested)
{
    if(requested.profile() == CoreProfile)
    {
        qDebug("Due to requested Core Profile, OpenGL Extensions are not verified.\n");
        return true;
    }

    QStringList unsupported;

    foreach(const QString & extension, requested.extensions())
        if(!GPUQuery::extensionSupported(qPrintable(extension)))
            unsupported << extension;

    if(unsupported.isEmpty())
        return true;

    if(unsupported.size() > 1)
        qWarning("The following requested OpenGL extensions are not supported:");
    else
        qWarning("The following requested OpenGL extension is not supported:");

    foreach(const QString & extension, unsupported)
        qWarning("%s", qPrintable(extension));

    qWarning("");

    return false;
}


const bool GLFormat::verifySwapInterval(
    const GLFormat & requested
,   const GLFormat & current)
{
    const int actualInterval    = current.swapInterval();
    const int requestedInterval = requested.swapInterval();

    if(actualInterval != -1 && actualInterval == requestedInterval)
        return true;

    bool result(false);

#ifdef WIN32

    typedef bool (WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

    if(!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if(wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(requestedInterval);

#elif __APPLE__

    qWarning("TODO: Setting swap interval is currently not implemented for __APPLE__");

#else

    typedef int (APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if(!glXSwapIntervalSGI)
    {
        const GLubyte * sgi = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glXGetProcAddress(sgi));
    }
    if(glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(requestedInterval);

#endif

    if(!result)
    {
        glError();
        qWarning("Setting swap interval to %i failed.", requestedInterval);
    }

    return result;
}

inline void GLFormat::verifyBufferSize(
    const unsigned int sizeRequested
,   const unsigned int sizeInitialized
,   const QString & warning
,   QStringList & issues)
{
    if(sizeRequested == sizeInitialized)
        return;

    issues << (warning + " size mismatch: %1 requested, %2 created.")
        .arg(sizeRequested).arg(sizeInitialized);
}

const bool GLFormat::verifyPixelFormat(
    const GLFormat & requested
,   const GLFormat & current)
{
    QStringList issues;

    if(requested.doubleBuffer() && !current.doubleBuffer())
        issues << QString("Double Buffering requested, but Single Buffering initialized.");

    if(requested.depth())
    {
        if(!current.depth())
            issues << QString("Depth Buffer requested, but none created.");
        else
            verifyBufferSize(requested.depthBufferSize(), current.depthBufferSize()
                , "Depth Buffer", issues);
    }

    verifyBufferSize(requested.redBufferSize(),   current.redBufferSize()
        , "Red Buffer", issues);
    verifyBufferSize(requested.greenBufferSize(), current.greenBufferSize()
        , "Green Buffer", issues);
    verifyBufferSize(requested.blueBufferSize(),  current.blueBufferSize()
        , "Blue Buffer",  issues);
    verifyBufferSize(requested.alphaBufferSize(), current.alphaBufferSize()
        , "Alpha Buffer", issues);

    if(requested.stencil())
    {
        if(!current.stencil())
            issues << QString("Stencil Buffer requested, but none created.");
        else
            verifyBufferSize(requested.stencilBufferSize(), current.stencilBufferSize()
                , "Stencil Buffer", issues);
    }

    if(requested.stereo() && !current.stereo())
        issues << QString("Stereo Buffering requested, but not initialized.");

    if(requested.doubleBuffer() && !current.doubleBuffer())
        issues << QString("Double Buffering requested, but not initialized.");

    if(requested.sampleBuffers())
    {
        if(!current.sampleBuffers())
            issues << QString("Sample Buffers requested, but none initialized.");
        else
            verifyBufferSize(requested.samples(), current.samples()
                , "Samples ", issues);
    }

    if(issues.isEmpty())
        return true;

    qWarning("Initialized Pixelformat did not match the Requested One:");
    foreach(const QString & issue, issues)
        qWarning("%s", qPrintable(issue));
    qWarning("");

    return false;
}
