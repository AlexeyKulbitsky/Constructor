#ifndef ROADELEMENT
#define ROADELEMENT

#include <QtGui>
#include <math.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QGLWidget>
#include "stepdialog.h"
#include <QMap>
#include "texturemanager.h"

struct vec3
{
    float x;
    float y;
    float z;
    vec3():x(0.0f),y(0.0f),z(0.0f){}
    vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};
struct vec2
{
    float x;
    float y;
    vec2():x(0.0f),y(0.0f){}
    vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

struct Point
{
    float position[3];// = {0.0f, 0.0f, 0.0f};
};
struct Textute
{
    float position[2];// = {0.0f, 0.0f};
    void operator= (Textute& source)
    {
        position[0] = source.position[0];
        position[1] = source.position[1];
    }
};

struct Face
{
    unsigned short point[3];// = {0, 0, 0};
    void operator= (Face& source)
    {
        point[0] = source.point[0];
        point[1] = source.point[1];
        point[2] = source.point[2];
    }
};

struct Vertex
{
    float position[3];
    float color[3];// = {0.5f, 0.5f, 0.5f};
    float texture[2];// = {0.0f, 0.0f};
    float normal[3];// = {0.0f, 0.0f, 0.0f};
};

struct Mesh
{
    char name[21];
    std::vector<Vertex> vertices;
    float Ka[3];
    float Kd[3];
    float Ks[3];
    float d; // Прозрачность 0..1
    float Ns; // Блеск
    int illum;

    char map_Ka[256];// = {0};
    char map_Kd[256];// = {0};
    char map_Ks[256];// = {0};

    unsigned map_Ka_ID;// = 0;
    unsigned map_Kd_ID;// = 0;
    unsigned map_Ks_ID;// = 0;

};

struct MaterialMesh
{
    QString materialName;
    QVector<GLushort> faces;
};

struct Mesh_3DS
{
    QString name;
    QVector<GLfloat> vertices;
    QVector<GLfloat> normals;
    QVector<GLfloat> textures;
    QVector<GLfloat> colors;
    QVector<GLushort> faces;

    QVector<MaterialMesh> materialMeshes;
};



struct Material
{
    QString name;
    float ambientColor[3];
    float diffuseColor[3];
    float specularColor[3];

    QString textureSource;
    unsigned int textureID = 0;
};




enum RoadElementName{ROAD_SIMPLE = 0,
                     ROAD_BROKEN,
                     CROOKED_ROAD, LINE_SIMPLE, LINE_BROKEN, ROUNDING_ROAD};

class RoadElement: public QObject
{
    Q_OBJECT

public:

    virtual bool isSelected() = 0;
    virtual void setSelectedStatus(bool status) = 0;
    virtual void drawFigure(QGLWidget* render = 0) = 0;
    virtual void drawSelectionFrame() = 0;
    virtual void drawMeasurements(QGLWidget* render = 0) = 0;
    virtual void move(float dx, float dy, float dz = 0) = 0;
    virtual void drawControlElement(int index, float lineWidth, float pointSize) = 0;
    virtual std::vector<vec3> getCoordOfControl(int index) { std::vector<vec3> p; return p;}
    virtual void setCoordForControl(int index) {}
    virtual QCursor getCursorForControlElement(int index) = 0;
    virtual void resizeByControl(int index, float dx, float dy, float x, float y) = 0;
    virtual int getNumberOfControls() = 0;
    virtual int controlsForPoint() = 0;
    virtual void changeColorOfSelectedControl(int index) = 0;
    virtual QString getName() { return name; }
    virtual QJsonObject getJSONInfo() {  QJsonObject a; return a;}
    virtual void getProperties(QFormLayout* layout, QGLWidget* render = 0) = 0;
    virtual void clearProperties(QLayout* layout)
    {
        /*
        QLayoutItem *item;
        while((layout->count() > 0)) {
            item = layout->itemAt(0);
            if (item->layout()) {
                clearProperties(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        */
    }

    virtual bool isFixed() = 0;
    virtual void addElement(RoadElement* element){}
    virtual void deleteElement(int index){}
    virtual int getNumberOfElements(){return 0;}
    virtual RoadElement* getElement(int index) { return NULL; }
    virtual ~RoadElement(){}
    virtual int getLayer() = 0;
    virtual void clear() = 0;
    virtual void setStepDialog(StepDialog* dialog)
    {
        if (dialog)
        stepDialog = dialog;
    }
public slots:
    virtual bool setFixed(bool fixed) = 0;

protected:
    QString name;
    StepDialog* stepDialog;
public:
    //static QMap<QString, int> texturesPool;
};

//QMap<QString, int> RoadElement:ureManager:texturesPool;


#endif // ROADELEMENT

