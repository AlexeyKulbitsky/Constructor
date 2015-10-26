#include "voltageline.h"
#include <QDoubleSpinBox>
#include <QCheckBox>

VoltageLine::VoltageLine()
{
    selected = fixed = false;
}

VoltageLine::VoltageLine(float *axisArray, int size, float width,
                         QString name, int layer)
{
    axisVertexArray.resize(size);
    for (int i = 0; i < size; ++i)
        axisVertexArray[i] = axisArray[i];
    this->height = axisVertexArray[2];
    this->width = width;
    this->layer = layer;
    this->name = name;
    setVertexArray();
    setIndexArray();
    setColorArray(0.0f, 0.0f, 0.0f, 1.0f);
    selected = fixed = false;
}

VoltageLine::~VoltageLine()
{

}



bool VoltageLine::isSelected()
{
    return selected;
}

void VoltageLine::setSelectedStatus(bool status)
{
    selected = status;
}

void VoltageLine::drawFigure(QGLWidget *render)
{
    //if (!render)
    //    return;
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_COLOR_ARRAY);

    if (selected)
        drawSelectionFrame();
    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_LIGHTING);
}

void VoltageLine::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 10.0f, 10.0f);
}

void VoltageLine::drawMeasurements(QGLWidget *render)
{
}

void VoltageLine::move(float dx, float dy, float dz)
{
    if (fixed)
        return;
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
        vertexArray[i * 3 + 2] += dz;
    }
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
    {
        axisVertexArray[i * 3] += dx;
        axisVertexArray[i * 3 + 1] += dy;
        axisVertexArray[i * 3 + 2] += dz;
    }
}

void VoltageLine::drawControlElement(int index, float lineWidth, float pointSize)
{
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(axisVertexArray[index * 3],
               axisVertexArray[index * 3 + 1],
               axisVertexArray[index * 3 + 2]);
    glEnd();
}

QCursor VoltageLine::getCursorForControlElement(int index)
{
    if (index >=0 && index < getNumberOfControls())
        return Qt::CrossCursor;
    else
        return Qt::ArrowCursor;
}

void VoltageLine::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
        return;
    axisVertexArray[index * 3] += dx;
    axisVertexArray[index * 3 + 1] += dy;
    setVertexArray();
}

int VoltageLine::getNumberOfControls()
{
    return axisVertexArray.size() / 3;
}

int VoltageLine::controlsForPoint()
{
    return 1;
}

void VoltageLine::changeColorOfSelectedControl(int index)
{
}

void VoltageLine::getProperties(QFormLayout *layout, QGLWidget *render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
    widthSpinBox->setKeyboardTracking(false);
    widthSpinBox->setMinimum(0.001);
    widthSpinBox->setValue(width);
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));

    QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
    heightSpinBox->setKeyboardTracking(false);
    heightSpinBox->setMinimum(0.0);
    heightSpinBox->setValue(height);
    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
    connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));

    QCheckBox* fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    if (render)
    {
        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }
    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Толщина", widthSpinBox);
    layout->addRow("Высота", heightSpinBox);
}

bool VoltageLine::isFixed()
{
    return fixed;
}

int VoltageLine::getLayer()
{
    return layer;
}

void VoltageLine::clear()
{
}

void VoltageLine::addBreak(bool front)
{
    if (fixed)
        return;

    float x, y, z;
    if (front)
    {
        x = axisVertexArray[0];
        y = axisVertexArray[1];
        z = axisVertexArray[2];
        axisVertexArray.push_front(z);
        axisVertexArray.push_front(y);
        axisVertexArray.push_front(x);
    }
    else
    {
        int size = axisVertexArray.size();
        x = axisVertexArray[size - 3];
        y = axisVertexArray[size - 2];
        z = axisVertexArray[size - 1];
        axisVertexArray.push_back(x);
        axisVertexArray.push_back(y);
        axisVertexArray.push_back(z);
    }
    setVertexArray();
    setIndexArray();
    setColorArray(0.0f, 0.0f, 0.0f, 1.0f);
}

void VoltageLine::addBreak(bool front, float x, float y, float z)
{
    if (fixed)
        return;

    if (front)
    {
        axisVertexArray.push_front(z);
        axisVertexArray.push_front(y);
        axisVertexArray.push_front(x);
    }
    else
    {
        axisVertexArray.push_back(x);
        axisVertexArray.push_back(y);
        axisVertexArray.push_back(z);
    }
    setVertexArray();
    setIndexArray();
    setColorArray(0.0f, 0.0f, 0.0f, 1.0f);
}

