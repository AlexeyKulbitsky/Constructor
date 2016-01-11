#include "statemanager.h"

bool StateManager::log = true;

StateManager::StateManager(QObject *parent) : QObject(parent)
{

    defaultState = NULL;
    selectedState = NULL;
    //multiSelectedState = NULL;
    currentState = NULL;
    rulerState = NULL;
    lineBuilderState = NULL;
    roadBuilderState = NULL;
    //stepDialog = NULL;
    scene = NULL;
    model = NULL;
}

StateManager::StateManager(Model *model, Scene2D* scene, QFormLayout *properties)
{    
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::StateManager(Model *model, Scene2D* scene, QFormLayout *properties)\n";
    if (model == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Model* model = NULL, cannot create StateManager, program terminates");
        if (log)
        Logger::getLogger()->errorLog() << "Model* model = NULL, cannot create StateManager, program terminates\n";
        QApplication::exit(0);
    }
    this->model = model;
    if (scene == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Scene2D* scene = NULL, cannot create StateManager, program terminates");
        if (log)
        Logger::getLogger()->errorLog() << "Scene2D* scene = NULL, cannot create StateManager, program terminates\n";
        QApplication::exit(0);
    }
    this->scene = scene;
    if (properties == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "QFormLayout *properties = NULL, cannot create StateManager, program terminates");
        if (log)
        Logger::getLogger()->errorLog() << "QFormLayout *properties, cannot create StateManager, program terminates\n";
        QApplication::exit(0);
    }
    this->rulerActive = false;
    defaultState = new DefaultState(this, model, scene, properties);
    selectedState = new SelectedState(this, model, scene, properties);
    //multiSelectedState = new MultiSelectedState(this, model, scene, properties);
    rulerState = new RulerState(this, model, scene);
    connect(rulerState, SIGNAL(rulerStatusChanged(bool)), this, SLOT(setRulerActive(bool)));
    lineBuilderState = new LineBuilderState(this, model, scene, properties);
    roadBuilderState = new RoadBuilderState(this, model, scene, properties);
    stepDialog = new StepDialog(scene);
    for (int i = 0; i < 10; ++i)
    {
        stepDialogs[i] = new StepDialog(scene);
    }
    fileManager3DS = new _3DsFileManager();
    fileManagerOBJ = new OBJFileManager(model);
    fileManagerJSON = new JSONFileManager(model);
    currentState = defaultState;
}

StateManager::StateManager(Model *model, Scene2D *scene, QFormLayout *properties, CameraView *cam)
    : StateManager(model, scene, properties)
{
    camera = cam;
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
    if (rulerState)
        delete rulerState;
    rulerState = NULL;
    if (fileManagerOBJ)
        delete fileManagerOBJ;
    fileManagerOBJ = NULL;
    if (fileManager3DS)
        delete fileManager3DS;
    fileManager3DS = NULL;

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


    if (stepDialog)
        delete stepDialog;
    stepDialog = NULL;


    */
}


void StateManager::setState(State *state)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::setState(State *state)\n";
    if (state != NULL)
        currentState = state;
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::setState(State *state), state = NULL, currentState = NULL", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::setState(State *state), state = NULL, currentState = NULL\n";
        QApplication::exit(0);
    }
}


void StateManager::mousePressEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::mousePressEvent(QMouseEvent *pe)\n";
    if (currentState)
        currentState->mousePressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mousePressEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::mousePressEvent(QMouseEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::mouseMoveEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::mouseMoveEvent(QMouseEvent *pe)\n";
    if (currentState)
        currentState->mouseMoveEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mouseMoveEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::mouseMoveEvent(QMouseEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::mouseReleaseEvent(QMouseEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::mouseReleaseEvent(QMouseEvent *pe)\n";
    if (currentState)
        currentState->mouseReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::mouseReleaseEvent(QMouseEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::mouseReleaseEvent(QMouseEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::wheelEvent(QWheelEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::wheelEvent(QWheelEvent *pe)\n";
    if (currentState)
        currentState->wheelEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::wheelEvent(QWheelEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::wheelEvent(QWheelEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::keyPressEvent(QKeyEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::keyPressEvent(QKeyEvent *pe)\n";
    if (currentState)
        currentState->keyPressEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::keyPressEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::keyPressEvent(QKeyEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::keyReleaseEvent(QKeyEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::keyReleaseEvent(QKeyEvent *pe)\n";
    if (currentState)
        currentState->keyReleaseEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::keyReleaseEvent(QKeyEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::keyReleaseEvent(QKeyEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::dragEnterEvent(QDragEnterEvent *event)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::dragEnterEvent(QDragEnterEvent *event)\n";
    if (currentState)
        currentState->dragEnterEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::dragEnterEvent(QDragEnterEvent *event) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::dragEnterEvent(QDragEnterEvent *event) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::dropEvent(QDropEvent *event)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::dropEvent(QDropEvent *event)\n";
    if (currentState)
        currentState->dropEvent(event);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::dropEvent(QDropEvent *event) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::dropEvent(QDropEvent *event) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::contextMenuEvent(QContextMenuEvent *pe)
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::contextMenuEvent(QContextMenuEvent *pe)\n";
    if (currentState)
        currentState->contextMenuEvent(pe);
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::contextMenuEvent(QContextMenuEvent *pe) stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::contextMenuEvent(QContextMenuEvent *pe) stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::copy()
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::copy()\n";
    if (currentState)
        currentState->copy();
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::copy() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::copy() stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::paste()
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::paste()\n";
    if (currentState)
        currentState->paste();
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::paste() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::paste() stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::cut()
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::cut()\n";
    if (currentState)
        currentState->cut();
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::cut() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::cut() stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::del()
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::del()\n";
    if (currentState)
        currentState->del();
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::del() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::del() stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::saveToPresets()
{
    if (log)
    Logger::getLogger()->infoLog() << "StateManager::saveToPresets()\n";
    if (currentState)
        currentState->saveToPresets();
    else
    {
        QMessageBox::critical(0, "Ошибка", "StateManager::currentState = NULL,\n StateManager::saveToPresets() stopped", QMessageBox::Yes | QMessageBox::Default);
        if (log)
        Logger::getLogger()->errorLog() << "StateManager::currentState = NULL, StateManager::saveToPresets() stopped\n";
        QApplication::exit(0);
    }
}

void StateManager::processTemplate()
{
    emit templtateAdded();
}

void StateManager::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "StateManager::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

void StateManager::setRulerActive(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "StateManager::setRulerActive(bool status)\n"
                                       << " status = " << status << "\n";

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

