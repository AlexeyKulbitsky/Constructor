#include "commands.h"
#include "scene2d.h"
#include "model.h"
#include "logger.h"
#include "statemanager.h"

// MoveCommand..................................................

bool MoveCommand::log = true;

MoveCommand::MoveCommand(RoadElement *element, float oldX, float oldY, float newX, float newY, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    if (log)
        Logger::getLogger()->infoLog() << "MoveCommand::MoveCommand(RoadElement *element, float dx, float dy, Scene2D *scene2D, QUndoCommand *parent)"
                                       << " element = " << element
                                       << " oldX = " << oldX
                                       << " oldY = " << oldY
                                       << " newX = " << newX
                                       << " newY = " << newY << "\n";
    if (element != NULL)
        this->element = element;
    this->oneElement = true;
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
    this->firstTime = true;
    this->scene2D = scene2D;
}

MoveCommand::MoveCommand(QList<RoadElement *> &selectedElements, float oldX, float oldY, float newX, float newY, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    for (int i = 0; i < selectedElements.size(); ++i)
        this->selectedElements << selectedElements[i];
    this->oneElement = false;
    this->oldX = oldX;
    this->oldY = oldY;
    this->newX = newX;
    this->newY = newY;
    this->firstTime = true;
    this->scene2D = scene2D;
}

MoveCommand::~MoveCommand()
{    
    if (log)
        Logger::getLogger()->infoLog() << "MoveCommand::~MoveCommand()\n";
    element = NULL;
    scene2D = NULL;
}

void MoveCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "MoveCommand::undo()\n";
    if (oneElement)
    {
        element->move(oldX - newX, oldY - newY);
    }
    else
    {
        for (int i = 0; i < selectedElements.size(); ++i)
            selectedElements[i]->move(oldX - newX, oldY - newY);
    }
    scene2D->updateGL();
}

void MoveCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "MoveCommand::redo()\n";
    if (firstTime)
    {
        firstTime = false;
    }
    else
    {
        if (oneElement)
        {
            element->move(newX - oldX, newY - oldY);
        }
        else
        {
            for (int i = 0; i < selectedElements.size(); ++i)
                selectedElements[i]->move(newX - oldX, newY - oldY);
        }
        scene2D->updateGL();
    }
}

void MoveCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "MoveCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}
// MoveCommand.......................................................


// ResizeByControlCommand............................................

bool ResizeByControlCommand::log = true;

ResizeByControlCommand::ResizeByControlCommand(RoadElement* element, int index, Scene2D* scene2D, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    if (log)
        Logger::getLogger()->infoLog() << "ResizeByControlCommand::ResizeByControlCommand(RoadElement* element, int index, float x, float y, float dx, float dy, Scene2D* scene2D, QUndoCommand *parent)"
                                       << " element = " << element
                                       << " index = " << index << "\n";
    this->element= element;
    this->firstTime = true;
    this->index = index;
    this->scene2D = scene2D;
    for (int i = 0; i < element->getCoordOfControl(index).size(); ++i)
    {
        vec3 temp = element->getCoordOfControl(index)[i];
        oldPosition.push_back(temp);
    }
}

ResizeByControlCommand::~ResizeByControlCommand()
{
    if (log)
        Logger::getLogger()->infoLog() << "ResizeByControlCommand::~ResizeByControlCommand()\n";
    element = NULL;
    scene2D = NULL;
}

void ResizeByControlCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "ResizeByControlCommand::undo()\n";
    element->setCoordForControl(index, oldPosition);
    scene2D->updateGL();
}

void ResizeByControlCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "ResizeByControlCommand::undo()\n";
    if (firstTime)
    {
        firstTime = false;
    }
    else
    {
        element->setCoordForControl(index, newPosition);
        scene2D->updateGL();
    }

}

void ResizeByControlCommand::setNewPosition()
{
    if (log)
        Logger::getLogger()->infoLog() << "ResizeByControlCommand::setNewPosition()\n";
    for (int i = 0; i < element->getCoordOfControl(index).size(); ++i)
    {
        vec3 temp = element->getCoordOfControl(index)[i];
        newPosition.push_back(temp);
    }
}

void ResizeByControlCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "ResizeByControlCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}
// ResizeByControlCommand............................................



// DeleteCommand............................................

bool DeleteCommand::log = true;

