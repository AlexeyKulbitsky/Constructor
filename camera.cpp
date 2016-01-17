#include "camera.h"
#include <QDebug>
#include <QComboBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QtMath>
#include <QLineEdit>
#include <QObject>

QVector<Sensor> Camera::sensors;


Camera::Camera(float x, float y, CameraView *camera)
{

    deltaAngle = 0.0f;
    layer = 3;
    cameraName = "\0";
    cameraView = camera;
    name = "Camera";
    dx = x;
    dy = y;
    height = 0.0f;
    horizontalAngle = 0.0f;
    verticalAngle = 0.0f;
    currentSensor = 0;
    currentLens = 0;

    selected = fixed = false;
    nearPixels = 300;
    farPixels = 120;
    diagonalName = sensors[currentSensor].diagonal;
    sensorWidth = sensors[currentSensor].width;
    sensorHeight = sensors[currentSensor].height;
    resolutionWidth = sensors[currentSensor].resolutionWidth;
    resolutionHeight = sensors[currentSensor].resolutionHeight;
    minimumFocalLength = sensors[currentSensor].lenses[currentLens].minimum;
    maximumFocalLength = sensors[currentSensor].lenses[currentLens].maximum;
    focalLength = sensors[currentSensor].lenses[currentLens].minimum;
    usablePart = sensors[currentSensor].usablePart;
    zNear = 0.1f;
    zFar = 100.f;
    aspectRatio = sensorWidth / sensorHeight;
    calculateFOV();

    setVertexArray();
    setColorArray(0.0f, 0.0, 1.0f);
    setIndexArray();

    setFOVVertexArray();
    setFOVColorArray();
    setFOVIndexArray();

    setFOVVertexArrayNear();
    setFOVColorArrayNear();
    setFOVIndexArrayNear();

    setFOVVertexArrayFar();
    setFOVColorArrayFar();
    setFOVIndexArrayFar();
}

Camera::Camera(float x, float y, float height,
               float horizontalAngle, float verticalAngle,
               int sensor, int lens, float focalLength,
               const QString &cameraName,
               CameraView *camera)
{
    deltaAngle = 0.0f;
    layer = 3;
    this->cameraName = cameraName;
    cameraView = camera;
    name = "Camera";
    dx = x;
    dy = y;
    this->height = height;
    this->horizontalAngle = horizontalAngle;
    this->verticalAngle = verticalAngle;
    currentSensor = sensor;
    currentLens = lens;
    minimumFocalLength = sensors[currentSensor].lenses[currentLens].minimum;
    maximumFocalLength = sensors[currentSensor].lenses[currentLens].maximum;
    this->focalLength = focalLength;
    selected = fixed = false;
    nearPixels = 300;
    farPixels = 120;

    qDebug() << "1";
    diagonalName = sensors[currentSensor].diagonal;
    sensorWidth = sensors[currentSensor].width;
    sensorHeight = sensors[currentSensor].height;
    resolutionWidth = sensors[currentSensor].resolutionWidth;
    resolutionHeight = sensors[currentSensor].resolutionHeight;
    usablePart = sensors[currentSensor].usablePart;
    zNear = 0.1f;
    zFar = 100.f;
    aspectRatio = sensorWidth / sensorHeight;
    qDebug() << "2";
    calculateFOV();
    qDebug() << "3";
    //createProperties();

    setVertexArray();
    setColorArray(0.0f, 0.0, 1.0f);
    setIndexArray();

    setFOVVertexArray();
    setFOVColorArray();
    setFOVIndexArray();

    setFOVVertexArrayNear();
    setFOVColorArrayNear();
    setFOVIndexArrayNear();

    setFOVVertexArrayFar();
    setFOVColorArrayFar();
    setFOVIndexArrayFar();
    qDebug() << "gfhgf";
}




bool Camera::isSelected()
{
    return selected;
}

void Camera::setSelectedStatus(bool status)
{
    selected = status;
    if (selected)
    {
        setColorArray(0.0f, 0.0f, 0.0f);

        connect(this, SIGNAL(horizontalAngleChanged(double)), cameraView, SLOT(setHorizontalAngle(double)));
        connect(this, SIGNAL(verticalAngleChanged(double)), cameraView, SLOT(setVerticalAngle(double)));
        connect(this, SIGNAL(dxChanged(double)), cameraView, SLOT(setDX(double)));
        connect(this, SIGNAL(dyChanged(double)), cameraView, SLOT(setDY(double)));
        connect(this, SIGNAL(heightChanged(double)), cameraView, SLOT(setHeight(double)));
        connect(this, SIGNAL(yFOVChanged(double)), cameraView, SLOT(setFOV(double)));
        cameraView->setProjection(yFOV, aspectRatio, zNear, zFar);
        cameraView->setCamera(this);
    }
    else
    {
        setColorArray(0.0f, 0.0f, 1.0f);
        cameraView->setCamera(NULL);
        disconnect(this, 0, cameraView, 0);
    }
}

void Camera::drawFigure(QGLWidget *render)
{   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glPushMatrix();

    glTranslatef(dx, dy, height);
    glRotatef(horizontalAngle, 0.0f, 0.0f, 1.0f); // поворот по Z
    glRotatef(-verticalAngle/* + deltaAngle*/, 1.0f, 0.0f, 0.0f); // поворот по X

    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    glColorPointer(4, GL_FLOAT, 0, FOVColorArray.begin());
    glVertexPointer(3, GL_FLOAT, 0, FOVVertexArray.begin());
    glDrawElements(GL_TRIANGLES, FOVIndexArray.size(), GL_UNSIGNED_BYTE, FOVIndexArray.begin());

    glColorPointer(4, GL_FLOAT, 0, FOVColorArrayNear.begin());
    glVertexPointer(3, GL_FLOAT, 0, FOVVertexArrayNear.begin());
    glDrawElements(GL_TRIANGLES, FOVIndexArrayNear.size(), GL_UNSIGNED_BYTE, FOVIndexArrayNear.begin());

    glColorPointer(4, GL_FLOAT, 0, FOVColorArrayFar.begin());
    glVertexPointer(3, GL_FLOAT, 0, FOVVertexArrayFar.begin());
    glDrawElements(GL_TRIANGLES, FOVIndexArrayFar.size(), GL_UNSIGNED_BYTE, FOVIndexArrayFar.begin());

    glPopMatrix();

    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
        if (render)
            drawMeasurements(render);
    }
}

