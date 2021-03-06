#ifndef ROADELEMENTOBJ_H
#define ROADELEMENTOBJ_H
#include <QObject>
#include "roadelement.h"

class RoadElementOBJ: public RoadElement
{
    Q_OBJECT
public:
    RoadElementOBJ();
    RoadElementOBJ(float x, float y, QString folder, QString filename);
    RoadElementOBJ(const RoadElementOBJ &source);
    virtual ~RoadElementOBJ();
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


signals:

    void zRotationChanged(double value);
    void xScaleChanged(double value);
    void yScaleChanged(double value);
    void zScaleChanged(double value);
    void scaleChanged(double value);
    void zTranslationChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setZRotation(double value);
    void setXScale(double value);
    void setYScale(double value);
    void setZScale(double value);
    void setScale(double scale);
    void setZTranslation(double translation);

public:

    bool selected;
    bool fixed;
    int layer;
    float scaleFactor;
    int indexOfSelectedControl;

    GLuint textureID[1];
    GLuint figureList;

    int listNumber;
    float deltaX, deltaY, deltaZ;
    float zRadius;
    GLdouble xRot;
    GLdouble yRot;
    GLdouble zRot;

    GLdouble xScale;
    GLdouble yScale;
    GLdouble zScale;
    QVector<float> zRotVertexArray;
    QVector<float> zRotColorArray;
    QVector<unsigned> zRotIndexArray;
    //std::vector<vec3> vertexArray;
    //std::vector<vec3> normalArray;
    //std::vector<vec2> textureArray;

     //std::vector<Vertex> vertices;
     std::vector<Mesh*> meshes;

     //QMap<QString, Material> materials;

     //Mesh mesh;
     QString folder;
     QString filename;
     // RoadElement interface
public:
     virtual void clear();
     void setZRotVertexArray();
     void setZRotColorArray(float r, float g, float b);
     void setZRotIndexArray();

     GLuint generateList();
     void setList(GLuint id);
     // RoadElement interface
public:
     virtual RoadElement *getCopy();

     // RoadElement interface
public:
     virtual void clearProperties(QLayout *layout);

     // RoadElement interface
public:
     virtual QJsonObject getJSONInfo();

     // RoadElement interface
public:
     virtual std::vector<vec3> getCoordOfControl(int index);
     virtual void setCoordForControl(int index, std::vector<vec3> &controls);
};

#endif // ROADELEMENTOBJ_H
