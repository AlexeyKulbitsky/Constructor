#ifndef LINESIMPLE_H
#define LINESIMPLE_H

#include <QGLWidget>
#include "roadelement.h"
#include <QCheckBox>

class LineSimple: public RoadElement
{
    Q_OBJECT
public:
    void setVertexArray(float x1, float y1, float x2, float y2, float width);
    void setColorArray(float red, float green, float blue, float alpha);
    void setTextureArray();
    void setIndexArray();
    void getTextures(QString source);

    // Рамка для выбора фигуры
    void setVertexArrayForSelection();
    void setIndexArrayForSelectionFrame();
    void setColorArrayForSelectionFrame(float red, float green, float blue);

    virtual bool isSelected() { return selected; }
    virtual void setSelectedStatus(bool status) { selected = status; }
protected:
    GLfloat VertexArray[4][3];
    GLfloat ColorArray[4][4];
    GLubyte IndexArray[2][3]; // количество полигонов
    GLfloat TextureArray[4][2];
    GLuint textureID[1];
    GLfloat textureSize;
    QString textureSource;
    GLfloat red, green, blue, alpha;
    GLfloat x1, y1, x2, y2;
    float width, length;
    float size;
    int layer;
    bool useColor;
    GLint hits;
    int indexOfSelectedControl;
    bool fixed;
    // Рамка для выделения
    GLfloat VertexArrayForSelection[8][3];
    GLubyte IndexArrayForSelection[4];
    GLfloat ColorArrayForSelection[4][3];
    bool selected;
    QString description;

public:
    LineSimple();
    LineSimple(float x1, float y1, float x2, float y2, QString name,int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name,int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer, QString description);

    bool hasPoint(GLfloat x, GLfloat y);
    void drawFigure(QGLWidget* render = 0);
    void drawSelectionFrame();
    void move(float dx, float dy, float dz = 0);
    void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    QCursor getCursorForControlElement(int index);
    void resizeByControl(int index, float dx, float dy, float x, float y);
    int getNumberOfControls() { return 9; }
    int controlsForPoint() { return 1;}
    void changeColorOfSelectedControl(int index);
    vec2 getAxisPoint_1(){return vec2(x1, y1);}
    vec2 getAxisPoint_2(){return vec2(x2, y2);}
    QPoint getCoorninateOfPointControl(int index);
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    void drawDescription(QGLWidget* render = 0, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();

    // RoadElement interface
public:
    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);

signals:
    void widthChanged(double width);
    void lengthChanged(double length);

public slots:
    void setWidth(double width);
    void setLength(double length);
    void setDescription(QString description);
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

#endif // LINESIMPLE_H
