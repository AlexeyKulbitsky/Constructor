#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "roadelement.h"
#include "roadsimple.h"
#include "curve.h"
#include <QVector>
#include <QScrollArea>

class Intersection: public RoadElement
{
    Q_OBJECT
public:
    Intersection();
    Intersection(float x, float y);
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

    virtual bool isFixed();
    virtual int getLayer();
    void setRoadsTextures();
    bool calculateLinesIntersection(float a1, float b1, float c1,
                                    float a2, float b2, float c2,
                                    float& x, float& y);
    float calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
    void calculateRoadForAngle(int i, int index);
    void calculateRoadForRounding(int i, int index);
    void calculateRoadForMoving(int i, float dx, float dy);

signals:
    void intersectionsChanged();
    void intersectionsChanged(QFormLayout*,QGLWidget*);
    void roundingsChanged();
    void roadAdded();
    void linesChanged(QFormLayout* layout, QGLWidget* render);

public slots:
    virtual bool setFixed(bool fixed);
    virtual void getProperties(QFormLayout *layout, QGLWidget *render);

    bool calculateRoadIntersections();
    void calculateRoundings();

    void setVertexArray();
    void setIndexArray();
    void setTextureArray(float textureUSize, float textureVSize);

    void addRoad();
    void deleteRoad();
    void recalculateRoads();

    void setAngle(double angle);
    void setAngle(double angle, int index);
    void deleteLine();
    void addLine();

private:
    bool selected;
    bool fixed;
    int layer;

    QVector<RoadSimple*> roads;
    QVector<Curve*> curves;

    GLuint textureID[2];
    float texture1USize, texture1VSize;
    float texture2USize, texture2VSize;
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLubyte> indexArray;

    QVector<bool> showBoardStatus;

    QVector<float> angles;
    QFormLayout* layout;
    QGLWidget* render;

    // RoadElement interface
public:
    virtual void clear();

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual void setModel(Model *model);
};

#endif // INTERSECTION_H
