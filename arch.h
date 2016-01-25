#ifndef ARCH_H
#define ARCH_H
#include "roadelement.h"
#include "pole.h"

class Arch : public RoadElement
{
    Q_OBJECT
public:
    Arch();
    Arch(float x, float y, float width, float z, float height);
    Arch(float x, float y, float width, float z, float height, float rotation);
    Arch(const Arch& source);
    virtual ~Arch();
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
    virtual void getProperties(QVBoxLayout *layout, QGLWidget *render);
    virtual bool isFixed();
    virtual int getLayer();
    virtual void clear();
    virtual RoadElement *getCopy();

signals:
    void widthChanged(double value);
    void rotationChanged(double value);
    void heightChanged(double value);
    void zChanged(double value);

public slots:
    virtual bool setFixed(bool fixed);
    void setWidth(double value);
    void setRotation(double value);
    void setHeight(double value);
    void setZ(double value);

private:
    bool fixed;
    bool selected;
    int layer;
    QVector<Pole*> polesVertical;
    QVector<Pole*> polesHorizontal;
    QVector<Pole*> poles;

    float width;
    float z;
    float height;
    float rotation;

    // RoadElement interface
public:
    virtual void rotate(float angle, float x, float y, float z);
    virtual void clearProperties(QLayout *layout);
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3> &controls);
    virtual QJsonObject getJSONInfo();
};

#endif // ARCH_H
