#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <QListWidgetItem>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <vector>
#include <iostream>

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

    void scale_plus();
    void scale_minus();
    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();

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
};

#endif // SCENE3D_H