void Camera::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 2.0f, 7.0f);

    glPushMatrix();

    glTranslatef(dx, dy, height);
    glRotatef(horizontalAngle, 0.0f, 0.0f, 1.0f); // поворот по Z
    glRotatef(-verticalAngle, 1.0f, 0.0f, 0.0f); // поворот по X

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0FFF);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.0f, farLength + 5.0f, 0.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glPopMatrix();
}

void Camera::drawMeasurements(QGLWidget *render)
{
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);

    glPushMatrix();

    glTranslatef(dx, dy, height);
    glRotatef(horizontalAngle, 0.0f, 0.0f, 1.0f); // поворот по Z
    glRotatef(-verticalAngle, 1.0f, 0.0f, 0.0f); // поворот по X

    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    getWindowCoord(x, y, z, wx, wy, wz);
    glColor3f(0.0f, 0.0f, 0.0f);

    render->renderText(wx, wy, cameraName, shrift);

    glPopMatrix();
}

void Camera::move(float dx, float dy, float)
{
    if (fixed)
        return;
    this->dx += dx;
    this->dy += dy;
}

void Camera::drawControlElement(int index, float lineWidth, float pointSize)
{
    glPushMatrix();

    glTranslatef(dx, dy, height);
    glRotatef(horizontalAngle, 0.0f, 0.0f, 1.0f); // поворот по Z
    glRotatef(-verticalAngle, 1.0f, 0.0f, 0.0f); // поворот по X


    switch (index)
    {
    case 0:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, farLength + 5.0f, 0.0f);
        glEnd();
    }
        break;
    default:
        break;
    }
    glPopMatrix();
}

QCursor Camera::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void Camera::resizeByControl(int index, float dx, float dy, float x, float y)
{
    switch (index)
    {
    case 0:
    {
        float X1 = 0.0f;
        float Y1 = farLength + 5.0f;
        float X2 = 0.0f;
        float Y2 = 0.0f;
        float X3 = X1 + dx;
        float Y3 = Y1 + dy;
        float dx1 = X1 - X2;
        float dy1 = Y1 - Y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = X3 - X2;
        float dy2 = Y3 - Y2;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float pi = 3.14159265f;
        float t = dx1 / r1;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle1 = acos(t);
        if (dy1 < 0)
            angle1 = 2.0f * pi - angle1;
        t = dx2 / r2;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle2 = acos(t);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        float angle = angle2 - angle1;
        horizontalAngle += angle * 180.0f / pi;
        emit horizontalAngleChanged(horizontalAngle);
    }
        break;
    default:
        break;
    }
}

int Camera::getNumberOfControls()
{
    return 1;
}

int Camera::controlsForPoint()
{
    return 1;
}

void Camera::changeColorOfSelectedControl(int index)
{
}

