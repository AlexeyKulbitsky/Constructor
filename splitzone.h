#ifndef SPLITZONE_H
#define SPLITZONE_H

#include "linebroken.h"

class SplitZone: public RoadElement
{
    Q_OBJECT
public:
    SplitZone();
    SplitZone(float *pointsArray, int size,
              float width,
              bool beginRounding,
              bool endRounding);
    SplitZone(float x1, float y1, float z1,
              float x2, float y2, float z2,
              float width,
              bool beginRounding,
              bool endRounding);    
    ~SplitZone();
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
    virtual void getProperties(QFormLayout *layout, QGLWidget *render);
    virtual bool isFixed();
    virtual int getLayer();
    virtual void clear();
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
public slots:
    virtual bool setFixed(bool fixed);

private:
    LineBroken *line;
    bool selected, fixed;
    int layer;

    bool beginRounding;
    bool endRounding;
    vec3 pBegin, pEnd;
    vec3 pPerpBegin, pPerpEnd;
    float width;

    GLfloat *lineAxisArray;
    int size;
    float lineWidth;
    vec3 p1, p2, p3, p4;
    vec3 line1_p1, line1_p2, line2_p1, line2_p2;

public:
    void calculateLine(vec3 p1, vec3 p2, float width);
    void calculateLine(float *pointsArray, int size,
                       float width);
    float getWidth();
};

#endif // SPLITZONE_H
