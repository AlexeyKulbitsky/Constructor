#ifndef CUBE_H
#define CUBE_H

#include "roadelement.h"

class Cube: public RoadElement
{
    Q_OBJECT
public:
    Cube(float x, float y, float z, float delta, QString name, int layer);
    Cube(float x, float y, float z, float delta, QString texture, QString name, int layer);
    Cube(float x, float y, float z, float height, float width, float length, QString name, int layer);
    Cube(float x, float y, float z, float height, float width, float length, QString texture, QString name, int layer);
    Cube(const Cube& source);
    virtual ~Cube();

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLfloat> normalArray;
    QVector<GLubyte> indexArray;
    QVector<GLfloat> controlPoints;
    float height;
    float width;
    float length;
    bool selected;
    bool fixed;
    int layer;
    float z;
    QString texture;
    int textureId;
    bool withTexture;

private:
    void setVertexArray(float x, float y, float z, float delta);
    void setVertexArray(float x, float y, float z, float height, float width, float length);
    void setVertexArray();
    void setColorArray(float red, float green, float blue);
    void setNormalArray();
    void setIndexArray();

    // RoadElement interface
public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget *render);
    virtual void drawSelectionFrame();
    virtual void drawMeasurements(QGLWidget *render);
    virtual void move(float dx, float dy, float dz = 0);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual QJsonObject getJSONInfo();
    virtual void getProperties(QFormLayout *layout, QGLWidget *render);
    virtual bool isFixed();

signals:
    void widthChanged(double value);
    void lengthChanged(double value);
    void heightChanged(double value);
    void zChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double value);
    void setLength(double value);
    void setHeight(double value);
    void setZ(double value);
    // RoadElement interface
public:
    float getWidth() { return width; }
    float getLength() { return length; }
    float getHeight() { return height; }
    float getZ() { return z; }

    virtual int getLayer();
    virtual void clear();
    virtual RoadElement *getCopy();
    virtual void clearProperties(QLayout *layout);
    virtual void rotate(float angle, float x, float y, float z);
    void setColor(float red, float green, float blue);

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
};

#endif // CUBE_H
