#ifndef SELECTEDSTATE_H
#define SELECTEDSTATE_H

#include "state.h"

class StateManager;
#include "statemanager.h"

class SelectedState: public State
{
    Q_OBJECT
public:
    SelectedState();
    SelectedState(StateManager* manager, Model* model, Scene2D* scene, QFormLayout* properties);

    std::list<RoadElement*> selectedElements;

    virtual ~SelectedState();
    void clearProperties(QFormLayout *layout);
private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;
    QFormLayout* properties;
    std::list<RoadElement*>::iterator it;
    RoadElement* selectedElement;

    QMenu* contextMenu;

    QAction* fixAction;
    QAction* combineAction;
    QAction* breakAction;

    int keyboardKey;

    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    bool controlIsSelected;
    bool figureIsSelected;

    int elementIndex;
    int groupIndex;
    int controlIndex;
    std::list<RoadElement*>::iterator elementIterator;
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
    void setSelectedElement(int elementIndex, int groupIndex);
    void setSelectedElement(RoadElement* element);

private:
    bool tryToSelectFigures(QPoint mp, std::list<RoadElement *> &elements);
    bool tryToSelectFigures(QPoint mp);
    bool tryToSelectAllFigures(QPoint mp);
    bool tryToSelectControlsInSelectedFigure(QPoint mp);
    bool tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement* element, int& index);
    bool tryToSelectFigures(QPoint mp, RoadElement*& element);
    void getWindowCoord(double x, double y, double z, double  &wx, double  &wy, double  &wz);

    // State interface
public:
    virtual QString getName();

    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *pe);
    void createMenu();
    void createActions();

public slots:
    void combineElements();
    void breakElements();
    void clear();
};

#endif // SELECTEDSTATE_H
