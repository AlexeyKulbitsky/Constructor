#ifndef DOUBLEVOLTAGELINE_H
#define DOUBLEVOLTAGELINE_H

#include <QGLWidget>
#include <QVector>
#include "linebroken.h"
#include "voltageline.h"

class DoubleVoltageLine: public LineBroken
{
    Q_OBJECT
public:
    DoubleVoltageLine();
    DoubleVoltageLine(float* axisArray, int size, float width = 0.5f,
                QString name = "DoubleVoltageLine",
                int layer = 2);
    DoubleVoltageLine(QVector<float> &axisArray, float width = 0.5f,
                QString name = "DoubleVoltageLine",
                int layer = 2);
    DoubleVoltageLine(const DoubleVoltageLine& source);
    virtual ~DoubleVoltageLine();
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

    virtual void addBreak(bool front);
    void setVertexArray();


signals:
    void widthChanged(double value);
    void heightChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double width);
    void setHeight(double height);

private:
    bool selected, fixed;
    int layer;
    float width;
    float height;
    QVector<GLfloat> axisVertexArray;
    QVector<VoltageLine*> lines;


    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual QJsonObject getJSONInfo();
    virtual void clearProperties(QLayout *layout);
    virtual void deleteBreak(bool front);
    virtual std::vector<vec3> getCoordOfControl(int index);
};

#endif // DOUBLEVOLTAGELINE_H
