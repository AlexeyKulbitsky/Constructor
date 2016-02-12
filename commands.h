#ifndef COMMANDS
#define COMMANDS
#include <QVector>
#include <QUndoCommand>
#include <QFormLayout>
#include <QGLWidget>
#include "stepdialog.h"
class RoadElement;
class Scene2D;
class Model;
class RoadSimple;
class RoadBroken;
class RoundingRoad;
class LineBroken;
class StateManager;
class State;
struct vec3;
struct LineBrokenLinked;
struct LineBrokenLinkedToRoadBroken;
struct LineLinked;


class MoveCommand : public QUndoCommand
{
public:
    MoveCommand(RoadElement* element, float oldX, float oldY, float newX, float newY, Scene2D* scene2D, QUndoCommand *parent = 0);
    MoveCommand(QList<RoadElement*>& selectedElements, float oldX, float oldY, float newX, float newY, Scene2D* scene2D, QUndoCommand *parent = 0);
    ~MoveCommand();
    virtual void undo();
    virtual void redo();    
    static void setLogging(bool status);
    static bool getLogging() { return log; }

private:
    RoadElement* element;
    QList<RoadElement*> selectedElements;
    bool oneElement;
    bool firstTime;
    float oldX, oldY, newX, newY;
    Scene2D* scene2D;   
    static bool log;
};


class ResizeByControlCommand : public QUndoCommand
{
    // QUndoCommand interface
public:
    ResizeByControlCommand(RoadElement* element, int index, Scene2D* scene2D, QUndoCommand *parent = 0);
    ~ResizeByControlCommand();
    virtual void undo();
    virtual void redo();
    void setNewPosition();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    RoadElement* element;
    bool firstTime;
    int index;
    std::vector<vec3> oldPosition;
    std::vector<vec3> newPosition;
    Scene2D* scene2D;
    static bool log;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(RoadElement* element, Model* model, StateManager* stateManager, QVBoxLayout* layout, int groupIndex, int elementIndex, Scene2D *scene2D, QUndoCommand* parent = 0);
    DeleteCommand(QList<RoadElement*>& elements, Model* model, StateManager* stateManager, QVBoxLayout* layout, Scene2D* scene2D, QUndoCommand* parent = 0);
    ~DeleteCommand();
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    RoadElement* element;
    QList<RoadElement*> elements;
    bool oneElement;
    bool isInModel;
    Model* model;
    Scene2D* scene2D;
    StateManager* stateManager;
    QVBoxLayout* layout;
    int groupIndex;
    int elementIndex;
    static bool log;
};

class InsertCommand : public QUndoCommand
{
public:
    InsertCommand(RoadElement* element, StateManager* stateManager, QVBoxLayout* layout, Model* model, int groupIndex, Scene2D* scene2D, QUndoCommand* parent = 0);
    ~InsertCommand();
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    RoadElement* element;
    Model* model;
    Scene2D* scene2D;
    StateManager* stateManager;
    QVBoxLayout* layout;
    int groupIndex;
    int elementIndex;
    bool isInModel;
    static bool log;
};

class AddRoadBrokenBreakCommand : public QUndoCommand
{
public:
    AddRoadBrokenBreakCommand(RoadBroken* roadBroken, bool front, Scene2D* scene2D, QUndoCommand* parent = 0);
    ~AddRoadBrokenBreakCommand();

    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    RoadBroken* roadBroken;
    Scene2D* scene2D;
    bool front;
    static bool log;
};

class AddLineBrokenBreakCommand : public QUndoCommand
{
public:
    AddLineBrokenBreakCommand(LineBroken* lineBroken, bool front, Scene2D* scene2D, QUndoCommand* parent = 0);
    ~AddLineBrokenBreakCommand();
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    LineBroken* lineBroken;
    Scene2D* scene2D;
    bool front;
    static bool log;
};

class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(StateManager* stateManager,
                 QVBoxLayout* layout,
                 Scene2D* scene2D, Model* model, QUndoCommand* parent = 0);
    ~PasteCommand();
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    Scene2D* scene2D;
    Model* model;
    static bool log;
    QList<RoadElement*> elements;
    bool firstTime;
    bool isInModel;

    StateManager* stateManager;
    QVBoxLayout* layout;
};

class AddLineCommand : public QUndoCommand
{
public:
    AddLineCommand(RoadSimple* roadSimple, LineLinked linked, QGLWidget* scene, QUndoCommand* parent = 0);
    AddLineCommand(RoadBroken* roadBroken, LineBrokenLinkedToRoadBroken brokenLinkedToRoadBroken, QGLWidget *scene, QUndoCommand* parent = 0);
    AddLineCommand(RoundingRoad* roundingRoad, LineBrokenLinked brokenLinked, QGLWidget *scene, QUndoCommand* parent = 0);

    AddLineCommand(RoadSimple* roadSimple, LineLinkedToRoad line, QGLWidget* scene, QUndoCommand* parent = 0);
    AddLineCommand(RoundingRoad* roundingRoad, LineLinkedToRoad line, QGLWidget* scene, QUndoCommand* parent = 0);
    ~AddLineCommand();
private:
    int type;
    bool isInFigure;
    RoadSimple* roadSimple;
    RoadBroken* roadBroken;
    RoundingRoad* roundingRoad;
    LineLinked* lineLinked;
    LineBrokenLinkedToRoadBroken* lineBrokenLinkedToRoadBroken;
    LineBrokenLinked* lineBrokenLinked;
    QGLWidget* scene;

    LineLinkedToRoad line;
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
};

class DeleteLineCommand : public QUndoCommand
{
public:
    DeleteLineCommand(RoadSimple* roadSimple, LineLinked linked, QGLWidget* scene, QUndoCommand* parent = 0);
    DeleteLineCommand(RoadBroken* roadBroken, LineBrokenLinkedToRoadBroken brokenLinkedToRoadBroken, QGLWidget *scene, QUndoCommand* parent = 0);
    DeleteLineCommand(RoundingRoad* roundingRoad, LineBrokenLinked brokenLinked, QGLWidget *scene, QUndoCommand* parent = 0);
    ~DeleteLineCommand();
private:
    int type;
    bool isInFigure;
    RoadSimple* roadSimple;
    RoadBroken* roadBroken;
    RoundingRoad* roundingRoad;
    LineLinked* lineLinked;
    LineBrokenLinkedToRoadBroken* lineBrokenLinkedToRoadBroken;
    LineBrokenLinked* lineBrokenLinked;
    QGLWidget* scene;
    // QUndoCommand interface
public:
    virtual void undo();
    virtual void redo();
};

#endif // COMMANDS

