#include <GL/glew.h>

#include <cassert>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QOpenGLContext>
#include <QSettings>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QMenu>
#include <QFileSystemModel>
#include <QDir>

#include "ui_viewer.h"
#include "viewer.h"
#include "canvas.h"
#include "canvasexporter.h"
#include "fileNavigator.h"
#include "fileExplorer.h"

#include <core/navigation/abstractnavigation.h>
#include <core/navigation/flightnavigation.h>
#include <core/navigation/fpsnavigation.h>
#include <core/navigation/arcballnavigation.h>

#include <core/abstractscenepainter.h>
#include <core/fileassociatedshader.h>
#include <core/glformat.h>
#include <core/assimploader.h>


namespace
{
    const QString SETTINGS_GEOMETRY ("Geometry");
    const QString SETTINGS_STATE    ("State");
    const QString SETTINGS_SAVED_VIEWS    ("SavedViews");
}


Viewer::Viewer(
    QWidget  * parent,
    Qt::WindowFlags flags)

:   QMainWindow(parent, flags)
,   m_ui(new Ui_Viewer)
,   m_qtCanvas(nullptr)
,   m_saved_views(4)

,   m_dockNavigator(new QDockWidget(tr("Navigator")))
,   m_dockExplorer(new QDockWidget(tr("Explorer")))
,   m_navigator(new FileNavigator(m_dockNavigator))
,   m_explorer(new FileExplorer(m_dockExplorer))
,   m_loader(new AssimpLoader())
{
    m_ui->setupUi(this);
    
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;
    
    restoreGeometry(s.value(SETTINGS_GEOMETRY).toByteArray());
    restoreState(s.value(SETTINGS_STATE).toByteArray());
    
    restoreViews(s);
    initializeExplorer();
};

void Viewer::initializeExplorer()
{
    m_dockNavigator->setObjectName("fileNavigator");
    m_dockExplorer->setObjectName("fileExplorer");
    this->initializeDockWidgets(m_dockNavigator, m_navigator, Qt::LeftDockWidgetArea);
    this->initializeDockWidgets(m_dockExplorer, m_explorer, Qt::BottomDockWidgetArea);

    m_explorer->setAllLoadableTypes(m_loader->allLoadableTypes());
        
    QObject::connect(
        m_navigator, SIGNAL(clickedDirectory(const QString &)),
        m_explorer, SLOT(setRoot(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedItem(const QString &)),
        this, SLOT(on_loadFile(const QString &)));

    QObject::connect(
        m_explorer, SIGNAL(activatedDir(const QString &)),
        m_navigator, SLOT(on_activatedDir(const QString &)));

    QObject::connect(
        m_ui->openFileDialogAction, SIGNAL(changed()),
        this, SLOT(on_openFileDialogAction_triggered()));

    m_explorer->emitActivatedItem(m_explorer->model()->index(QDir::currentPath()));
}

void Viewer::initializeDockWidgets(QDockWidget * dockWidget, QWidget * widget, Qt::DockWidgetArea area)
{
    dockWidget->setWidget(widget);
    this->addDockWidget(area, dockWidget);
    
#ifdef __APPLE__
    /** 
     THIS IS A BUG WORKAROUND
     The bug lies somewhere in Canvas::Canvas().
     When called in Viewer::createQtContext(), the widgets get messed up.
    **/
    static int count = 0;
    dockWidget->setFloating(true);
    dockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
    
    dockWidget->move(QPoint(20, 40 + count++ * (dockWidget->height() + 35)));
#endif
}

#ifdef WIN32
const HGLRC Viewer::currentContextHandle()
{
    return  wglGetCurrentContext();
#elif __APPLE__
void * Viewer::currentContextHandle()
{
    return nullptr;
#else
const GLXContext Viewer::currentContextHandle()
{
    return glXGetCurrentContext();
#endif
}

#ifdef WIN32
const HGLRC Viewer::createQtContext(const GLFormat & format)
#elif __APPLE__
void * Viewer::createQtContext(const GLFormat & format)
#else
const GLXContext Viewer::createQtContext(const GLFormat & format)
#endif
{
    m_qtCanvas = new Canvas(format, this);
    setCentralWidget(m_qtCanvas);

    QGLContext * qContext(const_cast<QGLContext *>(m_qtCanvas->context()));

    if(!qContext)
        qFatal("Creating QtGL-Context failed.");

    qContext->makeCurrent();

    if(QGLContext::currentContext() != qContext)
        qFatal("Making QtGL-Context current failed.");

#ifdef WIN32
    const HGLRC qtContextHandle = currentContextHandle();
#elif __APPLE__
    void * qtContextHandle = currentContextHandle();
#else
    const GLXContext qtContextHandle = currentContextHandle();
#endif

    // NOTE: might work even if no context was returned. 
    // This just double checks...

    if(nullptr == qtContextHandle)
        qWarning("Acquiring QtGL-Context handle failed.");

    // canvas verifies the context itself.

    qContext->doneCurrent();

    return qtContextHandle;
}

void Viewer::initialize(const GLFormat & format)
{
    if(!QGLFormat::hasOpenGL())
        qFatal("OpenGL not supported.");

    createQtContext(format);
}

Viewer::~Viewer()
{
    QSettings s;
    s.setValue(SETTINGS_GEOMETRY, saveGeometry());
    s.setValue(SETTINGS_STATE, saveState());

    delete m_qtCanvas;

    delete m_dockNavigator;
    delete m_dockExplorer;
    delete m_loader;
}

void Viewer::setPainter(AbstractScenePainter * painter)
{
    if(!m_qtCanvas)
        return;

    m_qtCanvas->setPainter(painter);
}

AbstractScenePainter * Viewer::painter()
{
    if(!m_qtCanvas)
        return nullptr;

    return m_qtCanvas->painter();
}


void Viewer::on_captureAsImageAction_triggered()
{
    m_dockNavigator->show();
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this);
}

void Viewer::on_captureAsImageAdvancedAction_triggered()
{
    assert(m_qtCanvas);
    CanvasExporter::save(*m_qtCanvas, this, true);
}

void Viewer::on_reloadAllShadersAction_triggered()
{
    FileAssociatedShader::reloadAll();
}

void Viewer::on_openFileDialogAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 
        QDir::homePath(), m_loader->namedLoadableTypes().join(";;"), 
        0, QFileDialog::HideNameFilterDetails);
    if (fileName.isEmpty())
        return;
    
    on_loadFile(fileName);
}
    