void VoltageLine::setVertexArray()
{
    if (vertexArray.size() != axisVertexArray.size() * 4)
        vertexArray.resize(axisVertexArray.size() * 4);

    float r = width / 2.0f;

    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            float x1 = axisVertexArray[i * 3];
            float y1 = axisVertexArray[i * 3 + 1];
            float x2 = axisVertexArray[(i + 1) * 3];
            float y2 = axisVertexArray[(i + 1) * 3 + 1];
            float z = axisVertexArray[i * 3 + 2];
            float dx = sqrt(r*r*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
            float dy = sqrt(r*r/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
            if (x1 > x2 && y1 > y2)
            {
                dx *= -1.0f;
                dy *= -1.0f;
            }
            if (x1 < x2 && y1 > y2)
            {
                dx *= -1.0f;

            }
            if (x1 > x2 && y1 < y2)
            {
                dy *= -1.0f;

            }

            vertexArray[i * 4 * 3] = x1 + dx;
            vertexArray[i * 4 * 3 + 1] = y1 - dy;
            vertexArray[i * 4 * 3 + 2] = z - r;

            vertexArray[(i * 4 + 1) * 3] = x1 - dx;
            vertexArray[(i * 4 + 1) * 3 + 1] = y1 + dy;
            vertexArray[(i * 4 + 1) * 3 + 2] = z - r;

            vertexArray[(i * 4 + 2) * 3] = x1 - dx;
            vertexArray[(i * 4 + 2) * 3 + 1] = y1 + dy;
            vertexArray[(i * 4 + 2) * 3 + 2] = z + r;

            vertexArray[(i * 4 + 3) * 3] = x1 + dx;
            vertexArray[(i * 4 + 3) * 3 + 1] = y1 - dy;
            vertexArray[(i * 4 + 3) * 3 + 2] = z + r;


        }
        else
        {   // Если теукщий индекс - конец осевой линии,
            // то строим перпендикуляр
            if (i == axisVertexArray.size() / 3 - 1)
            {
                float x1 = axisVertexArray[i * 3];
                float y1 = axisVertexArray[i * 3 + 1];
                float z = axisVertexArray[i * 3 + 2];
                float x2 = axisVertexArray[(i - 1) * 3];
                float y2 = axisVertexArray[(i - 1) * 3 + 1];
                float dx = sqrt(r*r*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
                float dy = sqrt(r*r/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
                if (x1 > x2 && y1 > y2)
                {
                    dx *= -1.0f;
                    dy *= -1.0f;
                }
                if (x1 < x2 && y1 > y2)
                {
                    dx *= -1.0f;

                }
                if (x1 > x2 && y1 < y2)
                {
                    dy *= -1.0f;

                }


                vertexArray[i * 4 * 3] = x1 - dx;
                vertexArray[i * 4 * 3 + 1] = y1 + dy;
                vertexArray[i * 4 * 3 + 2] = z - r;

                vertexArray[(i * 4 + 1) * 3] = x1 + dx;
                vertexArray[(i * 4 + 1) * 3 + 1] = y1 - dy;
                vertexArray[(i * 4 + 1) * 3 + 2] = z - r;

                vertexArray[(i * 4 + 2) * 3] = x1 + dx;
                vertexArray[(i * 4 + 2) * 3 + 1] = y1 - dy;
                vertexArray[(i * 4 + 2) * 3 + 2] = z + r;

                vertexArray[(i * 4 + 3) * 3] = x1 - dx;
                vertexArray[(i * 4 + 3) * 3 + 1] = y1 + dy;
                vertexArray[(i * 4 + 3) * 3 + 2] = z + r;
            }
            else
            {

                float x1 = axisVertexArray[(i - 1) * 3];
                float y1 = axisVertexArray[(i - 1) * 3 + 1];
                float x2 = axisVertexArray[i * 3];
                float y2 = axisVertexArray[i * 3 + 1];
                float z = axisVertexArray[i * 3 + 2];
                float x3 = axisVertexArray[(i + 1) * 3];
                float y3 = axisVertexArray[(i + 1) * 3 + 1];
                float num = (x1-x2)*(x3-x2)+(y1-y2)*(y3-y2);
                float den = sqrt(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                float alpha = (acos(num / den))/2.0f;
                float sa = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
                float pi = 3.1415926f;
                if(sa < 0) // Точка находится справа
                {
                    alpha = pi - alpha;
                }


                float beta = acos((x3-x2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2))));
                if (asin((y3-y2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)))) < 0)
                {
                    beta *= -1.0f;
                }
                float hamma = alpha + beta;
                float dx = (r / sin(alpha)) * cos(hamma);
                float dy = (r / sin(alpha)) * sin(hamma);

                vertexArray[i * 4 * 3] = x2 - dx;
                vertexArray[i * 4 * 3 + 1] = y2 - dy;
                vertexArray[i * 4 * 3 + 2] = z - r;

                vertexArray[(i * 4 + 1) * 3] = x2 + dx;
                vertexArray[(i * 4 + 1) * 3 + 1] = y2 + dy;
                vertexArray[(i * 4 + 1) * 3 + 2] = z - r;

                vertexArray[(i * 4 + 2) * 3] = x2 + dx;
                vertexArray[(i * 4 + 2) * 3 + 1] = y2 + dy;
                vertexArray[(i * 4 + 2) * 3 + 2] = z + r;

                vertexArray[(i * 4 + 3) * 3] = x2 - dx;
                vertexArray[(i * 4 + 3) * 3 + 1] = y2 - dy;
                vertexArray[(i * 4 + 3) * 3 + 2] = z + r;

            }
        }
    }

}