DeleteCommand::DeleteCommand(RoadElement *element, Model *model, StateManager* stateManager, QFormLayout *layout, int groupIndex, int elementIndex, Scene2D* scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    if (log)
        Logger::getLogger()->infoLog() << "DeleteCommand::DeleteCommand(RoadElement *element, Model *model, int groupIndex, int elementIndex, Scene2D* scene2D, QUndoCommand *parent)"
                                       << " element = " << element
                                       << " groupIndex = " << groupIndex
                                       << " elementIndex = " << elementIndex << "\n";
    this->element = element;
    this->oneElement = true;
    this->isInModel = true;
    this->model = model;
    this->stateManager = stateManager;
    this->layout = layout;
    this->groupIndex = groupIndex;
    this->elementIndex = elementIndex;
    this->scene2D = scene2D;
}

DeleteCommand::DeleteCommand(QList<RoadElement *> &elements, Model *model, StateManager* stateManager, QFormLayout *layout, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    for (int i = 0; i < elements.size(); ++i)
    {
        this->elements.push_back(elements[i]);
    }
    this->oneElement = false;
    this->isInModel = true;
    this->model = model;
    this->stateManager = stateManager;
    this->layout = layout;
    this->scene2D = scene2D;
}

DeleteCommand::~DeleteCommand()
{
    if (log)
        Logger::getLogger()->infoLog() << "DeleteCommand::~DeleteCommand()\n";

    if (oneElement)
    {
        if (!isInModel)
            delete element;
    }
    else
    {
        if (!isInModel)
        {
            for (int i = 0; i < elements.size(); ++i)
            {
                delete elements[i];
                elements[i] = NULL;
            }
        }
    }
    element = NULL;
    model = NULL;
    scene2D = NULL;
    stateManager = NULL;
    layout = NULL;
}

void DeleteCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "DeleteCommand::undo()\n";
    if (oneElement)
    {
        model->getGroup(groupIndex).insert(elementIndex, element);

        element->setSelectedStatus(true);
        int selectedGroup = groupIndex;
        int selectedIndex = elementIndex;
        scene2D->setMouseTracking(true);
        if (element->getName() == "RoadBroken")
        {
            stateManager->roadBuilderState->setRoad(dynamic_cast<RoadBroken*>(element));
            stateManager->roadBuilderState->setGroupIndex(selectedGroup);
            stateManager->roadBuilderState->setElementIndex(selectedIndex);
            stateManager->roadBuilderState->clearProperties(layout);
            element->setStepDialog(stateManager->stepDialog);
            element->setStepDialogs(stateManager->stepDialogs, 10);
            element->getProperties(layout, scene2D);
            stateManager->setState(stateManager->roadBuilderState);
        }
        else
        {
            if (element->getName() == "LineBroken" ||
                    element->getName() == "Tramways" ||
                    element->getName() == "VoltageLine" ||
                    element->getName() == "DoubleVoltageLine" ||
                    element->getName() == "RailWay" ||
                    element->getName() == "SplitZone")
            {
                stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(element));
                stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                stateManager->lineBuilderState->setElementIndex(selectedIndex);
                stateManager->lineBuilderState->clearProperties(layout);
                element->setStepDialog(stateManager->stepDialog);
                element->setStepDialogs(stateManager->stepDialogs, 10);
                element->getProperties(layout, scene2D);
                stateManager->setState(stateManager->lineBuilderState);
            }
            else
            {
                stateManager->selectedState->setSelectedElement(selectedIndex, selectedGroup);
                stateManager->selectedState->setSelectedElement(element);
                stateManager->selectedState->clearProperties(layout);
                element->setStepDialog(stateManager->stepDialog);
                element->setStepDialogs(stateManager->stepDialogs, 10);
                element->getProperties(layout, scene2D);
                stateManager->setState(stateManager->selectedState);
            }
        }
    }
    else
    {
        for (int i = 0; i < elements.size(); ++i)
        {
            model->getGroup(elements[i]->getLayer()).push_back(elements[i]);
            stateManager->selectedState->selectedElements << elements[i];
            elements[i]->setSelectedStatus(true);
        }
        scene2D->setMouseTracking(true);
        stateManager->selectedState->clearProperties(layout);
        stateManager->setState(stateManager->selectedState);
    }
    scene2D->updateGL();
    isInModel = true;
}

