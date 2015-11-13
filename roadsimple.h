#ifndef ROADSIMPLE_H
#define ROADSIMPLE_H

#include <QGLWidget>
#include "roadelement.h"
#include <QCheckBox>
#include <GL/glu.h>
#include "linesimple.h"

struct LineLinked
{
    RoadElement *line;// = NULL;
    bool beginSide;
    int lineType;// = -1;
    float lineWidth;
    bool rightSide;// = true;
    float step;
    float beginStep;
    float endStep;
    bool differentDirections;
    float splitZoneWidth;
};

class RoadSimple: public RoadElement
{
    Q_OBJECT
public:
    RoadSimple();
    RoadSimple(float x1, float y1, float x2, float y2, QString name,int layer, QString description = '\0');
    RoadSimple(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name, int layer,
               QString description = '\0');
    RoadSimple(float x1, float y1, float x2, float y2, float width,
               QString source_1, float textureSize_1_Usize, float textureSize_1_Vsize,
               QString source_2, float textureSize_2_Usize, float textureSize_2_Vsize,
               QString name, int layer,
               QString description = '\0');
    RoadSimple(float x1, float y1, float x2, float y2,
               float leftWidth, float rightWidth, float leftBoardWidth, float rightBoardWidth,
               bool showRightBoard, bool showLeftBoard,
               QString source_1, float textureSize_1_Usize, float textureSize_1_Vsize,
               QString source_2, float textureSize_2_Usize, float textureSize_2_Vsize,
               QString name, int layer, bool fixed,
               QString description = '\0');
    RoadSimple(const RoadSimple& source);
    virtual ~RoadSimple();

    void setVertexArray(float x1, float y1, float x2, float y2, float width);
    void setVertexArray();
    void setColorArray(float red, float green, float blue, float alpha);
    void setTextureArray();
    void setIndexArray();
    void setVertexArrayBoard();
    void setIndexArrayBoard();
    void setTextureArrayBoard();

    void setVertexArrayForSelection();
    void setIndexArrayForSelectionFrame();
    void setColorArrayForSelectionFrame(float red, float green, float blue);

    QPoint getCoorninateOfPointControl(int index);

    vec2 getAxisPoint_1();
    vec2 getAxisPoint_2();
    void setCoordForAxisPoint(int index, float x, float y);
    float getWidth() { return width; }
    void setCoordForPoint(int index, float newX, float newY, float newZ);
    vec3 getCoordOfPoint(int index);
    void setDescription(const QString& description);
    void drawDescription(QGLWidget* render = 0, float red = 1.0f, float green = 1.0f, float blue = 1.0f);

public:
    QVector<GLfloat> vertexArrayRight;
    QVector<GLfloat> vertexArrayLeft;

    QVector<LineLinked> lines;
protected:
    QVector<GLfloat> textureArrayRight;
    QVector<GLfloat> textureArrayLeft;

    QVector<GLubyte> indexArrayLeft;
    QVector<GLubyte> indexArrayRight;

    GLfloat VertexArray[4][3];
    GLfloat ColorArray[4][4];
    GLubyte IndexArray[2][3]; // количество полигонов
    GLfloat TextureArray[4][2];
    GLuint textureID[2];
    GLfloat texture1USize, texture1VSize;
    QString texture1Source;
    GLfloat texture2USize, texture2VSize;
    QString texture2Source;
    GLfloat z;
    bool useColor;
    GLfloat red, green, blue, alpha;
    float x1, y1, x2, y2;
    float xP1, yP1, xP2, yP2;
    float width, length;
    float rightWidth;
    float leftWidth;
    float rightBoardWidth, leftBoardWidth;
    float size;
    int layer;
    GLint hits;
    int indexOfSelectedControl;
    // Рамка для выделения
    GLfloat VertexArrayForSelection[8][3];
    GLubyte IndexArrayForSelection[4];
    GLfloat ColorArrayForSelection[4][3];
    bool selected;
    bool fixed;
    bool showRightBoard;
    bool showLeftBoard;

    bool rightSide;
    bool beginSide;
    bool beginRounding;
    bool endRounding;
    double splitZoneWidth;
    bool differentDirections;
    double step;
    int lineType;
    double beginStep;
    double endStep;
    double axisStep;
    bool singleWay;
    int splitZoneType;
    double splitZoneHeight;
    static bool log;
    QFormLayout* layout;
    QGLWidget* render;

    QString description;
public:

    bool hasPoint(GLfloat x, GLfloat y);
    void drawFigure(QGLWidget* render = 0);
    void drawSelectionFrame();
    void move(float dx, float dy, float dz = 0);
    void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    QCursor getCursorForControlElement(int index);
    void resizeByControl(int index, float dx, float dy, float x, float y);
    int getNumberOfControls();
    int controlsForPoint() { return 3;}
    void changeColorOfSelectedControl(int index);
    virtual bool isFixed();
    virtual void drawMeasurements(QGLWidget *render);

    virtual int getLayer();
    virtual QJsonObject getJSONInfo();
    virtual bool isSelected() { return selected; }
    virtual void setSelectedStatus(bool status);
    virtual void clear();
    float getRightBoardWidth();
    float getLeftBoardWidth();
    static bool getLogging() { return log; }
    float getLength();
signals:
    void widthChanged(double width);
    void lengthChanged(double length);
    void linesChanged(QFormLayout* layout, QGLWidget* render);
    void rightBoardWidthChanged(double width);
    void leftBoardWidthChanged(double width);
    void rightWidthChanged(double width);
    void leftWidthChanged(double width);
    void lineDeleted();

public slots:
    static void setLogging(bool status);
    void setWidth(double width);
    void setRightWidth(double width);
    void setLeftWidth(double width);
    void setLength(double length);
    void setRightBoardShowStatus(bool status);
    void setLeftBoardShowStatus(bool status);

    void setRightBoardWidth(double width);
    void setLeftBoardWidth(double width);

    void addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide, float beginStep, float endStep);
    void addLine();
    void addLine(LineLinked line);

    void setRightSide(bool status);
    void setBeginSide(bool status);
    void setBeginRounding(bool status);
    void setEndRounding(bool status);
    void setDifferentDirections(bool status);
    void setStep(double value);
    void setLineType(int type);
    void deleteLine();
    void deleteLine(LineLinked line);
    void setBeginStep(double step);
    void setEndStep(double step);
    void setSplitZoneWidth(double value);
    void setSingleWay(bool status);
    void setAxisStep(double step);
    void setSplitZoneType(int type);
    void setSplitZoneHeight(double height);
    virtual void getProperties(QFormLayout *layout, QGLWidget* render = 0);
    virtual bool setFixed(bool fixed);
    void resetLines();

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual void deleteLine(RoadElement *line);

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);

    // RoadElement interface
public:
    virtual QString getName();
};

#endif // ROADSIMPLE_H
