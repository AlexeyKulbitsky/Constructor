#include "linebroken.h"
#include <gl/glu.h>

LineBroken::LineBroken()
{
    name = "noname";
    red = green = blue = alpha = 0.0f;
    width = 0.0f;
    hits = 0;
    indexOfSelectedControl = 0;
    numberOfVerticesOfAxis = 0;
    numberOfAxises = 0;
    selected = 0;
    useColor = true;
    fixed = false;
    indexOfSelectedControl = -1;
}



LineBroken::LineBroken(float width, float* axisVertices, int size, QString name, int layer)
{

    this->layer = layer;
    this->name = name;
    numberOfVertices = 0;
    numberOfPolygones = 0;
    red = green = blue = alpha = 1.0f;

    this->width = width;

    hits = 0;
    indexOfSelectedControl = 0;
    numberOfVerticesOfAxis = 0;
    numberOfAxises = 0;
    selected = 0;
    useColor = true;

    // Заполняем массив осевых точек
    setVertexArrayForAxis(axisVertices, size);
     setColorArrayForAxis(0.0f, 0.0f, 0.0f);
    // Заполняем массив индексов для осевых точек
    setIndexArrayForAxis();
    setVertexArray(width, axisVertices, size);
    setColorArray(1.0f, 1.0f, 1.0f, 1.0f);
    setIndexArray();
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    this->width = width;
    this->red = 1.0f;
    this->green = 1.0f;
    this->blue = 1.0f;
    this->alpha = 1.0f;

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
}

LineBroken::LineBroken(float width, float* axisVertices, int size, float red, float green, float blue, float alpha, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    numberOfVertices = 0;
    numberOfPolygones = 0;
    red = green = blue = alpha = 1.0f;

    this->width = width;

    hits = 0;
    indexOfSelectedControl = 0;
    // Рамка для выделения

    useColor = true;
    numberOfVerticesOfAxis = 0;
    numberOfAxises = 0;
    selected = 0;

    // Заполняем массив осевых точек
    setVertexArrayForAxis(axisVertices, size);
    setColorArrayForAxis(0.0f, 0.0f, 0.0f);
    // Заполняем массив индексов для осевых точек
    setIndexArrayForAxis();

    setVertexArray(width, axisVertices, size);
    setColorArray(red, green, blue, alpha);
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    this->width = width;
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
}

LineBroken::LineBroken(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    this->textureSize = textureSize;
    textureSource = source;
    useColor = false;
    this->width = width;
    hits = 0;
    indexOfSelectedControl = 0;
    numberOfVerticesOfAxis = 0;
    numberOfAxises = 0;
    selected = 0;

    // Заполняем массив осевых точек
    setVertexArrayForAxis(axisVertices, size);
    setColorArrayForAxis(0.0f, 0.0f, 0.0f);

    // Заполняем массив индексов для осевых точек
    setIndexArrayForAxis();

    setVertexArray(width, axisVertices, size);
    //getTextures(source);
    textureID[0] = TextureManager::getInstance()->getID(source);
    setTextureArray();
    setColorArray(red, green, blue, alpha);

    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
}

LineBroken::LineBroken(float width, float *axisVertices, int size, QString source, float textureSize, QString name, int layer, QString description):
    LineBroken(width, axisVertices, size, source, textureSize, name, layer)
{
    this->description = description;
}

LineBroken::~LineBroken()
{

}

