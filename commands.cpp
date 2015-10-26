#include "commands.h"
#include "roadelement.h"

MoveCommand::MoveCommand(RoadElement *element, float dx, float dy, QUndoCommand *parent)
: QUndoCommand(parent)
{
    if (element != NULL)
        this->element = element;
    this->dx = dx;
    this->dy = dy;

}

MoveCommand::~MoveCommand()
{
    element = NULL;
}

void MoveCommand::undo()
{
    element->move((-1.0f)*dx, (-1.0f)*dy);
}

void MoveCommand::redo()
{
    element->move(dx, dy);
}
