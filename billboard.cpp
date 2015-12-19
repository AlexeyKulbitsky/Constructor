#include "billboard.h"

Billboard::Billboard()
{

}

Billboard::Billboard(float x, float y, float width, float height, float z, QString texture)
{
    support = new Cube(x, y, 0.05f, 0.1f,  1.0f, 1.0f, "Cube", 2);
    pole = new Pole(x, y, 0.25f, z + height);
    Cube* c = new Cube(x, y - 0.175f, z + height / 2.0f, height, 0.1f, width, texture, "Cube", 2);
    plains.push_back(c);
    c = new Cube(x, y + 0.175f, z + height / 2.0f, height, 0.1f, width, texture, "Cube", 2);
    float pi = 3.14159265f;
    c->rotate(pi, x, y + 0.175f, 0.0f);
    plains.push_back(c);
    elementX = x;
    elementY = y;
    name = "Billboard";
    layer = 2;
    this->width = width;
    this->height = height;
    this->z = z;
    this->texture = texture;
    zRadius = 2.0f;
    zRot = 0.0f;
    selected = false;
    fixed = false;
    setZRotVertexArray();
    setZRotColorArray(0.0f, 1.0f, 0.0f);
    setZRotIndexArray();
}

Billboard::Billboard(float x, float y, float width, float height, float z, float rotation, QString texture)
    : Billboard(x, y, width, height, z, texture)
{
    setZRotation(rotation);
}

Billboard::Billboard(const Billboard &source)
{
    support = new Cube(*source.support);
    pole = new Pole(*source.pole);
    for (int i = 0; i < source.plains.size(); ++i)
        plains.push_back(new Cube(*source.plains[i]));
    elementX = source.elementX;
    elementY = source.elementY;
    width = source.width;
    height = source.height;
    z = source.z;    
    name = "Billboard";
    layer = 2;
    zRadius = 2.0f;
    texture = source.texture;
    selected = false;
    fixed = false;
    setZRotVertexArray();
    setZRotColorArray(0.0f, 1.0f, 0.0f);
    setZRotIndexArray();
    setZRotation(source.zRot);
}

Billboard::~Billboard()
{
    for (int i = 0; i < plains.size(); ++i)
        delete plains[i];
    delete pole;
    delete support;
}



bool Billboard::isSelected()
{
    return selected;
}

void Billboard::setSelectedStatus(bool status)
{
    selected = status;
//    support->setSelectedStatus(status);
//    pole->setSelectedStatus(status);
//    for (int i = 0; i < plains.size(); ++i)
//        plains[i]->setSelectedStatus(status);
}

void Billboard::drawFigure(QGLWidget *render)
{
    support->drawFigure(render);
    pole->drawFigure(render);
    for (int i = 0; i < plains.size(); ++i)
        plains[i]->drawFigure(render);
    if (selected)
        drawSelectionFrame();
}

void Billboard::drawSelectionFrame()
{
    drawControlElement(0, 5.0f, 10.f);
}

void Billboard::drawMeasurements(QGLWidget *render)
{
}

void Billboard::move(float dx, float dy, float dz)
{
    if (fixed)
        return;
    support->move(dx, dy, dz);
    pole->move(dx, dy, dz);
    for (int i = 0; i < plains.size(); ++i)
        plains[i]->move(dx, dy, dz);
    elementX += dx;
    elementY += dy;
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotVertexArray[i * 3] += dx;
        zRotVertexArray[i * 3 + 1] += dy;
    }
}

void Billboard::drawControlElement(int index, float lineWidth, float pointSize)
{
    glLineWidth(lineWidth);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(3, GL_FLOAT,0,zRotColorArray.begin());
    glVertexPointer(3, GL_FLOAT, 0, zRotVertexArray.begin());
    glDrawElements(GL_LINE_STRIP,zRotIndexArray.size(),GL_UNSIGNED_INT,zRotIndexArray.begin());
}

QCursor Billboard::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void Billboard::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
        return;
    float X1 = 0.0f;
    float Y1 = zRadius;
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
    zRot += angle * 180.0f / pi;
    support->rotate(angle, elementX, elementY, 0.0f);
    for (int i = 0; i < plains.size(); ++i)
        plains[i]->rotate(angle, elementX, elementY, 0.0f);
    emit zRotationChanged(zRot);
}

int Billboard::getNumberOfControls()
{
    return 1;
}

int Billboard::controlsForPoint()
{
    return 1;
}

void Billboard::changeColorOfSelectedControl(int index)
{
}

