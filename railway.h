#ifndef RAILWAY_H
#define RAILWAY_H

#include <QGLWidget>
#include <QVector>
#include "linebroken.h"

class RailWay: public LineBroken
{
     Q_OBJECT
public:
    RailWay();
    RailWay(float* axisArray, int size, QString textureSource, float textureUsize, float textureVsize);
    RailWay(QVector<float>& axisArray, QString textureSource, float textureUsize, float textureVsize);
    RailWay(const RailWay& source);
    ~RailWay();

public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget *);
    virtual void drawSelectionFrame();
    virtual void drawMeasurements(QGLWidget *);
    virtual void move(float dx, float dy, float dz);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual void getProperties(QFormLayout *layout, QGLWidget *);
    virtual bool isFixed();
    virtual int getLayer();
    virtual void clear();

    virtual void addBreak(bool front);
    virtual void deleteBreak(bool front);
    void setVertexArray();
    void setTextureArray(float textureUsize, float textureVsize);
    void setIndexArray();

    static bool getLogging() { return log; }
    static void setLogging(bool status);

signals:
    void widthChanged(double width);
public slots:
    virtual bool setFixed(bool fixed);

private:
    bool selected, fixed;
    int layer;
    int textureID[1];
    QString texture;
    float textureVsize, textureUsize;
    QVector<GLfloat> axisVertexArray;
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLuint> indexArray;

    static bool log;

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};

#endif // RAILWAY_H