// Индексы вершины для отрисовки
void LineBroken::setVertexArray(float width, float* axisVertices, int size)
{
    Logger::getLogger()->writeLog("LineBroken::setVertexArray(float width, float* axisVertices, int size)");
    this->width = width;
    float r = width / 2.0f;


    if (VertexArray.size() != size * 4 - 4)
    {
        VertexArray.clear();

    }
    for (int i = 0; i < size / 3; ++i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            float x1 = axisVertices[i * 3];
            float y1 = axisVertices[i * 3 + 1];
            float x2 = axisVertices[(i + 1) * 3];
            float y2 = axisVertices[(i + 1) * 3 + 1];
            float dx = sqrt(r*r*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
            float dy = sqrt(r*r/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
            if (x1 > x2 && y1 > y2)
            {
                dx *= -1.0f;
                dy *= -1.0f;
            }
            if (x1 <= x2 && y1 >= y2)
            {
                dx *= -1.0f;

            }
            if (x1 >= x2 && y1 <= y2)
            {
                dy *= -1.0f;

            }

            VertexArray.push_back(x1 + dx);
            VertexArray.push_back(y1 - dy);
            VertexArray.push_back(0.02f);

            VertexArray.push_back(x1 - dx);
            VertexArray.push_back(y1 + dy);
            VertexArray.push_back(0.02f);


        }
        else
        {   // Если теукщий индекс - конец осевой линии,
            // то строим перпендикуляр
            if (i == size / 3 - 1)
            {
                float x1 = axisVertices[i * 3];
                float y1 = axisVertices[i * 3 + 1];
                float x2 = axisVertices[(i - 1) * 3];
                float y2 = axisVertices[(i - 1) * 3 + 1];
                float dx = sqrt(r*r*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
                float dy = sqrt(r*r/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
                if (x1 > x2 && y1 > y2)
                {
                    dx *= -1.0f;
                    dy *= -1.0f;
                }
                if (x1 <= x2 && y1 >= y2)
                {
                    dx *= -1.0f;

                }
                if (x1 >= x2 && y1 <= y2)
                {
                    dy *= -1.0f;

                }

                VertexArray.push_back(x1 - dx);
                VertexArray.push_back(y1 + dy);
                VertexArray.push_back(0.02f);

                VertexArray.push_back(x1 + dx);
                VertexArray.push_back(y1 - dy);
                VertexArray.push_back(0.02f);
            }
            else
            {

                float x1 = axisVertices[(i - 1) * 3];
                float y1 = axisVertices[(i - 1) * 3 + 1];
                float x2 = axisVertices[i * 3];
                float y2 = axisVertices[i * 3 + 1];
                float x3 = axisVertices[(i + 1) * 3];
                float y3 = axisVertices[(i + 1) * 3 + 1];
                float num = (x1-x2)*(x3-x2)+(y1-y2)*(y3-y2);
                float den = sqrt(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                float t = num / den;
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float alpha = (acos(t))/2.0f;
                float sa = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
                float pi = 3.1415926f;
                if(sa < 0) // Точка находится справа
                {
                    alpha = pi - alpha;
                }


                t = (x3-x2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float beta = acos(t);
                t = (y3-y2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                if (asin(t) < 0)
                {
                    beta *= -1.0f;
                }
                float hamma = alpha + beta;
                t = sin(alpha);
                if (fabs(t) < 0.000001f)
                    t = 1.0f;
                float dx = (r / sin(alpha)) * cos(hamma);
                float dy = (r / sin(alpha)) * sin(hamma);

                VertexArray.push_back(x2 - dx);
                VertexArray.push_back(y2 - dy);
                VertexArray.push_back(0.02f);

                VertexArray.push_back(x2 + dx);
                VertexArray.push_back(y2 + dy);
                VertexArray.push_back(0.02f);

                VertexArray.push_back(x2 - dx);
                VertexArray.push_back(y2 - dy);
                VertexArray.push_back(0.02f);

                VertexArray.push_back(x2 + dx);
                VertexArray.push_back(y2 + dy);
                VertexArray.push_back(0.02f);
            }
        }
    }

}

// Индексы каждой вершины
void LineBroken::setColorArray(float red, float green, float blue, float alpha)
{
    Logger::getLogger()->writeLog("LineBroken::setColorArray(float red, float green, float blue, float alpha)");
   if (ColorArray.size() / 4 != VertexArray.size() / 3)
   {
       ColorArray.resize(VertexArray.size() / 3 * 4);
   }
   for (int i = 0; i < VertexArray.size() / 3; i++)
   {
      ColorArray[i * 4] = red;
      ColorArray[i * 4 + 1] = green;
      ColorArray[i * 4 + 2] = blue;
      ColorArray[i * 4 + 3] = alpha;
   }
}



void LineBroken::setTextureArray()
{
    Logger::getLogger()->writeLog("LineBroken::setTextureArray()");
    GLfloat x1, y1, x2, y2;
    GLfloat r1 = 0.0f, r2 = 0.0f;
   // GLfloat textureSize = 0.5f;
    if (TextureArray.size() / 2 != VertexArray.size() / 3)
    {
        TextureArray.resize(VertexArray.size() / 3 * 2);
    }
    TextureArray[0] = 0.0f;
    TextureArray[1] = 0.0f;

    TextureArray[2] = 1.0f;
    TextureArray[3] = 0.0f;


    for (int i = 0; i < VertexArray.size() / 3 - 3; i += 4)
    {
        TextureArray[i * 2] = 0.0f;
        TextureArray[i * 2 + 1] = r1 / textureSize;

        TextureArray[(i + 1) * 2] = 1.0f;
        TextureArray[(i + 1) * 2 + 1] = r2 / textureSize;

        x1 = VertexArray[i * 3];
        y1 = VertexArray[i * 3 + 1];
        x2 = VertexArray[(i + 2) * 3];
        y2 = VertexArray[(i + 2) * 3 + 1];
        r1 += sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

        x1 = VertexArray[(i + 1) * 3];
        y1 = VertexArray[(i + 1) * 3 + 1];
        x2 = VertexArray[(i + 3) * 3];
        y2 = VertexArray[(i + 3) * 3 + 1];
        r2 += sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

        TextureArray[(i + 2) * 2] = 0.0f;
        TextureArray[(i + 2) * 2 + 1] = r1 / textureSize;

        TextureArray[(i + 3) * 2] = 1.0f;
        TextureArray[(i + 3) * 2 + 1] = r2 / textureSize;

        if (r1 > r2)
            r2 += (r1 - r2) * 2;
        else
            if (r2 > r1)
                r1 += (r2 - r1) * 2;

    }


}

// Индексы для отрисовки фигуры
void LineBroken::setIndexArray()
{
    Logger::getLogger()->writeLog("LineBroken::setIndexArray()");
    if (IndexArray.size() != VertexArray.size() / 2)
        IndexArray.resize(VertexArray.size() / 2);
    for (int i = 0; i < VertexArray.size() / 3 - 3; i += 4)
    {
        IndexArray[i * 3 / 2] = i;
        IndexArray[i * 3 / 2 + 1] = i + 3;
        IndexArray[i * 3 / 2 + 2] = i + 1;

        IndexArray[i * 3 / 2 + 3] = i;
        IndexArray[i * 3 / 2 + 4] = i + 2;
        IndexArray[i * 3 / 2 + 5] = i + 3;

    }

}

void LineBroken::setVertexArrayForAxis(float *axisVertices, int size)
{
    Logger::getLogger()->writeLog("LineBroken::setVertexArrayForAxis(float *axisVertices, int size)");
    if (axisVertices == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "LineBroken::setVertexArrayForAxis(float *axisVertices, int size), axisVertices = NULL, program terminates",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("LineBroken::setVertexArrayForAxis(float *axisVertices, int size), axisVertices = NULL, program terminates", ERROR_T);
        QApplication::exit(0);
    }
    if (vertexArrayForAxis.size() != size)
    {
        vertexArrayForAxis.resize(size);
    }

    for (int i = 0; i < size; ++i)
    {
        vertexArrayForAxis[i] = axisVertices[i];
    }
    numberOfVerticesOfAxis = size / 3;
}

void LineBroken::setColorArrayForAxis(float red, float green, float blue)
{
    Logger::getLogger()->writeLog("LineBroken::setColorArrayForAxis(float red, float green, float blue)");
    if (colorArrayForAxis.size() != vertexArrayForAxis.size())
    {
        colorArrayForAxis.resize(vertexArrayForAxis.size());
    }
    for (int i = 0; i < vertexArrayForAxis.size() / 3; ++i)
    {
        colorArrayForAxis[i * 3] = red;
        colorArrayForAxis[i * 3 + 1] = green;
        colorArrayForAxis[i * 3 + 2] = blue;
    }
}

void LineBroken::setIndexArrayForAxis()
{
    Logger::getLogger()->writeLog("LineBroken::setIndexArrayForAxis()");
    if (indexArrayForAxis.size() != vertexArrayForAxis.size() / 3)
    {
        indexArrayForAxis.resize(vertexArrayForAxis.size() / 3);
    }

    for (int i = 0; i < vertexArrayForAxis.size() / 3; ++i)
    {
        indexArrayForAxis[i] = i;
    }
}

void LineBroken::drawFigure(QGLWidget* render)
{
    Logger::getLogger()->writeLog("LineBroken::drawFigure(QGLWidget* render)");
    /*
    if (selected == true)
    {
        // Если фигуры выбрана - изменяем цвет заливки
        setColorArray(0.7f, 0.7f, 0.7f, alpha);
        drawSelectionFrame();


        glVertexPointer(3, GL_FLOAT, 0, vertexArrayForAxis.begin());
        glColorPointer(3, GL_FLOAT, 0, colorArrayForAxis.begin());
        glDrawElements(GL_LINE_STRIP, indexArrayForAxis.size(), GL_UNSIGNED_BYTE, indexArrayForAxis.begin());


    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(red, green, blue, alpha);

    }
    */

    ////////////////////////////////////////////////
    if (!useColor)
    {
        glDisableClientState(GL_COLOR_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, textureID[0]);
        //getTextures(textureSource);
        glVertexPointer(3, GL_FLOAT, 0, VertexArray.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, TextureArray.begin());
        glDrawElements(GL_TRIANGLES, IndexArray.size(), GL_UNSIGNED_BYTE, IndexArray.begin());

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        glEnableClientState(GL_COLOR_ARRAY);
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, 0, VertexArray.begin());
        glColorPointer(4, GL_FLOAT, 0, ColorArray.begin());
        glDrawElements(GL_TRIANGLES, IndexArray.size(), GL_UNSIGNED_BYTE, IndexArray.begin());
    }
    if (selected == true)
    {
        glDisable(GL_DEPTH_TEST);
        // Если фигуры выбрана - изменяем цвет заливки
        setColorArray(0.7f, 0.7f, 0.7f, alpha);
        drawSelectionFrame();


        glVertexPointer(3, GL_FLOAT, 0, vertexArrayForAxis.begin());
        glColorPointer(3, GL_FLOAT, 0, colorArrayForAxis.begin());
        glDrawElements(GL_LINE_STRIP, indexArrayForAxis.size(), GL_UNSIGNED_BYTE, indexArrayForAxis.begin());        

    }
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        glDisable(GL_DEPTH_TEST);
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0f);
        glEnable(GL_DEPTH_TEST);
    }

}



//////////////////////////////////////

void LineBroken::setIndexArrayForSelectionFrame()
{
    Logger::getLogger()->writeLog("LineBroken::setIndexArrayForSelectionFrame()");
    if (IndexArrayForSelection.size() != VertexArray.size() / 3)
    {
        IndexArrayForSelection.resize(VertexArray.size() / 3);
    }
    int i = 0;
    int count = 0;
    IndexArrayForSelection[count++] = i++;


    for (; i < VertexArray.size() / 3; i += 2)
    {
       IndexArrayForSelection[count++] = i;
    }

    i = VertexArray.size() / 3 - 2;
    for (; i > 0; i -= 2)
    {
       IndexArrayForSelection[count++] = i;
    }
}

void LineBroken::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    Logger::getLogger()->writeLog("LineBroken::setColorArrayForSelectionFrame(float red, float green, float blue)");
    if (ColorArrayForSelection.size() != VertexArray.size())
    {
        ColorArrayForSelection.resize(VertexArray.size());
    }
    for (int i = 0; i < VertexArray.size() / 3; ++i)
    {
        ColorArrayForSelection[i * 3] = red;
        ColorArrayForSelection[i * 3 + 1] = green;
        ColorArrayForSelection[i * 3 + 2] = blue;
    }
}



void LineBroken::drawSelectionFrame()
{
    Logger::getLogger()->writeLog("LineBroken::drawSelectionFrame()");
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, VertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, ColorArrayForSelection.begin());
    glLineWidth(2.0);
    glDrawElements(GL_LINE_LOOP, IndexArrayForSelection.size(), GL_UNSIGNED_BYTE, IndexArrayForSelection.begin());

    // Угловые точки для изменения размера
    glPointSize(5.0);
    glDrawElements(GL_POINTS, IndexArrayForSelection.size(), GL_UNSIGNED_BYTE, IndexArrayForSelection.begin());

    glPointSize(10.0);
    glVertexPointer(3, GL_FLOAT, 0, vertexArrayForAxis.begin());
    glDrawElements(GL_POINTS, indexArrayForAxis.size(), GL_UNSIGNED_BYTE, indexArrayForAxis.begin());

}


void LineBroken::move(float dx, float dy, float dz)
{
    Logger::getLogger()->writeLog("LineBroken::move(float dx, float dy, float dz)");
    if (fixed)
    {
        return;
    }
    for (int i = 0; i < VertexArray.size() / 3; ++i)
    {
        VertexArray[i * 3] += dx; // X
        VertexArray[i * 3 + 1] += dy; // Y

    }
    for (int i = 0; i < vertexArrayForAxis.size() / 3; ++i)
    {
        vertexArrayForAxis[i * 3] += dx;
        vertexArrayForAxis[i * 3 + 1] += dy;
    }
    setTextureArray();
}

void LineBroken::drawControlElement(int index, float lineWidth, float pointSize)
{
    Logger::getLogger()->writeLog("LineBroken::drawControlElement(int index, float lineWidth, float pointSize)");
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(vertexArrayForAxis[index * 3],
            vertexArrayForAxis[index * 3 + 1],
            vertexArrayForAxis[index * 3 + 2] + 0.05f);
    glEnd();
}


void LineBroken::resizeByControl(int index, float dx, float dy, float x, float y)
{
    Logger::getLogger()->writeLog("LineBroken::resizeByControl(int index, float dx, float dy, float x, float y)");
    if (fixed)
    {
        return;
    }
    vertexArrayForAxis[index * 3] += dx;
    vertexArrayForAxis[index * 3 + 1] += dy;
    setVertexArray(this->width, vertexArrayForAxis.begin(), vertexArrayForAxis.size());

    setTextureArray();
}

int LineBroken::getNumberOfControls()
{
    Logger::getLogger()->writeLog("LineBroken::getNumberOfControls()");
    return indexArrayForAxis.size();
}

void LineBroken::changeColorOfSelectedControl(int index)
{
    Logger::getLogger()->writeLog("LineBroken::changeColorOfSelectedControl(int index)");
    indexOfSelectedControl = index;
}

QCursor LineBroken::getCursorForControlElement(int index)
{
    Logger::getLogger()->writeLog("LineBroken::getCursorForControlElement(int index)");
    if (index >= 0 && index < indexArrayForAxis.size())
        return Qt::CrossCursor;
    else
        return Qt::ArrowCursor;
}

bool LineBroken::hasPoint(GLfloat x, GLfloat y)
{
    Logger::getLogger()->writeLog("LineBroken::hasPoint(GLfloat x, GLfloat y)");
    if (x >= VertexArray[0] &&
            x <= VertexArray[6] &&
            y >= VertexArray[1] &&
            y <= VertexArray[7])
    {
        return true;
    }
    else
    {
        return false;
    }
}

QPoint LineBroken::getCoorninateOfPointControl(int index)
{
    Logger::getLogger()->writeLog("LineBroken::getCoorninateOfPointControl(int index)");
    QPoint p;
    p.setX(VertexArray[index * 3]);
    p.setY(VertexArray[index * 3 + 1]);

    return p;
}


void LineBroken::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    Logger::getLogger()->writeLog("LineBroken::drawDescription(QGLWidget *render, float red, float green, float blue)");
    glColor3f(red, green, blue);
    if (render && description[0] != '\0')
    {
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        x = vertexArrayForAxis[vertexArrayForAxis.size() / 6 * 3];
        y = vertexArrayForAxis[vertexArrayForAxis.size() / 6 * 3 + 1];
        z = 0.0f;
        QFont shrift = QFont("Times", 8, QFont::Black);
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, description, shrift);
    }
}

