#ifndef ROADELEMENT3D_H
#define ROADELEMENT3D_H

#include "roadelement.h"
#include <QGLBuffer>
#include <QMap>
class RoadElement3D: public RoadElement
{
    Q_OBJECT
public:
    RoadElement3D();
    RoadElement3D(float x, float y);
    virtual ~RoadElement3D();
    bool selected;
    bool fixed;
    int layer;
    float scaleFactor;
    int indexOfSelectedControl;

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

    void setVertexArray(QVector<GLfloat>& vertexArray);
    void setTextureArray(QVector<GLfloat> &textureArray);
    void setNormalArray(QVector<GLfloat>& normalArray);
    void setIndexArray(QVector<GLubyte> &indexArray);
    void setColorArray(float red, float green, float blue);
    void getTextures(const char *textureSource);
public slots:
    virtual bool setFixed(bool fixed);

private:
    GLuint textureID[1];
    int listNumber;
    float deltaX, deltaY;
    GLdouble xRot;
    GLdouble yRot;
    GLdouble zRot;

    GLdouble xScale;
    GLdouble yScale;

    std::vector<QGLBuffer*> buffersVBO;
public:

   std::vector<vec3> vertexArray;
   std::vector<vec3> normalArray;
   std::vector<vec2> textureArray;

    std::vector<Vertex> vertices;
    std::vector<Mesh_3DS*> meshes;
    QMap<QString, Material> materials;

    Mesh mesh;

signals:
    void zRotationChanged(double value);
    void xScaleChanged(double value);
    void yScaleChanged(double value);

public slots:
    void setZRotation(double value);
    void setXScale(double value);
    void setYScale(double value);


    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);

    // RoadElement interface
public:
    virtual void clear();
};

#endif // ROADELEMENT3D_H
