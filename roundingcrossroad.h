#ifndef ROUNDINGCROSSROAD
#define ROUNDINGCROSSROAD

#include "roadelement.h"

class RoundingCrossRoad: public RoadElement
{
    Q_OBJECT
public:

    bool isSelected() { return selected; }
    void setSelectedStatus(bool status) { selected = status; }

    void drawFigure(QGLWidget* render = 0);
    void drawSelectionFrame();
    void move(float dx, float dy, float dz = 0);
    void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    QCursor getCursorForControlElement(int index);
    void resizeByControl(int index, float dx, float dy, float x, float y);
    int getNumberOfControls() { return numberOfVertices; }
    int controlsForPoint() { return 1;}

private:
    GLfloat* vertexArray;
    GLfloat* colorArray;
    GLubyte* indexArray; // количество полигонов

    GLubyte* indexArrayForSelection;
    GLfloat* colorArrayForSelection;

    int numberOfVertices; // количество вершин
    int numberOfPolygones;
    float xCenter, yCenter;
    float nearRadius, farRadius;
    int numberOfSides;
    bool selected;
    int indexOfSelectedControl;
    int layer;
    bool fixed;
public:
    RoundingCrossRoad();
    RoundingCrossRoad(float x, float y, float nearRadius, float farRadius, int numberOfSides, QString name, int layer);
    ~RoundingCrossRoad();

private:
    void setVertexArray(float x, float y, float nearRadius, float farRadius, int numberOfSides);
    void setColorArray(float red, float green, float blue);
    void setIndexArray();

    void setColorArrayForSelectionFrame(float red, float green, float blue);
    void setIndexArrayForSelectionFrame();

    // RoadElement interface
public:
    void changeColorOfSelectedControl(int index);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();

    // RoadElement interface
public:
    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);

    // RoadElement interface
public:
    virtual bool isFixed();

    // RoadElement interface
public:
    virtual void drawMeasurements(QGLWidget *render);

    // RoadElement interface
public slots:
    virtual bool setFixed(bool fixed);

    // RoadElement interface
public:
    virtual int getLayer();

    // RoadElement interface
public:
    virtual void clear();
};

#endif // ROUNDINGCROSSROAD

