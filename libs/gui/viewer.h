#pragma once


#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>  // for GLXContext
#undef None
#endif

#include <QMainWindow>

#include <memory>


#include <glm/glm.hpp>


#include <core/declspec.h>


class GLFormat;
class QSettings;

class Ui_Viewer;
class Canvas;
class AbstractNavigation;
class Camera;
class AbstractScenePainter;

class FileNavigator;
class FileExplorer;
class Viewer;
class AbstractModelLoader;
class Group;
class DataBlockRegistry;


class CGSEE_API Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(
        std::shared_ptr<DataBlockRegistry> registry = nullptr,
        QWidget * parent = nullptr
    ,   Qt::WindowFlags flags = nullptr);

    void restoreViews( QSettings &s );

    virtual ~Viewer();

    void initialize(const GLFormat & format);

   void setNavigation(AbstractNavigation * navigation);
    AbstractNavigation * navigation();
    
    void setPainter(AbstractScenePainter * painter);
    AbstractScenePainter * painter();

    void setCamera(Camera * camera);
    Camera * camera();
    
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent (QKeyEvent *event);

    void setFlightManipulator();
    void setTrackballManipulator();
    void setFpsManipulator();
    void uncheckManipulatorActions();


public slots:
    void on_flightManipulatorAction_triggered();
    void on_trackballManipulatorAction_triggered();
    void on_fpsManipulatorAction_triggered();

    void on_actionFrontView_triggered();
    void on_actionLeftView_triggered();
    void on_actionBackView_triggered();
    void on_actionRightView_triggered();
    void on_actionTopView_triggered();
    void on_actionBottomView_triggered();
    void on_actionTopRightView_triggered();

    void on_actionSave_1_triggered();   
    void on_actionSave_2_triggered();
    void on_actionSave_3_triggered();
    void on_actionSave_4_triggered();
    void on_actionLoad_1_triggered();
    void on_actionLoad_2_triggered();
    void on_actionLoad_3_triggered();
    void on_actionLoad_4_triggered();

protected slots:
    void on_captureAsImageAction_triggered();
    void on_captureAsImageAdvancedAction_triggered();

    void on_reloadAllShadersAction_triggered();
    void on_phongShadingAction_triggered();
    void on_gouraudShadingAction_triggered();
    void on_flatShadingAction_triggered();
    void on_goochShadingAction_triggered();
    void on_wireframeShadingAction_triggered();
    void on_solidWireframeShadingAction_triggered();
    void on_primitiveWireframeShadingAction_triggered();
    void on_normalsAction_triggered();

    void on_openFileDialogAction_triggered();
    void on_quitAction_triggered();

    void on_loadFile(const QString & path);
    
    void on_toggleNavigator_triggered();
    void on_toggleExplorer_triggered();
protected:

    void initializeExplorer();
    void initializeDockWidgets(QDockWidget * dockWidget,
        QWidget * widget, Qt::DockWidgetArea area);

#ifdef WIN32
    const HGLRC createQtContext(const GLFormat & format);
    static const HGLRC currentContextHandle();
#elif __APPLE__
    void * createQtContext(const GLFormat & format);
    static void * currentContextHandle();
#else
    const GLXContext createQtContext(const GLFormat & format);
    static const GLXContext currentContextHandle();
#endif

    void saveView(int i);
    void loadView(int i);


protected:
    const std::unique_ptr<Ui_Viewer> m_ui;

    Canvas * m_qtCanvas;
    Camera * m_camera;
    QVector<glm::mat4> m_saved_views;

    QDockWidget * m_dockNavigator;
    QDockWidget * m_dockExplorer;

    FileNavigator * m_navigator;
    FileExplorer * m_explorer;
    AbstractModelLoader * m_loader;
};
