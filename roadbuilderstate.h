#ifndef ROADBUILDERSTATE_H
#define ROADBUILDERSTATE_H

class Scene2D;
#include "scene2d.h"
#include "state.h"
class StateManager;
#include "statemanager.h"

//#include "roadbroken.h"

class RoadBuilderState: public State
{
    Q_OBJECT
public:
    RoadBuilderState();
    RoadBuilderState(StateManager* manager, Model* model, Scene2D* scene, QFormLayout* properties);

    virtual ~RoadBuilderState();

private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;
    QFormLayout* properties;

    RoadBroken* roadBroken;

    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    bool controlIsSelected;

    float oldX, oldY, newX, newY;
    ResizeByControlCommand* resizeByControlCommand;
    int key;
    GLint controlIndex;
    int layer;
    QString name;
    int groupIndex, elementIndex;
    static bool log;
    // State interface
public:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void keyPressEvent(QKeyEvent *pe);
    virtual void keyReleaseEvent(QKeyEvent *pe);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

    bool tryToSelectControlsInSelectedFigure(QPoint mp);
    bool tryToSelectFigures(QPoint mp);
    void setName(QString name);
    void setLayer(int layer);
    void setGroupIndex(int index);
    void setElementIndex(int index);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
    // State interface
public:
    virtual QString getName();
    void clearProperties(QFormLayout* layout);
    virtual void clear();
    void setRoad(RoadBroken *roadBroken);
    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *pe);

    // State interface
public:
    virtual void copy();
    virtual void paste();

    // State interface
public:
    virtual void cut();
    virtual void del();
};

#endif // ROADBUILDERSTATE_H
