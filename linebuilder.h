#ifndef LINEBUILDER_H
#define LINEBUILDER_H

#include <GL/gl.h>
#include "roadelement.h"
#include <QPushButton>
#include <vector>
class LineBuilderState;
//#include <linebuilderstate.h>

class LineBuilder: public RoadElement
{
    Q_OBJECT
public:
    LineBuilder(LineBuilderState* lineBuilderState);

private:
    std::vector<GLfloat> vertexArray;
    GLfloat xCurrent,yCurrent;
    bool fixed;
    int layer;
    LineBuilderState* lineBuilderState;
    // RoadElement interface
public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget* render = 0);
    virtual void drawSelectionFrame();
    virtual void move(float dx, float dy, float dz = 0);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);

    void addPoint(float x, float y);
    void setCurrentPoint(float x, float y);
    std::vector<GLfloat>& getVertexArray();

    // RoadElement interface
public:
    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);

    // RoadElement interface
public:
    virtual bool isFixed();

    // RoadElement interface
public:
    virtual void drawMeasurements(QGLWidget *render);

    // RoadElement interface
public slots:
    virtual bool setFixed(bool fixed);

    // RoadElement interface
public:
    virtual int getLayer();

    // RoadElement interface
public:
    virtual void clear();
};

#endif // LINEBUILDER_H
