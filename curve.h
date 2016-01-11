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
    Curve(float *controls, int size,
          QString texture_1, float texture_1Usize, float texture_1Vsize,
          QString texture_2, float texture_2Usize, float texture_2Vsize,
          int numberOfSides, float boardWidth,
          bool fixed, bool showBoard, int layer, QString name);
    Curve(const Curve& source);
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

    void setDescirption(const QString& desc);

    void setVertexArray();
    void setTextureArray();
    void setIndexArray();

    void setVertexArrayBoard();
    void setTextureArrayBoard(float textureUSize, float textureVSize);
    void setIndexArrayBoard();

    float getLeftLength() { return leftLength; }
    float getRightLength() { return rightLength; }
    void setCoordForPoint(int index, float x, float y, float z);
    void setCoordsForControls(float x1, float y1, float z1,
                              float x2, float y2, float z2,
                              float x3, float y3, float z3);
    vec3 getCoordOfPoint(int index);
    void getWindowCoord(double x, double y, double z, double  &wx, double  &wy, double  &wz);
    float getBoardWidth();
    bool getBoardShowStatus();
    bool calculateLinesIntersection(float a1, float b1, float c1,
                                    float a2, float b2, float c2,
                                    float& x, float& y);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
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
    void setLeft(double value);
    void setRight(double value);
    void setLeftRightLength(float left, float right);
    void setBoardWidth(double width);
    void setBoardShowStatus(bool status);

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLubyte> indexArray;

    static bool log;
public:
    QVector<GLfloat> vertexArrayBoard;
    float texture1USize, texture1VSize;
    float texture2USize, texture2VSize;
    QString texture1, texture2;
    float angleRounding;

private:
    QVector<GLfloat> textureArrayBoard;
    QVector<GLubyte> indexArrayBoard;

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

    bool right;
    QString description;
    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual RoadElement *getCopy();
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual void clearProperties(QLayout *layout);
    virtual QJsonObject getJSONInfo();
};

#endif // CURVE_H
