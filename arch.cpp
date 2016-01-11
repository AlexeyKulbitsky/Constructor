#include "arch.h"

Arch::Arch()
{

}

Arch::Arch(float x, float y, float width, float z, float height)
{
    polesVertical.push_back(new Pole(x - width / 2.0f, y, 0.5f, z + height));
    polesVertical.push_back(new Pole(x + width / 2.0f, y, 0.5f, z + height));
    polesHorizontal.push_back(new Pole(x - width / 2.0f, y, z,
                                       x + width / 2.0f, y, z, 0.2f));
    polesHorizontal.push_back(new Pole(x - width / 2.0f, y, z + height,
                                       x + width / 2.0f, y, z + height, 0.2f));
    int count = 0;
    for (float i = width / (-2.0f); i < width / 2.0f; i += height)
    {
        if (count % 2)
        {
            float x1 = x + i;
            float y1 = y;
            float z1 = z + height;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = x + i + height;
                y2 = y;
                z2 = z;
            }
            else
            {
                x2 = x + width / 2.0f;
                y2 = y;
                z2 = z;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        else
        {
            float x1 = x + i;
            float y1 = y;
            float z1 = z;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = x + i + height;
                y2 = y;
                z2 = z + height;
            }
            else
            {
                x2 = x + width / 2.0f;
                y2 = y;
                z2 = z + height;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        count++;
    }
    selected = fixed = false;
    this->width = width;
    this->height = height;
    this->z = z;
    elementX = x;
    elementY = y;
    rotation = 0.0f;
    layer = 2;
    name = "Arch";
}

Arch::Arch(float x, float y, float width, float z, float height, float rotation)
    : Arch(x, y, width, z, height)
{
    setRotation(rotation);
}

Arch::Arch(const Arch &source)
{
    for (int i = 0; i < source.polesVertical.size(); ++i)
    {
        polesVertical.push_back(qobject_cast<Pole*>(source.polesVertical[i]->getCopy()));
    }
    for (int i = 0; i < source.polesHorizontal.size(); ++i)
    {
        polesHorizontal.push_back(qobject_cast<Pole*>(source.polesHorizontal[i]->getCopy()));
    }
    for (int i = 0; i < source.poles.size(); ++i)
    {
        poles.push_back(qobject_cast<Pole*>(source.poles[i]->getCopy()));
    }
    fixed = false;
    selected = false;
    width = source.width;
    height = source.height;
    z = source.z;
    elementX = source.elementX;
    elementY = source.elementY;
    rotation = source.rotation;
    layer = 2;
    name = "Arch";
}

Arch::~Arch()
{
    for (int i = 0; i < polesVertical.size(); ++i)
    {
        if (polesVertical[i])
            delete polesVertical[i];
        polesVertical[i] = NULL;
    }

    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        if (polesHorizontal[i])
            delete polesHorizontal[i];
        polesHorizontal[i] = NULL;
    }

    for (int i = 0; i < poles.size(); ++i)
    {
        if (poles[i])
            delete poles[i];
        poles[i] = NULL;
    }
    model = NULL;
}



bool Arch::isSelected()
{
    return selected;
}

void Arch::setSelectedStatus(bool status)
{
    selected = status;
}

void Arch::drawFigure(QGLWidget *render)
{
    for (int i = 0; i < polesVertical.size(); ++i)
    {
        polesVertical[i]->drawFigure(render);
    }
    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        polesHorizontal[i]->drawFigure(render);
    }
    for (int i = 0; i < poles.size(); ++i)
    {
        poles[i]->drawFigure(render);
    }
    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
}

void Arch::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.0f);
}

void Arch::drawMeasurements(QGLWidget *)
{
}

void Arch::move(float dx, float dy, float dz)
{
    if (fixed)
        return;
    for (int i = 0; i < polesVertical.size(); ++i)
    {
        polesVertical[i]->move(dx, dy, dz);
    }
    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        polesHorizontal[i]->move(dx, dy, dz);
    }
    for (int i = 0; i < poles.size(); ++i)
    {
        poles[i]->move(dx, dy, dz);
    }
    elementX += dx;
    elementY += dy;
}

void Arch::drawControlElement(int index, float, float pointSize)
{
    switch(index)
    {
    case 0:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(polesVertical[0]->getElementX(), polesVertical[0]->getElementY(), 0.0f);
        glEnd();
    }
        break;
    case 1:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(polesVertical[1]->getElementX(), polesVertical[1]->getElementY(), 0.0f);
        glEnd();
    }
        break;
    default:
        break;
    }
}

