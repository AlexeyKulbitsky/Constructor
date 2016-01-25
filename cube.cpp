#include "cube.h"

Cube::Cube(float x, float y, float z, float delta, QString name, int layer)
{    
    this->height = delta * 2.0f;
    this->width = delta * 2.0f;
    this->length = delta * 2.0f;
    this->name = name;
    this->layer = layer;
    elementX = x;
    elementY = y;
    this->z = z - height / 2.0f;
    fixed = false;
    selected = false;
    withTexture = false;
    setVertexArray(x, y, z, delta);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setNormalArray();
}

Cube::Cube(float x, float y, float z, float delta, QString texture, QString name, int layer)
{
    this->height = delta * 2.0f;
    this->width = delta * 2.0f;
    this->length = delta * 2.0f;
    this->name = name;
    this->layer = layer;
    elementX = x;
    elementY = y;
    this->z = z - height / 2.0f;
    fixed = false;
    selected = false;
    withTexture = true;
    this->texture = texture;
    textureId = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture);
    setVertexArray(x, y, z, delta);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setNormalArray();
}

Cube::Cube(float x, float y, float z,
           float height, float width, float length,
           QString name, int layer)
{
    this->height = height;
    this->width = width;
    this->length = length;
    this->name = name;
    this->layer = layer;
    fixed = false;
    selected = false;
    elementX = x;
    elementY = y;
    withTexture = false;
    this->z = z - height / 2.0f;
    setVertexArray(x, y, z, height, width, length);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setNormalArray();

}

Cube::Cube(float x, float y, float z, float height, float width, float length, QString texture, QString name, int layer)
{
    this->height = height;
    this->width = width;
    this->length = length;
    this->name = name;
    this->layer = layer;
    fixed = false;
    selected = false;
    elementX = x;
    elementY = y;
    withTexture = true;
    this->texture = texture;
    textureId = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture);
    this->z = z - height / 2.0f;
    setVertexArray(x, y, z, height, width, length);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setNormalArray();
}

Cube::Cube(const Cube &source)
{
    height = source.height;
    width = source.width;
    length = source.length;
    name = source.name;
    layer = source.layer;
    fixed = false;
    selected = false;
    elementX = source.elementX;
    elementY = source.elementY;
    withTexture = source.withTexture;
    if (withTexture)
    {
        texture = source.texture;
        textureId = source.textureId;
    }
    z = source.z;
    float zed = z + height / 2.0f;
    setVertexArray(elementX, elementY, zed, height, width, length);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setNormalArray();
}

Cube::~Cube()
{
    model = NULL;
}

void Cube::setVertexArray(float x, float y, float z, float delta)
{
    vertexArray.clear();

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

    controlPoints.push_back(x - delta);
    controlPoints.push_back(y);
    controlPoints.push_back(z);
    controlPoints.push_back(x + delta);
    controlPoints.push_back(y);
    controlPoints.push_back(z);

    controlPoints.push_back(x);
    controlPoints.push_back(y - delta);
    controlPoints.push_back(z);
    controlPoints.push_back(x);
    controlPoints.push_back(y + delta);
    controlPoints.push_back(z);

    controlPoints.push_back(x);
    controlPoints.push_back(y);
    controlPoints.push_back(z - delta);
    controlPoints.push_back(x);
    controlPoints.push_back(y);
    controlPoints.push_back(z + delta);
}

void Cube::setVertexArray(float x, float y, float z, float height, float width, float length)
{
    vertexArray.clear();

    float dx = length / 2.0f;
    float dy = width / 2.0f;
    float dz = height / 2.0f;

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z + dz);

    controlPoints.push_back(x - dx);
    controlPoints.push_back(y);
    controlPoints.push_back(z);
    controlPoints.push_back(x + dx);
    controlPoints.push_back(y);
    controlPoints.push_back(z);

    controlPoints.push_back(x);
    controlPoints.push_back(y - dy);
    controlPoints.push_back(z);
    controlPoints.push_back(x);
    controlPoints.push_back(y + dy);
    controlPoints.push_back(z);

    controlPoints.push_back(x);
    controlPoints.push_back(y);
    controlPoints.push_back(z - dz);
    controlPoints.push_back(x);
    controlPoints.push_back(y);
    controlPoints.push_back(z + dz);
}

