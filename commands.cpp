#include "commands.h"
#include "roadelement.h"
#include "scene2d.h"

MoveCommand::MoveCommand(RoadElement *element, float dx, float dy, Scene2D *scene2D, QUndoCommand *parent)
: QUndoCommand(parent)
{
    if (element != NULL)
        this->element = element;
    this->dx = dx;
    this->dy = dy;
    this->scene2D = scene2D;
}

MoveCommand::~MoveCommand()
{
    element = NULL;
    scene2D = NULL;
}

void MoveCommand::undo()
{
    element->move((-1.0f)*dx, (-1.0f)*dy);
    scene2D->updateGL();
}

void MoveCommand::redo()
{
    element->move(dx, dy);
    scene2D->updateGL();
}


ResizeByControlCommand::ResizeByControlCommand(RoadElement* element, int index, float x, float y, float dx, float dy, Scene2D* scene2D, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    this->element = element;
    this->index = index;
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->scene2D = scene2D;
}

ResizeByControlCommand::~ResizeByControlCommand()
{
    element = NULL;
    scene2D = NULL;
}

void ResizeByControlCommand::undo()
{
    element->resizeByControl(index, (-1.0f) * dx, (-1.0f) * dy, x + dx, y + dy);
    scene2D->updateGL();
}

void ResizeByControlCommand::redo()
{
    element->resizeByControl(index, dx, dy, x, y);
    scene2D->updateGL();
}