void Billboard::getProperties(QFormLayout *layout, QGLWidget *render)
{
    clearProperties(layout);

    QCheckBox* fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
    widthSpinBox->setKeyboardTracking(false);
    widthSpinBox->setMinimum(0.001);
    widthSpinBox->setValue(width);
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
    heightSpinBox->setKeyboardTracking(false);
    heightSpinBox->setMinimum(0.001);
    heightSpinBox->setValue(height);
    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
    connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* zSpinBox = new QDoubleSpinBox();
    zSpinBox->setKeyboardTracking(false);
    zSpinBox->setValue(z);
    connect(zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
    connect(this, SIGNAL(zChanged(double)), zSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* rotationSpinBox = new QDoubleSpinBox();
    rotationSpinBox->setKeyboardTracking(false);
    rotationSpinBox->setMinimum(-720.0f);
    rotationSpinBox->setMaximum(720.0f);
    rotationSpinBox->setValue(zRot);
    connect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZRotation(double)));
    connect(this, SIGNAL(zRotationChanged(double)), rotationSpinBox, SLOT(setValue(double)));

    if (render)
    {
        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(zSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(rotationSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }
    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Ширина", widthSpinBox);
    layout->addRow("Высота", heightSpinBox);
    layout->addRow("Высота от уровня земли", zSpinBox);
    layout->addRow("Поворот", rotationSpinBox);
}

bool Billboard::isFixed()
{
    return fixed;
}

int Billboard::getLayer()
{
    return layer;
}

void Billboard::clear()
{
}

RoadElement *Billboard::getCopy()
{
    Billboard* copyElement = new Billboard(*this);
    return copyElement;
}

Billboard::setZRotVertexArray()
{
    float pi = 3.14159265f;
    int numberOfSides = 40;
    for (int i = 0; i <= numberOfSides; ++i)
    {
        float angle = 2.0f * pi / (float(numberOfSides)) * float(i);
        zRotVertexArray.push_back(elementX + zRadius * cosf(angle));
        zRotVertexArray.push_back(elementY + zRadius * sinf(angle));
        zRotVertexArray.push_back(0.0f);
    }
}

Billboard::setZRotColorArray(float r, float g, float b)
{
    zRotColorArray.resize(zRotVertexArray.size());
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotColorArray[i * 3] = r;
        zRotColorArray[i * 3 + 1] = g;
        zRotColorArray[i * 3 + 2] = b;
    }
}

Billboard::setZRotIndexArray()
{
    zRotIndexArray.resize(zRotVertexArray.size() / 3);
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotIndexArray[i] = i;
    }
}

bool Billboard::setFixed(bool fixed)
{
    this->fixed = fixed;
    support->setFixed(fixed);
    pole->setFixed(fixed);
    for (int i = 0; i < plains.size(); ++i)
        plains[i]->setFixed(fixed);
    return true;
}

void Billboard::setWidth(double value)
{
    if (width == value)
        return;
    width = value;
    for (int i = 0; i < plains.size(); ++i)
        plains[i]->setLength(value);
    emit widthChanged(value);
}

void Billboard::setHeight(double value)
{
    if (height == value)
        return;
    height = value;
    for (int i = 0; i < plains.size(); ++i)
    {
        plains[i]->setHeight(value);
        plains[i]->setZ(z);
    }
    pole->setHeight(z + height);
    emit heightChanged(value);
}

void Billboard::setZ(double value)
{
    if (z == value)
        return;
    z = value;
    for (int i = 0; i < plains.size(); ++i)
    {
        plains[i]->setZ(z);
    }
    pole->setHeight(z + height);
    emit zChanged(value);
}

void Billboard::setZRotation(double value)
{
    if (zRot != value)
    {
        float pi = 3.14159265f;
        float angle = (value - zRot) * pi / 180.0f;
        support->rotate(angle, elementX, elementY, 0.0f);
        for (int i = 0; i < plains.size(); ++i)
            plains[i]->rotate(angle, elementX, elementY, 0.0f);
        zRot = value;
        emit zRotationChanged(value);
    }
}


void Billboard::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


QJsonObject Billboard::getJSONInfo()
{
    QJsonObject element;
    element["X"] = elementX;
    element["Y"] = elementY;
    element["Width"] = width;
    element["Height"] = height;
    element["Z"] = z;
    element["Texture"] = texture;
    element["ZRot"] = zRot;
    element["Name"] = name;
    element["Layer"] = layer;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    return element;
}
