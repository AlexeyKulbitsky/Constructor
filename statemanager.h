#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include <QWidget>
#include <QFormLayout>

#include "state.h"

class DefaultState;
#include "defaultstate.h"

class SelectedState;
#include "selectedstate.h"

//class MultiSelectedState;
//#include "multiselectedstate.h"

class RulerState;
#include "rulerstate.h"

class LineBuilderState;
#include "linebuilderstate.h"

class RoadBuilderState;
#include "roadbuilderstate.h"
//class Scene2D;////////////
//class Model;////////////////
#include "scene2d.h"

#include "_3dsfilemanager.h"
#include "objfilemanager.h"
#include "jsonfilemanager.h"
//#include "stepdialog.h"
#include "cameraview.h"

class StateManager : public QObject
{
    Q_OBJECT
public:
    explicit StateManager(QObject *parent = 0);
    StateManager(Model* model, Scene2D* scene, QVBoxLayout* properties);
    StateManager(Model* model, Scene2D* scene, QVBoxLayout* properties, CameraView* cam);
    ~StateManager();
private:

    Model* model;
    Scene2D* scene;


    bool rulerActive;
    static bool log;
public:

    DefaultState* defaultState;
    SelectedState* selectedState;
    //MultiSelectedState* multiSelectedState;
    RulerState* rulerState;
    LineBuilderState* lineBuilderState;
    RoadBuilderState* roadBuilderState;

    StepDialog* stepDialog;
    StepDialog* stepDialogs[10];
    _3DsFileManager* fileManager3DS;
    OBJFileManager* fileManagerOBJ;
    JSONFileManager* fileManagerJSON;
    State* currentState;
    CameraView* camera;

public:
    void setState(State* state);

    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
    void keyReleaseEvent(QKeyEvent *pe);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent * event);

    void contextMenuEvent(QContextMenuEvent *pe);

    void copy();
    void paste();
    void cut();
    void del();

    void saveToPresets();
    void processTemplate();
    static void setLogging(bool status);
    static bool getLogging() { return log; }

signals:
    void templtateAdded();

public slots:
    void setRulerActive(bool status);
};

#endif // STATEMANAGER_H
