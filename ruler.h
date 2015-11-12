#ifndef RULER_H
#define RULER_H

#include <QPoint>
#include "roadelement.h"
#include <GL/glu.h>

class Ruler: public RoadElement
{
    Q_OBJECT
public:
    Ruler();
    Ruler(const Ruler& source);
    virtual ~Ruler();
private:
    float x1, y1, z1;
    float x2, y2, z2;
    float *x1_ptr, *y1_ptr, *z1_ptr;
    float *x2_ptr, *y2_ptr, *z2_ptr;
    bool startPointIsActivated;
    bool endPointIsActivated;
    bool selected;
    bool fixed;
    int layer;
    int indexOfSelectedControl;

    RoadElement* element1;
    RoadElement* element2;
    int elementId1, elementId2;
    int index1, index2;
    float partOfStartLine, partOfEndLine;
    static bool log;

public:
    void activateStartPoint();
    void deActivateStartPoint();
    void setStartPoint(float x, float y, float z);

    void activateEndPoint();
    void deActivateEndPoint();
    void setEndPoint(float x, float y, float z);

    float getX1(){return x1;}
    float getY1(){return y1;}
    float getX2(){return x2;}
    float getY2(){return y2;}

    bool isStartPointActivated();
    bool isEndPointActivated();
    QPoint getStartPoint();
    QPoint getEndPoint();

    void setStartLinkPoint(RoadElement* element, int indexOfControl, float x, float y, float z);
    void setEndLinkPoint(RoadElement* element, int indexOfControl, float x, float y, float z);
    void clearStartLinkPoint();
    void clearEndLinkPoint();

    static bool getLogging();
    static void setLogging(bool status);

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

    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);

public slots:
    virtual bool setFixed(bool fixed);

    // RoadElement interface
public:
    virtual void clear();

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};

#endif // RULER_H