void LineBroken::addControl(float x, float y)
{

}


QJsonObject LineBroken::getJSONInfo()
{
    Logger::getLogger()->writeLog("LineBroken::getJSONInfo()");
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    element["UseColor"] = useColor;
    if (useColor)
    {
        QJsonArray temp;
        temp.append(QJsonValue(red));
        temp.append(QJsonValue(green));
        temp.append(QJsonValue(blue));
        temp.append(QJsonValue(alpha));

        element["Color"] = temp;
    }
    else
    {
        element["TextureSource"] = textureSource;
        element["TextureSize"] = textureSize;
    }
    element["Width"] = width;


    QJsonArray temp;

    for (int i = 0; i < vertexArrayForAxis.size(); ++i)
    {
        temp.append(QJsonValue(vertexArrayForAxis[i]));
    }
    element["Vertices"] = temp;

    return element;


}


void LineBroken::getProperties(QFormLayout *layout, QGLWidget* render)
{
    Logger::getLogger()->writeLog("LineBroken::getProperties(QFormLayout *layout, QGLWidget* render)");
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}



bool LineBroken::isFixed()
{
    Logger::getLogger()->writeLog("LineBroken::isFixed()");
    return fixed;
}

void LineBroken::addBreak(bool front)
{
    Logger::getLogger()->writeLog("LineBroken::addBreak(bool front)");
    float x, y, z;
    if (front)
    {
        x = vertexArrayForAxis[0];
        y = vertexArrayForAxis[1];
        z = vertexArrayForAxis[2];
        vertexArrayForAxis.push_front(z);
        vertexArrayForAxis.push_front(y);
        vertexArrayForAxis.push_front(x);
    }
    else
    {
        int size = vertexArrayForAxis.size();
        x = vertexArrayForAxis[size - 3];
        y = vertexArrayForAxis[size - 2];
        z = vertexArrayForAxis[size - 1];
        vertexArrayForAxis.push_back(x);
        vertexArrayForAxis.push_back(y);
        vertexArrayForAxis.push_back(z);
    }
    setColorArrayForAxis(0.0f, 0.0f, 0.0f);
    setIndexArrayForAxis();
    setVertexArray(this->width, vertexArrayForAxis.begin(), vertexArrayForAxis.size());
    setTextureArray();
    setColorArray(red, green, blue, alpha);
    setIndexArray();
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);
}


