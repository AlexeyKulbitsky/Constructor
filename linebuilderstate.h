#ifndef LINEBUILDERSTATE_H
#define LINEBUILDERSTATE_H


class Scene2D;
#include "scene2d.h"

#include "state.h"
class StateManager;
#include "statemanager.h"




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

    LineBuilder* lineBuilder;
    QPoint ptrMousePosition;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;

    QString textureSource;
    float textureSize;
    float width;
    QString name;
    int layer;

    bool linking;
    // State interface
public:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void keyPressEvent(QKeyEvent *pe);
    virtual void keyReleaseEvent(QKeyEvent *pe);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

    void setTexture(QString source, float size);
    void setWidth(float width);
    void setName(QString name);
    void setLayer(int layer);
public:
    bool useColor;



    // State interface
public:
    virtual QString getName();

    // State interface
public:
    virtual void contextMenuEvent(QContextMenuEvent *pe);

public slots:
    void setLinking(bool state);
};

#endif // LINEBUILDERSTATE_H