void DeleteCommand::redo()
{ 
    if (log)
        Logger::getLogger()->infoLog() << "DeleteCommand::redo()\n";

    stateManager->currentState->clear();
    if (oneElement)
    {
        model->getGroup(groupIndex).removeAt(elementIndex);
    }
    else
    {        
        for (int i = 0; i < elements.size(); ++i)
        {
            for (int j = 0; j < model->getGroup(elements[i]->getLayer()).size(); ++j)
            {
                if (elements[i] == model->getGroup(elements[i]->getLayer())[j])
                {
                    model->getGroup(elements[i]->getLayer()).removeAt(j);
                    break;
                }
            }
        }
    }
    isInModel = false;
    scene2D->setMouseTracking(false);
    scene2D->setCursor(Qt::ArrowCursor);
    scene2D->updateGL();
    stateManager->setState(stateManager->defaultState);
}

void DeleteCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "DeleteCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}
// DeleteCommand............................................


// InsertCommand............................................

bool InsertCommand::log = true;

InsertCommand::InsertCommand(RoadElement *element, StateManager *stateManager, QFormLayout *layout, Model *model, int groupIndex, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    if (log)
        Logger::getLogger()->infoLog() << "InsertCommand::InsertCommand(RoadElement *element, Model *model, int groupIndex, Scene2D *scene2D, QUndoCommand *parent)"
                                       << " element = " << element
                                       << " groupIndex = " << groupIndex << "\n";
    this->element = element;
    this->model = model;
    this->isInModel = false;
    this->groupIndex = groupIndex;
    this->scene2D = scene2D;
    this->stateManager = stateManager;
    this->layout - layout;
}

InsertCommand::~InsertCommand()
{
    if (!isInModel)
        delete element;
    this->element = NULL;
    this->model = NULL;
    this->groupIndex = NULL;
    this->scene2D = NULL;
}

void InsertCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "InsertCommand::undo()\n";
    if (element->isSelected())
    {
        stateManager->currentState->clear();
        element->setSelectedStatus(false);
        scene2D->setMouseTracking(false);
        scene2D->setCursor(Qt::ArrowCursor);
        stateManager->setState(stateManager->defaultState);
    }
    model->getGroup(groupIndex).pop_back();
    scene2D->updateGL();
    isInModel = false;
}

void InsertCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "InsertCommand::redo()\n";
    model->getGroup(groupIndex).push_back(element);
    scene2D->updateGL();
    isInModel = true;
}

void InsertCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "InsertCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

// InsertCommand............................................


// AddRoadBrokenBreakCommand................................
bool AddRoadBrokenBreakCommand::log = true;

AddRoadBrokenBreakCommand::AddRoadBrokenBreakCommand(RoadBroken *roadBroken, bool front, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->roadBroken = roadBroken;
    this->front = front;
    this->scene2D = scene2D;
}

AddRoadBrokenBreakCommand::~AddRoadBrokenBreakCommand()
{
    this->roadBroken = NULL;
    this->scene2D = NULL;
}

void AddRoadBrokenBreakCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "AddRoadBrokenBreakCommand::undo()\n";
    roadBroken->deleteBreak(front);
    scene2D->updateGL();
}

void AddRoadBrokenBreakCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "AddRoadBrokenBreakCommand::redo()\n";
    roadBroken->addBreak(front);
    scene2D->updateGL();
}

void AddRoadBrokenBreakCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "AddRoadBrokenBreakCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

// AddRoadBrokenBreakCommand................................



// AddLineBrokenBreakCommand................................
bool AddLineBrokenBreakCommand::log = true;

AddLineBrokenBreakCommand::AddLineBrokenBreakCommand(LineBroken *lineBroken, bool front, Scene2D *scene2D, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->lineBroken = lineBroken;
    this->front = front;
    this->scene2D = scene2D;
}

AddLineBrokenBreakCommand::~AddLineBrokenBreakCommand()
{
    lineBroken = NULL;
    scene2D = NULL;
}

void AddLineBrokenBreakCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "AddLineBrokenBreakCommand::undo()\n";
    lineBroken->deleteBreak(front);
    scene2D->updateGL();
}

void AddLineBrokenBreakCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "AddLineBrokenBreakCommand::redo()\n";
    lineBroken->addBreak(front);
    scene2D->updateGL();
}

void AddLineBrokenBreakCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "AddLineBrokenBreakCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}
// AddLineBrokenBreakCommand................................

// PasteCommand................................
bool PasteCommand::log = true;

PasteCommand::PasteCommand(StateManager* stateManager, QFormLayout *layout,
                           Scene2D* scene2D, Model* model, QUndoCommand* parent)
    : QUndoCommand(parent)
{
    this->stateManager = stateManager;
    this->layout = layout;
    this->scene2D = scene2D;
    this->model = model;
    this->firstTime = true;
    this->isInModel = false;

    QClipboard *buffer = QApplication::clipboard();
    const RoadElementMimeData* data = qobject_cast<const RoadElementMimeData*>(buffer->mimeData());
    QList<RoadElement*> tempElements = data->getElements();
    for (int i = 0; i < tempElements.size(); ++i)
        elements << tempElements[i]->getCopy();
}

