#ifndef CURVE_H
#define CURVE_H
#include <QObject>
#include "roadelement.h"

class Curve: public RoadElement
{
    Q_OBJECT
public:
    Curve(float xCenter, float yCenter, float zCenter,
          float xLeft, float yLeft, float zLeft,
          float xRight, float yRight, float zRight,
          QString texture_1, float texture_1Usize, float texture_1Vsize,
          QString texture_2, float texture_2Usize, float texture_2Vsize,
          int numberOfSides);
    ~Curve();
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
    virtual void clear();

    void setVertexArray();
    void setTextureArray();
    void setIndexArray();

    void setVertexArrayBoard();
    void setTextureArrayBoard(float textureUSize, float textureVSize);
    void setIndexArrayBoard();

    GLuint getTextures(QString source);
    float getLeftLength() { return leftLength; }
    float getRightLength() { return rightLength; }
    void setCoordForPoint(int index, float x, float y, float z);
    vec3 getCoordOfPoint(int index);
    void getWindowCoord(double x, double y, double z, double  &wx, double  &wy, double  &wz);
    float getBoardWidth();
    bool getBoardShowStatus();
    void setAngleVertexArray();
    void setAngleColorArray(float red, float green, float blue);
    void setAngleIndexArray();
    void calculateAngle();

signals:
    void leftLengthChanged(double value);
    void rightLengthChanged(double value);
    void boardWidthChanged(double value);
    void showBoardStatusChanged(bool status);
    void angleChanged(double angle);

public slots:
    virtual bool setFixed(bool fixed);
    void setLeftLength(double length);
    void setRightLength(double length);
    void setBoardWidth(double width);
    void setBoardShowStatus(bool status);
    void setAngle(double angle);

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLubyte> indexArray;

public:
    QVector<GLfloat> vertexArrayBoard;
    float texture1USize, texture1VSize;
    float texture2USize, texture2VSize;
    float angleRounding;

private:
    QVector<GLfloat> textureArrayBoard;
    QVector<GLubyte> indexArrayBoard;

    QVector <GLfloat> angleVertexArray;
    QVector <GLfloat> angleColorArray;
    QVector <GLubyte> angleIndexArray;



    float controlPoints[9];
    float leftLength, rightLength;
    float angle1, angle2;
    double angle;
    float xRadius, yRadius;
    float radius;
    int numberOfSides;
    GLuint textureID[2];

    float boardWidth;
    bool fixed, selected, showBoard;
    int layer;
    int indexOfSelectedControl;
    QFormLayout* layout;
    QGLWidget* render;

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
};

#endif // CURVE_H
