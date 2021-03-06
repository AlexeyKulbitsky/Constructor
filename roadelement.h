#ifndef ROADELEMENT
#define ROADELEMENT

#include <QMessageBox>
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
#include "logger.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <QUndoStack>
#include "commands.h"

class Model;
//#include "model.h"


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
    vec3 operator- (const vec3& p)
    {
        vec3 res;
        res.x = this->x - p.x;
        res.y = this->y - p.y;
        res.z = this->x - p.z;
        return res;
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

    Vertex& operator= (const Vertex& source)
    {
        for (int i = 0; i < 3; ++i)
        {
            position[i] = source.position[i];
            color[i] = source.color[i];
            normal[i] = source.normal[i];
        }
        texture[0] = source.texture[0];
        texture[1] = source.texture[1];
        return *this;
    }
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
    unsigned int textureID;
    Material()
    {
        name = '\0';
        ambientColor[0] = ambientColor[1] = ambientColor[2] = 0.0f;
        diffuseColor[0] = diffuseColor[1] = diffuseColor[2] = 0.0f;
        specularColor[0] = specularColor[1] = specularColor[2] = 0.0f;
        textureSource = '\0';
        textureID = 0;
    }
};




enum RoadElementName{ROAD_SIMPLE = 0, ROAD_BROKEN,
                     CROOKED_ROAD, LINE_SIMPLE, LINE_BROKEN, ROUNDING_ROAD};

class RoadElement: public QObject
{
    Q_OBJECT
public:
    RoadElement();
    virtual ~RoadElement();
    static QUndoStack* undoStack;
    virtual bool isSelected() = 0;
    virtual void setSelectedStatus(bool status) = 0;
    virtual void drawFigure(QGLWidget* render = 0) = 0;
    virtual void drawSelectionFrame() = 0;
    virtual void drawMeasurements(QGLWidget* render = 0) = 0;
    virtual void move(float dx, float dy, float dz = 0) = 0;
    virtual void drawControlElement(int index, float lineWidth, float pointSize) = 0;
    virtual std::vector<vec3> getCoordOfControl(int index);
    virtual void setCoordForControl(int index, std::vector<vec3>& controls);
    virtual QCursor getCursorForControlElement(int index) = 0;
    virtual void resizeByControl(int index, float dx, float dy, float x, float y) = 0;
    virtual int getNumberOfControls() = 0;
    virtual int controlsForPoint() = 0;
    virtual void changeColorOfSelectedControl(int index) = 0;
    virtual QString getName();
    virtual void setName(const QString& name) { this->name = name; }
    virtual QJsonObject getJSONInfo();
    virtual void getProperties(QVBoxLayout* layout, QGLWidget* render = 0) = 0;
    virtual void clearProperties(QLayout* layout);
    virtual void setModel(Model* model);
    virtual bool isFixed() = 0;
    virtual void addElement(RoadElement* element);
    virtual void deleteElement(int index);
    virtual int getNumberOfElements();
    virtual RoadElement* getElement(int index);    
    virtual int getLayer() = 0;
    virtual void clear() = 0;
    virtual void rotate(float angle, float x, float y, float z);
    virtual void rotate(float angle, float x0, float y0, float &x, float &y);
    virtual void setStepDialog(StepDialog* dialog);
    virtual void setStepDialogs(StepDialog** dialogs, int size);
    virtual void deleteLine(RoadElement* line);
    static void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    static void getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    virtual float getElementX() { return elementX; }
    virtual float getElementY() { return elementY; }
    virtual RoadElement* getCopy() = 0;
    virtual int getId() { return Id; }
    virtual void setId(int Id);
    virtual bool calculateLinesIntersection(float a1, float b1, float c1,
                                    float a2, float b2, float c2,
                                    float& x, float& y);
    virtual bool calculateLinesIntersection(vec2 p1, vec2 p2,
                                            vec2 s1, vec2 s2,
                                            float& x, float& y);
    virtual int calculateLineCircleIntersection(float x0, float y0, float r, // центр и рдиус окружности
                                                float x1, float y1,           // точки
                                                float x2, float y2,           //    отрезка
                                                float& xa, float &ya,         // резуль-
                                                float& xb, float &yb);
    virtual float calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
    virtual float calculateAngle(vec3 v1, vec3 v2);
    virtual unsigned int getSize() { return sizeof(*this); }
    virtual void setDrawRoadStatus(bool status) { drawRoad = status; qDebug() << name << "drawRoad =" << status;}
    virtual void setDrawLinesStatus(bool status) { drawLines = status; qDebug() << name << "drawLine =" << status;}
    virtual inline StepDialog* getStepDialog() { return stepDialog; }
public slots:
    virtual bool setFixed(bool fixed) = 0;
    virtual void setShowMeasurements(bool status);

protected:
    QString name;
    StepDialog* stepDialog;
    StepDialog* stepDialogs[10];
    Model* model;
    float elementX, elementY;
    int Id;
    bool showMeasurements;
    bool drawRoad;
    bool drawLines;

private:
    static const unsigned SIZE = 10000;
    static bool Ids[SIZE];
};
Q_DECLARE_METATYPE(RoadElement*)

class RoadElementMimeData : public QMimeData
{
    Q_OBJECT
    // QMimeData interface
public:
    RoadElementMimeData(RoadElement* element);
    RoadElementMimeData(QList<RoadElement*>& elements);
    RoadElementMimeData();
    ~RoadElementMimeData();
    QList<RoadElement *> getElements() const;
protected:
    virtual QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const;

private:
    QList<RoadElement*> elements;
    QStringList myFormats;
    // QMimeData interface
public:
    virtual QStringList formats() const;
    virtual bool hasFormat(const QString &mimetype) const;
};


#endif // ROADELEMENT

