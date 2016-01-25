#ifndef MAPPLANE_H
#define MAPPLANE_H

#include "roadelement.h"


class MapPlane : public RoadElement
{
    Q_OBJECT
public:
    MapPlane(GLfloat width, GLfloat height, QString& texture);
    MapPlane(GLfloat width, GLfloat height, QImage& image);
    MapPlane(const MapPlane &source);
    virtual ~MapPlane();

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
    virtual QJsonObject getJSONInfo();

public slots:
    virtual bool setFixed(bool fixed);

private:
    bool fixed;
    bool selected;
    int layer;

    GLfloat vertexArray[4][3];
    GLfloat textureArray[4][2];
    QString texture;
    GLuint textureID;
    GLfloat width, height;
    QImage image;
};

#endif // MAPPLANE_H