void VoltageLine::setIndexArray()
{
    if (indexArray.size() / 3 - 4 != vertexArray.size() / 3 * 2 - 8)
        indexArray.resize((vertexArray.size() / 3 * 2 - 4) * 3);
    int i;
    for (i = 0; i < vertexArray.size() / 3 - 4; i += 4)
    {
        indexArray[i * 6] = i;
        indexArray[i * 6 + 1] = i + 4;
        indexArray[i * 6 + 2] = i + 7;
        indexArray[i * 6 + 3] = i;
        indexArray[i * 6 + 4] = i + 7;
        indexArray[i * 6 + 5] = i + 3;

        indexArray[i * 6 + 6] = i + 3;
        indexArray[i * 6 + 7] = i + 7;
        indexArray[i * 6 + 8] = i + 6;
        indexArray[i * 6 + 9] = i + 3;
        indexArray[i * 6 + 10] = i + 6;
        indexArray[i * 6 + 11] = i + 2;

        indexArray[i * 6 + 12] = i + 2;
        indexArray[i * 6 + 13] = i + 6;
        indexArray[i * 6 + 14] = i + 5;
        indexArray[i * 6 + 15] = i + 2;
        indexArray[i * 6 + 16] = i + 5;
        indexArray[i * 6 + 17] = i + 1;

        indexArray[i * 6 + 18] = i + 1;
        indexArray[i * 6 + 19] = i + 5;
        indexArray[i * 6 + 20] = i + 4;
        indexArray[i * 6 + 21] = i + 1;
        indexArray[i * 6 + 22] = i + 4;
        indexArray[i * 6 + 23] = i;
    }
    indexArray[i * 6] = 1;
    indexArray[i * 6 + 1] = 0;
    indexArray[i * 6 + 2] = 3;
    indexArray[i * 6 + 3] = 1;
    indexArray[i * 6 + 4] = 3;
    indexArray[i * 6 + 5] = 2;

    int size = vertexArray.size() / 3;
    indexArray[i * 6 + 6] = size - 4;
    indexArray[i * 6 + 7] = size - 3;
    indexArray[i * 6 + 8] = size - 2;
    indexArray[i * 6 + 9] = size - 4;
    indexArray[i * 6 + 10] = size - 2;
    indexArray[i * 6 + 11] = size - 1;
}

void VoltageLine::setColorArray(float red, float green, float blue, float alpha)
{
    if (colorArray.size() != vertexArray.size())
        colorArray.resize(vertexArray.size());
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        colorArray[i * 3] = red;
        colorArray[i * 3 + 1] = green;
        colorArray[i * 3 + 2] = blue;
    }
}

bool VoltageLine::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void VoltageLine::setWidth(double width)
{
    if (this->width == width)
        return;
    this->width = width;
    setVertexArray();
    emit widthChanged(width);
}

void VoltageLine::setHeight(double height)
{
    if (this->height == height)
        return;
    this->height = height;
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
        axisVertexArray[i * 3 + 2] = height;
    setVertexArray();
    emit heightChanged(height);
}
