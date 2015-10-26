#ifndef COMMANDS
#define COMMANDS

#include <QUndoCommand>
class RoadElement;

class MoveCommand : public QUndoCommand
{
    // QUndoCommand interface
public:
    MoveCommand(RoadElement* element, float dx, float dy, QUndoCommand *parent = 0);
    ~MoveCommand();
    virtual void undo();
    virtual void redo();

private:
    RoadElement* element;
    float dx, dy;
};




#endif // COMMANDS

