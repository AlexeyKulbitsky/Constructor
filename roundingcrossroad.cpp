#include "roundingcrossroad.h"

RoundingCrossRoad::RoundingCrossRoad()
{

}

RoundingCrossRoad::RoundingCrossRoad(float x, float y, float nearRadius, float farRadius, int numberOfSides, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    numberOfVertices = numberOfSides * 2;
    numberOfPolygones = numberOfSides * 2;

    vertexArray = new GLfloat[numberOfVertices * 3];
    colorArray = new GLfloat[numberOfVertices * 3];
    indexArray = new GLubyte[numberOfVertices * 3];

    indexArrayForSelection = new GLubyte[numberOfVertices * 2];
    colorArrayForSelection = new GLfloat[numberOfVertices * 3];



    setVertexArray(x, y, nearRadius, farRadius, numberOfSides);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    this->xCenter = x;
    this->yCenter = y;
    this->nearRadius = nearRadius;
    this->farRadius = farRadius;
    this->numberOfSides = numberOfSides;
    selected = false;
    fixed = false;
}

RoundingCrossRoad::~RoundingCrossRoad()
{

    delete []vertexArray;
    delete []colorArray;
    delete []indexArray;

    delete []indexArrayForSelection;
    delete []colorArrayForSelection;

    vertexArray = NULL;
    colorArray = NULL;
    indexArray = NULL;

    indexArrayForSelection = NULL;
    colorArrayForSelection = NULL;

}

void RoundingCrossRoad::setVertexArray(float x, float y, float nearRadius, float farRadius, int numberOfSides)
{
    for (int i = 0; i < numberOfSides; ++i)
    {
        float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
        float dx = nearRadius * cosf(angle);
        float dy = nearRadius * sinf(angle);
        vertexArray[i * 6] = x + dx;
        vertexArray[i * 6 + 1] = y + dy;
        vertexArray[i * 6 + 2] = 0.01f;

        dx = farRadius * cosf(angle);
        dy = farRadius * sinf(angle);
        vertexArray[i * 6 + 3] = x + dx;
        vertexArray[i * 6 + 4] = y + dy;
        vertexArray[i * 6 + 5] = 0.01f;
    }
}

void RoundingCrossRoad::setColorArray(float red, float green, float blue)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArray[i * 3] = red;
        colorArray[i * 3 + 1] = green;
        colorArray[i * 3 + 2] = blue;
    }
}

void RoundingCrossRoad::setIndexArray()
{
    int i;
    for (i = 0; i < numberOfVertices - 2; i += 2)
    {
        indexArray[i * 3] = i;
        indexArray[i * 3 + 1] = i + 1;
        indexArray[i * 3 + 2] = i + 3;

        indexArray[i * 3 + 3] = i;
        indexArray[i * 3 + 4] = i + 3;
        indexArray[i * 3 + 5] = i + 2;
    }
    indexArray[i * 3] = i;
    indexArray[i * 3 + 1] = i + 1;
    indexArray[i * 3 + 2] = 1;

    indexArray[i * 3 + 3] = i;
    indexArray[i * 3 + 4] = 1;
    indexArray[i * 3 + 5] = 0;

}

void RoundingCrossRoad::drawFigure(QGLWidget* render)
{
    if (selected == true)
    {
        /*
        if (indexOfSelectedControl >= 0)
        {
            if (indexOfSelectedControl < numberOfVertices / 2)
            {
                for (int i = 0; i < numberOfVertices / 2; ++i)

                    drawControlElement(i, 2.0, 5.0);
            }
            else
            {
                if (indexOfSelectedControl < numberOfVertices)
                {

                    for (int i = numberOfVertices / 2; i < numberOfVertices; ++i)
                    drawControlElement(i, 2.0, 5.0);
                }
            }
        }
        */
        // Если фигуры выбрана - изменяем цвет заливки
        setColorArray(0.7f, 0.7f, 0.7f);
        drawSelectionFrame();
    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(0.5f, 0.5f, 0.5f);

    }
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glColorPointer(3, GL_FLOAT, 0, colorArray);
    glDrawElements(GL_TRIANGLES, numberOfVertices * 3, GL_UNSIGNED_BYTE, indexArray);



}

void RoundingCrossRoad::drawSelectionFrame()
{
    // Боковые грани для изменения размера


    ///////////////////////////////////////////////////////
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glColorPointer(3, GL_FLOAT, 0, colorArrayForSelection);
    glLineWidth(2.0);
    glDrawElements(GL_LINES, numberOfPolygones * 2, GL_UNSIGNED_BYTE, indexArrayForSelection);


}

