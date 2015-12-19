#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "roadelement.h"
#include "roadsimple.h"
#include "curve.h"
#include <QVector>

class Angle : public QObject
{
    Q_OBJECT
public:
    Angle():angle(0.0f) {}
    Angle(float a):angle(a) {}
    float getAngle() { return angle; }

signals:
    void angleChanged(double angle);

public slots:
    void setAngle(double angle)
    {
        if (float(angle) == this->angle)
            return;
        this->angle = float(angle);
        emit angleChanged(angle);
    }

private:
    float angle;
};

class Intersection: public RoadElement
{
    Q_OBJECT
public:
    Intersection();
    Intersection(float x, float y, int numberOfRoads);
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
    void setRoadsTextures();
    void recalculateRoads();

    void resetWidth();
    void setAngle(double angle);
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

    QVector<Angle*> angles;
    QFormLayout* layout;
    QGLWidget* render;
    static bool log;
    // RoadElement interface
public:
    virtual void clear();
    static bool getLogging(){ return log; }
    static void setLogging(bool status);
    virtual void clearProperties(QLayout *layout);
    virtual void setModel(Model *model);
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual RoadElement *getCopy();
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual QJsonObject getJSONInfo();
};

#endif // INTERSECTION_H