QCursor Arch::getCursorForControlElement(int)
{
    return Qt::CrossCursor;
}

void Arch::resizeByControl(int index, float dx, float dy, float, float)
{
    if (fixed)
        return;
    switch (index)
    {
    case 0:
    {
        float X1 = polesVertical[0]->getElementX();
        float Y1 = polesVertical[0]->getElementY();
        float X2 = elementX;
        float Y2 = elementY;
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
        rotate(angle, elementX, elementY, 0.0f);
        rotation += angle / pi * 180.0f;
        emit rotationChanged(rotation);
    }
        break;
    case 1:
    {
        float X1 = polesVertical[1]->getElementX();
        float Y1 = polesVertical[1]->getElementY();
        float X2 = elementX;
        float Y2 = elementY;
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
        rotate(angle, elementX, elementY, 0.0f);
        rotation += angle / pi * 180.0f;
        emit rotationChanged(rotation);
    }
        break;
    default:
        break;
    }
}

int Arch::getNumberOfControls()
{
    return 2;
}

int Arch::controlsForPoint()
{
    return 1;
}

void Arch::changeColorOfSelectedControl(int)
{
}

void Arch::getProperties(QFormLayout *layout, QGLWidget *render)
{
    clearProperties(layout);

    QCheckBox* fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    QDoubleSpinBox* rotationSpinBox = new QDoubleSpinBox();
    rotationSpinBox->setKeyboardTracking(false);
    rotationSpinBox->setMinimum(-720.0);
    rotationSpinBox->setMaximum(720.0f);
    rotationSpinBox->setValue(rotation);
    connect(rotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRotation(double)));
    connect(this, SIGNAL(rotationChanged(double)), rotationSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
    widthSpinBox->setKeyboardTracking(false);
    widthSpinBox->setValue(width);
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
    heightSpinBox->setKeyboardTracking(false);
    heightSpinBox->setValue(height);
    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
    connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* zSpinBox = new QDoubleSpinBox();
    zSpinBox->setKeyboardTracking(false);
    zSpinBox->setValue(z);
    connect(zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
    connect(this, SIGNAL(zChanged(double)), zSpinBox, SLOT(setValue(double)));


    if (render)
    {
        connect(rotationSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(zSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Поворот", rotationSpinBox);
    layout->addRow("Ширина", widthSpinBox);
    layout->addRow("Высота арки", heightSpinBox);
    layout->addRow("Высота от уровня земли", zSpinBox);
}

bool Arch::isFixed()
{
    return fixed;
}

int Arch::getLayer()
{
    return layer;
}

void Arch::clear()
{
}

RoadElement *Arch::getCopy()
{
    Arch* copyElement = new Arch(*this);
    return copyElement;
}

bool Arch::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}

void Arch::setWidth(double value)
{
    if (width == float(value))
        return;
    width = float(value);

    for (int i = 0; i < polesVertical.size(); ++i)
    {
        if (polesVertical[i])
            delete polesVertical[i];
        polesVertical[i] = NULL;
    }
    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        if (polesHorizontal[i])
            delete polesHorizontal[i];
        polesHorizontal[i] = NULL;
    }
    for (int i = 0; i < poles.size(); ++i)
    {
        if (poles[i])
            delete poles[i];
        poles[i] = NULL;
    }

    polesVertical.clear();
    polesHorizontal.clear();
    poles.clear();

    polesVertical.push_back(new Pole(elementX - width / 2.0f, elementY, 0.5f, z + height));
    polesVertical.push_back(new Pole(elementX + width / 2.0f, elementY, 0.5f, z + height));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z,
                                       elementX + width / 2.0f, elementY, z, 0.2f));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z + height,
                                       elementX + width / 2.0f, elementY, z + height, 0.2f));
    int count = 0;
    for (float i = width / (-2.0f); i < width / 2.0f; i += height)
    {
        if (count % 2)
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z + height;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        else
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z + height;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z + height;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        count++;
    }
    float pi = 3.14159265f;
    rotate(rotation * pi / 180.0f, elementX, elementY, 0.0f);
    emit widthChanged(width);
}

void Arch::setRotation(double value)
{
    if (rotation == float(value))
        return;
    float pi = 3.14159265f;
    float angle = (float(value) - rotation) * pi / 180.0f;
    rotate(angle, elementX, elementY, 0.0f);
    rotation = float(value);
    emit rotationChanged(rotation);
}

