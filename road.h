#ifndef ROAD
#define ROAD

#include <GL/gl.h>
#include <QtGui>
#include <QGLWidget>

#include "roadelement.h"

class Road: public RoadElement
{
    Q_OBJECT
public:


    void setVertexArray(float x, float y, float size);
    void setColorArray(float red, float green, float blue, float alpha);
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
    GLfloat TextureArray[4][2];
    GLubyte IndexArray[2][3]; // количество полигонов

    GLubyte IndexArrayForSelection[4];
    GLfloat ColorArrayForSelection[4][3];

    GLfloat red, green, blue, alpha;

    float size;
    GLint hits;
    int indexOfSelectedControl;
    int layer;
    bool selected;
    bool fixed;
    GLuint textureID[1];
public:
    Road();
    Road(float x, float y, float size);
    Road(float x, float y, float size, float red, float green, float blue, float alpha);
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

    QPoint getCoorninateOfPointControl(int index);


    void getTextures();


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

#endif // ROAD

