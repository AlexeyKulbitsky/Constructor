#ifndef BEZIERS_H
#define BEZIERS_H

#include "roadelement.h"

class Beziers : public RoadElement
{
    Q_OBJECT
public:
    Beziers();
    Beziers(float x1, float y1, float x2, float y2, float x3, float y3, int numberOfSides);
    Beziers(QVector<float>& points, int numberOfSides);
    virtual ~Beziers();
    // RoadElement interface
public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget *render);
    virtual void drawSelectionFrame();
    virtual void drawMeasurements(QGLWidget *render);
    virtual void move(float dx, float dy, float dz);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual void getProperties(QVBoxLayout *layout, QGLWidget *render);
    virtual bool isFixed();
    virtual int getLayer();
    virtual void clear();
    virtual RoadElement *getCopy();
    virtual void rotate(float angle, float x, float y, float);

    void setVertexArray();
    void setColorArray(float r, float g, float b);
    void setIndexArray();
    vec3 findPoint(QVector<GLfloat> &points, float step);

signals:
    void numberOfSidesChanged(int value);

public slots:
    virtual bool setFixed(bool fixed);
    void setNumberOfSides(int value);

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLuint> indexArray;

    QVector<GLfloat> controlPoints;

    bool fixed;
    bool selected;
    int layer;
    int indexOfSelectedControl;
    float step;
    int numberOfSides;
};

#endif // BEZIERS_H