PasteCommand::~PasteCommand()
{
    if (!isInModel)
    {
        for (int i = 0; i < elements.size(); ++i)
        {
            delete elements[i];
            elements[i] = NULL;
        }
    }
    this->stateManager = NULL;
    this->scene2D = NULL;
    this->model = NULL;
}

void PasteCommand::undo()
{
    if (log)
        Logger::getLogger()->infoLog() << "PasteCommand::undo()\n";
    stateManager->currentState->clear();
    for (int i = 0; i < elements.size(); ++i)
    {
        model->getGroup(elements[i]->getLayer()).pop_back();
    }
    isInModel = false;
}

void PasteCommand::redo()
{
    if (log)
        Logger::getLogger()->infoLog() << "PasteCommand::redo()\n";
    for (int i = 0; i < elements.size(); ++i)
        model->getGroup(elements[i]->getLayer()).push_back(elements[i]);
    if (firstTime)
    {
        stateManager->currentState->clear();
        firstTime = false;
    }
    if (elements.size() > 1)
    {
        for (int i = 0; i < elements.size(); ++i)
        {
            stateManager->selectedState->selectedElements << elements[i];
            elements[i]->setSelectedStatus(true);
            elements[i]->move(-1.0f, 1.0f);
        }
        scene2D->setMouseTracking(true);
        stateManager->selectedState->clearProperties(layout);
        stateManager->setState(stateManager->selectedState);
    }
    else
    {
        RoadElement* element = elements[0];
        element->setSelectedStatus(true);

        int selectedGroup = element->getLayer();
        int selectedIndex = model->getGroup(selectedGroup).size() - 1;
        scene2D->setMouseTracking(true);
        if (element->getName() == "RoadBroken")
        {
            stateManager->roadBuilderState->setRoad(dynamic_cast<RoadBroken*>(element));
            stateManager->roadBuilderState->setGroupIndex(selectedGroup);
            stateManager->roadBuilderState->setElementIndex(selectedIndex);
            stateManager->roadBuilderState->clearProperties(layout);
            element->setStepDialog(stateManager->stepDialog);
            element->setStepDialogs(stateManager->stepDialogs, 10);
            element->getProperties(layout, scene2D);
            element->move(-1.0f, 1.0f);
            stateManager->setState(stateManager->roadBuilderState);

        }
        else
        {
            if (element->getName() == "LineBroken" ||
                    element->getName() == "Tramways" ||
                    element->getName() == "VoltageLine" ||
                    element->getName() == "DoubleVoltageLine" ||
                    element->getName() == "RailWay" ||
                    element->getName() == "SplitZone")
            {
                stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(element));
                stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                stateManager->lineBuilderState->setElementIndex(selectedIndex);
                stateManager->lineBuilderState->clearProperties(layout);
                element->setStepDialog(stateManager->stepDialog);
                element->setStepDialogs(stateManager->stepDialogs, 10);
                element->getProperties(layout, scene2D);
                element->move(-1.0f, 1.0f);
                stateManager->setState(stateManager->lineBuilderState);

            }

            else
            {
                stateManager->selectedState->setSelectedElement(selectedIndex, selectedGroup);
                stateManager->selectedState->setSelectedElement(element);
                stateManager->selectedState->clearProperties(layout);
                element->setStepDialog(stateManager->stepDialog);
                element->setStepDialogs(stateManager->stepDialogs, 10);
                element->getProperties(layout, scene2D);
                element->move(-1.0f, 1.0f);
                stateManager->setState(stateManager->selectedState);

            }
        }

    }
    isInModel = true;
    scene2D->updateGL();
}

void PasteCommand::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "PasteCommand::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


AddLineCommand::AddLineCommand(RoadSimple *roadSimple, LineLinked linked, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 0;
    isInFigure = false;
    this->roadSimple = roadSimple;
    lineLinked = new LineLinked();
    *lineLinked = linked;
    this->scene = scene;
}

AddLineCommand::AddLineCommand(RoadBroken *roadBroken, LineBrokenLinkedToRoadBroken brokenLinkedToRoadBroken, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 1;
    isInFigure = false;
    this->roadBroken = roadBroken;
    lineBrokenLinkedToRoadBroken = new LineBrokenLinkedToRoadBroken();
    *lineBrokenLinkedToRoadBroken = brokenLinkedToRoadBroken;
    this->scene = scene;
}

