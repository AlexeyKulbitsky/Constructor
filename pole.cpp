#include "pole.h"

Pole::Pole()
{

}

Pole::Pole(float x, float y, float diameter, float height)
{
    elementX = x;
    elementY = y;
    x1 = x;
    y1 = y;
    z1 = 0.0f;
    x2 = x;
    y2 = y;
    z2 = height;
    z = 0.0f;
    this->diameter = diameter;
    this->height = height;
    layer = 2;
    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
    name = "Pole";
    numberOfSides = 20;
    setVertexArray();
    setIndexArray();
    setNormalArray();
    setColorArray(0.8f, 0.8f, 0.8f);

}

Pole::Pole(float x1, float y1, float z1, float x2, float y2, float z2, float diameter)
{
    this->diameter = diameter;
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    height = sqrt(dx * dx + dy * dy + dz * dz);

    this->x1 = x1;
    this->y1 = y1;
    this->z1 = 0.0f;
    this->x2 = x1;
    this->y2 = y1;
    this->z2 = height;
    elementX = 0.0f;
    elementY = 0.0f;
    z = height / (-2.0f);

    layer = 2;
    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
    name = "Pole";
    numberOfSides = 20;

    setVertexArray();
    setIndexArray();
    setColorArray(0.8f, 0.8f, 0.8f);
    float dx1 = 0.0f;
    float dy1 = 0.0f;
    float dz1 = height;
    float dx2 = x2 - x1;
    float dy2 = y2 - y1;
    float dz2 = z2 - z1;

    float x = dy1 * dz2 - dz1 * dy2;
    float y = dz1 * dx2 - dx1 * dz2;
    float z = dx1 * dy2 - dy1 * dx2;
    float r = sqrt(x*x + y*y + z*z);
    if (r == 0) r = 1.0f;
    x /= r;
    y /= r;
    z /= r;
    vec3 v1(dx1, dy1, dz1);
    vec3 v2(dx2, dy2, dz2);
    float angle = calculateAngle(v1, v2);
    //float pi = 3.14159265f;
    float cos_a = cosf(angle / 2.0f);
    float sin_a = sinf(angle / 2.0f);
    float X = x * sin_a;
    float Y = y * sin_a;
    float Z = z * sin_a;
    float W = cos_a;
    float xx = X * X;
    float xy = X * Y;
    float xz = X * Z;
    float xw = X * W;

    float yy = Y * Y;
    float yz = Y * Z;
    float yw = Y * W;

    float zz = Z * Z;
    float zw = Z * W;

    float mat[9];

    mat[0]  = 1.0f - 2.0f * ( yy + zz );
    mat[1]  =     2.0f * ( xy - zw );
    mat[2]  =     2.0f * ( xz + yw );

    mat[3]  =     2.0f * ( xy + zw );
    mat[4]  = 1.0f - 2.0f * ( xx + zz );
    mat[5]  =     2.0f * ( yz - xw );

    mat[6]  =     2.0f * ( xz - yw );
    mat[7]  =     2.0f * ( yz + xw );
    mat[8]  = 1.0f - 2.0f * ( xx + yy );

    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        float x = vertexArray[i * 3];
        float y = vertexArray[i * 3 + 1];
        float z = vertexArray[i * 3 + 2];
        vertexArray[i * 3] = mat[0] * x + mat[1] * y + mat[2] * z + (x1 + x2) / 2.0f;
        vertexArray[i * 3 + 1] = mat[3] * x + mat[4] * y + mat[5] * z + (y1 + y2) / 2.0f;
        vertexArray[i * 3 + 2] = mat[6] * x + mat[7] * y + mat[8] * z + (z1 + z2) / 2.0f;
    }
    setNormalArray();
    elementX = x1;
    elementY = y1;
}

