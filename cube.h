#ifndef CUBE_H
#define CUBE_H

#include "roadelement.h"

class Cube: public RoadElement
{
    Q_OBJECT
public:
    Cube(float x, float y, float z, float delta, QString name, int layer);
    virtual ~Cube();

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLfloat> normalArray;
    QVector<GLubyte> indexArray;

    bool selected;
    int layer;

private:
    void setVertexArray(float x, float y, float z, float delta);
    void setColorArray(float red, float green, float blue);
    void setIndexArray();

    // RoadElement interface
public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget *render);
    virtual void drawSelectionFrame();
    virtual void drawMeasurements(QGLWidget *render);
    virtual void move(float dx, float dy, float dz = 0);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual QJsonObject getJSONInfo();
    virtual void getProperties(QFormLayout *layout, QGLWidget *render);
    virtual bool isFixed();

public slots:
    virtual bool setFixed(bool fixed);

    // RoadElement interface
public:
    virtual int getLayer();

    // RoadElement interface
public:
    virtual void clear();
};

#endif // CUBE_H