void Camera::getProperties(QFormLayout *layout, QGLWidget *render)
{
    properties = layout;
    createProperties();

    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Название камеры", cameraNameLineEdit);
    layout->addRow("Сенсор", sensorTypeComboBox);
    layout->addRow("Редактировать сенсор", editSensorCheckBox);
    layout->addRow("Диагональ", sensorDiagonalLineEdit);
    layout->addRow("Ширина", sensorWidthSpinBox);
    layout->addRow("Высота", sensorHeightSpinBox);
    layout->addRow("Разрешение (ширина)", resolutionWidthSpinBox);
    layout->addRow("Разрешение (высота)", resolutionHeightSpinBox);
    layout->addRow("Используемая часть", usableSensorSpinBox);
    layout->addRow("Сохранить изменения", saveSensorButton);
    layout->addRow("Удалить сенсор", deleteSensorButton);
    layout->addRow("Объектив", lensComboBox);
    layout->addRow("Редактировать", editLensCheckBox);
    layout->addRow("Фокус (минимум)", minimumFocalSpinBox);
    layout->addRow("Фокус (максимум)", maximumFocalSpinBox);
    layout->addRow("Фокусное расстояние", focalLengthSpinBox);
    layout->addRow("Угол обзора(гор)", xFOVSpinBox);
    layout->addRow("Угол обзора(верт)", yFOVSpinBox);
    layout->addRow("Сохранить изменения", saveLensButton);
    layout->addRow("Удалить объектив", deleteLensButton);
    layout->addRow("Высота установки", mountingHeightSpinBox);
    layout->addRow("Угол поворота", horizontalAngleSpinBox);
    layout->addRow("Угол наклона", verticalAngleSpinBox);


    if (render)
    {
        connect(mountingHeightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(horizontalAngleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(verticalAngleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(focalLengthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(cameraNameLineEdit, SIGNAL(textChanged(QString)), render, SLOT(updateGL()));
    }
}

bool Camera::isFixed()
{
    return fixed;
}

int Camera::getLayer()
{
    return layer;
}

void Camera::clear()
{
}

RoadElement *Camera::getCopy()
{
    return NULL;
}

void Camera::setVertexArray()
{
    vertexArray.push_back(-0.1f);
    vertexArray.push_back(-0.5f);
    vertexArray.push_back(-0.1f);

    vertexArray.push_back(0.1f);
    vertexArray.push_back(-0.5f);
    vertexArray.push_back(-0.1f);

    vertexArray.push_back(0.1f);
    vertexArray.push_back(-0.5f);
    vertexArray.push_back(0.1f);

    vertexArray.push_back(-0.1f);
    vertexArray.push_back(-0.5f);
    vertexArray.push_back(0.1f);

    vertexArray.push_back(-0.1f);
    vertexArray.push_back(0.0f);
    vertexArray.push_back(-0.1f);

    vertexArray.push_back(0.1f);
    vertexArray.push_back(-0.0f);
    vertexArray.push_back(-0.1f);

    vertexArray.push_back(0.1f);
    vertexArray.push_back(0.0f);
    vertexArray.push_back(0.1f);

    vertexArray.push_back(-0.1f);
    vertexArray.push_back(0.0f);
    vertexArray.push_back(0.1f);
}

void Camera::setColorArray(float red, float green, float blue)
{
    if (colorArray.size() != vertexArray.size())
        colorArray.resize(vertexArray.size());
    for (int i = 0; i < colorArray.size() / 3; ++i)
    {
        colorArray[i * 3] = red;
        colorArray[i * 3 + 1] = green;
        colorArray[i * 3 + 2] = blue;
    }
}

void Camera::setIndexArray()
{
    indexArray.push_back(0);
    indexArray.push_back(1);
    indexArray.push_back(2);
    indexArray.push_back(0);
    indexArray.push_back(2);
    indexArray.push_back(3);

    indexArray.push_back(4);
    indexArray.push_back(5);
    indexArray.push_back(6);
    indexArray.push_back(4);
    indexArray.push_back(6);
    indexArray.push_back(7);

    indexArray.push_back(1);
    indexArray.push_back(5);
    indexArray.push_back(6);
    indexArray.push_back(1);
    indexArray.push_back(6);
    indexArray.push_back(2);

    indexArray.push_back(2);
    indexArray.push_back(6);
    indexArray.push_back(7);
    indexArray.push_back(2);
    indexArray.push_back(7);
    indexArray.push_back(3);

    indexArray.push_back(3);
    indexArray.push_back(7);
    indexArray.push_back(4);
    indexArray.push_back(3);
    indexArray.push_back(4);
    indexArray.push_back(0);

    indexArray.push_back(0);
    indexArray.push_back(4);
    indexArray.push_back(5);
    indexArray.push_back(0);
    indexArray.push_back(5);
    indexArray.push_back(1);
}

void Camera::setFOVVertexArray()
{
    if (FOVVertexArray.size() != 24)
        FOVVertexArray.resize(24);
    FOVVertexArray[0] = -nearWidth / 2.0f;
    FOVVertexArray[1] = nearLength;
    FOVVertexArray[2] = nearHeight / 2.0f;

    FOVVertexArray[3] = -nearWidth / 2.0f;
    FOVVertexArray[4] = nearLength;
    FOVVertexArray[5] = -nearHeight / 2.0f;

    FOVVertexArray[6] = nearWidth / 2.0f;
    FOVVertexArray[7] = nearLength;
    FOVVertexArray[8] = -nearHeight / 2.0f;

    FOVVertexArray[9] = nearWidth / 2.0f;
    FOVVertexArray[10] = nearLength;
    FOVVertexArray[11] = nearHeight / 2.0f;



    FOVVertexArray[12] = -farWidth / 2.0f;
    FOVVertexArray[13] = farLength;
    FOVVertexArray[14] = farHeight / 2.0f;

    FOVVertexArray[15] = -farWidth / 2.0f;
    FOVVertexArray[16] = farLength;
    FOVVertexArray[17] = -farHeight / 2.0f;

    FOVVertexArray[18] = farWidth / 2.0f;
    FOVVertexArray[19] = farLength;
    FOVVertexArray[20] = -farHeight / 2.0f;

    FOVVertexArray[21] = farWidth / 2.0f;
    FOVVertexArray[22] = farLength;
    FOVVertexArray[23] = farHeight / 2.0f;

}

void Camera::setFOVColorArray()
{
    if (FOVColorArray.size() / 4 != FOVVertexArray.size() / 3)
        FOVColorArray.resize(FOVVertexArray.size() * 4 / 3);
    for (int i = 0; i < FOVColorArray.size() / 4; ++i)
    {
        FOVColorArray[i * 4] = 0.0f;
        FOVColorArray[i * 4 + 1] = 1.0f;
        FOVColorArray[i * 4 + 2] = 0.0f;
        FOVColorArray[i * 4 + 3] = 0.5f;
    }
}

void Camera::setFOVIndexArray()
{
    FOVIndexArray.push_back(4);
    FOVIndexArray.push_back(0);
    FOVIndexArray.push_back(3);
    FOVIndexArray.push_back(4);
    FOVIndexArray.push_back(3);
    FOVIndexArray.push_back(7);

    FOVIndexArray.push_back(3);
    FOVIndexArray.push_back(2);
    FOVIndexArray.push_back(6);
    FOVIndexArray.push_back(3);
    FOVIndexArray.push_back(6);
    FOVIndexArray.push_back(7);

    FOVIndexArray.push_back(2);
    FOVIndexArray.push_back(1);
    FOVIndexArray.push_back(5);
    FOVIndexArray.push_back(2);
    FOVIndexArray.push_back(5);
    FOVIndexArray.push_back(6);

    FOVIndexArray.push_back(1);
    FOVIndexArray.push_back(0);
    FOVIndexArray.push_back(4);
    FOVIndexArray.push_back(1);
    FOVIndexArray.push_back(4);
    FOVIndexArray.push_back(5);

    FOVIndexArray.push_back(0);
    FOVIndexArray.push_back(1);
    FOVIndexArray.push_back(3);
    FOVIndexArray.push_back(1);
    FOVIndexArray.push_back(2);
    FOVIndexArray.push_back(3);

    FOVIndexArray.push_back(7);
    FOVIndexArray.push_back(6);
    FOVIndexArray.push_back(5);
    FOVIndexArray.push_back(7);
    FOVIndexArray.push_back(5);
    FOVIndexArray.push_back(4);
}

void Camera::setFOVVertexArrayNear()
{
    if (FOVVertexArrayNear.size() != 24)
        FOVVertexArrayNear.resize(24);
    FOVVertexArrayNear[0] = 0.0f;
    FOVVertexArrayNear[1] = 0.0f;
    FOVVertexArrayNear[2] = 0.0f;

    FOVVertexArrayNear[3] = 0.0f;
    FOVVertexArrayNear[4] = 0.0f;
    FOVVertexArrayNear[5] = 0.0f;

    FOVVertexArrayNear[6] = 0.0f;
    FOVVertexArrayNear[7] = 0.0f;
    FOVVertexArrayNear[8] = 0.0f;

    FOVVertexArrayNear[9] = 0.0f;
    FOVVertexArrayNear[10] = 0.0f;
    FOVVertexArrayNear[11] = 0.0f;



    FOVVertexArrayNear[12] = -nearWidth / 2.0f;
    FOVVertexArrayNear[13] = nearLength;
    FOVVertexArrayNear[14] = nearHeight / 2.0f;

    FOVVertexArrayNear[15] = -nearWidth / 2.0f;
    FOVVertexArrayNear[16] = nearLength;
    FOVVertexArrayNear[17] = -nearHeight / 2.0f;

    FOVVertexArrayNear[18] = nearWidth / 2.0f;
    FOVVertexArrayNear[19] = nearLength;
    FOVVertexArrayNear[20] = -nearHeight / 2.0f;

    FOVVertexArrayNear[21] = nearWidth / 2.0f;
    FOVVertexArrayNear[22] = nearLength;
    FOVVertexArrayNear[23] = nearHeight / 2.0f;
}

void Camera::setFOVColorArrayNear()
{
    if (FOVColorArrayNear.size() / 4 != FOVVertexArrayNear.size() / 3)
        FOVColorArrayNear.resize(FOVVertexArrayNear.size() * 4 / 3);
    for (int i = 0; i < FOVColorArrayNear.size() / 4; ++i)
    {
        FOVColorArrayNear[i * 4] = 1.0f;
        FOVColorArrayNear[i * 4 + 1] = 1.0f;
        FOVColorArrayNear[i * 4 + 2] = 0.0f;
        FOVColorArrayNear[i * 4 + 3] = 0.5f;
    }
}

void Camera::setFOVIndexArrayNear()
{
    FOVIndexArrayNear.push_back(4);
    FOVIndexArrayNear.push_back(0);
    FOVIndexArrayNear.push_back(3);
    FOVIndexArrayNear.push_back(4);
    FOVIndexArrayNear.push_back(3);
    FOVIndexArrayNear.push_back(7);

    FOVIndexArrayNear.push_back(3);
    FOVIndexArrayNear.push_back(2);
    FOVIndexArrayNear.push_back(6);
    FOVIndexArrayNear.push_back(3);
    FOVIndexArrayNear.push_back(6);
    FOVIndexArrayNear.push_back(7);

    FOVIndexArrayNear.push_back(2);
    FOVIndexArrayNear.push_back(1);
    FOVIndexArrayNear.push_back(5);
    FOVIndexArrayNear.push_back(2);
    FOVIndexArrayNear.push_back(5);
    FOVIndexArrayNear.push_back(6);

    FOVIndexArrayNear.push_back(1);
    FOVIndexArrayNear.push_back(0);
    FOVIndexArrayNear.push_back(4);
    FOVIndexArrayNear.push_back(1);
    FOVIndexArrayNear.push_back(4);
    FOVIndexArrayNear.push_back(5);

    FOVIndexArrayNear.push_back(7);
    FOVIndexArrayNear.push_back(6);
    FOVIndexArrayNear.push_back(5);
    FOVIndexArrayNear.push_back(7);
    FOVIndexArrayNear.push_back(5);
    FOVIndexArrayNear.push_back(4);
}

void Camera::setFOVVertexArrayFar()
{
    if (FOVVertexArrayFar.size() != 24)
        FOVVertexArrayFar.resize(24);
    FOVVertexArrayFar[0] = -farWidth / 2.0f;
    FOVVertexArrayFar[1] = farLength;
    FOVVertexArrayFar[2] = farHeight / 2.0f;

    FOVVertexArrayFar[3] = -farWidth / 2.0f;
    FOVVertexArrayFar[4] = farLength;
    FOVVertexArrayFar[5] = -farHeight / 2.0f;

    FOVVertexArrayFar[6] = farWidth / 2.0f;
    FOVVertexArrayFar[7] = farLength;
    FOVVertexArrayFar[8] = -farHeight / 2.0f;

    FOVVertexArrayFar[9] = farWidth / 2.0f;
    FOVVertexArrayFar[10] = farLength;
    FOVVertexArrayFar[11] = farHeight / 2.0f;



    FOVVertexArrayFar[12] = (-farWidth / 2.0f) / farLength * (farLength + 5.0f);
    FOVVertexArrayFar[13] = farLength + 5.0f;
    FOVVertexArrayFar[14] = (farHeight / 2.0f) / farLength * (farLength + 5.0f);

    FOVVertexArrayFar[15] = (-farWidth / 2.0f) / farLength * (farLength + 5.0f);
    FOVVertexArrayFar[16] = farLength + 5.0f;
    FOVVertexArrayFar[17] = (-farHeight / 2.0f) / farLength * (farLength + 5.0f);

    FOVVertexArrayFar[18] = (farWidth / 2.0f) / farLength * (farLength + 5.0f);
    FOVVertexArrayFar[19] = farLength + 5.0f;
    FOVVertexArrayFar[20] = (-farHeight / 2.0f) / farLength * (farLength + 5.0f);

    FOVVertexArrayFar[21] = (farWidth / 2.0f) / farLength * (farLength + 5.0f);
    FOVVertexArrayFar[22] = farLength + 5.0f;
    FOVVertexArrayFar[23] = (farHeight / 2.0f) / farLength * (farLength + 5.0f);
}

void Camera::setFOVColorArrayFar()
{
    if (FOVColorArrayFar.size() / 4 != FOVVertexArrayFar.size() / 3)
        FOVColorArrayFar.resize(FOVVertexArrayFar.size() * 4 / 3);
    for (int i = 0; i < FOVColorArrayFar.size() / 4; ++i)
    {
        FOVColorArrayFar[i * 4] = 1.0f;
        FOVColorArrayFar[i * 4 + 1] = 0.0f;
        FOVColorArrayFar[i * 4 + 2] = 0.0f;
        FOVColorArrayFar[i * 4 + 3] = 0.5f;
    }
}

void Camera::setFOVIndexArrayFar()
{
    FOVIndexArrayFar.push_back(4);
    FOVIndexArrayFar.push_back(0);
    FOVIndexArrayFar.push_back(3);
    FOVIndexArrayFar.push_back(4);
    FOVIndexArrayFar.push_back(3);
    FOVIndexArrayFar.push_back(7);

    FOVIndexArrayFar.push_back(3);
    FOVIndexArrayFar.push_back(2);
    FOVIndexArrayFar.push_back(6);
    FOVIndexArrayFar.push_back(3);
    FOVIndexArrayFar.push_back(6);
    FOVIndexArrayFar.push_back(7);

    FOVIndexArrayFar.push_back(2);
    FOVIndexArrayFar.push_back(1);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(2);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(6);

    FOVIndexArrayFar.push_back(1);
    FOVIndexArrayFar.push_back(0);
    FOVIndexArrayFar.push_back(4);
    FOVIndexArrayFar.push_back(1);
    FOVIndexArrayFar.push_back(4);
    FOVIndexArrayFar.push_back(5);

    FOVIndexArrayFar.push_back(0);
    FOVIndexArrayFar.push_back(1);
    FOVIndexArrayFar.push_back(3);
    FOVIndexArrayFar.push_back(1);
    FOVIndexArrayFar.push_back(2);
    FOVIndexArrayFar.push_back(3);

    FOVIndexArrayFar.push_back(4);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(7);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(6);
    FOVIndexArrayFar.push_back(7);

    FOVIndexArrayFar.push_back(7);
    FOVIndexArrayFar.push_back(6);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(7);
    FOVIndexArrayFar.push_back(5);
    FOVIndexArrayFar.push_back(4);
}

void Camera::setCamera(CameraView *camera)
{
    cameraView = camera;
}

bool Camera::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}

void Camera::setHorizontalAngle(double value)
{
    if (horizontalAngle == value)
        return;
    horizontalAngle = value;
    emit horizontalAngleChanged(value);

}

void Camera::setVerticalAngle(double value)
{
    if (verticalAngle == value)
        return;
    verticalAngle = value;
    emit verticalAngleChanged(value);

}

void Camera::setHeight(double value)
{
    if (height == value)
        return;
    height = value;
    emit heightChanged(value);
}

void Camera::setDX(double value)
{
    if (dx == value)
        return;
    dx = value;
    emit dxChanged(value);
}

void Camera::setDY(double value)
{
    if (dy == value)
        return;
    dy = value;
    emit dyChanged(value);
}

void Camera::setDiagonalName(const QString &name)
{
    if (diagonalName == name)
        return;
    diagonalName = name;
    emit diagonalNameChanged(name);
}

void Camera::setCameraName(const QString &name)
{
    if (cameraName == name)
        return;
    cameraName = name;
}

void Camera::setSensorName(const QString &name)
{
    sensors[currentSensor].name = name;
    sensorTypeComboBox->setItemText(currentSensor, name);
}

void Camera::setLensName(const QString &name)
{
    sensors[currentSensor].lenses[currentLens].name = name;
    lensComboBox->setItemText(currentLens, name);
}

void Camera::calculateFOV()
{    
    verticalAngle += deltaAngle;
    float angle = atan(sensorHeight / (focalLength * 2)) * 2.0f;

    float angle1 = angle * 180.0f / 3.14159265f;
    float factor = float(usablePart) / float(resolutionHeight);
    float angle2 = angle1 * factor;
    deltaAngle = (angle1 - angle2) / 2.0f;
    nearHeight = (resolutionHeight * factor) / float(nearPixels);
    farHeight = (resolutionHeight * factor) / float(farPixels);
    aspectRatio = sensorWidth / (sensorHeight * factor);
    //yFOV = angle2;
    //emit yFOVChanged(yFOV);
    setYFOV(angle2);
    setVerticalAngle(verticalAngle - deltaAngle);

    angle = atan(sensorWidth / (focalLength * 2)) * 2.0f;
    nearWidth = resolutionWidth / float(nearPixels);
    farWidth = resolutionWidth / float(farPixels);
    nearLength = nearWidth / (2.0f * sensorWidth / (focalLength * 2));
    farLength = farWidth / (2.0f * sensorWidth / (focalLength * 2));
    //xFOV = angle * 180.0f / 3.14159265f;
    //emit xFOVChanged(angle * 180.0f / 3.14159265f);
    setXFOV(angle * 180.0f / 3.14159265f);

    setFOVVertexArray();
    setFOVVertexArrayNear();
    setFOVVertexArrayFar();
    if (cameraView->isActive())
    {
        cameraView->setDeltaAngle(deltaAngle);
        cameraView->setProjection(yFOV, aspectRatio, 0.1f, 100.0f);
    }
}

void Camera::setSensorWidth(double value)
{
    if (sensorWidth == value)
        return;
    sensorWidth = value;
    calculateFOV();
    emit sensorWidthChanged(value);
}

void Camera::setSensorHeight(double value)
{
    if (sensorHeight == value)
        return;
    sensorHeight = value;
    calculateFOV();
    emit sensorHeightChanged(value);
}

void Camera::setResolutionWidth(int value)
{
    if (resolutionWidth == value)
        return;
    resolutionWidth = value;
    calculateFOV();
    emit resolutionWidthChanged(value);
}

void Camera::setResolutionHeight(int value)
{
    if (resolutionHeight == value)
        return;
    resolutionHeight = value;
    calculateFOV();
    emit resolutionHeightChanged(value);
}

void Camera::setUsablePart(int value)
{
    if (usablePart == value)
        return;
    usablePart = value;
    calculateFOV();
    emit usablePartChanged(value);
}


void Camera::setFocalLength(double value)
{
    if (focalLength == value)
        return;
    focalLength = value;

    calculateFOV();
    emit focalLengthChanged(value);
}



void Camera::setSensor(int sensorType)
{
    if (sensorType < 0 ||
            sensorType > sensors.size() + 1)
        return;
    if (sensorType == sensors.size() + 1)
    {
        addSensor();
        return;
    }
    int i = sensorType;
    int temp = currentSensor;
    currentSensor = sensorType;

    setDiagonalName(sensors[i].diagonal);
    setSensorWidth(sensors[i].width);
    setSensorHeight(sensors[i].height);
    setResolutionWidth(sensors[i].resolutionWidth);
    setResolutionHeight(sensors[i].resolutionHeight);
    setUsablePart(sensors[i].usablePart);
    qDebug() << sensors[i].usablePart;
    //aspectRatio = sensorWidth / sensorHeight;
    //cameraView->setAspectRatio(aspectRatio);


    if (temp == currentSensor)
        return;

    lensComboBox->clear();
    QStringList lensList;
    for (int i = 0; i < sensors[currentSensor].lenses.size(); ++i)
    {
        lensList << sensors[currentSensor].lenses[i].name;
    }
    lensComboBox->addItems(lensList);
    lensComboBox->insertSeparator(sensors[currentSensor].lenses.size());
    lensComboBox->addItem("Добавить объектив+");
    setLens(0);
}

void Camera::setLens(int lensType)
{
    if (lensType < 0 ||
            lensType > sensors[currentSensor].lenses.size() + 1)
        return;
    if (lensType == sensors[currentSensor].lenses.size() + 1)
    {
        addLens();
        return;
    }
    currentLens = lensType;

    setMinimumFocalLength(sensors[currentSensor].lenses[lensType].minimum);
    setMaximumFocalLength(sensors[currentSensor].lenses[lensType].maximum);
    //focalLengthSpinBox->setMinimum(sensors[currentSensor].lenses[lensType].minimum);
    //focalLengthSpinBox->setMaximum(sensors[currentSensor].lenses[lensType].maximum);
    focalLengthSpinBox->setValue(sensors[currentSensor].lenses[lensType].minimum);
}

void Camera::setXFOV(double value)
{
    if (xFOV == value)
        return;
    xFOV = value;

    //    focalLength = (sensorWidth / 2.0f) / tan((xFOV * 180.0f / 3.14159265f) / 2.0f);
    //    emit focalLengthChanged(focalLength);
    //    float angle = atan(sensorHeight / (focalLength * 2)) * 2.0f;
    //    verticalAngle += deltaAngle;
    //    float angle1 = angle * 180.0f / 3.14159265f;
    //    float factor = float(usablePart) / float(resolutionHeight);
    //    float angle2 = angle1 * factor;
    //    deltaAngle = (angle1 - angle2) / 2.0f;
    //    aspectRatio = sensorWidth / (sensorHeight * factor);
    //    yFOV = angle2;
    //    setVerticalAngle(verticalAngle - deltaAngle);

    //    setFOVVertexArray();
    //    setFOVVertexArrayNear();
    //    setFOVVertexArrayFar();
    //    if (cameraView->isActive())
    //    {
    //        cameraView->setDeltaAngle(deltaAngle);
    //        cameraView->setProjection(yFOV, aspectRatio, 0.1f, 100.0f);
    //    }
    emit xFOVChanged(xFOV);
}

void Camera::setYFOV(double value)
{
    if (yFOV == value)
        return;
    yFOV = value;
    emit yFOVChanged(yFOV);
}

void Camera::addSensor()
{
    bool ok;
    QString text = QInputDialog::getText(0, tr("Введите название сенсора"),
                                         tr("Сенсор:"), QLineEdit::Normal,
                                         QString(""), &ok);
    if (ok)
    {
        Sensor sensor;
        sensor.name = text;
        sensor.diagonal = "\0";
        sensor.width = 0.0f;
        sensor.height = 0.0f;
        sensor.resolutionWidth = 0;
        sensor.resolutionHeight = 0;
        sensor.usablePart = 0;

        sensors.push_back(sensor);
        sensorTypeComboBox->insertItem(sensors.size() - 1, text);
        sensorTypeComboBox->setCurrentIndex(sensors.size() - 1);
        setSensor(sensors.size() - 1);
    }
    else
    {
        sensorTypeComboBox->setCurrentIndex(currentSensor);
    }
}

void Camera::addLens()
{
    bool ok;
    QString text = QInputDialog::getText(0, tr("Введите название объектива"),
                                         tr("Объектив:"), QLineEdit::Normal,
                                         QString(""), &ok);
    if (ok)
    {
        Lens lens;
        lens.name = text;
        lens.minimum = 0.0f;
        lens.maximum = 0.0f;

        sensors[currentSensor].lenses.push_back(lens);
        lensComboBox->insertItem(sensors[currentSensor].lenses.size() - 1, text);
        lensComboBox->setCurrentIndex(sensors[currentSensor].lenses.size() - 1);
        setLens(sensors[currentSensor].lenses.size() - 1);
    }
    else
    {
        lensComboBox->setCurrentIndex(currentLens);
    }
}

void Camera::setMinimumFocalLength(double value)
{
    if (minimumFocalLength == value)
        return;
    minimumFocalLength = value;
    focalLengthSpinBox->setMinimum(value);
    emit minimumFocalLengthChanged(value);
}

void Camera::setMaximumFocalLength(double value)
{
    if (maximumFocalLength == value)
        return;
    maximumFocalLength = value;
    focalLengthSpinBox->setMaximum(value);
    emit maximumFocalLengthChanged(value);
}

void Camera::deleteSensor()
{
    sensorTypeComboBox->removeItem(currentSensor);
    sensors.removeAt(currentSensor);

    if (currentSensor >= sensors.size())
        currentSensor = sensors.size() - 1;

    sensorTypeComboBox->setCurrentIndex(currentSensor);
    setSensor(currentSensor);
}

void Camera::deleteLens()
{
    lensComboBox->removeItem(currentLens);
    sensors[currentSensor].lenses.removeAt(currentLens);

    if (currentLens >= sensors[currentSensor].lenses.size())
        currentLens = sensors[currentSensor].lenses.size() - 1;

    lensComboBox->setCurrentIndex(currentLens);
    setLens(currentLens);
}

void Camera::saveSensor()
{
    sensors[currentSensor].diagonal = diagonalName;
    sensors[currentSensor].width = sensorWidth;
    sensors[currentSensor].height = sensorHeight;
    sensors[currentSensor].resolutionWidth = resolutionWidth;
    sensors[currentSensor].resolutionHeight = resolutionHeight;
    sensors[currentSensor].usablePart = usablePart;
}

void Camera::saveLens()
{
    sensors[currentSensor].lenses[currentLens].minimum = minimumFocalLength;
    sensors[currentSensor].lenses[currentLens].maximum = maximumFocalLength;
}


void Camera::createProperties()
{
    fixedCheckBox = new QCheckBox();

    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    cameraNameLineEdit = new QLineEdit();
    connect(cameraNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setCameraName(QString)));
    cameraNameLineEdit->setText(cameraName);

    sensorTypeComboBox = new QComboBox();
    sensorLineEdit = new QLineEdit();
    connect(sensorLineEdit, SIGNAL(textEdited(QString)), this, SLOT(setSensorName(QString)));
    sensorTypeComboBox->setLineEdit(sensorLineEdit);


    QStringList sensorList;
    for (int i = 0; i < sensors.size(); ++i)
    {
        sensorList << sensors[i].name;
    }
    sensorTypeComboBox->addItems(sensorList);
    sensorTypeComboBox->insertSeparator(sensors.size());
    sensorTypeComboBox->addItem("Добавить сенсор+");
    connect(sensorTypeComboBox, SIGNAL(activated(int)), this, SLOT(setSensor(int)));
    connect(this, SIGNAL(sensorTypeChanged(int)), sensorTypeComboBox, SLOT(setCurrentIndex(int)));
    //sensorTypeComboBox->setCurrentIndex(currentSensor);

    sensorDiagonalLineEdit = new QLineEdit();
    sensorDiagonalLineEdit->setText(diagonalName);
    connect(sensorDiagonalLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setDiagonalName(QString)));
    connect(this, SIGNAL(diagonalNameChanged(QString)), sensorDiagonalLineEdit, SLOT(setText(QString)));


    sensorWidthSpinBox = new QDoubleSpinBox();
    sensorWidthSpinBox->setSingleStep(0.1);
    sensorWidthSpinBox->setValue(sensorWidth);
    connect(sensorWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSensorWidth(double)));
    connect(this, SIGNAL(sensorWidthChanged(double)), sensorWidthSpinBox, SLOT(setValue(double)));


    sensorHeightSpinBox = new QDoubleSpinBox();
    sensorHeightSpinBox->setSingleStep(0.1);
    sensorHeightSpinBox->setValue(sensorHeight);
    connect(sensorHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setSensorHeight(double)));
    connect(this, SIGNAL(sensorHeightChanged(double)), sensorHeightSpinBox, SLOT(setValue(double)));


    resolutionWidthSpinBox = new QSpinBox();
    resolutionWidthSpinBox->setMaximum(1000000);
    resolutionWidthSpinBox->setValue(resolutionWidth);
    connect(resolutionWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setResolutionWidth(int)));
    connect(this, SIGNAL(resolutionWidthChanged(int)), resolutionWidthSpinBox, SLOT(setValue(int)));


    resolutionHeightSpinBox = new QSpinBox();
    resolutionHeightSpinBox->setMaximum(1000000);
    resolutionHeightSpinBox->setValue(resolutionHeight);
    connect(resolutionHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setResolutionHeight(int)));
    connect(this, SIGNAL(resolutionHeightChanged(int)), resolutionHeightSpinBox, SLOT(setValue(int)));


    usableSensorSpinBox = new QSpinBox();
    usableSensorSpinBox->setMaximum(1000000);
    usableSensorSpinBox->setKeyboardTracking(false);
    usableSensorSpinBox->setValue(usablePart);
    connect(usableSensorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setUsablePart(int)));
    connect(this, SIGNAL(usablePartChanged(int)), usableSensorSpinBox, SLOT(setValue(int)));



    lensComboBox = new QComboBox();
    lensLineEdit = new QLineEdit();
    connect(lensLineEdit, SIGNAL(textEdited(QString)), this, SLOT(setLensName(QString)));
    lensComboBox->setLineEdit(lensLineEdit);

    QStringList lensList;
    for (int i = 0; i < sensors[currentSensor].lenses.size(); ++i)
    {
        lensList << sensors[currentSensor].lenses[i].name;
    }
    lensComboBox->addItems(lensList);
    lensComboBox->insertSeparator(sensors[currentSensor].lenses.size());
    lensComboBox->addItem("Добавить объектив+");
    //connect(lensComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLens(int)));
    connect(lensComboBox, SIGNAL(activated(int)), this, SLOT(setLens(int)));

    xFOVSpinBox = new QDoubleSpinBox();
    xFOVSpinBox->setMinimum(-720.0);
    xFOVSpinBox->setMaximum(720.0);
    connect(xFOVSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setXFOV(double)));
    connect(this, SIGNAL(xFOVChanged(double)), xFOVSpinBox, SLOT(setValue(double)));
    xFOVSpinBox->setValue(xFOV);

    yFOVSpinBox = new QDoubleSpinBox();
    yFOVSpinBox->setMinimum(-720.0);
    yFOVSpinBox->setMaximum(720.0);
    connect(yFOVSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setYFOV(double)));
    connect(this, SIGNAL(yFOVChanged(double)), yFOVSpinBox, SLOT(setValue(double)));
    yFOVSpinBox->setValue(yFOV);

    minimumFocalSpinBox = new QDoubleSpinBox();
    minimumFocalSpinBox->setSingleStep(0.1);
    minimumFocalSpinBox->setKeyboardTracking(false);
    minimumFocalSpinBox->setValue(minimumFocalLength);
    connect(minimumFocalSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMinimumFocalLength(double)));
    connect(this, SIGNAL(minimumFocalLengthChanged(double)), minimumFocalSpinBox, SLOT(setValue(double)));


    maximumFocalSpinBox = new QDoubleSpinBox();
    maximumFocalSpinBox->setSingleStep(0.1);
    maximumFocalSpinBox->setKeyboardTracking(false);
    maximumFocalSpinBox->setValue(maximumFocalLength);
    connect(maximumFocalSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMaximumFocalLength(double)));
    connect(this, SIGNAL(maximumFocalLengthChanged(double)), maximumFocalSpinBox, SLOT(setValue(double)));

    
    focalLengthSpinBox = new QDoubleSpinBox();
    focalLengthSpinBox->setSingleStep(0.1);
    focalLengthSpinBox->setKeyboardTracking(false);
    connect(focalLengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setFocalLength(double)));
    connect(this, SIGNAL(focalLengthChanged(double)), focalLengthSpinBox, SLOT(setValue(double)));
    focalLengthSpinBox->setValue(focalLength);

    mountingHeightSpinBox = new QDoubleSpinBox();
    mountingHeightSpinBox->setKeyboardTracking(false);
    connect(mountingHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
    connect(this, SIGNAL(heightChanged(double)), mountingHeightSpinBox, SLOT(setValue(double)));
    mountingHeightSpinBox->setValue(height);

    horizontalAngleSpinBox = new QDoubleSpinBox();
    horizontalAngleSpinBox->setKeyboardTracking(false);
    horizontalAngleSpinBox->setMinimum(-1024.0);
    horizontalAngleSpinBox->setMaximum(1024.0);
    connect(horizontalAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHorizontalAngle(double)));
    connect(this, SIGNAL(horizontalAngleChanged(double)), horizontalAngleSpinBox, SLOT(setValue(double)));
    horizontalAngleSpinBox->setValue(horizontalAngle);

    verticalAngleSpinBox = new QDoubleSpinBox();
    verticalAngleSpinBox->setKeyboardTracking(false);
    verticalAngleSpinBox->setMinimum(-1024.0);
    verticalAngleSpinBox->setMaximum(1024.0);
    connect(verticalAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setVerticalAngle(double)));
    connect(this, SIGNAL(verticalAngleChanged(double)), verticalAngleSpinBox, SLOT(setValue(double)));
    verticalAngleSpinBox->setValue(verticalAngle);

    deleteSensorButton = new QPushButton("Удалить");
    connect(deleteSensorButton, SIGNAL(clicked(bool)), this, SLOT(deleteSensor()));

    deleteLensButton = new QPushButton("Удалить");
    connect(deleteLensButton, SIGNAL(clicked(bool)), this, SLOT(deleteLens()));

    saveSensorButton = new QPushButton("Сохранить");
    connect(saveSensorButton, SIGNAL(clicked(bool)), this, SLOT(saveSensor()));

    saveLensButton = new QPushButton("Сохранить");
    connect(saveLensButton, SIGNAL(clicked(bool)), this, SLOT(saveLens()));

    connect(fixedCheckBox, SIGNAL(toggled(bool)), focalLengthSpinBox, SLOT(setDisabled(bool)));
    connect(fixedCheckBox, SIGNAL(toggled(bool)), mountingHeightSpinBox, SLOT(setDisabled(bool)));
    connect(fixedCheckBox, SIGNAL(toggled(bool)), horizontalAngleSpinBox, SLOT(setDisabled(bool)));
    connect(fixedCheckBox, SIGNAL(toggled(bool)), verticalAngleSpinBox, SLOT(setDisabled(bool)));
    connect(fixedCheckBox, SIGNAL(toggled(bool)), xFOVSpinBox, SLOT(setDisabled(bool)));
    connect(fixedCheckBox, SIGNAL(toggled(bool)), yFOVSpinBox, SLOT(setDisabled(bool)));

    fixedCheckBox->setChecked(fixed);

    editSensorCheckBox = new QCheckBox();
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), sensorDiagonalLineEdit, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), sensorWidthSpinBox, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), sensorHeightSpinBox, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), resolutionWidthSpinBox, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), resolutionHeightSpinBox, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), usableSensorSpinBox, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), sensorLineEdit, SLOT(setEnabled(bool)));
    connect(editSensorCheckBox, SIGNAL(toggled(bool)), deleteSensorButton, SLOT(setEnabled(bool)));
    editSensorCheckBox->toggle();
    editSensorCheckBox->toggle();
    
    editLensCheckBox = new QCheckBox();
    connect(editLensCheckBox, SIGNAL(toggled(bool)), minimumFocalSpinBox, SLOT(setEnabled(bool)));
    connect(editLensCheckBox, SIGNAL(toggled(bool)), maximumFocalSpinBox, SLOT(setEnabled(bool)));
    connect(editLensCheckBox, SIGNAL(toggled(bool)), lensLineEdit, SLOT(setEnabled(bool)));
    connect(editLensCheckBox, SIGNAL(toggled(bool)), deleteLensButton, SLOT(setEnabled(bool)));
    editLensCheckBox->toggle();
    editLensCheckBox->toggle();



    float temp = focalLength;
    sensorTypeComboBox->setCurrentIndex(currentSensor);
    lensComboBox->setCurrentIndex(currentLens);
    focalLengthSpinBox->setValue(temp);
}