void LineBroken::drawMeasurements(QGLWidget *render)
{
}


bool LineBroken::setFixed(bool fixed)
{
    Logger::getLogger()->writeLog("LineBroken::setFixed(bool fixed)");
    this->fixed = fixed;
}

void LineBroken::setDescription(QString description)
{
    Logger::getLogger()->writeLog("LineBroken::setDescription(QString description)");
    this->description = description;
}


int LineBroken::getLayer()
{
    Logger::getLogger()->writeLog("LineBroken::getLayer()");
    return layer;
}


void LineBroken::clear()
{
}


std::vector<vec3> LineBroken::getCoordOfControl(int index)
{
    Logger::getLogger()->writeLog("LineBroken::getCoordOfControl(int index)");
    std::vector<vec3> res;
    vec3 p(vertexArrayForAxis[index * 3],
            vertexArrayForAxis[index * 3 + 1],
            vertexArrayForAxis[index * 3 + 2]);
    res.push_back(p);
    return res;
}

void LineBroken::rotate(float angle, float x, float y, float z)
{
    Logger::getLogger()->writeLog("LineBroken::rotate(float angle, float x, float y, float z)");
    for (int i = 0; i < vertexArrayForAxis.size() / 3; ++i)
    {
        vertexArrayForAxis[i * 3] -= x;
        vertexArrayForAxis[i * 3 + 1] -= y;
        float tx = vertexArrayForAxis[i * 3];
        float ty = vertexArrayForAxis[i * 3 + 1];
        vertexArrayForAxis[i * 3] = tx * cos(angle) - ty * sin(angle);
        vertexArrayForAxis[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
        vertexArrayForAxis[i * 3] += x;
        vertexArrayForAxis[i * 3 + 1] += y;
    }
    setVertexArray(this->width, vertexArrayForAxis.begin(), vertexArrayForAxis.size());
    setTextureArray();
}