Pole::Pole(const Pole &source)
{
    elementX = source.elementX;
    elementY = source.elementY;
    diameter = source.diameter;
    height = source.height;
    x1 = source.x1;
    y1 = source.y1;
    z1 = source.z1;
    x2 = source.x2;
    y2 = source.y2;
    z2 = source.z2;
    layer = 2;
    indexOfSelectedControl = -1;
    name = "Pole";
    numberOfSides = source.numberOfSides;
    selected = fixed = false;
    vertexArray.resize(source.vertexArray.size());
    for (int i = 0; i < source.vertexArray.size(); ++i)
        vertexArray[i] = source.vertexArray[i];

    normalArray.resize(source.normalArray.size());
    for (int i = 0; i < source.normalArray.size(); ++i)
        normalArray[i] = source.normalArray[i];

    colorArray.resize(source.colorArray.size());
    for (int i = 0; i < source.colorArray.size(); ++i)
        colorArray[i] = source.colorArray[i];

    indexArray.resize(source.indexArray.size());
    for (int i = 0; i < source.indexArray.size(); ++i)
        indexArray[i] = source.indexArray[i];
}

Pole::~Pole()
{

}



bool Pole::isSelected()
{
    return selected;
}

void Pole::setSelectedStatus(bool status)
{
    selected = status;
}

void Pole::drawFigure(QGLWidget *)
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);

    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glNormalPointer(GL_FLOAT, 0, normalArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, indexArray.begin());

    glDisable(GL_LIGHTING);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
}

void Pole::drawSelectionFrame()
{
    drawControlElement(0, 5.0f, 10.0f);
}

void Pole::drawMeasurements(QGLWidget *)
{
}

void Pole::move(float dx, float dy, float dz)
{
    if (fixed)
        return;
    elementX += dx;
    elementY += dy;
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
        vertexArray[i * 3 + 2] += dz;
    }
    x1 += dx;
    y1 += dy;
    z1 += dz;
    x2 += dx;
    y2 += dy;
    z2 += dz;
}

void Pole::drawControlElement(int index, float, float pointSize)
{
    switch (index)
    {
    case 0:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(elementX, elementY, 0.0f);
        glEnd();
    }
        break;
    default:
        break;
    }
}

QCursor Pole::getCursorForControlElement(int)
{
    return Qt::CrossCursor;
}

void Pole::resizeByControl(int, float, float, float, float)
{
}

int Pole::getNumberOfControls()
{
    return 1;
}

int Pole::controlsForPoint()
{
    return 1;
}

void Pole::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}

void Pole::getProperties(QFormLayout *layout, QGLWidget *render)
{
    clearProperties(layout);

    QCheckBox* fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    QDoubleSpinBox* diameterSpinBox = new QDoubleSpinBox();
    diameterSpinBox->setKeyboardTracking(false);
    diameterSpinBox->setMinimum(0.0);
    diameterSpinBox->setValue(diameter);
    connect(diameterSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDiameter(double)));
    connect(this, SIGNAL(diameterChanged(double)), diameterSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
    heightSpinBox->setKeyboardTracking(false);
    heightSpinBox->setMinimum(0.0);
    heightSpinBox->setValue(height);
    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
    connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));

    if (render)
    {
        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(diameterSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Диаметр", diameterSpinBox);
    layout->addRow("Высота", heightSpinBox);
}

bool Pole::isFixed()
{
    return fixed;
}

int Pole::getLayer()
{
    return layer;
}

void Pole::clear()
{
}

RoadElement *Pole::getCopy()
{
    Pole* copyElement = new Pole(*this);
    return copyElement;
}

void Pole::setVertexArray()
{
    float r = diameter / 2.0f;
    float pi = 3.14159265f;
    vertexArray.clear();
    float factor = 2.0f * pi / float(numberOfSides);
    for (int i = 0; i <= numberOfSides; ++i)
    {
        float angle = factor * float(i);
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        vertexArray.push_back(elementX + dx);
        vertexArray.push_back(elementY + dy);
        vertexArray.push_back(z);

        vertexArray.push_back(elementX + dx);
        vertexArray.push_back(elementY + dy);
        vertexArray.push_back(z + height);
    }
}

void Pole::setNormalArray()
{
    normalArray.clear();
    for (int i = 0; i < indexArray.size() / 3; ++i)
    {
        float x0 = vertexArray[indexArray[i * 3] * 3];
        float y0 = vertexArray[indexArray[i * 3] * 3 + 1];
        float z0 = vertexArray[indexArray[i * 3] * 3 + 2];

        float x1 = vertexArray[indexArray[i * 3 + 1] * 3];
        float y1 = vertexArray[indexArray[i * 3 + 1] * 3 + 1];
        float z1 = vertexArray[indexArray[i * 3 + 1] * 3 + 2];

        float x2 = vertexArray[indexArray[i * 3 + 2] * 3];
        float y2 = vertexArray[indexArray[i * 3 + 2] * 3 + 1];
        float z2 = vertexArray[indexArray[i * 3 + 2] * 3 + 2];

        float x = (y0 - y1) * (z2 - z1) - (z0 - z1) * (y2 - y1);
        float y = (z0 - z1) * (x2 - x1) - (x0 - x1) * (z2 - z1);
        float z = (x0 - x1) * (y2 - y1) - (y0 - y1) * (x2 - x1);
        float r = sqrt(x*x + y*y + z*z);
        x /= r;
        y /= r;
        z /= r;
        normalArray.push_back((-1.0f) * x);
        normalArray.push_back((-1.0f) * y);
        normalArray.push_back((-1.0f) * z);
    }
}

void Pole::setColorArray(float r, float g, float b)
{
    if (colorArray.size() != vertexArray.size())
        colorArray.resize(vertexArray.size());
    for (int i = 0; i < colorArray.size() / 3; ++i)
    {
        colorArray[i * 3] = r;
        colorArray[i * 3 + 1] = g;
        colorArray[i * 3 + 2] = b;
    }
}

void Pole::setIndexArray()
{
    indexArray.clear();
    for (int i = 0; i < numberOfSides * 2; i += 2)
    {
        indexArray.push_back(i + 1);
        indexArray.push_back(i);
        indexArray.push_back(i + 2);

        indexArray.push_back(i + 1);
        indexArray.push_back(i + 2);
        indexArray.push_back(i + 3);
    }
    int size = (numberOfSides + 1) * 2;

    for (int i = 4; i < size; i += 2)
    {
        indexArray.push_back(0);
        indexArray.push_back(i);
        indexArray.push_back(i - 2);
    }
    for (int i = 5; i < size; i += 2)
    {
        indexArray.push_back(1);
        indexArray.push_back(i - 2);
        indexArray.push_back(i);
    }
}

bool Pole::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}

