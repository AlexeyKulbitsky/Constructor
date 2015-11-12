#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QListWidgetItem>
#include <vector>
#include <iostream>
#include <GL/glu.h>
class StateManager;
#include "statemanager.h"
#include "model.h"

class Scene3D : public QGLWidget
{
    Q_OBJECT
public:
    explicit Scene3D(QWidget* parent = 0, QGLWidget *shared = 0);
    ~Scene3D();
    void setModel(Model* model);

    void scalePlus();
    void scaleMinus();
    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();
    void getProperties(QFormLayout* layout);
    static bool getLogging() { return log; }
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

    // QGLWidget interface
protected:
    virtual void initializeGL();
    virtual void resizeGL(int nWidth, int nHeight);
    virtual void paintGL();

signals:

public slots:
    static void setLogging(bool status);
    void setBackGroundColor();

public:
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    bool middleButtonIsPressed;
    GLfloat ratio; // отношение высоты окна виджета к его ширине
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;

    GLfloat xTra;
    GLfloat yTra;
    GLfloat zTra;

    GLfloat nSca;

    GLfloat xDelta;
    GLfloat yDelta;
    GLfloat dX;
    GLfloat dY;
    QPoint ptrMousePosition;

private:
    StateManager* stateManager;
    Model* model;
    QFormLayout* layout;
    QColor backgroundColor;
    static bool log;
};

#endif // SCENE3D_H
