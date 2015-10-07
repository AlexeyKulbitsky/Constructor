#ifndef ROADBROKEN_H
#define ROADBROKEN_H

#include "roadelement.h"
#include <QVector>
#include <GL/glu.h>
#include "linebroken.h"
#include "splitzone.h"

struct LineBrokenLinkedToRoadBroken
{
    RoadElement *line;// = NULL;
    int lineType;// = -1;
    float lineWidth;
    bool rightSide;// = true;
    float step;
     bool beginSide;
     float beginStep;
     float endStep;
     bool differentDirections;
     float splitZoneWidth;
     bool beginRounding;
     bool endRounding;
    QString textureSource;
};

class RoadBroken: public RoadElement
{
    Q_OBJECT
public:
    RoadBroken();
    RoadBroken(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name,int layer);
    RoadBroken(QVector<GLfloat>& vertexArray,float red, float green, float blue, float alpha, QString name, int layer);
    RoadBroken(float x1, float y1, float x2, float y2, float width,
               QString texture_1, float texture_1Usize, float texture_1Vsize,
               QString texture_2, float texture_2Usize, float texture_2Vsize,
               QString name, int layer);

protected:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLubyte> indexArray; // количество полигонов
    QVector<GLfloat> textureArray;

    QVector<GLfloat> vertexArrayForSelection;
    QVector<GLubyte> indexArrayForSelection;
    QVector<GLfloat> colorArrayForSelection;

    QVector<GLfloat> vertexArrayRight;
    QVector<GLfloat> vertexArrayLeft;

    QVector<GLfloat> textureArrayRight;
    QVector<GLfloat> textureArrayLeft;

    QVector<GLubyte> indexArrayRight;
    QVector<GLubyte> indexArrayLeft;

    QVector<LineBrokenLinkedToRoadBroken> lines;

    int textureID[2];
    float texture_1Usize, texture_1Vsize;
    float texture_2Usize, texture_2Vsize;
    float rightBoardWidth;
    float leftBoardWidth;
    bool showRightBoard;
    bool showLeftBoard;
    bool fixedRightWidth, fixedLeftWidth;
    GLfloat red, green, blue, alpha;
    GLfloat x1, y1, x2, y2;
    GLfloat width;
    GLfloat size;
    int layer;
    GLint hits;
    GLint indexOfSelectedControl;
    // Рамка для выделения
    bool selected;
    bool fixed;

    bool rightSide;
    double step;
    int lineType;

    bool beginSide;
    double beginStep;
    double endStep;
    bool beginRounding;
    bool endRounding;
    double splitZoneWidth;
    bool differentDirections;

    QFormLayout* layout;
    QGLWidget* render;

public:
    void setVertexArray(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat width);
    void setVertexArray(QVector<GLfloat>& vertexArray);
    void resetVertexArray(float dx, float dy, bool right);
    void setColorArray(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void setTextureArray(float textureUsize, float textureVsize);

    void setRightVertexArray();
    void resetRightVertexArray();
    void resetRightVertexArray(float dx, float dy);
    void setRightTextureArray(float textureUsize, float textureVsize);
    void setRightIndexArray();

    void setLeftVertexArray();
    void resetLeftVertexArray();
    void resetLeftVertexArray(float dx, float dy);
    void setLeftTextureArray(float textureUsize, float textureVsize);
    void setLeftIndexArray();

    void getVertexArrayForLineAxis(QVector<float> &axisArray, bool rightSide, float step, float beginStep, float endStep);

    GLuint getTextures(QString source);
    void setIndexArray();
    void addBreak(bool front);
    void setIndexArrayForSelectionFrame();
    void setColorArrayForSelectionFrame(float red, float green, float blue);

    // RoadElement interface
public:
    virtual bool isSelected();
    virtual void setSelectedStatus(bool status);
    virtual void drawFigure(QGLWidget* render = 0);
    virtual void drawSelectionFrame();
    virtual void move(float dx, float dy, float dz = 0);
    virtual void drawControlElement(int index, float lineWidth, float pointSize);
    virtual QCursor getCursorForControlElement(int index);
    virtual void resizeByControl(int index, float dx, float dy, float x, float y);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual void clear();
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual int getLayer();
    virtual void drawMeasurements(QGLWidget *render);

    virtual QJsonObject getJSONInfo();
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);

signals:
    void rightBoardWidthChanged(double width);
    void leftBoardWidthChanged(double width);

    void linesChanged(QFormLayout* layout, QGLWidget* render);

public slots:
    virtual bool setFixed(bool fixed);

    void setRightBoardShowStatus(bool status);
    void setLeftBoardShowStatus(bool status);

    void setRightBoardWidth(double width);
    void setLeftBoardWidth(double width);

    virtual bool isFixed();

    void addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide);
    void addLine();

    void setRightSide(bool status);
    void setStep(double value);
    void setLineType(int type);
    void deleteLine();
    void resetLines();
    void setBeginStep(double step);
    void setEndStep(double step);
    void setSplitZoneWidth(double value);
    void setBeginSide(bool status);
    void setBeginRounding(bool status);
    void setEndRounding(bool status);
    void setDifferentDirections(bool status);

    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);


    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);
};

#endif // ROADBROKEN_H
