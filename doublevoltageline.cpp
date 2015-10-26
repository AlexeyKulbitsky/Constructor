#include "doublevoltageline.h"

DoubleVoltageLine::DoubleVoltageLine()
{

}

DoubleVoltageLine::DoubleVoltageLine(float *axisArray, int size, float width,
                                     QString name, int layer)
{
    axisVertexArray.resize(size);
    for (int i = 0; i < size; ++i)
        axisVertexArray[i] = axisArray[i];
    this->height = axisVertexArray[2];
    this->width = width;
    this->name = name;
    this->layer = layer;
    selected = fixed = false;
    setVertexArray();
}

DoubleVoltageLine::~DoubleVoltageLine()
{
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i])
        {
            delete lines[i];
        }
        lines[i] = NULL;
    }
    model = NULL;
}

bool DoubleVoltageLine::isSelected()
{
    return selected;
}

void DoubleVoltageLine::setSelectedStatus(bool status)
{
    selected = status;
}

void DoubleVoltageLine::drawFigure(QGLWidget *render)
{
    if (selected)
        drawSelectionFrame();
    for (int i = 0; i < lines.size(); ++i)
        lines[i]->drawFigure(render);
}

void DoubleVoltageLine::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 10.0f, 10.0f);
}

void DoubleVoltageLine::drawMeasurements(QGLWidget *render)
{

}

void DoubleVoltageLine::move(float dx, float dy, float dz)
{
    if (fixed)
        return;
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
    {
        axisVertexArray[i * 3] += dx;
        axisVertexArray[i * 3 + 1] += dy;
        axisVertexArray[i * 3 + 2] += dz;
    }
    for (int i = 0; i < lines.size(); ++i)
        lines[i]->move(dx, dy, dz);
}

void DoubleVoltageLine::drawControlElement(int index, float lineWidth, float pointSize)
{
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glColor3d(0.0f, 0.0f, 0.0f);
    glVertex3f(axisVertexArray[index * 3],
                axisVertexArray[index * 3 + 1],
                axisVertexArray[index * 3 + 1]);
    glEnd();
}

QCursor DoubleVoltageLine::getCursorForControlElement(int index)
{
    if (index >=0 && index < getNumberOfControls())
        return Qt::CrossCursor;
    else
        return Qt::ArrowCursor;
}

void DoubleVoltageLine::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
        return;
    axisVertexArray[index * 3] += dx;
    axisVertexArray[index * 3 + 1] += dy;
    setVertexArray();
}

int DoubleVoltageLine::getNumberOfControls()
{
    return axisVertexArray.size() / 3;
}

int DoubleVoltageLine::controlsForPoint()
{
    return 1;
}

void DoubleVoltageLine::changeColorOfSelectedControl(int index)
{

}

void DoubleVoltageLine::getProperties(QFormLayout *layout, QGLWidget *render)
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
    layout->addRow("Ширина между проводами", widthSpinBox);
    layout->addRow("Высота", heightSpinBox);
}

bool DoubleVoltageLine::isFixed()
{
    return fixed;
}

int DoubleVoltageLine::getLayer()
{
    return layer;
}

void DoubleVoltageLine::clear()
{
    for (int i = 0; i < lines.size(); ++i)
        delete lines[i];
}

void DoubleVoltageLine::addBreak(bool front)
{
    if (fixed)
        return;
    float x, y, z;
    if (front)
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            x = lines[i]->axisVertexArray[0];
            y = lines[i]->axisVertexArray[1];
            z = lines[i]->axisVertexArray[2];\
            lines[i]->addBreak(front, x, y, z);
        }
        x = axisVertexArray[0];
        y = axisVertexArray[1];
        z = axisVertexArray[2];
        axisVertexArray.push_front(z);
        axisVertexArray.push_front(y);
        axisVertexArray.push_front(x);

    }
    else
    {
        for (int i = 0; i < lines.size(); ++i)
        {

            int size = lines[i]->axisVertexArray.size();
            x = lines[i]->axisVertexArray[size - 3];
            y = lines[i]->axisVertexArray[size - 2];
            z = lines[i]->axisVertexArray[size - 1];
            lines[i]->addBreak(front, x, y, z);
        }
        int size = axisVertexArray.size();
        x = axisVertexArray[size - 3];
        y = axisVertexArray[size - 2];
        z = axisVertexArray[size - 1];
        axisVertexArray.push_back(x);
        axisVertexArray.push_back(y);
        axisVertexArray.push_back(z);
    }
}

void DoubleVoltageLine::setVertexArray()
{
    if (lines.size() == 0)
    {
        int size = 6;
        float axis[size];
        for (int i = 0; i < size; ++i)
            axis[i] = 0.0f;
        lines.push_back(new VoltageLine(axis, size));
        lines.push_back(new VoltageLine(axis, size));
    }

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

            lines[0]->axisVertexArray[i * 3] = x1 + dx;
            lines[0]->axisVertexArray[i * 3 + 1] = y1 - dy;
            lines[0]->axisVertexArray[i * 3 + 2] = z;

            lines[1]->axisVertexArray[i * 3] = x1 - dx;
            lines[1]->axisVertexArray[i * 3 + 1] = y1 + dy;
            lines[1]->axisVertexArray[i * 3 + 2] = z;

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

                lines[0]->axisVertexArray[i * 3] = x1 - dx;
                lines[0]->axisVertexArray[i * 3 + 1] = y1 + dy;
                lines[0]->axisVertexArray[i * 3 + 2] = z;

                lines[1]->axisVertexArray[i * 3] = x1 + dx;
                lines[1]->axisVertexArray[i * 3 + 1] = y1 - dy;
                lines[1]->axisVertexArray[i * 3 + 2] = z;
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

                lines[0]->axisVertexArray[i * 3] = x2 - dx;
                lines[0]->axisVertexArray[i * 3 + 1] = y2 - dy;
                lines[0]->axisVertexArray[i * 3 + 2] = z;

                lines[1]->axisVertexArray[i * 3] = x2 + dx;
                lines[1]->axisVertexArray[i * 3 + 1] = y2 + dy;
                lines[1]->axisVertexArray[i * 3 + 2] = z;

            }
        }
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        lines[i]->setVertexArray();
    }
}

bool DoubleVoltageLine::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void DoubleVoltageLine::setWidth(double width)
{
    if (this->width == width)
        return;
    this->width = width;
    setVertexArray();
    emit widthChanged(width);
}

void DoubleVoltageLine::setHeight(double height)
{
    if (this->height == height)
        return;
    this->height = height;
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
        axisVertexArray[i * 3 + 2] = height;
    for (int i = 0; i < lines.size(); ++i)
        lines[i]->setHeight(height);
    emit heightChanged(height);
}

