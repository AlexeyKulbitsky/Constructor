#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "roadelement.h"
#include "cube.h"
#include "pole.h"

class Billboard : public RoadElement
{
    Q_OBJECT
public:
    Billboard();
    Billboard(float x, float y, float width, float height, float z, QString texture);
    Billboard(float x, float y, float width, float height, float z, float rotation, QString texture);
    Billboard(const Billboard& source);
    virtual ~Billboard();
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
    void setZRotVertexArray();
    void setZRotColorArray(float r, float g, float b);
    void setZRotIndexArray();

signals:
    void widthChanged(double value);
    void heightChanged(double value);
    void zChanged(double value);
    void zRotationChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double value);
    void setHeight(double value);
    void setZ(double value);
    void setZRotation(double value);

private:
    bool selected;
    bool fixed;
    int layer;
    float width;
    float height;
    float z;
    QVector<Cube*> plains;
    Pole* pole;
    Cube* support;
    bool doubleSide;
    QString texture;
    QVector<float> zRotVertexArray;
    QVector<float> zRotColorArray;
    QVector<unsigned> zRotIndexArray;
    float zRadius;
    float zRot;
    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};

#endif // BILLBOARD_H
