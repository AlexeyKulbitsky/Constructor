#ifndef POLE_H
#define POLE_H
#include "roadelement.h"
#include <QVector>

class Pole : public RoadElement
{
    Q_OBJECT
public:
    Pole();
    Pole(float x, float y, float diameter, float height);
    Pole(float x1, float y1, float z1, float x2, float y2, float z2, float diameter);
    Pole(const Pole& source);
    virtual ~Pole();
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
    virtual RoadElement *getCopy();
    virtual std::vector<vec3> getCoordOfControl(int);
    virtual void setCoordForControl(int, std::vector<vec3> &);
    virtual void clearProperties(QLayout *layout);
    virtual QJsonObject getJSONInfo();

    void setVertexArray();
    void setNormalArray();
    void setColorArray(float r, float g, float b);
    void setIndexArray();
signals:
    void diameterChanged(double value);
    void heightChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setDiameter(double value);
    void setHeight(double value);

private:
    float height;
    float diameter;
    bool fixed;
    bool selected;
    int numberOfSides;
    int layer;
    int indexOfSelectedControl;
    QVector <GLfloat> vertexArray;
    QVector <GLfloat> colorArray;
    QVector <GLfloat> normalArray;
    QVector <GLuint> indexArray;
    float x1, y1, z1;
    float x2, y2, z2;
    float z;
    // RoadElement interface
public:
    virtual void rotate(float angle, float x, float y, float z);
};

#endif // POLE_H
