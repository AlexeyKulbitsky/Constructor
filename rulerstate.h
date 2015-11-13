#ifndef RULERSTATE_H
#define RULERSTATE_H

#include "scene2d.h"
#include "state.h"
#include "ruler.h"
class StateManager;
#include "statemanager.h"

class RulerState: public State
{
    Q_OBJECT
public:
    RulerState();
    RulerState(StateManager* manager, Model* model, Scene2D* scene);

    virtual ~RulerState();

private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;

    Ruler* ruler;

    RoadElement* selectedFigure;
    int indexOfControl;

    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;

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
    virtual void contextMenuEvent(QContextMenuEvent *pe);

    // State interface
public:
    virtual QString getName();
    void getGlobalCoord(double wx, double wy, double wz, double &x, double &y, double &z);
    void clear();
    static bool getLogging();
    static void setLogging(bool status);
signals:
    void rulerStatusChanged(bool status);

    // State interface
public:
    virtual void copy();
    virtual void paste();
    virtual void cut();
    virtual void del();
    virtual void clearProperties(QFormLayout *layout);
    bool tryToSelectFigures(QPoint mp, RoadElement*& element);
    bool tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement* element, int& index);

    // State interface
public:
    virtual void saveToPresets();
};

#endif // RULERSTATE_H