AddLineCommand::AddLineCommand(RoundingRoad *roundingRoad, LineBrokenLinked brokenLinked, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 2;
    isInFigure = false;
    this->roundingRoad = roundingRoad;
    lineBrokenLinked = new LineBrokenLinked();
    *lineBrokenLinked = brokenLinked;
    this->scene = scene;
}

AddLineCommand::~AddLineCommand()
{
    if (!isInFigure)
    {
        switch (type)
        {
        case 0:
            roadSimple = NULL;
            delete lineLinked->line;
            delete lineLinked;
            break;
        case 1:
            roadBroken = NULL;
            delete lineBrokenLinkedToRoadBroken->line;
            delete lineBrokenLinkedToRoadBroken;
            break;
        case 2:
            roundingRoad = NULL;
            delete lineBrokenLinked->line;
            delete lineBrokenLinked;
            break;
        default:
            break;
        }
    }
}


void AddLineCommand::undo()
{
    switch (type)
    {
    case 0:
        roadSimple->deleteLine(*lineLinked);
        break;
    case 1:
        roadBroken->deleteLine(*lineBrokenLinkedToRoadBroken);
        break;
    case 2:
        roundingRoad->deleteLine(*lineBrokenLinked);
        break;
    default:
        break;
    }
    isInFigure = false;
    if (scene)
        scene->updateGL();
}

void AddLineCommand::redo()
{
    switch (type)
    {
    case 0:
        roadSimple->addLine(*lineLinked);
        break;
    case 1:
        roadBroken->addLine(*lineBrokenLinkedToRoadBroken);
        break;
    case 2:
        roundingRoad->addLine(*lineBrokenLinked);
        break;
    default:
        break;
    }
    isInFigure = true;
    if (scene)
        scene->updateGL();
}


DeleteLineCommand::DeleteLineCommand(RoadSimple *roadSimple, LineLinked linked, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 0;
    isInFigure = false;
    this->roadSimple = roadSimple;
    lineLinked = new LineLinked();
    *lineLinked = linked;
    this->scene = scene;
}

DeleteLineCommand::DeleteLineCommand(RoadBroken *roadBroken, LineBrokenLinkedToRoadBroken brokenLinkedToRoadBroken, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 1;
    isInFigure = false;
    this->roadBroken = roadBroken;
    lineBrokenLinkedToRoadBroken = new LineBrokenLinkedToRoadBroken();
    *lineBrokenLinkedToRoadBroken = brokenLinkedToRoadBroken;
    this->scene = scene;
}

DeleteLineCommand::DeleteLineCommand(RoundingRoad *roundingRoad, LineBrokenLinked brokenLinked, QGLWidget *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    type = 2;
    isInFigure = false;
    this->roundingRoad = roundingRoad;
    lineBrokenLinked = new LineBrokenLinked();
    *lineBrokenLinked = brokenLinked;
    this->scene = scene;
}

DeleteLineCommand::~DeleteLineCommand()
{
    if (!isInFigure)
    {
        switch (type)
        {
        case 0:
            roadSimple = NULL;
            delete lineLinked->line;
            delete lineLinked;
            break;
        case 1:
            roadBroken = NULL;
            delete lineBrokenLinkedToRoadBroken->line;
            delete lineBrokenLinkedToRoadBroken;
            break;
        case 2:
            roundingRoad = NULL;
            delete lineBrokenLinked->line;
            delete lineBrokenLinked;
            break;
        default:
            break;
        }
    }
}

void DeleteLineCommand::undo()
{
    switch (type)
    {
    case 0:
        roadSimple->addLine(*lineLinked);
        break;
    case 1:
        roadBroken->addLine(*lineBrokenLinkedToRoadBroken);
        break;
    case 2:
        roundingRoad->addLine(*lineBrokenLinked);
        break;
    default:
        break;
    }
    isInFigure = true;
    if (scene)
        scene->updateGL();
}

void DeleteLineCommand::redo()
{
    switch (type)
    {
    case 0:
        roadSimple->deleteLine(*lineLinked);
        break;
    case 1:
        roadBroken->deleteLine(*lineBrokenLinkedToRoadBroken);
        break;
    case 2:
        roundingRoad->deleteLine(*lineBrokenLinked);
        break;
    default:
        break;
    }
    isInFigure = false;
    if (scene)
        scene->updateGL();
}
