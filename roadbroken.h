#ifndef ROADBROKEN_H
#define ROADBROKEN_H

#include "roadelement.h"
#include <QVector>
#include <GL/glu.h>
#include <QListWidget>
#include <QTreeWidget>
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

class Poly: public QObject
{
    Q_OBJECT
public:
    Poly():leftLength(0.0f), rightLength(0.0f), topLength(0.0f), bottomLength(0.0f){}
    Poly(const Poly &) {}
    virtual ~Poly() {}
public slots:
    void setLeftLength(double value)
    {
        if (value == leftLength)
            return;
        leftLength = value;
        emit leftLengthChanged(value);
    }
    void setRightLength(double value)
    {
        if (value == rightLength)
            return;
        rightLength = value;
        emit rightLengthChanged(value);
    }
    void setTopLength(double value)
    {
        if (value == topLength)
            return;
        topLength = value;
        emit topLengthChanged(value);
    }
    void setBottomLength(double value)
    {
        if (value == bottomLength)
            return;
        bottomLength = value;
        emit bottomLengthChanged(value);
    }

signals:
    void leftLengthChanged(double value);
    void rightLengthChanged(double value);
    void topLengthChanged(double value);
    void bottomLengthChanged(double value);

private:
    float leftLength;
    float rightLength;
    float topLength;
    float bottomLength;

public:
    float getLeftLength() { return leftLength; }
    float getRightLength() { return rightLength; }
    float getTopLength() { return topLength; }
    float getBottomLength() { return bottomLength; }
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
    RoadBroken(QVector<float>& vertexArray,
               QVector<float>& vertexArrayRight,
               QVector<float>& vertexArrayLeft,
               QString texture_1, float texture_1Usize, float texture_1Vsize,
               QString texture_2, float texture_2Usize, float texture_2Vsize,
               bool showRightBoard, bool showLeftBoard, bool fixed,
               QString name, int layer);

    RoadBroken(const RoadBroken& source);
    ~RoadBroken();
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

    QVector<LineLinkedToRoad> lines;

    QVector<Poly> polys;

    int textureID[2];
    float texture_1Usize, texture_1Vsize;
    float texture_2Usize, texture_2Vsize;
    QString texture1, texture2;
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
    double axisStep;
    bool singleWay;
    int splitZoneType;
    double splitZoneHeight;
    QVBoxLayout* layout;
    QGLWidget* render;
    static bool log;
    QListWidget *list;
    QTreeWidget* tree;
    LineLinkedToRoad currentLineLinked;

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

    void getVertexArrayForLineAxis(QVector<float> &axisArray, LineLinkedToRoad &line);
    void getVertexArrayForStopLine(QVector<float> &axisArray, LineLinkedToRoad &line);

    GLuint getTextures(QString source);
    void setIndexArray();
    void addBreak(bool front);
    void deleteBreak(bool front);
    void setIndexArrayForSelectionFrame();
    void setColorArrayForSelectionFrame(float red, float green, float blue);
    void calculatePolys();

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
    void resizeByControl(int index, float dx, float dy, float x, float y, int keyBoardKey);
    virtual int getNumberOfControls();
    virtual int controlsForPoint();
    virtual void changeColorOfSelectedControl(int index);
    virtual void clear();
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual int getLayer();
    virtual void drawMeasurements(QGLWidget *render);

    virtual QJsonObject getJSONInfo();
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    static bool getLogging();
signals:
    void rightBoardWidthChanged(double width);
    void leftBoardWidthChanged(double width);

    void linesChanged(QVBoxLayout* layout, QGLWidget* render);
    void lineAdded();
    void lineDeleted();

public slots:
    static void setLogging(bool status);
    virtual bool setFixed(bool fixed);

    void setRightBoardShowStatus(bool status);
    void setLeftBoardShowStatus(bool status);

    void setRightBoardWidth(double width);
    void setLeftBoardWidth(double width);

    virtual bool isFixed();

    void constructLine(QString textureSource, float textureSize);
    void constructLine(LineLinkedToRoad line);

    void addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide);
    void addLine();
    void addLine(LineBrokenLinkedToRoadBroken line);
    void addLine(LineLinkedToRoad line);
    void editLine();
    void editLine(LineLinkedToRoad line);

    void setRightSide(bool status);
    void setStep(double value);
    void setLineType(int type);
    void deleteLine();
    void deleteLine(LineBrokenLinkedToRoadBroken line);
    void deleteLine(LineLinkedToRoad line);
    void removeLine(LineLinkedToRoad line);

    void resetLines();
    void setBeginStep(double step);
    void setEndStep(double step);
    void setSplitZoneWidth(double value);
    void setBeginSide(bool status);
    void setBeginRounding(bool status);
    void setEndRounding(bool status);
    void setDifferentDirections(bool status);
    void setSingleWay(bool status);
    void setAxisStep(double step);
    void setSplitZoneType(int type);
    void setSplitZoneHeight(double height);
    virtual void getProperties(QVBoxLayout *layout, QGLWidget* render = 0);
    void updateListWidget();
    void updateTreeWidget();

    void setLeftLength(double value);
    void setRightLength(double value);
    void setTopLength(double value);
    void setBottomLength(double value);
    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);
    virtual void deleteLine(RoadElement *line);
    virtual RoadElement *getCopy();
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
};

#endif // ROADBROKEN_H
