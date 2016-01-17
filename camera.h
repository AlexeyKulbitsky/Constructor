#ifndef CAMERA_H
#define CAMERA_H

#include <QFormLayout>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QSplitter>
#include "model.h"
#include <GL/glu.h>
#include "roadelement.h"
#include "cameraview.h"

struct Lens
{
    QString name;
    float minimum;
    float maximum;
};

struct Sensor
{
    QString name;
    QString diagonal;
    float width;
    float height;
    int resolutionWidth;
    int resolutionHeight;
    int usablePart;
    QVector<Lens> lenses;
};

class Camera : public RoadElement
{
    Q_OBJECT
public:
    Camera(float x, float y, CameraView* camera);
    Camera(float x, float y, float height,
           float horizontalAngle, float verticalAngle,
           int sensor, int lens, float focalLength,
           const QString &cameraName,
           CameraView* camera);
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
    virtual RoadElement *getCopy();

    void setVertexArray();
    void setColorArray(float red, float green, float blue);
    void setIndexArray();

    void setFOVVertexArray();
    void setFOVColorArray();
    void setFOVIndexArray();

    void setFOVVertexArrayNear();
    void setFOVColorArrayNear();
    void setFOVIndexArrayNear();

    void setFOVVertexArrayFar();
    void setFOVColorArrayFar();
    void setFOVIndexArrayFar();

    void setCamera(CameraView* camera);
    void calculateFOV();
    static void readSensors(const QString& source);
    static void saveSensors(const QString& source);

    float getDX() { return dx; }
    float getDY() { return dy; }
    float getHeight() { return height; }
    float getHorizontalAngle() { return horizontalAngle; }
    float getVerticalAngle() { return verticalAngle; }
    float getFOV() { return yFOV; }
    float getDeltaAngle() { return deltaAngle; }
    float getAspectRatio() { return aspectRatio; }

signals:
    void horizontalAngleChanged(double value);
    void verticalAngleChanged(double value);
    void heightChanged(double value);
    void dxChanged(double value);
    void dyChanged(double value);
    void FOVChanged(double value);
    void yFOVChanged(double value);
    void xFOVChanged(double value);
    void diagonalNameChanged(QString name);
    void sensorWidthChanged(double value);
    void sensorHeightChanged(double value);
    void resolutionWidthChanged(int value);
    void resolutionHeightChanged(int value);
    void usablePartChanged(int value);
    void focalLengthChanged(double value);
    void sensorNameChanged(const QString &name);
    void sensorTypeChanged(int value);
    void minimumFocalLengthChanged(double value);
    void maximumFocalLengthChanged(double value);
    
public slots:
    virtual bool setFixed(bool fixed);
    void setHorizontalAngle(double value);
    void setVerticalAngle(double value);
    void setHeight(double value);
    void setDX(double value);
    void setDY(double value);
    void setDiagonalName(const QString &name);
    void setCameraName(const QString &name);
    void setSensorName(const QString &name);
    void setLensName(const QString &name);
    void setSensorWidth(double value);
    void setSensorHeight(double value);
    void setResolutionWidth(int value);
    void setResolutionHeight(int value);
    void setUsablePart(int value);
    void setFocalLength(double value);
    void setSensor(int sensorType);
    void setLens(int lensType);
    void setXFOV(double value);
    void setYFOV(double value);
    void addSensor();
    void addLens();
    void setMinimumFocalLength(double value);
    void setMaximumFocalLength(double value);
    void deleteSensor();
    void deleteLens();
    void saveSensor();
    void saveLens();

private:
    static QVector<Sensor> sensors;

    QVector<GLfloat> vertexArray;
    QVector<GLfloat> colorArray;
    QVector<GLubyte> indexArray;

    QVector<GLfloat> FOVVertexArray;
    QVector<GLfloat> FOVColorArray;
    QVector<GLubyte> FOVIndexArray;

    QVector<GLfloat> FOVVertexArrayNear;
    QVector<GLfloat> FOVColorArrayNear;
    QVector<GLubyte> FOVIndexArrayNear;

    QVector<GLfloat> FOVVertexArrayFar;
    QVector<GLfloat> FOVColorArrayFar;
    QVector<GLubyte> FOVIndexArrayFar;

    bool selected, fixed;
    int layer;

    CameraView* cameraView;

    float focalLength;
    float sensorWidth;
    float sensorHeight;
    int resolutionWidth;
    int resolutionHeight;
    int usablePart;
    float deltaAngle;
    float minimumFocalLength;
    float maximumFocalLength;
    static const float inchToMm = 16.93f;
    float zNear;
    float zFar;
    float aspectRatio;
    QString diagonalName;
    QString cameraName;
    float horizontalAngle;
    float verticalAngle;
    float dx, dy;
    float height;
    float xFOV, yFOV;
    int nearPixels;
    int farPixels;
    float nearWidth, nearHeight;
    float nearLength;
    float farWidth, farHeight;
    float farLength;
    int currentSensor;
    int currentLens;

public:
    QFormLayout *properties;

    QSpinBox *usableSensorSpinBox;
    QCheckBox *editSensorCheckBox;
    QCheckBox *editLensCheckBox;
    QDoubleSpinBox *minimumFocalSpinBox;
    QDoubleSpinBox *maximumFocalSpinBox;
    QCheckBox *fixedCheckBox;
    QLineEdit *cameraNameLineEdit;
    QComboBox *sensorTypeComboBox;
    QLineEdit *sensorDiagonalLineEdit;
    QDoubleSpinBox *sensorWidthSpinBox;
    QDoubleSpinBox *sensorHeightSpinBox;
    QSpinBox *resolutionWidthSpinBox;
    QSpinBox *resolutionHeightSpinBox;
    QComboBox *lensComboBox;
    QDoubleSpinBox *focalLengthSpinBox;
    QDoubleSpinBox *xFOVSpinBox;
    QDoubleSpinBox *yFOVSpinBox;
    QDoubleSpinBox *mountingHeightSpinBox;
    QDoubleSpinBox *horizontalAngleSpinBox;
    QDoubleSpinBox *verticalAngleSpinBox;
    QLineEdit *lensLineEdit;
    QLineEdit *sensorLineEdit;
    QPushButton *saveCameraButton;
    QPushButton *deleteSensorButton;
    QPushButton *deleteLensButton;
    QPushButton *saveSensorButton;
    QPushButton *saveLensButton;

    void createProperties();
    // RoadElement interface
public:
    virtual void clearProperties(QLayout *layout);
    
    // RoadElement interface
public:
    virtual QJsonObject getJSONInfo();
};



#endif // CAMERA3D_H
