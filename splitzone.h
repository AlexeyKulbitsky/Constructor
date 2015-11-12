#ifndef SPLITZONE_H
#define SPLITZONE_H

#include "linebroken.h"

class SplitZone: public LineBroken
{
    Q_OBJECT
public:
    SplitZone();
    SplitZone(float *pointsArray, int size,
              float width,
              bool beginRounding,
              bool endRounding,
              QString description = '\0');
    SplitZone(float *pointsArray, int size,
              float width,
              bool beginRounding,
              bool endRounding,
              int type,
              double height,
              QString texture_1,
              float texture1USize, float texture1VSize,
              QString texture_2,
              float texture2USize, float texture2VSize,
              QString description = '\0');
    SplitZone(float x1, float y1, float z1,
              float x2, float y2, float z2,
              float width,
              bool beginRounding,
              bool endRounding,
              QString description = '\0');
    SplitZone(float x1, float y1, float z1,
              float x2, float y2, float z2,
              float width,
              bool beginRounding,
              bool endRounding,
              int type,
              double height,
              QString texture_1,
              float texture1USize, float texture1VSize,
              QString texture_2,
              float texture2USize, float texture2VSize,
              QString description = '\0');
    SplitZone(const SplitZone& source);
    virtual ~SplitZone();
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

signals:
    void widthChanged(double width);
    void heightChanged(double height);
    void lineWidthChanged(double lineWidth);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double width);
    void setHeight(double height);
    void setLineWidth(double value);

private:
    LineBroken *line;
    bool selected, fixed;
    int layer;

    bool beginRounding;
    bool endRounding;
    vec3 pBegin, pEnd;
    vec3 pPerpBegin, pPerpEnd;
    float width;
    int numberOfSides;

    GLfloat *lineAxisArray;
    int size;
    QVector<GLfloat> axisArray;
    float lineWidth;
    vec3 p1, p2, p3, p4;
    vec3 line1_p1, line1_p2, line2_p1, line2_p2;
    QString description;
    float xCenter, yCenter;
    int type;
    float height;
    int indexOfSelectedControl;

    QVector<GLfloat> boardVertexArray;
    QVector<GLfloat> boardTextureArray;
    QVector<GLuint> boardIndexArray;
    QVector<GLfloat> vertexArray;
    QVector<GLfloat> textureArray;
    QVector<GLuint> indexArray;

    int textureID[2];
    float texture1USize, texture1VSize;
    float texture2USize, texture2VSize;
    QString texture1, texture2;
    static bool log;

public:
    void calculateLine(vec3 p1, vec3 p2, float width);
    void calculateLine(float *pointsArray, int size,
                       float width);
    void calculateLine(float *pointsArray, int size);
    void calculateLine(QVector<GLfloat>& pointsArray, float width);

    void reset();
    void setBoardVertexArray();
    void setBoardTextureArray(float textureUsize, float textureVsize);
    void setBoardIndexArray();

    void setVertexArray();
    void setTextureArray(float textureUSize, float textureVSize);
    void setIndexArray();

    float getWidth();
    void setDescription(const QString &description);
    void drawDescription(QGLWidget* render = 0, float red = 1.0f, float green = 1.0f, float blue = 1.0f);
    void rotate(float angle, float x, float y, float z);
    void addBreak(bool front);
    void deleteBreak(bool front);
    static bool getLogging() { return log; }
    static void setLogging(bool status);

    // RoadElement interface
public:
    virtual RoadElement *getCopy();

    // RoadElement interface
public:
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);

    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);

    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};

#endif // SPLITZONE_H
