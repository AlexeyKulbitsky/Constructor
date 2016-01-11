#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H


#include <QApplication>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QGLWidget>
class Model;
class Camera;

class CameraView : public QGLWidget
{
    Q_OBJECT
public:
    explicit CameraView(QWidget* parent = 0, QGLWidget *shared = 0);

    void getProperties(QFormLayout *layout);
    void setModel(Model* model);
    void setTranslation(float dx, float dy, float dz);
    void translate(float dx, float dy, float dz);
    void setProjection(float FOV, float aspectRatio, float zNear, float zFar);
    void setActive(bool status);
    bool isActive() { return active; }
    void setCamera(Camera *camera);

private:
    Model *model;
    Camera *camera;
    float dx, dy, m_height;
    float m_horizontalAngle;
    float m_verticalAngle;

    float m_FOV, m_aspectRatio, m_zFar, m_zNear;
    float deltaAngle;
    bool leftButton;
    QPoint mousePosition;
    bool active;
    bool firstTime;

signals:
    void horizontalAngleChanged(double value);
    void verticalAngleChanged(double value);
    void heightChanged(double value);
    void dxChanged(double value);
    void dyChanged(double value);

public slots:
    void setHeight(double value);
    void setDX(double value);
    void setDY(double value);
    void setHorizontalAngle(double value);
    void setVerticalAngle(double value);
    void setFOV(double value);
    void setAspectRatio(double value);
    void setDeltaAngle(double value);
    // QGLWidget interface
protected:


    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);
};

#endif // CAMERAVIEW_H