void Camera::readSensors(const QString &source)
{
    QFile file(source);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0,"Ошибка","Невозможно прочитать информацию о сенсорах");
    }
    else
    {
        QByteArray saveData = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonArray ar = loadDoc.array();

        for (int i = 0; i < ar.size(); ++i)
        {
            QJsonObject obj = ar[i].toObject();
            Sensor sensor;
            sensor.name = obj["Name"].toString();
            sensor.diagonal = obj["Diagonal"].toString();
            sensor.width = obj["Width"].toDouble();
            sensor.height = obj["Height"].toDouble();
            sensor.resolutionWidth = obj["ResolutionWidth"].toInt();
            sensor.resolutionHeight = obj["ResolutionHeight"].toInt();
            sensor.usablePart = obj["UsablePart"].toInt();
            QJsonArray lensesArray = obj["Lenses"].toArray();
            for (int j = 0; j < lensesArray.size(); ++j)
            {
                QJsonObject lensObj = lensesArray[j].toObject();
                Lens lens;
                lens.name = lensObj["Name"].toString();
                lens.minimum = lensObj["Minimum"].toDouble();
                lens.maximum = lensObj["Maximum"].toDouble();

                sensor.lenses.push_back(lens);
            }

            sensors.push_back(sensor);
        }
    }
}

