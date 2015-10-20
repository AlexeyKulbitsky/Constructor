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
    ~RailWay();

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

    virtual void addBreak(bool front);
    void setVertexArray();
    void setTextureArray(float textureUsize, float textureVsize);
    void setIndexArray();

signals:
    void widthChanged(double width);
public slots:
    virtual bool setFixed(bool fixed);

private:
    bool selected, fixed;
    int layer;
    int textureID[1];
    float textureVsize, textureUsize;
    QVector<GLfloat> axisVertexArray;
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLuint> indexArray;
};

#endif // RAILWAY_H