void Arch::setHeight(double value)
{
    if (height == float(value))
        return;
    height = float(value);
    for (int i = 0; i < polesVertical.size(); ++i)
    {
        if (polesVertical[i])
            delete polesVertical[i];
        polesVertical[i] = NULL;
    }
    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        if (polesHorizontal[i])
            delete polesHorizontal[i];
        polesHorizontal[i] = NULL;
    }
    for (int i = 0; i < poles.size(); ++i)
    {
        if (poles[i])
            delete poles[i];
        poles[i] = NULL;
    }

    polesVertical.clear();
    polesHorizontal.clear();
    poles.clear();

    polesVertical.push_back(new Pole(elementX - width / 2.0f, elementY, 0.5f, z + height));
    polesVertical.push_back(new Pole(elementX + width / 2.0f, elementY, 0.5f, z + height));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z,
                                       elementX + width / 2.0f, elementY, z, 0.2f));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z + height,
                                       elementX + width / 2.0f, elementY, z + height, 0.2f));
    int count = 0;
    for (float i = width / (-2.0f); i < width / 2.0f; i += height)
    {
        if (count % 2)
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z + height;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        else
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z + height;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z + height;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        count++;
    }
    float pi = 3.14159265f;
    rotate(rotation * pi / 180.0f, elementX, elementY, 0.0f);
    emit heightChanged(value);
}

void Arch::setZ(double value)
{
    if (z == float(value))
        return;
    z = float(value);
    for (int i = 0; i < polesVertical.size(); ++i)
    {
        if (polesVertical[i])
            delete polesVertical[i];
        polesVertical[i] = NULL;
    }
    for (int i = 0; i < polesHorizontal.size(); ++i)
    {
        if (polesHorizontal[i])
            delete polesHorizontal[i];
        polesHorizontal[i] = NULL;
    }
    for (int i = 0; i < poles.size(); ++i)
    {
        if (poles[i])
            delete poles[i];
        poles[i] = NULL;
    }

    polesVertical.clear();
    polesHorizontal.clear();
    poles.clear();

    polesVertical.push_back(new Pole(elementX - width / 2.0f, elementY, 0.5f, z + height));
    polesVertical.push_back(new Pole(elementX + width / 2.0f, elementY, 0.5f, z + height));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z,
                                       elementX + width / 2.0f, elementY, z, 0.2f));
    polesHorizontal.push_back(new Pole(elementX - width / 2.0f, elementY, z + height,
                                       elementX + width / 2.0f, elementY, z + height, 0.2f));
    int count = 0;
    for (float i = width / (-2.0f); i < width / 2.0f; i += height)
    {
        if (count % 2)
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z + height;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        else
        {
            float x1 = elementX + i;
            float y1 = elementY;
            float z1 = z;
            float x2, y2, z2;
            if ((i + height) < width / 2.0f)
            {
                x2 = elementX + i + height;
                y2 = elementY;
                z2 = z + height;
            }
            else
            {
                x2 = elementX + width / 2.0f;
                y2 = elementY;
                z2 = z + height;
            }
            poles.push_back(new Pole(x1, y1, z1, x2, y2, z2, 0.05f));
        }
        count++;
    }
    float pi = 3.14159265f;
    rotate(rotation * pi / 180.0f, elementX, elementY, 0.0f);
    emit zChanged(value);
}


void Arch::rotate(float angle, float x, float y, float z)
{
    for (int i = 0; i < polesVertical.size(); ++i)
        polesVertical[i]->rotate(angle, x, y, z);
    for (int i = 0; i < polesHorizontal.size(); ++i)
        polesHorizontal[i]->rotate(angle, x, y, z);
    for (int i = 0; i < poles.size(); ++i)
        poles[i]->rotate(angle, x, y, z);
}


void Arch::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


std::vector<vec3> Arch::getCoordOfControl(int)
{
    std::vector<vec3> res;
    return res;
}

void Arch::setCoordForControl(int, std::vector<vec3>&)
{
}

QJsonObject Arch::getJSONInfo()
{
    QJsonObject element;
    element["Name"] = name;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    element["Height"] = height;
    element["Width"] = width;
    element["Z"] = z;
    element["Rotation"] = rotation;
    element["X"] = elementX;
    element["Y"] = elementY;
    return element;
}