void Pole::setDiameter(double value)
{
    if (diameter == float(value))
        return;
    diameter = float(value);
    setVertexArray();
    setNormalArray();
    emit diameterChanged(value);
}

void Pole::setHeight(double value)
{
    if (height == float(value))
        return;
    height = float(value);
    z2 = height;
    setVertexArray();
    setNormalArray();
    emit heightChanged(value);
}


std::vector<vec3> Pole::getCoordOfControl(int)
{
    std::vector<vec3> res;
    return res;
}

void Pole::setCoordForControl(int , std::vector<vec3> &)
{
}

void Pole::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


QJsonObject Pole::getJSONInfo()
{
    QJsonObject element;
    element["Name"] = name;
    //element["X"] = elementX;
    //element["Y"] = elementY;
    element["X1"] = x1;
    element["Y1"] = y1;
    element["Z1"] = z1;
    element["X2"] = x2;
    element["Y2"] = y2;
    element["Z2"] = z2;
    element["Diameter"] = diameter;
    //element["Height"] = height;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    return element;
}


void Pole::rotate(float angle, float x, float y, float)
{
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        float tx = 0.0f, ty = 0.0f;
        vertexArray[i * 3] -= x;
        vertexArray[i * 3 + 1] -= y;
        tx = vertexArray[i * 3];
        ty = vertexArray[i * 3 + 1];
        vertexArray[i * 3] = tx * cos(angle) - ty * sin(angle);
        vertexArray[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
        vertexArray[i * 3] += x;
        vertexArray[i * 3 + 1] += y;
    }
    float tx = 0.0f, ty = 0.0f;
    x1 -= x;
    y1 -= y;
    tx = x1;
    ty = y1;
    x1 = tx * cos(angle) - ty * sin(angle);
    y1 = tx * sin(angle) + ty * cos(angle);
    x1 += x;
    y1 += y;

    tx = 0.0f, ty = 0.0f;
    x2 -= x;
    y2 -= y;
    tx = x2;
    ty = y2;
    x2 = tx * cos(angle) - ty * sin(angle);
    y2 = tx * sin(angle) + ty * cos(angle);
    x2 += x;
    y2 += y;
    setNormalArray();
    elementX = x1;
    elementY = y1;
}
