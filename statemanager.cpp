#include "statemanager.h"

StateManager::StateManager(QObject *parent) : QObject(parent)
{

    defaultState = NULL;
    //selectedState = NULL;
    //multiSelectedState = NULL;
    currentState = NULL;
    //rulerState = NULL;
    //lineBuilderState = NULL;
    //roadBuilderState = NULL;
    //stepDialog = NULL;
    scene = NULL;
    model = NULL;
}

StateManager::StateManager(Model *model, Scene2D* scene, QFormLayout *properties)
{    
    Logger::getLogger()->writeLog("Creating StateManager");
    if (model == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Model* model = NULL, cannot create StateManager, program terminates");
        Logger::getLogger()->writeLog("Model* model = NULL, cannot create StateManager, program terminates");
        QApplication::exit(0);
    }
    this->model = model;
    if (scene == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D* scene = NULL, cannot create StateManager, program terminates");
        Logger::getLogger()->writeLog("Scene2D* scene = NULL, cannot create StateManager, program terminates");
        QApplication::exit(0);
    }
    this->scene = scene;
    if (properties == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "QFormLayout *properties = NULL, cannot create StateManager, program terminates");
        Logger::getLogger()->writeLog("QFormLayout *properties, cannot create StateManager, program terminates");
        QApplication::exit(0);
    }
    this->rulerActive = false;
    defaultState = new DefaultState(this, model, scene, properties);
    selectedState = new SelectedState(this, model, scene, properties);
    //multiSelectedState = new MultiSelectedState(this, model, scene, properties);
    //rulerState = new RulerState(this, model, scene);
    lineBuilderState = new LineBuilderState(this, model, scene, properties);
    roadBuilderState = new RoadBuilderState(this, model, scene, properties);
    stepDialog = new StepDialog(scene);
    for (int i = 0; i < 10; ++i)
    {
        stepDialogs[i] = new StepDialog(scene);
    }
    //fileManager3DS = new _3DsFileManager();
    //fileManagerOBJ = new OBJFileManager(model);
    //connect(rulerState, SIGNAL(rulerStatusChanged(bool)), this, SLOT(setRulerActive(bool)));
    currentState = defaultState;
}

StateManager::~StateManager()
{
    model = NULL;
    scene = NULL;
    currentState = NULL;

    if (defaultState)
        delete defaultState;
    defaultState = NULL;

    if (selectedState)
        delete selectedState;
    selectedState = NULL;
    if (lineBuilderState)
        delete lineBuilderState;
    lineBuilderState = NULL;
    if (roadBuilderState)
        delete roadBuilderState;
    roadBuilderState = NULL;
    /*
    for (int i = 0; i < 10; ++i)
    {
        delete stepDialogs[i];
        stepDialogs[i] = NULL;
    }
    if (stepDialog)
        delete stepDialog;
    stepDialog = NULL;




    if (multiSelectedState)
        delete multiSelectedState;
    multiSelectedState = NULL;
    if (rulerState)
        delete rulerState;
    rulerState = NULL;

    if (stepDialog)
        delete stepDialog;
    stepDialog = NULL;
    if (fileManager3DS)
        delete fileManager3DS;
    fileManager3DS = NULL;
    if (fileManagerOBJ)
        delete fileManagerOBJ;
    fileManagerOBJ = NULL;
    */
}


void StateManager::setState(State *state)
{
    Logger::getLogger()->writeLog("StateManager::setState(State *state)");
    if (state != NULL)
        currentState = state;
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::setState(State *state), state = NULL, currentState = NULL", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::setState(State *state), state = NULL, currentState = NULL");
    }
}


void StateManager::mousePressEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::mousePressEvent(QMouseEvent *pe)");
    if (currentState)
        currentState->mousePressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mousePressEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::mousePressEvent(QMouseEvent *pe) stopped");
    }
}

void StateManager::mouseMoveEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::mouseMoveEvent(QMouseEvent *pe)");
    if (currentState)
        currentState->mouseMoveEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mouseMoveEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::mouseMoveEvent(QMouseEvent *pe) stopped");
    }
}

void StateManager::mouseReleaseEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::mouseReleaseEvent(QMouseEvent *pe)");
    if (currentState)
        currentState->mouseReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mouseReleaseEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::mouseReleaseEvent(QMouseEvent *pe) stopped");
    }
}

void StateManager::wheelEvent(QWheelEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::wheelEvent(QWheelEvent *pe)");
    if (currentState)
        currentState->wheelEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::wheelEvent(QWheelEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::wheelEvent(QWheelEvent *pe) stopped");
    }
}

void StateManager::keyPressEvent(QKeyEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::keyPressEvent(QKeyEvent *pe)");
    if (currentState)
        currentState->keyPressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::keyPressEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::keyPressEvent(QKeyEvent *pe) stopped");
    }
}

void StateManager::keyReleaseEvent(QKeyEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::keyReleaseEvent(QKeyEvent *pe)");
    if (currentState)
        currentState->keyReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::keyReleaseEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::keyReleaseEvent(QKeyEvent *pe) stopped");
    }
}

void StateManager::dragEnterEvent(QDragEnterEvent *event)
{
    Logger::getLogger()->writeLog("StateManager::dragEnterEvent(QDragEnterEvent *event)");
    if (currentState)
        currentState->dragEnterEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::dragEnterEvent(QDragEnterEvent *event) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::dragEnterEvent(QDragEnterEvent *event) stopped");
    }
}

void StateManager::dropEvent(QDropEvent *event)
{
    Logger::getLogger()->writeLog("StateManager::dropEvent(QDropEvent *event)");
    if (currentState)
        currentState->dropEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::dropEvent(QDropEvent *event) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::dropEvent(QDropEvent *event) stopped");
    }
}

void StateManager::contextMenuEvent(QContextMenuEvent *pe)
{
    Logger::getLogger()->writeLog("StateManager::contextMenuEvent(QContextMenuEvent *pe)");
    if (currentState)
        currentState->contextMenuEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::contextMenuEvent(QContextMenuEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        Logger::getLogger()->writeLog("StateManager::currentState = NULL, StateManager::contextMenuEvent(QContextMenuEvent *pe) stopped");
    }
}

void StateManager::setRulerActive(bool status)
{
    /*
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
    */
}