void Cube::setVertexArray()
{

    int i = 0;
    float dx = controlPoints[(i + 1) * 3] - controlPoints[i * 3];
    i = 2;
    float dy = controlPoints[(i + 1) * 3 + 1] - controlPoints[i * 3 + 1];
    i = 3;
    float dz = controlPoints[(i + 1) * 3 + 2] - controlPoints[i * 3 + 2];
    i = 0;
    float x = (controlPoints[(i + 1) * 3] + controlPoints[i * 3]) / 2.0f;
    float y = (controlPoints[(i + 1) * 3 + 1] + controlPoints[i * 3 + 1]) / 2.0f;
    float z = (controlPoints[(i + 1) * 3 + 2] + controlPoints[i * 3 + 2]) / 2.0f;
    elementX = x;
    elementY = y;
    vertexArray.clear();

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z - dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y - dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x + dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z + dz);

    vertexArray.push_back(x - dx);
    vertexArray.push_back(y + dy);
    vertexArray.push_back(z + dz);

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

void Cube::setNormalArray()
{
    normalArray.clear();

    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);

    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(-1.0f);

    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);

    normalArray.push_back(0.0f);
    normalArray.push_back(0.0f);
    normalArray.push_back(1.0f);

    float dx = vertexArray[0 * 3] - vertexArray[3 * 3];
    float dy = vertexArray[0 * 3 + 1] - vertexArray[3 * 3 + 1];

    normalArray.push_back(dx / width);
    normalArray.push_back(dy / width);
    normalArray.push_back(0.0f);

    normalArray.push_back(dx / width);
    normalArray.push_back(dy / width);
    normalArray.push_back(0.0f);

    dx = vertexArray[1 * 3] - vertexArray[0 * 3];
    dy = vertexArray[1 * 3 + 1] - vertexArray[0 * 3 + 1];

    normalArray.push_back(dx / length);
    normalArray.push_back(dy / length);
    normalArray.push_back(0.0f);

    normalArray.push_back(dx / length);
    normalArray.push_back(dy / length);
    normalArray.push_back(0.0f);

    dx = vertexArray[2 * 3] - vertexArray[1 * 3];
    dy = vertexArray[2 * 3 + 1] - vertexArray[1 * 3 + 1];

    normalArray.push_back(dx / width);
    normalArray.push_back(dy / width);
    normalArray.push_back(0.0f);

    normalArray.push_back(dx / width);
    normalArray.push_back(dy / width);
    normalArray.push_back(0.0f);

    dx = vertexArray[0] - vertexArray[1 * 3];
    dy = vertexArray[1] - vertexArray[1 * 3 + 1];

    normalArray.push_back(dx / length);
    normalArray.push_back(dy / length);
    normalArray.push_back(0.0f);

    normalArray.push_back(dx / length);
    normalArray.push_back(dy / length);
    normalArray.push_back(0.0f);

}

void Cube::setIndexArray()
{
    indexArray.clear();

    indexArray.push_back(0);
    indexArray.push_back(3);
    indexArray.push_back(2);

    indexArray.push_back(0);
    indexArray.push_back(2);
    indexArray.push_back(1);


    indexArray.push_back(4);
    indexArray.push_back(5);
    indexArray.push_back(6);

    indexArray.push_back(4);
    indexArray.push_back(6);
    indexArray.push_back(7);


    indexArray.push_back(0);    
    indexArray.push_back(1);
    indexArray.push_back(5);

    indexArray.push_back(0);
    indexArray.push_back(5);
    indexArray.push_back(4);

    indexArray.push_back(1);
    indexArray.push_back(2);
    indexArray.push_back(6);

    indexArray.push_back(1);
    indexArray.push_back(6);
    indexArray.push_back(5);

    indexArray.push_back(2);
    indexArray.push_back(3);
    indexArray.push_back(7);

    indexArray.push_back(2);
    indexArray.push_back(7);
    indexArray.push_back(6);

    indexArray.push_back(3);
    indexArray.push_back(0);
    indexArray.push_back(4);

    indexArray.push_back(3);
    indexArray.push_back(4);
    indexArray.push_back(7);
}



bool Cube::isSelected()
{
    return selected;
}

void Cube::setSelectedStatus(bool status)
{
    selected = status;
}

