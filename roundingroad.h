#ifndef ROUNDING
#define ROUNDING
#include <GL/gl.h>
#include <GL/glu.h>
#include "roadelement.h"
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include "linebroken.h"
#include "splitzone.h"

struct LineBrokenLinked
{
    RoadElement *line;// = NULL;
    int lineType;// = -1;
    float lineWidth;
    bool nearSide;// = true;
    bool beginSide;
    float step;
    float beginStep;
    float endStep;
    bool differentDirections;
    float splitZoneWidth;
};



class RoundingRoad: public RoadElement
{
    Q_OBJECT
public:

    bool isSelected() { return selected; }
    void setSelectedStatus(bool status);

    void drawFigure(QGLWidget* render = 0);
    void drawSelectionFrame();
    void move(float dx, float dy, float dz = 0);
    void drawControlElement(int index, float lineWidth = 5.0, float pointSize = 5.0);
    QCursor getCursorForControlElement(int index);
    void resizeByControl(int index, float dx, float dy, float x, float y);
    int getNumberOfControls();
    int controlsForPoint() { return 1;}

private:
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLuint> indexArray; // количество полигонов
    QVector<GLfloat> textureArray;

    QVector<LineLinkedToRoad> lines;

    GLubyte* indexArrayForSelection;
    GLfloat* colorArrayForSelection;

    int numberOfVertices; // количество вершин
    int numberOfPolygones;
    bool selected;
    bool fixed;

    float xCenterNearRadius, yCenterNearRadius;
    float angel1NearRadius, angel2NearRadius;
    float xCenterFarRadius, yCenterFarRadius;
    float angel1FarRadius, angel2FarRadius;
    float farRadius, nearRadius;
    float numberOfSides;
    int indexOfSelectedControl;
    int layer;
    float texture_1Usize, texture_1Vsize;
    float texture_2Usize, texture_2Vsize;
    QString texture1, texture2;
    bool showNearBoard;
    bool showFarBoard;
    float nearBoardWidth;
    float farBoardWidth;
    QVector<GLfloat> vertexArrayNear;
    QVector<GLfloat> vertexArrayFar;

    QVector<GLfloat> textureArrayNear;
    QVector<GLfloat> textureArrayFar;

    QVector<GLuint> indexArrayNear;
    QVector<GLuint> indexArrayFar;

    int textureID[2];

    bool nearSide;
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
    LineLinkedToRoad currentLineLinked;
    QListWidget *list;

public:
    RoundingRoad();
    RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                 float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius,
                 int numberOfSides, QString name, int layer);
    RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                 float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius,
                 int numberOfSides, QString name, int layer,
                 QString texture_1, float texture_1Usize, float texture_1Vsize,
                 QString texture_2, float texture_2Usize, float texture_2Vsize);
    RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                 float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius,
                 int numberOfSides, QString name, int layer,
                 float nearBoardWidth, float farBoardWidth,
                 bool showNearBoard, bool showFarBoard, bool fixed,
                 QString texture_1, float texture_1Usize, float texture_1Vsize,
                 QString texture_2, float texture_2Usize, float texture_2Vsize);
    RoundingRoad(const RoundingRoad& source);
    ~RoundingRoad();

private:
    void setVertexArray(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                        float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius,
                        int numberOfSides);
    void setColorArray(float red, float green, float blue);
    void setIndexArray();
    void setTextureArray(float textUsize, float textVsize);
    void setNearTextureArray(float textUsize, float textVsize);
    void setFarTextureArray(float textUsize, float textVsize);
    void setColorArrayForSelectionFrame(float red, float green, float blue);
    void setIndexArrayForSelectionFrame();
    GLuint getTextures(QString source);
    void getWindowCoord(double x, double y, double z, double  &wx, double  &wy, double  &wz);

    // RoadElement interface
public:
    void changeColorOfSelectedControl(int index);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();

    // RoadElement interface
public:


signals:
    void nearRadiusChanged(double nearRadius);
    void angel_1_NearRadiusChanged(double angel1);
    void angel_2_NearRadiusChanged(double angel2);

    void farRadiusChanged(double farRadius);
    void angel_1_FarRadiusChanged(double angel1);
    void angel_2_FarRadiusChanged(double angel2);

    void linesChanged(QVBoxLayout* layout, QGLWidget* render);

    void nearBoardWidthChanged(double width);
    void farBoardWidthChanged(double width);
    void lineAdded();
    void lineDeleted();

public slots:
    void setNearRadius(double nearRadius);
    void setAngel_1_NearRadius(double angel1);
    void setAngel_2_NearRadius(double angel2);

    void setFarRadius(double farRadius);
    void setAngel_1_FarRadius(double angel1);
    void setAngel_2_FarRadius(double angel2);

    void addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool nearSide);
    void addLine();
    void addLine(LineBrokenLinked line);
    void addLine(LineLinkedToRoad line);

    void constructLine(QString textureSource, float textureSize);
    void constructLine(LineLinkedToRoad line);
    void calculateVertexArray(LineLinkedToRoad &line, QVector<float>& vertexArray);

    void setNearSide(bool status);
    void setStep(double value);
    void setLineType(int type);
    void deleteLine();
    void deleteLine(LineBrokenLinked line);
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

    void setShowNearBoard(bool status);
    void setShowFarBoard(bool status);

    void setNearBoardWidth(double width);
    void setFarBoardWidth(double width);

    virtual void getProperties(QVBoxLayout *layout, QGLWidget* render = 0);

    void updateListWidget();
    void editLine();
    void editLine(LineLinkedToRoad line);

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

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual void deleteLine(RoadElement *line);
    static bool getLogging() { return log; }
    static void setLogging(bool status);

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
};

#endif // ROUNDING