void Viewer::on_quitAction_triggered()
{
    QApplication::quit();
}

void Viewer::on_loadFile(const QString & path)
{
    Group * scene = m_loader->importFromFile(path);
    if (!scene)
        QMessageBox::critical(this, "Loading failed", "The loader was not able to load from \n" + path);
    else {
        this->painter()->assignScene(scene);
        this->m_qtCanvas->navigation()->sceneChanged(scene);
        this->m_qtCanvas->update();
    }
}

void Viewer::on_toggleNavigator_triggered()
{
    bool visible = m_dockNavigator->isVisible();
    m_dockNavigator->setVisible(!visible);
}

void Viewer::on_toggleExplorer_triggered()
{
    bool visible = m_dockExplorer->isVisible();
    m_dockExplorer->setVisible(!visible);
}

void Viewer::on_phongShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('p');
    m_qtCanvas->repaint();
}

void Viewer::on_gouraudShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('g');
    m_qtCanvas->repaint();
}

void Viewer::on_flatShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('f');
    m_qtCanvas->repaint();
}

void Viewer::on_goochShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('o');
    m_qtCanvas->repaint();
}

void Viewer::on_wireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('w');
    m_qtCanvas->repaint();
}

void Viewer::on_solidWireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('s');
    m_qtCanvas->repaint();
}

void Viewer::on_primitiveWireframeShadingAction_triggered()
{
    m_qtCanvas->painter()->setShading('r');
    m_qtCanvas->repaint();
}

void Viewer::on_normalsAction_triggered()
{
    m_qtCanvas->painter()->setShading('n');
    m_qtCanvas->repaint();
}


void Viewer::setNavigation(AbstractNavigation * navigation)
{
    m_qtCanvas->setNavigation(navigation);
}

AbstractNavigation * Viewer::navigation() {
    if(!m_qtCanvas)
        return nullptr;
    return m_qtCanvas->navigation();
}