void RoundingCrossRoad::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArrayForSelection[i * 3] = red;
        colorArrayForSelection[i * 3 + 1] = green;
        colorArrayForSelection[i * 3 + 2] = blue;
    }
}

void RoundingCrossRoad::setIndexArrayForSelectionFrame()
{
    int count = 0;
    int i = 0;
    while (i < numberOfVertices - 2)
    {
        indexArrayForSelection[count] = i;
        indexArrayForSelection[count + 1] = i + 2;
        i += 2;
        count += 2;
    }
    indexArrayForSelection[count] = i;
    indexArrayForSelection[count + 1] = 0;
    count += 2;

    i = 1;
    while (i < numberOfVertices - 1)
    {
        indexArrayForSelection[count] = i;
        indexArrayForSelection[count + 1] = i + 2;
        i += 2;
        count += 2;
    }
    indexArrayForSelection[count] = i;
    indexArrayForSelection[count + 1] = 1;
}

void RoundingCrossRoad::move(float dx, float dy, float dz)
{
    if (fixed)
    {
        return;
    }
    for (int i = 0; i < numberOfVertices; ++i)
    {
        vertexArray[i * 3] += dx; // X
        vertexArray[i* 3 + 1] += dy; // Y

    }
    this->xCenter += dx;
    this->yCenter += dy;

}

void RoundingCrossRoad::drawControlElement(int index, float lineWidth, float pointSize)
{
    int i, j;
    i = indexArrayForSelection[index * 2];
    j = indexArrayForSelection[index * 2 + 1];

    glLineWidth(lineWidth);
    glBegin(GL_LINES);
        glColor3d(1.0, 1.0, 0.1);
       glVertex3d(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                     vertexArray[i * 2 + 2]);
       glColor3d(1.0, 1.0, 0.1);
       glVertex3d(vertexArray[j * 3],
                    vertexArray[j * 3 + 1],
                    vertexArray[j * 2 + 2]);
    glEnd();

}

QCursor RoundingCrossRoad::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void RoundingCrossRoad::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    int j;
    float x1, y1, x2, y2;
    float dr, dR;
    float X, Y;
    X = xCenter - x;
    X = yCenter - y;
    float factor;
    float res = (X * dx + Y * dy);
    factor = res < 0 ? 1 : -1;
    dr = sqrt(dx * dx + dy * dy);
    if (index < numberOfVertices / 2)
    {
        // Изменение координат внутреннего радиуса
        for (int i = 0; i < numberOfVertices / 2; ++i)
        {

            j = indexArrayForSelection[i * 2];
            x1 = vertexArray[j * 3];
            y1 = vertexArray[j * 3 + 1];
            x2 = vertexArray[(j + 1) * 3];
            y2 = vertexArray[(j + 1) * 3 + 1];
            dR = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

            vertexArray[j * 3] += factor * ((x2 - x1) * dr) / dR;
            vertexArray[j * 3 + 1] += factor * ((y2 - y1) * dr) / dR;
            nearRadius += factor * dr;
        }

    }
    else
    {

        if (index < numberOfVertices)
        {
            // Изменение координат наружного радиуса
            for (int i = numberOfVertices / 2; i < numberOfVertices; ++i)
            {

                j = indexArrayForSelection[i * 2];
                x1 = vertexArray[j * 3];
                y1 = vertexArray[j * 3 + 1];
                x2 = vertexArray[(j - 1) * 3];
                y2 = vertexArray[(j - 1) * 3 + 1];
                dR = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

                vertexArray[j * 3] -= factor * ((x2 - x1) * dr) / dR;
                vertexArray[j * 3 + 1] -= factor * ((y2 - y1) * dr) / dR;
                farRadius -= factor * dr;
            }
        }


    }
}




void RoundingCrossRoad::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}


QJsonObject RoundingCrossRoad::getJSONInfo()
{
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    {
        QJsonArray temp;
        temp.append(QJsonValue(0.5f));
        temp.append(QJsonValue(0.5f));
        temp.append(QJsonValue(0.5f));

        element["Color"] = temp;
    }

    element["xCenter"] = xCenter;
    element["yCenter"] = yCenter;
    element["NearRadius"] = nearRadius;
    element["FarRadius"] = farRadius;
    element["NumberOfSides"] = numberOfSides;

    return element;

}


void RoundingCrossRoad::getProperties(QFormLayout *layout, QGLWidget* render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}


bool RoundingCrossRoad::isFixed()
{
    return fixed;
}


void RoundingCrossRoad::drawMeasurements(QGLWidget *render)
{
}


bool RoundingCrossRoad::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int RoundingCrossRoad::getLayer()
{
    return layer;
}


void RoundingCrossRoad::clear()
{
}