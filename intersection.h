#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "roadelement.h"
#include "roadsimple.h"
#include "curve.h"
#include <QVector>

class Intersection: public RoadElement
{
    Q_OBJECT
public:
    Intersection();
    Intersection(float x, float y);
    Intersection(const Intersection& source);
    Intersection(QVector<RoadSimple*>& roads, QVector<Curve*>& curves);
    virtual ~Intersection();
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
    void calculateRoadIntersections(int roadIndex, int controlIndex);
    void calculateRoundings();

    void setVertexArray();
    void setIndexArray();
    void setTextureArray(float textureUSize, float textureVSize);

    void addRoad();
    void deleteRoad();
    void recalculateRoads();

    void resetWidth();
    void setAngle(double angle);
    void setAngle(double angle, int index);
    void deleteLine();
    void addLine();

private:
    bool selected;
    bool fixed;
    int layer;
    int indexOfSelectedControl;

    QVector<RoadSimple*> roads;
    QVector<Curve*> curves;

    GLuint textureID[2];
    float texture1USize, texture1VSize;
    float texture2USize, texture2VSize;
    QString texture1, texture2;
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLubyte> indexArray;

    QVector<bool> showBoardStatus;

    QVector<float> angles;
    QFormLayout* layout;
    QGLWidget* render;
    static bool log;
    // RoadElement interface
public:
    virtual void clear();
    static bool getLogging(){ return log; }
    static void setLogging(bool status);
    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual void setModel(Model *model);

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};

#endif // INTERSECTION_H