void Cube::drawFigure(QGLWidget *)
{

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_LIGHTING);

    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, colorArray.begin());
    glNormalPointer(GL_FLOAT, 0, normalArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    if (withTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBegin(GL_TRIANGLES);
        glNormal3f(normalArray[12], normalArray[13], normalArray[14]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(vertexArray[0], vertexArray[1], vertexArray[2]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(vertexArray[15], vertexArray[16], vertexArray[17]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(vertexArray[12], vertexArray[13], vertexArray[14]);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(vertexArray[0], vertexArray[1], vertexArray[2]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(vertexArray[3], vertexArray[4], vertexArray[5]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(vertexArray[15], vertexArray[16], vertexArray[17]);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_LIGHTING);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (selected)
    {
        //glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        //glEnable(GL_DEPTH_TEST);
    }

}

void Cube::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.f);
}

void Cube::drawMeasurements(QGLWidget *)
{
}

void Cube::move(float dx, float dy, float)
{
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
    elementX += dx;
    elementY += dy;
}

void Cube::drawControlElement(int index, float lineWidth, float pointSize)
{
    switch(index)
    {
    case 0:
    {
        int i = 4;
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 1:
    {
        int i = 5;
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 2:
    {
        int i = 6;
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 3:
    {
        int i = 7;
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 4:
    {
        int i = 4;
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[(i + 1) * 3],
                vertexArray[(i + 1) * 3 + 1],
                vertexArray[(i + 1) * 3 + 2]);
        glEnd();
    }
        break;
    case 5:
    {
        int i = 5;
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[(i + 1) * 3],
                vertexArray[(i + 1) * 3 + 1],
                vertexArray[(i + 1) * 3 + 2]);
        glEnd();
    }
        break;
    case 6:
    {
        int i = 6;
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[(i + 1) * 3],
                vertexArray[(i + 1) * 3 + 1],
                vertexArray[(i + 1) * 3 + 2]);
        glEnd();
    }
        break;
    case 7:
    {
        int i = 7;
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArray[(i - 3) * 3],
                vertexArray[(i - 3) * 3 + 1],
                vertexArray[(i - 3) * 3 + 2]);
        glEnd();
    }
        break;
    default:
        break;
    }
}

QCursor Cube::getCursorForControlElement(int)
{
    return Qt::CrossCursor;
}

void Cube::resizeByControl(int index, float dx, float dy, float, float)
{
    switch(index)
    {
    case 0:
    {
        int i = 4;
        float X1 = vertexArray[i * 3];
        float Y1 = vertexArray[i * 3 + 1];
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
    }
        break;
    case 1:
    {
        int i = 5;
        float X1 = vertexArray[i * 3];
        float Y1 = vertexArray[i * 3 + 1];
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
    }
        break;
    case 2:
    {
        int i = 6;
        float X1 = vertexArray[i * 3];
        float Y1 = vertexArray[i * 3 + 1];
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
    }
        break;
    case 3:
    {
        int i = 7;
        float X1 = vertexArray[i * 3];
        float Y1 = vertexArray[i * 3 + 1];
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
    }
        break;
    case 4:
    {
        int i = 5;
        float dx1 = vertexArray[i * 3] - vertexArray[(i + 1) * 3];
        float dy1 = vertexArray[i * 3 + 1] - vertexArray[(i + 1) * 3 + 1];
        float dr = (dx * dx1 + dy * dy1) / width;
        i = 0;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 1;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 4;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 5;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        width += dr;
        emit widthChanged(width);
        float x1 = (vertexArray[0 * 3] + vertexArray[3 * 3]) / 2.0f;
        float y1 = (vertexArray[0 * 3 + 1] + vertexArray[3 * 3 + 1]) / 2.0f;
        float x2 = (vertexArray[1 * 3] + vertexArray[2 * 3]) / 2.0f;
        float y2 = (vertexArray[1 * 3 + 1] + vertexArray[2 * 3 + 1]) / 2.0f;
        elementX = (x1 + x2) / 2.0f;
        elementY = (y1 + y2) / 2.0f;
    }
        break;
    case 5:
    {
        int i = 5;
        float dx1 = vertexArray[i * 3] - vertexArray[(i - 1) * 3];
        float dy1 = vertexArray[i * 3 + 1] - vertexArray[(i - 1) * 3 + 1];
        float dr = (dx * dx1 + dy * dy1) / length;
        i = 1;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 2;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 5;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 6;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        length += dr;
        lengthChanged(length);
        float x1 = (vertexArray[0 * 3] + vertexArray[3 * 3]) / 2.0f;
        float y1 = (vertexArray[0 * 3 + 1] + vertexArray[3 * 3 + 1]) / 2.0f;
        float x2 = (vertexArray[1 * 3] + vertexArray[2 * 3]) / 2.0f;
        float y2 = (vertexArray[1 * 3 + 1] + vertexArray[2 * 3 + 1]) / 2.0f;
        elementX = (x1 + x2) / 2.0f;
        elementY = (y1 + y2) / 2.0f;
    }
        break;
    case 6:
    {
        int i = 6;
        float dx1 = vertexArray[i * 3] - vertexArray[(i - 1) * 3];
        float dy1 = vertexArray[i * 3 + 1] - vertexArray[(i - 1) * 3 + 1];
        float dr = (dx * dx1 + dy * dy1) / width;
        i = 2;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 3;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 6;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        i = 7;
        vertexArray[i * 3] += dx1 / width * dr;
        vertexArray[i * 3 + 1] += dy1 / width * dr;
        width += dr;
        emit widthChanged(width);
        float x1 = (vertexArray[0 * 3] + vertexArray[3 * 3]) / 2.0f;
        float y1 = (vertexArray[0 * 3 + 1] + vertexArray[3 * 3 + 1]) / 2.0f;
        float x2 = (vertexArray[1 * 3] + vertexArray[2 * 3]) / 2.0f;
        float y2 = (vertexArray[1 * 3 + 1] + vertexArray[2 * 3 + 1]) / 2.0f;
        elementX = (x1 + x2) / 2.0f;
        elementY = (y1 + y2) / 2.0f;
    }
        break;
    case 7:
    {
        int i = 4;
        float dx1 = vertexArray[i * 3] - vertexArray[(i + 1) * 3];
        float dy1 = vertexArray[i * 3 + 1] - vertexArray[(i + 1) * 3 + 1];
        float dr = (dx * dx1 + dy * dy1) / length;
        i = 0;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 3;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 4;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        i = 7;
        vertexArray[i * 3] += dx1 / length * dr;
        vertexArray[i * 3 + 1] += dy1 / length * dr;
        length += dr;
        lengthChanged(length);
        float x1 = (vertexArray[0 * 3] + vertexArray[3 * 3]) / 2.0f;
        float y1 = (vertexArray[0 * 3 + 1] + vertexArray[3 * 3 + 1]) / 2.0f;
        float x2 = (vertexArray[1 * 3] + vertexArray[2 * 3]) / 2.0f;
        float y2 = (vertexArray[1 * 3 + 1] + vertexArray[2 * 3 + 1]) / 2.0f;
        elementX = (x1 + x2) / 2.0f;
        elementY = (y1 + y2) / 2.0f;
    }
        break;
    default:
        break;
    }
}

int Cube::getNumberOfControls()
{
    return 8;
}

int Cube::controlsForPoint()
{
    return 4;
}

void Cube::changeColorOfSelectedControl(int)
{
}

QJsonObject Cube::getJSONInfo()
{
    QJsonObject element;
    element["Name"] = name;
    element["Layer"] = layer;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    element["Height"] = height;
    element["Width"] = width;
    element["Length"] = length;
    element["WithTexture"] = withTexture;
    if (withTexture)
        element["Texture"] = texture;
    element["X"] = elementX;
    element["Y"] = elementY;
    element["Z"] = z;

    return element;
}

void Cube::getProperties(QVBoxLayout *layout, QGLWidget *render)
{
//    clearProperties(layout);

//    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
//    widthSpinBox->setKeyboardTracking(false);
//    widthSpinBox->setMinimum(0.001);
//    widthSpinBox->setValue(width);
//    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
//    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));

//    QDoubleSpinBox* lengthSpinBox = new QDoubleSpinBox();
//    lengthSpinBox->setKeyboardTracking(false);
//    lengthSpinBox->setMinimum(0.001);
//    lengthSpinBox->setValue(length);
//    connect(lengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLength(double)));
//    connect(this, SIGNAL(lengthChanged(double)), lengthSpinBox, SLOT(setValue(double)));

//    QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
//    heightSpinBox->setKeyboardTracking(false);
//    heightSpinBox->setMinimum(0.001);
//    heightSpinBox->setValue(height);
//    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
//    connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));

//    QDoubleSpinBox* zSpinBox = new QDoubleSpinBox();
//    zSpinBox->setKeyboardTracking(false);
//    zSpinBox->setMinimum(-1000.0);
//    zSpinBox->setValue(z);
//    connect(zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
//    connect(this, SIGNAL(zChanged(double)), zSpinBox, SLOT(setValue(double)));

//    if (render)
//    {
//        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//        connect(lengthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//        connect(zSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//    }
//    layout->addRow("Ширина", widthSpinBox);
//    layout->addRow("Длина", lengthSpinBox);
//    layout->addRow("Высота элемента", heightSpinBox);
//    layout->addRow("Высота от уровня земли", zSpinBox);
}

bool Cube::isFixed()
{
    return fixed;
}

bool Cube::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}

void Cube::setWidth(double value)
{
    if (width == float(value))
        return;
    float dr = (value - width) / 2.0f;
    float dx = (vertexArray[2 * 3] - vertexArray[1 * 3]) / width * dr;
    float dy = (vertexArray[2 * 3 + 1] - vertexArray[1 * 3 + 1]) / width * dr;

    vertexArray[0] -= dx;
    vertexArray[1] -= dy;
    vertexArray[3] += dx;
    vertexArray[4] -= dy;
    vertexArray[6] += dx;
    vertexArray[7] += dy;
    vertexArray[9] -= dx;
    vertexArray[10] += dy;

    vertexArray[12] -= dx;
    vertexArray[13] -= dy;
    vertexArray[15] += dx;
    vertexArray[16] -= dy;
    vertexArray[18] += dx;
    vertexArray[19] += dy;
    vertexArray[21] -= dx;
    vertexArray[22] += dy;

    width = value;
    emit widthChanged(value);
}

void Cube::setLength(double value)
{
    if (length == float(value))
        return;
    float dr = (value - length) / 2.0f;
    float dx = (vertexArray[1 * 3] - vertexArray[0 * 3]) / length * dr;
    float dy = (vertexArray[1 * 3 + 1] - vertexArray[0 * 3 + 1]) / length * dr;

    vertexArray[0] -= dx;
    vertexArray[1] -= dy;
    vertexArray[3] += dx;
    vertexArray[4] -= dy;
    vertexArray[6] += dx;
    vertexArray[7] += dy;
    vertexArray[9] -= dx;
    vertexArray[10] += dy;

    vertexArray[12] -= dx;
    vertexArray[13] -= dy;
    vertexArray[15] += dx;
    vertexArray[16] -= dy;
    vertexArray[18] += dx;
    vertexArray[19] += dy;
    vertexArray[21] -= dx;
    vertexArray[22] += dy;

    length = value;
    emit lengthChanged(value);
}

void Cube::setHeight(double value)
{
    if (height == float(value))
        return;
    float dr = value - height;
    float dz = (vertexArray[4 * 3 + 2] - vertexArray[0 * 3 + 2]) / height * dr;

    vertexArray[14] += dz;
    vertexArray[17] += dz;
    vertexArray[20] += dz;
    vertexArray[23] += dz;

    height = value;
    emit heightChanged(value);
}

void Cube::setZ(double value)
{
    if (z == float(value))
        return;

    z = value;

    vertexArray[2] = z;
    vertexArray[5] = z;
    vertexArray[8] = z;
    vertexArray[11] = z;

    vertexArray[14] = z + height;
    vertexArray[17] = z + height;
    vertexArray[20] = z + height;
    vertexArray[23] = z + height;


    emit zChanged(value);
}


int Cube::getLayer()
{
    return layer;
}


void Cube::clear()
{
}


RoadElement *Cube::getCopy()
{
    Cube* copyElement = new Cube(*this);
    return copyElement;
}

void Cube::clearProperties(QLayout *layout)
{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


void Cube::rotate(float angle, float x, float y, float)
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
    setNormalArray();
}

void Cube::setColor(float red, float green, float blue)
{
    setColorArray(red, green, blue);
}


std::vector<vec3> Cube::getCoordOfControl(int)
{
    std::vector<vec3> res;
    return res;
}

void Cube::setCoordForControl(int, std::vector<vec3>&)
{
}