void Camera::saveSensors(const QString &source)
{
    QJsonArray elements;

    for (int i = 0; i < sensors.size(); ++i)
    {
        QJsonObject obj;
        obj["Name"] = sensors[i].name;
        obj["Diagonal"] = sensors[i].diagonal;
        obj["Width"] = sensors[i].width;
        obj["Height"] = sensors[i].height;
        obj["ResolutionWidth"] = sensors[i].resolutionWidth;
        obj["ResolutionHeight"] = sensors[i].resolutionHeight;
        obj["UsablePart"] = sensors[i].usablePart;
        QJsonArray lensesArray;
        for (int j = 0; j < sensors[i].lenses.size(); ++j)
        {
            QJsonObject lensObj;
            lensObj["Name"] = sensors[i].lenses[j].name;
            lensObj["Minimum"] = sensors[i].lenses[j].minimum;
            lensObj["Maximum"] = sensors[i].lenses[j].maximum;

            lensesArray.append(lensObj);
        }
        obj["Lenses"] = lensesArray;

        elements.append(obj);
    }

    QFile file(source);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0, "Ошибка", "Невозможно открыть файл для записи сенсора");
    }
    else
    {
        QJsonDocument saveDoc(elements);
        file.write(saveDoc.toJson());
        file.close();
    }
}


void Camera::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
        //item->widget()->hide();
        //layout->removeItem(item);
    }
}


QJsonObject Camera::getJSONInfo()
{
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    element["X"] = dx;
    element["Y"] = dy;
    element["Height"] = height;
    element["HorizontalAngle"] = horizontalAngle;
    element["VerticalAngle"] = verticalAngle;
    element["Sensor"] = currentSensor;
    element["Lens"] = currentLens;
    element["FocalLength"] = focalLength;
    element["CameraName"] = cameraName;
    
    return element;
}
