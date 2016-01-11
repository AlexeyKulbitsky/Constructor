#ifndef LINEBUILDERSTATE_H
#define LINEBUILDERSTATE_H


class Scene2D;
#include "scene2d.h"

#include "state.h"
class StateManager;
#include "statemanager.h"


//#include "linebroken.h"

class LineBuilderState: public State
{
    Q_OBJECT
public:
    LineBuilderState();
    LineBuilderState(StateManager* manager, Model* model, Scene2D* scene, QFormLayout* properties);

    virtual ~LineBuilderState();

private:
    StateManager* stateManager;
    Model* model;
    Scene2D* scene;
    QFormLayout* properties;

    LineBroken* lineBroken;

    ResizeByControlCommand* resizeByControlCommand;
    float oldX, oldY, newX, newY;
    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;

    QString textureSource;
    float textureSize;
    float width;
    QString name;
    int layer;
    int controlIndex;
    int groupIndex, elementIndex;
    bool controlIsSelected;
    int key;

    bool linking;

    static bool log;
    // State interface
public:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void keyPressEvent(QKeyEvent *pe);
    virtual void keyReleaseEvent(QKeyEvent *);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

    void setTexture(QString source, float size);
    void setWidth(float width);
    void setName(QString name);
    void setLayer(int layer);

    bool tryToSelectControlsInSelectedFigure(QPoint mp);
    bool tryToSelectFigures(QPoint mp);
    void clearProperties(QFormLayout* layout);
    void setGroupIndex(int index);
    void setElementIndex(int index);
    virtual void clear();

    static void setLogging(bool status);
    static bool getLogging() { return log; }
public:
    bool useColor;



    // State interface
public:
    virtual QString getName();
    void setLine(LineBroken* lineBroken);
    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *);

public slots:
    void setLinking(bool state);

    // State interface
public:
    virtual void copy();
    virtual void paste();

    // State interface
public:
    virtual void cut();
    virtual void del();

    // State interface
public:
    virtual void saveToPresets();
};

#endif // LINEBUILDERSTATE_H
