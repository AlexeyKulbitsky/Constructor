#ifndef COMMANDS
#define COMMANDS

#include <QUndoCommand>
class RoadElement;
class Scene2D;

class MoveCommand : public QUndoCommand
{
public:
    MoveCommand(RoadElement* element, float dx, float dy, Scene2D* scene2D, QUndoCommand *parent = 0);
    ~MoveCommand();
    virtual void undo();
    virtual void redo();

private:
    RoadElement* element;
    float dx, dy;
    Scene2D* scene2D;
};


class ResizeByControlCommand : public QUndoCommand
{
    // QUndoCommand interface
public:
    ResizeByControlCommand(RoadElement* element, int index, float x, float y, float dx, float dy, Scene2D* scene2D, QUndoCommand *parent = 0);
    ~ResizeByControlCommand();
    virtual void undo();
    virtual void redo();

private:
    RoadElement* element;
    int index;
    float x, y;
    float dx, dy;
    Scene2D* scene2D;
};



#endif // COMMANDS

