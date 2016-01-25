#ifndef VOLTAGELINE_H
#define VOLTAGELINE_H

#include <QGLWidget>
#include <QVector>
#include "linebroken.h"

class VoltageLine: public LineBroken
{
    Q_OBJECT
public:
    VoltageLine();
    VoltageLine(float* axisArray, int size, float width = 0.01f,
                QString name = "VoltageLine",
                int layer = 2);
    VoltageLine(QVector<float> &axisArray, float width = 0.01f,
                QString name = "VoltageLine",
                int layer = 2);
    VoltageLine(const VoltageLine& source);
    virtual ~VoltageLine();
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
    void addBreak(bool front, float x, float y, float z);
    void setVertexArray();
    void setIndexArray();
    void setColorArray(float red, float green, float blue, float alpha);

signals:
    void widthChanged(double value);
    void heightChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double width);
    void setHeight(double height);

public:
    QVector<GLfloat> axisVertexArray;

private:
    bool selected, fixed;
    int layer;
    float width;
    float height;
    QVector<GLfloat> vertexArray;
    QVector<GLubyte> indexArray;
    QVector<GLfloat> colorArray;

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual QJsonObject getJSONInfo();
    virtual void clearProperties(QLayout *layout);

    // LineBroken interface
public:
    virtual void deleteBreak(bool front);
};

#endif // VOLTAGELINE_H