void Viewer::setCamera(Camera * camera )
{
    m_camera = camera;
}

Camera * Viewer::camera()
{
    return m_camera;
}

void Viewer::keyPressEvent(QKeyEvent * event)
{
    if(!event->isAutoRepeat()) {
        navigation()->keyPressEvent(event);
    }
}

void Viewer::keyReleaseEvent( QKeyEvent *event )
{
    if(!event->isAutoRepeat()) {
        navigation()->keyReleaseEvent(event);
    }
}

void Viewer::on_flightManipulatorAction_triggered() {
    setNavigation(new FlightNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->flightManipulatorAction->setChecked(true);
    qDebug("Flight navigation, use WASD and arrow keys");
}
    
void Viewer::on_trackballManipulatorAction_triggered() {
    setNavigation(new ArcballNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->trackballManipulatorAction->setChecked(true);
    qDebug("Arcball navigation, use left and right mouse buttons");
}
    
void Viewer::on_fpsManipulatorAction_triggered() {
    setNavigation(new FpsNavigation(m_camera));
    uncheckManipulatorActions();
    m_ui->fpsManipulatorAction->setChecked(true);
    qDebug("FPS Navigation, use mouse and WASD");
}

void Viewer::uncheckManipulatorActions() {
    m_ui->flightManipulatorAction->setChecked(false);
    m_ui->fpsManipulatorAction->setChecked(false);
    m_ui->trackballManipulatorAction->setChecked(false);
}


// helper to restore mat4
glm::mat4 string2mat(QString s) {
    glm::mat4 mat;
    QStringList list = s.split(';');
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[j][i] = list.at(j + 4*i).toFloat();
        }
    }
    return mat;
}

// helper to save mat4
QString mat2string(glm::mat4 mat) {
    QString s("");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            s += QString::number(mat[j][i]);
            s += ';';
        }
    }
    return s;
}

void Viewer::restoreViews( QSettings &s ) {
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    for (int i = 0; i < m_saved_views.size(); i++)
    {
        QString name = "view_" + QString::number(i+1);
        QString str = s.value(name, "").value<QString>();
        if (str.size() > 0) {
            m_saved_views[i] = string2mat(str);
        } else {
            m_saved_views[i] = glm::mat4(0);
        }

    }
    s.endGroup();
}

void Viewer::saveView(int i) {
    m_saved_views[i] = navigation()->viewMatrix();
    QSettings s;
    s.beginGroup(SETTINGS_SAVED_VIEWS);
    s.setValue("view_" + QString::number(i+1), mat2string(navigation()->viewMatrix()));
    s.endGroup();
}

void Viewer::loadView(int i) {
    if (m_saved_views[i] == glm::mat4(0)) {
        navigation()->loadView(navigation()->defaultView());
    } else {
        navigation()->loadView(m_saved_views[i]);
    }
}


void Viewer::on_actionFrontView_triggered() {
    navigation()->loadView(navigation()->frontview());
}
void Viewer::on_actionLeftView_triggered() {
    navigation()->loadView(navigation()->leftview());
}
void Viewer::on_actionBackView_triggered() {
    navigation()->loadView(navigation()->backview());
}
void Viewer::on_actionRightView_triggered() {
    navigation()->loadView(navigation()->rightview());
}
void Viewer::on_actionTopView_triggered() {
    navigation()->loadView(navigation()->topview());
}
void Viewer::on_actionBottomView_triggered() {
    navigation()->loadView(navigation()->bottomview());
}
void Viewer::on_actionTopRightView_triggered() {
    navigation()->loadView(navigation()->topRightView());
}


void Viewer::on_actionLoad_1_triggered() { loadView(0); }
void Viewer::on_actionLoad_2_triggered() { loadView(1); }
void Viewer::on_actionLoad_3_triggered() { loadView(2); }
void Viewer::on_actionLoad_4_triggered() { loadView(3); }

void Viewer::on_actionSave_1_triggered() { saveView(0); }
void Viewer::on_actionSave_2_triggered() { saveView(1); }
void Viewer::on_actionSave_3_triggered() { saveView(2); }
void Viewer::on_actionSave_4_triggered() { saveView(3); }

