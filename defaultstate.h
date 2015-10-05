#ifndef DEFAULTSTATE_H
#define DEFAULTSTATE_H

class Scene2D;
#include "scene2d.h"
#include "state.h"
class StateManager;
#include "statemanager.h"



class DefaultState : public State
{
    Q_OBJECT
public:
    DefaultState();
    DefaultState(StateManager* manager, Model* model, Scene2D* scene, QFormLayout* properties);

    virtual ~DefaultState();
    void clearProperties(QFormLayout* layout);
private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;
    QFormLayout* properties;

    QPoint ptrMousePosition;
    QPoint rectPoint1, rectPoint2;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    bool rectSelection;
    int selectedElementsCount;
    int selectedGroup;
    int selectedIndex;

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


private:
    bool tryToSelectFigures(QPoint mp, bool withResult);
    bool tryToSelectFigures(QPoint mp1, QPoint mp2, bool withResult);



    // State interface
public:
    virtual QString getName();
    void drawRect(QPoint p1, QPoint p2);
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    void getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *pe);
};

#endif // DEFAULTSTATE_H
