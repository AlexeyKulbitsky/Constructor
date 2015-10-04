#ifndef SCENE2D_H
#define SCENE2D_H

#include <QGLWidget>
#include <QListWidgetItem>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <vector>
#include <iostream>
#include <QMenu>
#include <QAction>

#include "road.h"
#include "roundingcrossroad.h"
#include "roundingroad.h"


#include "linesimple.h"

#include "ruler.h"
#include "roadsimple.h"
#include "linebroken.h"
#include "linebuilder.h"
class StateManager;
#include "statemanager.h"
#include "model.h"

class Scene2D : public QGLWidget
{
    Q_OBJECT
public:

    QFont shrift;
    bool rulerIsActive;
    Ruler* ruler;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    GLfloat ratio; // отношение высоты окна виджета к его ширине
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zTra;
    GLfloat nSca;
    GLfloat xDelta;
    GLfloat yDelta;
    GLfloat dX;
    GLfloat dY;
    QPoint ptrMousePosition;

    bool figureIsSelected;
    bool controlIsSelected;

    void scale_plus();
    void scale_minus();
    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();
    void translate_down();
    void translate_up();
    void defaultScene();

    bool tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement* element, int& index);
    bool tryToSelectFigures(QPoint mp, RoadElement*& element);
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);

    void createMenu();
    void createActions();

protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
    void keyReleaseEvent(QKeyEvent *pe);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent * event);

public:
    explicit Scene2D(QWidget* parent = 0);
    ~Scene2D();
    void drawGrid();
    void drawAxis();
    bool isRulerActive();
    void setModel(Model* model);
    void setProperties(QFormLayout* properties);
    void drawBackground(QPainter *painter);

signals:
    void selectedFigureChanged(RoadElement* roadElement);
    void rulerStatusChanged(bool status);

public slots:
    void activateRuler();
    void deActivateRuler();
    void setRulerActive(bool status);
    void listItemClicked(QListWidgetItem* item);
    void setShowGrid(bool);
private:
    StateManager* stateManager;
    Model* model;
    QFormLayout* properties;
    bool showGrid;

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *pe);
};
#endif
