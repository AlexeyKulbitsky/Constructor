#ifndef LINEBROKEN_H
#define LINEBROKEN_H

#include <QGLWidget>
#include "roadelement.h"
#include <QVector>
#include <QPushButton>

class LineBroken: public RoadElement
{
    Q_OBJECT
public:
    void setVertexArray(float width, float* axisVertices, int size);
    void setColorArray(float red, float green, float blue, float alpha);
    void getTextures(QString source);
    void setTextureArray();
    void setIndexArray();

    void setVertexArrayForAxis(float* axisVertices, int size);
    void setColorArrayForAxis(float red, float green, float blue);
    void setIndexArrayForAxis();
    // Рамка для выбора фигуры

    void setIndexArrayForSelectionFrame();
    void setColorArrayForSelectionFrame(float red, float green, float blue);

    virtual bool isSelected() { return selected; }
    virtual void setSelectedStatus(bool status) { selected = status; }
protected:

    QVector<GLfloat> VertexArray;
    QVector<GLfloat> ColorArray;
    QVector<GLfloat> TextureArray;
    QVector<GLubyte> IndexArray;

    //GLfloat* VertexArray;
    //GLfloat* ColorArray;
    //GLubyte* IndexArray; // количество полигонов
    GLuint textureID[1];
    int numberOfVertices;
    int numberOfPolygones;
    GLfloat red, green, blue, alpha;
    GLfloat textureSize;
    QString textureSource;
    float width;
    bool useColor;
    int layer;
    GLint hits;
    int indexOfSelectedControl;
    bool fixed;
    // Рамка для выделения

    QVector<GLubyte> IndexArrayForSelection;
    QVector<GLfloat> ColorArrayForSelection;
    //GLubyte* IndexArrayForSelection;
    //GLfloat* ColorArrayForSelection;


    QVector<GLfloat> vertexArrayForAxis;
    QVector<GLfloat> colorArrayForAxis;
    QVector<GLubyte> indexArrayForAxis;
    //GLfloat* vertexArrayForAxis;
    //GLfloat* colorArrayForAxis;
    //GLubyte* indexArrayForAxis;
    int numberOfVerticesOfAxis;
    int numberOfAxises;
    bool selected;

public:
    LineBroken();
    LineBroken(float width, float* axisVertices, int size, QString name, int layer);
    LineBroken(float width, float* axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer);
    LineBroken(float width, float* axisVertices, int size, QString source, float textureSize, QString name, int layer);
    bool hasPoint(GLfloat x, GLfloat y);
    void drawFigure(QGLWidget* render = 0);
    void drawSelectionFrame();
    void move(float dx, float dy, float dz = 0);
    void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    QCursor getCursorForControlElement(int index);
    void resizeByControl(int index, float dx, float dy, float x, float y);
    int getNumberOfControls();
    int controlsForPoint() { return 1;}
    void changeColorOfSelectedControl(int index);

    QPoint getCoorninateOfPointControl(int index);

protected:
    void addControl(float x, float y);

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

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
};

#endif // LINEBROKEN_H
