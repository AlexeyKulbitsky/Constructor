#include "cube.h"

Cube::Cube(float x, float y, float z, float delta, QString name, int layer)
{
    setVertexArray(x, y, z, delta);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    this->name = name;
    this->layer = layer;
}

Cube::~Cube()
{
    model = NULL;
}

void Cube::setVertexArray(float x, float y, float z, float delta)
{
    vertexArray.push_back(x - delta);
    vertexArray.push_back(y - delta);
    vertexArray.push_back(z - delta);

    vertexArray.push_back(x + delta);
    vertexArray.push_back(y - delta);
    vertexArray.push_back(z - delta);

    vertexArray.push_back(x + delta);
    vertexArray.push_back(y + delta);
    vertexArray.push_back(z - delta);

    vertexArray.push_back(x - delta);
    vertexArray.push_back(y + delta);
    vertexArray.push_back(z - delta);

    vertexArray.push_back(x - delta);
    vertexArray.push_back(y - delta);
    vertexArray.push_back(z + delta);

    vertexArray.push_back(x + delta);
    vertexArray.push_back(y - delta);
    vertexArray.push_back(z + delta);

    vertexArray.push_back(x + delta);
    vertexArray.push_back(y + delta);
    vertexArray.push_back(z + delta);

    vertexArray.push_back(x - delta);
    vertexArray.push_back(y + delta);
    vertexArray.push_back(z + delta);
}

void Cube::setColorArray(float red, float green, float blue)
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

void Cube::setIndexArray()
{
    indexArray.clear();
    normalArray.clear();

    indexArray.push_back(0);
    indexArray.push_back(3);
    indexArray.push_back(2);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    indexArray.push_back(0);
    indexArray.push_back(2);
    indexArray.push_back(1);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);

    indexArray.push_back(4);
    indexArray.push_back(5);
    indexArray.push_back(6);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    indexArray.push_back(4);
    indexArray.push_back(6);
    indexArray.push_back(7);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);

    indexArray.push_back(0);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(1);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(5);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);

    indexArray.push_back(0);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(5);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(4);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);

    indexArray.push_back(1);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(2);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(6);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);

    indexArray.push_back(1);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(6);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(5);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);

    indexArray.push_back(2);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(3);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(7);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(2);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(7);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(6);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);
    normalArray.push_back(0.0f);

    indexArray.push_back(3);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(0);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(4);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(3);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(4);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    indexArray.push_back(7);
    normalArray.push_back(-1.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);

}



bool Cube::isSelected()
{
    return selected;
}

void Cube::setSelectedStatus(bool status)
{
    selected = status;
}

void Cube::drawFigure(QGLWidget *render)
{

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);

    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glNormalPointer(GL_FLOAT, 0, normalArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    glDisable(GL_LIGHTING);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void Cube::drawSelectionFrame()
{
}

void Cube::drawMeasurements(QGLWidget *render)
{
}

void Cube::move(float dx, float dy, float dz)
{
}

void Cube::drawControlElement(int index, float lineWidth, float pointSize)
{
}

QCursor Cube::getCursorForControlElement(int index)
{
    return Qt::ArrowCursor;
}

void Cube::resizeByControl(int index, float dx, float dy, float x, float y)
{
}

int Cube::getNumberOfControls()
{
    return 20;
}

int Cube::controlsForPoint()
{
    return 4;
}

void Cube::changeColorOfSelectedControl(int index)
{
}

QJsonObject Cube::getJSONInfo()
{
    QJsonObject o;
    return o;
}

void Cube::getProperties(QFormLayout *layout, QGLWidget *render)
{
}

bool Cube::isFixed()
{
}

bool Cube::setFixed(bool fixed)
{
}


int Cube::getLayer()
{
    return layer;
}


void Cube::clear()
{
}
