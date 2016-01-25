#include "beziers.h"

Beziers::Beziers()
{

}

Beziers::Beziers(float x1, float y1, float x2, float y2, float x3, float y3, int numberOfSides)
{
    name = "Beziers";
    layer = 1;
    indexOfSelectedControl = -1;

    controlPoints.push_back(x1);
    controlPoints.push_back(y1);
    controlPoints.push_back(0.0f);
    controlPoints.push_back(x2);
    controlPoints.push_back(y2);
    controlPoints.push_back(0.0f);
    controlPoints.push_back(x3);
    controlPoints.push_back(y3);
    controlPoints.push_back(0.0f);

    this->step = 1.0f / float(numberOfSides);
    this->numberOfSides = numberOfSides;
    setVertexArray();
    setColorArray(0.0f, 0.0f, 0.0f);
    setIndexArray();

}

Beziers::Beziers(QVector<float> &points, int numberOfSides)
{
    name = "Beziers";
    layer = 1;
    indexOfSelectedControl = -1;

    for (int i = 0; i < points.size(); ++i)
        controlPoints.push_back(points[i]);

    this->step = 1.0f / float(numberOfSides);
    this->numberOfSides = numberOfSides;

    setVertexArray();
    setColorArray(0.0f, 0.0f, 0.0f);
    setIndexArray();
}

Beziers::~Beziers()
{

}



bool Beziers::isSelected()
{
    return selected;
}

void Beziers::setSelectedStatus(bool status)
{
    selected = status;
}

void Beziers::drawFigure(QGLWidget *)
{
    glDisable(GL_LIGHTING);
    glDisableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glDrawElements(GL_LINE_STRIP, indexArray.size(), GL_UNSIGNED_INT, indexArray.begin());

    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);


}

void Beziers::drawSelectionFrame()
{
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < controlPoints.size() / 3; ++i)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(controlPoints[i * 3], controlPoints[i * 3 + 1], controlPoints[i * 3 + 2]);
    }
    glEnd();
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.0f);
}

void Beziers::drawMeasurements(QGLWidget *)
{
}

void Beziers::move(float dx, float dy, float)
{
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
    for (int i = 0; i < controlPoints.size() / 3; ++i)
    {
        controlPoints[i * 3] += dx;
        controlPoints[i * 3 + 1] += dy;
    }
}

void Beziers::drawControlElement(int index, float, float pointSize)
{
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(controlPoints[index * 3], controlPoints[index * 3 + 1], controlPoints[index * 3 + 2]);
    glEnd();
}

QCursor Beziers::getCursorForControlElement(int)
{
    return Qt::CrossCursor;
}

void Beziers::resizeByControl(int index, float dx, float dy, float, float)
{
    controlPoints[index * 3] += dx;
    controlPoints[index * 3 + 1] += dy;
    setVertexArray();
}

int Beziers::getNumberOfControls()
{
    return controlPoints.size() / 3;
}

int Beziers::controlsForPoint()
{
    return 1;
}

void Beziers::changeColorOfSelectedControl(int)
{
}

void Beziers::getProperties(QVBoxLayout *, QGLWidget *)
{
}

bool Beziers::isFixed()
{
    return fixed;
}

int Beziers::getLayer()
{
    return layer;
}

void Beziers::clear()
{
}

RoadElement *Beziers::getCopy()
{
    return NULL;
}

void Beziers::setVertexArray()
{
    vertexArray.clear();
    for (float i = 0.0f; i <= 1.0001f; i += step)
    {
        vec3 p = findPoint(controlPoints, i);
        vertexArray.push_back(p.x);
        vertexArray.push_back(p.y);
        vertexArray.push_back(p.z);
    }
}

void Beziers::setColorArray(float r, float g, float b)
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

void Beziers::setIndexArray()
{
    indexArray.clear();
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        indexArray.push_back(i);
    }
}

vec3 Beziers::findPoint(QVector<GLfloat>& points, float step)
{
    QVector<GLfloat> newPoints;
    for (int i = 0; i < points.size() / 3 - 1; ++i)
    {
        float x = points[i * 3] + (points[(i + 1) * 3] - points[i * 3]) * step;
        float y = points[i * 3 + 1] + (points[(i + 1) * 3 + 1] - points[i * 3 + 1]) * step;
        newPoints.push_back(x);
        newPoints.push_back(y);
        newPoints.push_back(0.0f);
    }
    if (newPoints.size() / 3 == 2)
    {
        float x1 = newPoints[0];
        float y1 = newPoints[1];
        float x2 = newPoints[3];
        float y2 = newPoints[4];
        float x = x1 + (x2 - x1) * step;
        float y = y1 + (y2 - y1) * step;
        vec3 res(x, y, 0.0f);
        return res;
    }
    else
        return findPoint(newPoints, step);

}

bool Beziers::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}

void Beziers::setNumberOfSides(int value)
{
    if (numberOfSides == value)
        return;
    numberOfSides = value;
    step = 1.0f / float(numberOfSides);
    setVertexArray();
    setColorArray(0.0f, 0.0f, 0.0f);
    setIndexArray();
    emit numberOfSidesChanged(value);
}


void Beziers::rotate(float angle, float x, float y, float)
{
    for (int i = 0; i < controlPoints.size() / 3; ++i)
    {
        float tx = 0.0f, ty = 0.0f;
        controlPoints[i * 3] -= x;
        controlPoints[i * 3 + 1] -= y;
        tx = controlPoints[i * 3];
        ty = controlPoints[i * 3 + 1];
        controlPoints[i * 3] = tx * cos(angle) - ty * sin(angle);
        controlPoints[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
        controlPoints[i * 3] += x;
        controlPoints[i * 3 + 1] += y;
    }
    setVertexArray();
}
