#include "statemanager.h"

StateManager::StateManager(QObject *parent) : QObject(parent)
{
    defaultState = NULL;
    selectedState = NULL;
    multiSelectedState = NULL;
    currentState = NULL;
    rulerState = NULL;
    lineBuilderState = NULL;
    roadBuilderState = NULL;
    stepDialog = NULL;
    this->scene = NULL;
}

StateManager::StateManager(Model *model, Scene2D* scene, QFormLayout *properties)
{
    this->model = model;
    this->scene = scene;
    this->rulerActive = false;
    defaultState = new DefaultState(this, model, scene, properties);
    selectedState = new SelectedState(this, model, scene, properties);
    multiSelectedState = new MultiSelectedState(this, model, scene, properties);
    rulerState = new RulerState(this, model, scene);
    lineBuilderState = new LineBuilderState(this, model, scene, properties);
    roadBuilderState = new RoadBuilderState(this, model, scene, properties);
    stepDialog = new StepDialog();
    fileManager3DS = new _3DsFileManager();
    fileManagerOBJ = new OBJFileManager(model);
    connect(rulerState, SIGNAL(rulerStatusChanged(bool)), this, SLOT(setRulerActive(bool)));
    currentState = defaultState;
    //currentState = lineBuilderState;
    //currentState = roadBuilderState;
}

void StateManager::setState(State *state)
{
    currentState = state;
}

void StateManager::mousePressEvent(QMouseEvent *pe)
{
    currentState->mousePressEvent(pe);
}

void StateManager::mouseMoveEvent(QMouseEvent *pe)
{
    currentState->mouseMoveEvent(pe);
}

void StateManager::mouseReleaseEvent(QMouseEvent *pe)
{
    currentState->mouseReleaseEvent(pe);
}

void StateManager::wheelEvent(QWheelEvent *pe)
{
    currentState->wheelEvent(pe);
}

void StateManager::keyPressEvent(QKeyEvent *pe)
{
    currentState->keyPressEvent(pe);
}

void StateManager::keyReleaseEvent(QKeyEvent *pe)
{
    currentState->keyReleaseEvent(pe);
}

void StateManager::dragEnterEvent(QDragEnterEvent *event)
{
    currentState->dragEnterEvent(event);
}

void StateManager::dropEvent(QDropEvent *event)
{
    currentState->dropEvent(event);
}

void StateManager::contextMenuEvent(QContextMenuEvent *pe)
{
    currentState->contextMenuEvent(pe);
}

void StateManager::setRulerActive(bool status)
{
    if (rulerActive == status)
        return;
    rulerActive = status;
    if (rulerActive)
    {
        scene->setMouseTracking(true);
        this->setState(rulerState);
    }
    else
    {
        rulerState->clear();
        this->setState(defaultState);
    }

    emit scene->rulerStatusChanged(status);
}

