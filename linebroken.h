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
    virtual void setVertexArray(float width, float* axisVertices, int size);
    virtual void setColorArray(float red, float green, float blue, float alpha);
    virtual void setTextureArray();
    virtual void setIndexArray();

    virtual void setVertexArrayForAxis(float* axisVertices, int size);
    virtual void setColorArrayForAxis(float red, float green, float blue);
    virtual void setIndexArrayForAxis();
    // Рамка для выбора фигуры

    virtual void setIndexArrayForSelectionFrame();
    virtual void setColorArrayForSelectionFrame(float red, float green, float blue);

    virtual bool isSelected() { return selected; }
    virtual void setSelectedStatus(bool status) { selected = status; }

    static bool getLogging() { return log; }
    static void setLogging(bool status);

private:
    static bool log;

protected:

    QVector<GLfloat> VertexArray;
    QVector<GLfloat> ColorArray;
    QVector<GLfloat> TextureArray;
    QVector<GLubyte> IndexArray;

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


    QVector<GLfloat> vertexArrayForAxis;
    QVector<GLfloat> colorArrayForAxis;
    QVector<GLubyte> indexArrayForAxis;
    int numberOfVerticesOfAxis;
    int numberOfAxises;
    bool selected;
    QString description;

public:
    LineBroken();
    LineBroken(float width, float* axisVertices, int size, QString name, int layer);
    LineBroken(float width, float* axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer);
    LineBroken(float width, float* axisVertices, int size, QString source, float textureSize, QString name, int layer);
    LineBroken(float width, float* axisVertices, int size, QString source, float textureSize, QString name, int layer, QString description);
    LineBroken(const LineBroken& source);
    virtual ~LineBroken();
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
    virtual std::vector<vec3> getCoordOfControl(int index);
    void rotate(float angle, float x, float y, float z);
    virtual QJsonObject getJSONInfo();
    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);
    virtual bool isFixed();
    virtual void addBreak(bool front);
    virtual void deleteBreak(bool front);
    virtual void drawMeasurements(QGLWidget *render);
    virtual int getLayer();
    virtual void clear();

    QPoint getCoorninateOfPointControl(int index);
    void drawDescription(QGLWidget* render = 0, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

protected:
    void addControl(float x, float y);

signals:
    void widthChanged(double value);

    // RoadElement interface
public slots:
    virtual bool setFixed(bool fixed);
    void setDescription(QString description);
    void setWidth(double value);

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);
};

#endif // LINEBROKEN_H
