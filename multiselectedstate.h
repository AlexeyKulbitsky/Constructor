#ifndef MULTISELECTEDSTATE_H
#define MULTISELECTEDSTATE_H

#include "state.h"

class StateManager;
#include "statemanager.h"

#include "compositeroad.h"

struct ElementInfo
{
    int elementIndex;
    int groupIndex;
    int controlIndex;
};

class MultiSelectedState: public State
{
    Q_OBJECT
public:
    MultiSelectedState();
    MultiSelectedState(StateManager* manager, Model* model, Scene2D* scene, QFormLayout* properties);

    virtual ~MultiSelectedState();
    void clearProperties(QFormLayout* layout);

private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;
    QFormLayout* properties;
    std::list<RoadElement*>::iterator it;
    std::list<ElementInfo> selectedElements;

    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    bool controlIsSelected;
    bool figureIsSelected;

    int elementIndex;
    int groupIndex;
    int controlIndex;

    QMenu* contextMenu;

    QAction* combineAction;

    int keyboardKey;

private:
    bool tryToSelectFigures(QPoint mp);

    void createActions();
    void createMenu();
    // State interface
public:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void keyPressEvent(QKeyEvent *pe);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

    void addSelectedElementToTheGroup(int elementIndex, int groupIndex);
    void clearSelectedElementsGroup();

    // State interface
public:
    virtual QString getName();

    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *pe);

public slots:
    void combineElements();
};

#endif // MULTISELECTEDSTATE_H
