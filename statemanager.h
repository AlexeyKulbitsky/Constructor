#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include <QWidget>

#include "state.h"

class DefaultState;
#include "defaultstate.h"

class SelectedState;
#include "selectedstate.h"

class MultiSelectedState;
#include "multiselectedstate.h"

class RulerState;
#include "rulerstate.h"

class LineBuilderState;
#include "linebuilderstate.h"

class RoadBuilderState;
#include "roadbuilderstate.h"

#include "scene2d.h"

class StateManager : public QObject
{
    Q_OBJECT
public:
    explicit StateManager(QObject *parent = 0);
    StateManager(Model* model, Scene2D* scene, QFormLayout* properties);

private:

    Model* model;
    Scene2D* scene;

    State* currentState;

    bool rulerActive;

public:
    DefaultState* defaultState;
    SelectedState* selectedState;
    MultiSelectedState* multiSelectedState;
    RulerState* rulerState;
    LineBuilderState* lineBuilderState;
    RoadBuilderState* roadBuilderState;

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

signals:

public slots:
    void setRulerActive(bool status);
};

#endif // STATEMANAGER_H
