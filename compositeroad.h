#ifndef COMPOSITEROAD_H
#define COMPOSITEROAD_H

#include "roadelement.h"
#include <QList>

class CompositeRoad: public RoadElement
{
    Q_OBJECT
public:
    CompositeRoad();
    virtual ~CompositeRoad();
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
    virtual void getProperties(QFormLayout *layout, QGLWidget *render);
    virtual bool isFixed();
    virtual void addElement(RoadElement *element);

private:
    bool selected;
    bool fixed;
    QList<RoadElement*> elements;
    int layer;

    // RoadElement interface
public slots:
    virtual bool setFixed(bool fixed);

    // RoadElement interface
public:
    virtual int getLayer();

    // RoadElement interface
public:
    virtual void deleteElement(int index);
    virtual int getNumberOfElements();
    virtual RoadElement *getElement(int index);

    // RoadElement interface
public:
    virtual void clear();
};

#endif // COMPOSITEROAD_H
