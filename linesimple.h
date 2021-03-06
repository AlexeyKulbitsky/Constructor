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
    GLfloat xStep, yStep;
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
    static bool log;
public:
    LineSimple();
    LineSimple(float x1, float y1, float x2, float y2, QString name,int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name,int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer);
    LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer, QString description);
    LineSimple(const LineSimple& source);

    bool hasPoint(GLfloat x, GLfloat y);
    virtual void drawFigure(QGLWidget* render = 0);
    virtual void drawSelectionFrame();
    virtual void move(float dx, float dy, float dz = 0);
    virtual void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls() { return 2; }
    virtual int controlsForPoint() { return 1;}
    virtual void changeColorOfSelectedControl(int index);
    vec2 getAxisPoint_1(){return vec2(x1, y1);}
    vec2 getAxisPoint_2(){return vec2(x2, y2);}
    QPoint getCoorninateOfPointControl(int index);
    void drawDescription(QGLWidget* render = 0, float red = 1.0f, float green = 1.0f, float blue = 1.0f);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
    void setXStep(float x) { xStep = x; }
    void setYStep(float y) { yStep = y; }

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();

    // RoadElement interface
public:
    virtual void getProperties(QVBoxLayout *layout, QGLWidget* render = 0);

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
    void setLength(float length, bool front);
    float getLength() { return length; }
    float getWidth() { return width; }
    // RoadElement interface
public:
    virtual void drawMeasurements(QGLWidget *render);
    virtual bool setFixed(bool fixed);
    virtual int getLayer();
    virtual void clear();
    virtual std::vector<vec3> getCoordOfControl(int index);
    void rotate(float angle, float x, float y, float z);
    virtual RoadElement *getCopy();
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual void clearProperties(QLayout *layout);
};

#endif // LINESIMPLE_H
