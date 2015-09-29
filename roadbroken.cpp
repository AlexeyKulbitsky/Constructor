#include "roadbroken.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QApplication>

RoadBroken::RoadBroken()
{

}

RoadBroken::RoadBroken(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    this->size = size;
    this->width = width;
    setVertexArray(x1, y1, x2, y2, width);
    setColorArray(red, green, blue, alpha);
    setIndexArray();

    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    // Рвмка для выбора фигуры
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
}

RoadBroken::RoadBroken(QVector<GLfloat> &vetrexArray, float red, float green, float blue, float alpha, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    setVertexArray(vetrexArray);
    setColorArray(red, green, blue, alpha);
    setIndexArray();

    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;

    // Рвмка для выбора фигуры
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
}

RoadBroken::RoadBroken(float x1, float y1, float x2, float y2, float width,
                       QString texture_1, float texture_1Usize, float texture_1Vsize,
                       QString texture_2, float texture_2Usize, float texture_2Vsize,
                       QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    this->size = size;
    this->width = width;
    this->rightBoardWidth = 2.75f;
    this->leftBoardWidth = 2.75f;
    this->showRightBoard = true;
    this->showLeftBoard = true;
    setVertexArray(x1, y1, x2, y2, width);
    setRightVertexArray();
    setLeftVertexArray();
    setIndexArray();
    setRightIndexArray();
    setLeftIndexArray();
    //textureID[0] = getTextures(texture_1);
    //textureID[1] = getTextures(texture_2);
    textureID[0] = TextureManager::getInstance()->getID(texture_1);
    textureID[1] = TextureManager::getInstance()->getID(texture_2);
    this->texture_1Usize = texture_1Usize;
    this->texture_1Vsize = texture_1Vsize;
    this->texture_2Usize = texture_2Usize;
    this->texture_2Vsize = texture_2Vsize;

    setTextureArray(texture_1Usize,texture_1Vsize);
    setRightTextureArray(texture_2Usize,texture_2Vsize);
    setLeftTextureArray(texture_2Usize,texture_2Vsize);
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    // Рвмка для выбора фигуры
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(1.0f, 1.0f, 1.0f);

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

void RoadBroken::setVertexArray(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat width)
{
    this->width = width;
    float r = width / 2.0f;

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
    vertexArray.push_back(x1 + dx);
    vertexArray.push_back(y1 - dy);
    vertexArray.push_back(0.0f);

    vertexArray.push_back(x1 - dx);
    vertexArray.push_back(y1 + dy);
    vertexArray.push_back(0.0f);

    vertexArray.push_back(x2 + dx);
    vertexArray.push_back(y2 - dy);
    vertexArray.push_back(0.0f);

    vertexArray.push_back(x2 - dx);
    vertexArray.push_back(y2 + dy);
    vertexArray.push_back(0.0f);
}

void RoadBroken::setVertexArray(QVector<GLfloat> &vertexArray)
{
    this->vertexArray.resize(vertexArray.size());
    for (int i = 0; i < vertexArray.size(); ++i)
    {
        this->vertexArray[i] = vertexArray[i];
    }
}

void RoadBroken::setColorArray(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    if (colorArray.size() != vertexArray.size() / 3 * 4)
        colorArray.resize(vertexArray.size() / 3 * 4);
    for (int i = 0; i < vertexArray.size()/3; ++i)
    {
        colorArray[i * 4] = red;
        colorArray[i * 4 + 1] = green;
        colorArray[i * 4 + 2] = blue;
        colorArray[i * 4 + 3] = alpha;
    }
}

void RoadBroken::setTextureArray(float textureUsize, float textureVsize)
{
    textureArray.clear();
    for (int i = 0; i < vertexArray.size() / 3 - 2; i += 4)
    {
        float x0 = vertexArray[i * 3];
        float y0 = vertexArray[i * 3 + 1];
        float x1 = vertexArray[(i + 1) * 3];
        float y1 = vertexArray[(i + 1) * 3 + 1];
        float x2 = vertexArray[(i + 2) * 3];
        float y2 = vertexArray[(i + 2) * 3 + 1];
        float x3 = vertexArray[(i + 3) * 3];
        float y3 = vertexArray[(i + 3) * 3 + 1];
        float pi = 3.1415926f;
        float r1 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
        float r2 = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
        float angle = acos(((x3 - x1) * (x0 - x1) + (y3 - y1) * (y0 - y1))/
                          (r1 * r2));
        textureArray.push_back(r2 * sin(angle)/textureUsize);
        textureArray.push_back(r2 * cos(angle)/textureVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back(0.0f);

        r2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        angle = acos(((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1))/
                     (r1 * r2));
        textureArray.push_back(r2 * sin(angle)/textureUsize);
        textureArray.push_back(r2 * cos(angle)/textureVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back(r1 / textureVsize);
    }
}

void RoadBroken::setLeftVertexArray()
{

    vertexArrayLeft.clear();
    for (int i = 0; i < vertexArray.size() / 3;)
    {
        if (i == 0)
        {
            float x0 = vertexArray[(i + 1) * 3];
            float y0 = vertexArray[(i + 1) * 3 + 1];
            float z0 = vertexArray[(i + 1) * 3 + 2];
            float x1 = vertexArray[i * 3];
            float y1 = vertexArray[i * 3 + 1];
            float z1 = vertexArray[i * 3 + 2];
            float x2 = vertexArray[(i + 3) * 3];
            float y2 = vertexArray[(i + 3) * 3 + 1];
            float z2 = vertexArray[(i + 3) * 3 + 2];
            if (x0 == x2 && y0 == y2)
            {
                x2 = vertexArray[(i + 7) * 3];
                y2 = vertexArray[(i + 7) * 3 + 1];
            }
            float dx = x0 - x1;
            float dy = y0 - y1;
            float r01 = sqrt(dx*dx + dy*dy);
            float pi = 3.1415926f;
            float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                               sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));
            vertexArrayLeft.push_back(x0);
            vertexArrayLeft.push_back(y0);
            vertexArrayLeft.push_back(z0);

            vertexArrayLeft.push_back(x0);
            vertexArrayLeft.push_back(y0);
            vertexArrayLeft.push_back(0.08f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            i += 2;
        }
        else
        if (i == vertexArray.size() / 3 - 2)
        {
            float x0 = vertexArray[(i + 1) * 3];
            float y0 = vertexArray[(i + 1) * 3 + 1];
            float z0 = vertexArray[(i + 1) * 3 + 2];
            float x1 = vertexArray[i * 3];
            float y1 = vertexArray[i * 3 + 1];
            float z1 = vertexArray[i * 3 + 2];
            float x2 = vertexArray[(i - 1) * 3];
            float y2 = vertexArray[(i - 1) * 3 + 1];
            float z2 = vertexArray[(i - 1) * 3 + 2];
            if (x0 == x2 && y0 == y2)
            {
                x2 = vertexArray[(i - 5) * 3];
                y2 = vertexArray[(i - 5) * 3 + 1];
            }
            float dx = x0 - x1;
            float dy = y0 - y1;
            float r01 = sqrt(dx*dx + dy*dy);
            float pi = 3.1415926f;
            float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                    sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

            vertexArrayLeft.push_back(x0);
            vertexArrayLeft.push_back(y0);
            vertexArrayLeft.push_back(z0);

            vertexArrayLeft.push_back(x0);
            vertexArrayLeft.push_back(y0);
            vertexArrayLeft.push_back(0.08f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            vertexArrayLeft.push_back(x0 + dx/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(y0 + dy/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayLeft.push_back(0.1f);

            i += 2;
        }
        else
        {
            float x1 = vertexArray[(i - 1) * 3];
            float y1 = vertexArray[(i - 1) * 3 + 1];
            float x2 = vertexArray[(i + 1) * 3];
            float y2 = vertexArray[(i + 1) * 3 + 1];
            float x3 = vertexArray[(i + 5) * 3];
            float y3 = vertexArray[(i + 5) * 3 + 1];

            if (x1 == x2 && y1 == y2)
            {
                float x0 = vertexArrayLeft[((i - 2)/2*5) * 3];
                float y0 = vertexArrayLeft[((i - 2)/2*5) * 3 + 1];
                float z0 = vertexArrayLeft[((i - 2)/2*5) * 3 + 2];
                float x1 = vertexArrayLeft[((i - 2)/2*5 + 1) * 3];
                float y1 = vertexArrayLeft[((i - 2)/2*5 + 1) * 3 + 1];
                float z1 = vertexArrayLeft[((i - 2)/2*5 + 1) * 3 + 2];
                float x2 = vertexArrayLeft[((i - 2)/2*5 + 2) * 3];
                float y2 = vertexArrayLeft[((i - 2)/2*5 + 2) * 3 + 1];
                float z2 = vertexArrayLeft[((i - 2)/2*5 + 2) * 3 + 2];
                float x3 = vertexArrayLeft[((i - 2)/2*5 + 3) * 3];
                float y3 = vertexArrayLeft[((i - 2)/2*5 + 3) * 3 + 1];
                float z3 = vertexArrayLeft[((i - 2)/2*5 + 3) * 3 + 2];
                float x4 = vertexArrayLeft[((i - 2)/2*5 + 4) * 3];
                float y4 = vertexArrayLeft[((i - 2)/2*5 + 4) * 3 + 1];
                float z4 = vertexArrayLeft[((i - 2)/2*5 + 4) * 3 + 2];

                vertexArrayLeft.push_back(x0);
                vertexArrayLeft.push_back(y0);
                vertexArrayLeft.push_back(z0);

                vertexArrayLeft.push_back(x1);
                vertexArrayLeft.push_back(y1);
                vertexArrayLeft.push_back(z1);

                vertexArrayLeft.push_back(x2);
                vertexArrayLeft.push_back(y2);
                vertexArrayLeft.push_back(z2);

                vertexArrayLeft.push_back(x3);
                vertexArrayLeft.push_back(y3);
                vertexArrayLeft.push_back(z3);

                vertexArrayLeft.push_back(x4);
                vertexArrayLeft.push_back(y4);
                vertexArrayLeft.push_back(z4);

                vertexArrayLeft.push_back(x0);
                vertexArrayLeft.push_back(y0);
                vertexArrayLeft.push_back(z0);

                vertexArrayLeft.push_back(x1);
                vertexArrayLeft.push_back(y1);
                vertexArrayLeft.push_back(z1);

                vertexArrayLeft.push_back(x2);
                vertexArrayLeft.push_back(y2);
                vertexArrayLeft.push_back(z2);

                vertexArrayLeft.push_back(x3);
                vertexArrayLeft.push_back(y3);
                vertexArrayLeft.push_back(z3);

                vertexArrayLeft.push_back(x4);
                vertexArrayLeft.push_back(y4);
                vertexArrayLeft.push_back(z4);

                i += 4;
                continue;
            }
            else
                if (x3 == x2 && y3 == y2)
                {
                    float x0 = vertexArray[(i + 1) * 3];
                    float y0 = vertexArray[(i + 1) * 3 + 1];
                    float z0 = vertexArray[(i + 1) * 3 + 2];
                    float x1 = vertexArray[i * 3];
                    float y1 = vertexArray[i * 3 + 1];
                    float z1 = vertexArray[i * 3 + 2];
                    float x2 = vertexArray[(i - 1) * 3];
                    float y2 = vertexArray[(i - 1) * 3 + 1];
                    float z2 = vertexArray[(i - 1) * 3 + 2];
                    if (x0 == x2 && y0 == y2)
                    {
                        x2 = vertexArray[(i - 5) * 3];
                        y2 = vertexArray[(i - 5) * 3 + 1];
                    }
                    float dx = x0 - x1;
                    float dy = y0 - y1;
                    float r01 = sqrt(dx*dx + dy*dy);
                    float pi = 3.1415926f;
                    float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                            sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

                    vertexArrayLeft.push_back(x0);
                    vertexArrayLeft.push_back(y0);
                    vertexArrayLeft.push_back(z0);

                    vertexArrayLeft.push_back(x0);
                    vertexArrayLeft.push_back(y0);
                    vertexArrayLeft.push_back(0.08f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    vertexArrayLeft.push_back(x0);
                    vertexArrayLeft.push_back(y0);
                    vertexArrayLeft.push_back(z0);

                    vertexArrayLeft.push_back(x0);
                    vertexArrayLeft.push_back(y0);
                    vertexArrayLeft.push_back(0.08f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    vertexArrayLeft.push_back(x0 + dx/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(y0 + dy/r01 * (leftBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayLeft.push_back(0.1f);

                    i += 4;
                    continue;
                }


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


            vertexArrayLeft.push_back(x2);
            vertexArrayLeft.push_back(y2);
            vertexArrayLeft.push_back(0.0f);

            vertexArrayLeft.push_back(x2);
            vertexArrayLeft.push_back(y2);
            vertexArrayLeft.push_back(0.08f);

            float dx = (0.03f / sin(alpha)) * cos(hamma);
            float dy = (0.03f / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            dx = (0.25f / sin(alpha)) * cos(hamma);
            dy = (0.25f / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            dx = (leftBoardWidth / sin(alpha)) * cos(hamma);
            dy = (leftBoardWidth / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            //////////////////////////////////////////////////

            vertexArrayLeft.push_back(x2);
            vertexArrayLeft.push_back(y2);
            vertexArrayLeft.push_back(0.0f);

            vertexArrayLeft.push_back(x2);
            vertexArrayLeft.push_back(y2);
            vertexArrayLeft.push_back(0.08f);

            dx = (0.03f / sin(alpha)) * cos(hamma);
            dy = (0.03f / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            dx = (0.25f / sin(alpha)) * cos(hamma);
            dy = (0.25f / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            dx = (leftBoardWidth / sin(alpha)) * cos(hamma);
            dy = (leftBoardWidth / sin(alpha)) * sin(hamma);

            vertexArrayLeft.push_back(x2 + dx);
            vertexArrayLeft.push_back(y2 + dy);
            vertexArrayLeft.push_back(0.1f);

            i += 4;

        }

    }
}

void RoadBroken::setRightTextureArray(float textureUsize, float textureVsize)
{
    textureArrayRight.clear();
    for (int i = 0; i < vertexArrayRight.size() / 3 - 5; i += 10)
    {
    float x0 = vertexArrayRight[i * 3];
    float y0 = vertexArrayRight[i * 3 + 1];
    float x1 = vertexArrayRight[(i + 1) * 3];
    float y1 = vertexArrayRight[(i + 1) * 3 + 1];
    float x2 = vertexArrayRight[(i + 2) * 3];
    float y2 = vertexArrayRight[(i + 2) * 3 + 1];
    float x3 = vertexArrayRight[(i + 3) * 3];
    float y3 = vertexArrayRight[(i + 3) * 3 + 1];
    float x4 = vertexArrayRight[(i + 4) * 3];
    float y4 = vertexArrayRight[(i + 4) * 3 + 1];
    float x5 = vertexArrayRight[(i + 5) * 3];
    float y5 = vertexArrayRight[(i + 5) * 3 + 1];
    float x6 = vertexArrayRight[(i + 6) * 3];
    float y6 = vertexArrayRight[(i + 6) * 3 + 1];
    float x7 = vertexArrayRight[(i + 7) * 3];
    float y7 = vertexArrayRight[(i + 7) * 3 + 1];
    float x8 = vertexArrayRight[(i + 8) * 3];
    float y8 = vertexArrayRight[(i + 8) * 3 + 1];
    float x9 = vertexArrayRight[(i + 9) * 3];
    float y9 = vertexArrayRight[(i + 9) * 3 + 1];
    float pi = 3.1415926f;

    textureArrayRight.push_back(0.0f);
    textureArrayRight.push_back(0.0f);

    float r1 = sqrt((x5 - x0) * (x5 - x0) + (y5 - y0) * (y5 - y0));

    float factor;

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
        factor = ((x5 - x0) * (x1 - x0) + (y5 - y0) * (y1 - y0))/r1;

    textureArrayRight.push_back(0.08f);
    textureArrayRight.push_back(factor/textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x2 - x0) + (y5 - y0) * (y2 - y0))/r1;

    textureArrayRight.push_back(0.09f);
    textureArrayRight.push_back(factor/textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x3 - x0) + (y5 - y0) * (y3 - y0))/r1;

    textureArrayRight.push_back(0.16666f);
    textureArrayRight.push_back(factor/textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x4 - x0) + (y5 - y0) * (y4 - y0))/r1;

    textureArrayRight.push_back(1.0f);
    textureArrayRight.push_back(factor/textureVsize);

    //////////////////////////////////////////////////////////////

    textureArrayRight.push_back(0.0f);
    textureArrayRight.push_back(r1 / textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x6 - x0) + (y5 - y0) * (y6 - y0))/r1;

    textureArrayRight.push_back(0.08f);
    textureArrayRight.push_back(r1 / textureVsize);

    //r2 = sqrt((x7 - x5) * (x7 - x5) + (y7 - y5) * (y7 - y5));
    //cosAngle = ((x0 - x5) * (x7 - x5) + (y0 - y5) * (y7 - y5))/
    //                  (r1 * r2);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x7 - x0) + (y5 - y0) * (y7 - y0))/r1;

    textureArrayRight.push_back(0.09f);
    textureArrayRight.push_back(factor / textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x8 - x0) + (y5 - y0) * (y8 - y0))/r1;

    textureArrayRight.push_back(0.16666f);
    textureArrayRight.push_back(factor / textureVsize);

    if (x5 == x0 && y5 == y0)
        factor = 0.0f;
    else
    factor = ((x5 - x0) * (x9 - x0) + (y5 - y0) * (y9 - y0))/r1;

    textureArrayRight.push_back(1.0f);
    textureArrayRight.push_back(factor / textureVsize);
    }

}

void RoadBroken::setRightIndexArray()
{
    indexArrayRight.clear();
    for (int i = 0; i < vertexArrayRight.size() / 3 - 5; i += 10)
    {
        indexArrayRight.push_back(i + 1);
        indexArrayRight.push_back(i + 5);
        indexArrayRight.push_back(i);


        indexArrayRight.push_back(i + 1);
        indexArrayRight.push_back(i + 6);
        indexArrayRight.push_back(i + 5);

        indexArrayRight.push_back(i + 2);
        indexArrayRight.push_back(i + 6);
        indexArrayRight.push_back(i + 1);

        indexArrayRight.push_back(i + 2);
        indexArrayRight.push_back(i + 7);
        indexArrayRight.push_back(i + 6);

        indexArrayRight.push_back(i + 3);
        indexArrayRight.push_back(i + 7);
        indexArrayRight.push_back(i + 2);

        indexArrayRight.push_back(i + 3);
        indexArrayRight.push_back(i + 8);
        indexArrayRight.push_back(i + 7);

        indexArrayRight.push_back(i + 4);
        indexArrayRight.push_back(i + 8);
        indexArrayRight.push_back(i + 3);

        indexArrayRight.push_back(i + 4);
        indexArrayRight.push_back(i + 9);
        indexArrayRight.push_back(i + 8);
    }
}

void RoadBroken::setRightVertexArray()
{
    vertexArrayRight.clear();
    for (int i = 0; i < vertexArray.size() / 3;)
    {
        if (i == 0)
        {
            float x0 = vertexArray[i * 3];
            float y0 = vertexArray[i * 3 + 1];
            float z0 = vertexArray[i * 3 + 2];
            float x1 = vertexArray[(i + 1) * 3];
            float y1 = vertexArray[(i + 1) * 3 + 1];
            float z1 = vertexArray[(i + 1) * 3 + 2];
            float x2 = vertexArray[(i + 2) * 3];
            float y2 = vertexArray[(i + 2) * 3 + 1];
            float z2 = vertexArray[(i + 2) * 3 + 2];
            if (x0 == x2 && y0 == y2)
            {
                x2 = vertexArray[(i + 6) * 3];
                y2 = vertexArray[(i + 6) * 3 + 1];
            }
            float dx = x0 - x1;
            float dy = y0 - y1;
            float r01 = sqrt(dx*dx + dy*dy);
            float pi = 3.1415926f;
            float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                               sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

            vertexArrayRight.push_back(x0);
            vertexArrayRight.push_back(y0);
            vertexArrayRight.push_back(z0);

            vertexArrayRight.push_back(x0);
            vertexArrayRight.push_back(y0);
            vertexArrayRight.push_back(0.08f);

            vertexArrayRight.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            vertexArrayRight.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            vertexArrayRight.push_back(x0 + dx/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            i += 2;
        }
        else
        if (i == vertexArray.size() / 3 - 2)
        {

            float x0 = vertexArray[i * 3];
            float y0 = vertexArray[i * 3 + 1];
            float z0 = vertexArray[i * 3 + 2];
            float x1 = vertexArray[(i + 1) * 3];
            float y1 = vertexArray[(i + 1) * 3 + 1];
            float z1 = vertexArray[(i + 1) * 3 + 2];
            float x2 = vertexArray[(i - 2) * 3];
            float y2 = vertexArray[(i - 2) * 3 + 1];
            float z2 = vertexArray[(i - 2) * 3 + 2];
            if (x0 == x2 && y0 == y2)
            {
                x2 = vertexArray[(i - 6) * 3];
                y2 = vertexArray[(i - 6) * 3 + 1];
            }
            float dx = x0 - x1;
            float dy = y0 - y1;
            float r01 = sqrt(dx*dx + dy*dy);
            float pi = 3.1415926f;
            float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                    sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

            vertexArrayRight.push_back(x0);
            vertexArrayRight.push_back(y0);
            vertexArrayRight.push_back(z0);

            vertexArrayRight.push_back(x0);
            vertexArrayRight.push_back(y0);
            vertexArrayRight.push_back(0.08f);

            vertexArrayRight.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            vertexArrayRight.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            vertexArrayRight.push_back(x0 + dx/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(y0 + dy/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
            vertexArrayRight.push_back(0.1f);

            i += 2;

        }
        else
        {
            float x1 = vertexArray[(i - 2) * 3];
            float y1 = vertexArray[(i - 2) * 3 + 1];
            float x2 = vertexArray[i * 3];
            float y2 = vertexArray[i * 3 + 1];
            float x3 = vertexArray[(i + 4) * 3];
            float y3 = vertexArray[(i + 4) * 3 + 1];
            if (x1 == x2 && y1 == y2)

            {

                float x0 = vertexArrayRight[((i - 2)/2*5) * 3];
                float y0 = vertexArrayRight[((i - 2)/2*5) * 3 + 1];
                float z0 = vertexArrayRight[((i - 2)/2*5) * 3 + 2];
                float x1 = vertexArrayRight[((i - 2)/2*5 + 1) * 3];
                float y1 = vertexArrayRight[((i - 2)/2*5 + 1) * 3 + 1];
                float z1 = vertexArrayRight[((i - 2)/2*5 + 1) * 3 + 2];
                float x2 = vertexArrayRight[((i - 2)/2*5 + 2) * 3];
                float y2 = vertexArrayRight[((i - 2)/2*5 + 2) * 3 + 1];
                float z2 = vertexArrayRight[((i - 2)/2*5 + 2) * 3 + 2];
                float x3 = vertexArrayRight[((i - 2)/2*5 + 3) * 3];
                float y3 = vertexArrayRight[((i - 2)/2*5 + 3) * 3 + 1];
                float z3 = vertexArrayRight[((i - 2)/2*5 + 3) * 3 + 2];
                float x4 = vertexArrayRight[((i - 2)/2*5 + 4) * 3];
                float y4 = vertexArrayRight[((i - 2)/2*5 + 4) * 3 + 1];
                float z4 = vertexArrayRight[((i - 2)/2*5 + 4) * 3 + 2];

                vertexArrayRight.push_back(x0);
                vertexArrayRight.push_back(y0);
                vertexArrayRight.push_back(z0);

                vertexArrayRight.push_back(x1);
                vertexArrayRight.push_back(y1);
                vertexArrayRight.push_back(z1);

                vertexArrayRight.push_back(x2);
                vertexArrayRight.push_back(y2);
                vertexArrayRight.push_back(z2);

                vertexArrayRight.push_back(x3);
                vertexArrayRight.push_back(y3);
                vertexArrayRight.push_back(z3);

                vertexArrayRight.push_back(x4);
                vertexArrayRight.push_back(y4);
                vertexArrayRight.push_back(z4);

                vertexArrayRight.push_back(x0);
                vertexArrayRight.push_back(y0);
                vertexArrayRight.push_back(z0);

                vertexArrayRight.push_back(x1);
                vertexArrayRight.push_back(y1);
                vertexArrayRight.push_back(z1);

                vertexArrayRight.push_back(x2);
                vertexArrayRight.push_back(y2);
                vertexArrayRight.push_back(z2);

                vertexArrayRight.push_back(x3);
                vertexArrayRight.push_back(y3);
                vertexArrayRight.push_back(z3);

                vertexArrayRight.push_back(x4);
                vertexArrayRight.push_back(y4);
                vertexArrayRight.push_back(z4);

                i += 4;
                continue;
            }
            else
                if (x2 == x3 && y2 == y3)
                {
                    float x0 = vertexArray[i * 3];
                    float y0 = vertexArray[i * 3 + 1];
                    float z0 = vertexArray[i * 3 + 2];
                    float x1 = vertexArray[(i + 1) * 3];
                    float y1 = vertexArray[(i + 1) * 3 + 1];
                    float z1 = vertexArray[(i + 1) * 3 + 2];
                    float x2 = vertexArray[(i - 2) * 3];
                    float y2 = vertexArray[(i - 2) * 3 + 1];
                    float z2 = vertexArray[(i - 2) * 3 + 2];
                    if (x0 == x2 && y0 == y2)
                    {
                        x2 = vertexArray[(i - 6) * 3];
                        y2 = vertexArray[(i - 6) * 3 + 1];
                    }
                    float dx = x0 - x1;
                    float dy = y0 - y1;
                    float r01 = sqrt(dx*dx + dy*dy);
                    float pi = 3.1415926f;
                    float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                            sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

                    vertexArrayRight.push_back(x0);
                    vertexArrayRight.push_back(y0);
                    vertexArrayRight.push_back(z0);

                    vertexArrayRight.push_back(x0);
                    vertexArrayRight.push_back(y0);
                    vertexArrayRight.push_back(0.08f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    vertexArrayRight.push_back(x0);
                    vertexArrayRight.push_back(y0);
                    vertexArrayRight.push_back(z0);

                    vertexArrayRight.push_back(x0);
                    vertexArrayRight.push_back(y0);
                    vertexArrayRight.push_back(0.08f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (0.03f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (0.25f / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    vertexArrayRight.push_back(x0 + dx/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(y0 + dy/r01 * (rightBoardWidth / cos(angle - pi / 2.0f)));
                    vertexArrayRight.push_back(0.1f);

                    i += 4;
                    continue;
                }
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


            vertexArrayRight.push_back(x2);
            vertexArrayRight.push_back(y2);
            vertexArrayRight.push_back(0.0f);

            vertexArrayRight.push_back(x2);
            vertexArrayRight.push_back(y2);
            vertexArrayRight.push_back(0.08f);

            float dx = (0.03f / sin(alpha)) * cos(hamma);
            float dy = (0.03f / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            dx = (0.25f / sin(alpha)) * cos(hamma);
            dy = (0.25f / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            dx = (rightBoardWidth / sin(alpha)) * cos(hamma);
            dy = (rightBoardWidth / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            //////////////////////////////////////////////////

            vertexArrayRight.push_back(x2);
            vertexArrayRight.push_back(y2);
            vertexArrayRight.push_back(0.0f);

            vertexArrayRight.push_back(x2);
            vertexArrayRight.push_back(y2);
            vertexArrayRight.push_back(0.08f);

            dx = (0.03f / sin(alpha)) * cos(hamma);
            dy = (0.03f / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            dx = (0.25f / sin(alpha)) * cos(hamma);
            dy = (0.25f / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            dx = (rightBoardWidth / sin(alpha)) * cos(hamma);
            dy = (rightBoardWidth / sin(alpha)) * sin(hamma);

            vertexArrayRight.push_back(x2 - dx);
            vertexArrayRight.push_back(y2 - dy);
            vertexArrayRight.push_back(0.1f);

            i += 4;

        }

    }
}

void RoadBroken::setLeftTextureArray(float textureUsize, float textureVsize)
{
    textureArrayLeft.clear();
    for (int i = 0; i < vertexArrayLeft.size() / 3 - 5; i += 10)
    {
    float x0 = vertexArrayLeft[i * 3];
    float y0 = vertexArrayLeft[i * 3 + 1];
    float x1 = vertexArrayLeft[(i + 1) * 3];
    float y1 = vertexArrayLeft[(i + 1) * 3 + 1];
    float x2 = vertexArrayLeft[(i + 2) * 3];
    float y2 = vertexArrayLeft[(i + 2) * 3 + 1];
    float x3 = vertexArrayLeft[(i + 3) * 3];
    float y3 = vertexArrayLeft[(i + 3) * 3 + 1];
    float x4 = vertexArrayLeft[(i + 4) * 3];
    float y4 = vertexArrayLeft[(i + 4) * 3 + 1];
    float x5 = vertexArrayLeft[(i + 5) * 3];
    float y5 = vertexArrayLeft[(i + 5) * 3 + 1];
    float x6 = vertexArrayLeft[(i + 6) * 3];
    float y6 = vertexArrayLeft[(i + 6) * 3 + 1];
    float x7 = vertexArrayLeft[(i + 7) * 3];
    float y7 = vertexArrayLeft[(i + 7) * 3 + 1];
    float x8 = vertexArrayLeft[(i + 8) * 3];
    float y8 = vertexArrayLeft[(i + 8) * 3 + 1];
    float x9 = vertexArrayLeft[(i + 9) * 3];
    float y9 = vertexArrayLeft[(i + 9) * 3 + 1];
    float pi = 3.1415926f;

    textureArrayLeft.push_back(0.0f);
    textureArrayLeft.push_back(0.0f);

    float r1 = sqrt((x5 - x0) * (x5 - x0) + (y5 - y0) * (y5 - y0));
    float factor = ((x5 - x0) * (x1 - x0) + (y5 - y0) * (y1 - y0))/r1;

    textureArrayLeft.push_back(0.08f);
    textureArrayLeft.push_back(factor/textureVsize);

    factor = ((x5 - x0) * (x2 - x0) + (y5 - y0) * (y2 - y0))/r1;

    textureArrayLeft.push_back(0.09f);
    textureArrayLeft.push_back(factor/textureVsize);

    factor = ((x5 - x0) * (x3 - x0) + (y5 - y0) * (y3 - y0))/r1;

    textureArrayLeft.push_back(0.16666f);
    textureArrayLeft.push_back(factor/textureVsize);

    factor = ((x5 - x0) * (x4 - x0) + (y5 - y0) * (y4 - y0))/r1;

    textureArrayLeft.push_back(1.0f);
    textureArrayLeft.push_back(factor/textureVsize);

    //////////////////////////////////////////////////////////////

    textureArrayLeft.push_back(0.0f);
    textureArrayLeft.push_back(r1 / textureVsize);

    factor = ((x5 - x0) * (x6 - x0) + (y5 - y0) * (y6 - y0))/r1;

    textureArrayLeft.push_back(0.08f);
    textureArrayLeft.push_back(r1 / textureVsize);

    //r2 = sqrt((x7 - x5) * (x7 - x5) + (y7 - y5) * (y7 - y5));
    //cosAngle = ((x0 - x5) * (x7 - x5) + (y0 - y5) * (y7 - y5))/
    //                  (r1 * r2);

    factor = ((x5 - x0) * (x7 - x0) + (y5 - y0) * (y7 - y0))/r1;

    textureArrayLeft.push_back(0.09f);
    textureArrayLeft.push_back(factor / textureVsize);

    factor = ((x5 - x0) * (x8 - x0) + (y5 - y0) * (y8 - y0))/r1;

    textureArrayLeft.push_back(0.16666f);
    textureArrayLeft.push_back(factor / textureVsize);

    factor = ((x5 - x0) * (x9 - x0) + (y5 - y0) * (y9 - y0))/r1;

    textureArrayLeft.push_back(1.0f);
    textureArrayLeft.push_back(factor / textureVsize);
    }
}

void RoadBroken::setLeftIndexArray()
{
    indexArrayLeft.clear();
    for (int i = 0; i < vertexArrayLeft.size() / 3 - 5; i += 10)
    {
        indexArrayLeft.push_back(i);
        indexArrayLeft.push_back(i + 5);
        indexArrayLeft.push_back(i + 6);


        indexArrayLeft.push_back(i);
        indexArrayLeft.push_back(i + 6);
        indexArrayLeft.push_back(i + 1);

        indexArrayLeft.push_back(i + 1);
        indexArrayLeft.push_back(i + 6);
        indexArrayLeft.push_back(i + 7);

        indexArrayLeft.push_back(i + 1);
        indexArrayLeft.push_back(i + 7);
        indexArrayLeft.push_back(i + 2);

        indexArrayLeft.push_back(i + 2);
        indexArrayLeft.push_back(i + 7);
        indexArrayLeft.push_back(i + 8);

        indexArrayLeft.push_back(i + 2);
        indexArrayLeft.push_back(i + 8);
        indexArrayLeft.push_back(i + 3);

        indexArrayLeft.push_back(i + 3);
        indexArrayLeft.push_back(i + 8);
        indexArrayLeft.push_back(i + 9);

        indexArrayLeft.push_back(i + 3);
        indexArrayLeft.push_back(i + 9);
        indexArrayLeft.push_back(i + 4);
    }
}

void RoadBroken::getVertexArrayForLineAxis(QVector<float> &axisArray, bool rightSide, float step, float beginStep, float endStep)
{

    float begin_step = beginStep;
    float end_step = endStep;
    if (rightSide)
    {
        for (int i = 0; i < vertexArray.size() / 3;)
        {
            if (i == 0)
            {
                float x0 = vertexArray[i * 3];
                float y0 = vertexArray[i * 3 + 1];
                float z0 = vertexArray[i * 3 + 2];
                float x1 = vertexArray[(i + 1) * 3];
                float y1 = vertexArray[(i + 1) * 3 + 1];
                float z1 = vertexArray[(i + 1) * 3 + 2];
                float x2 = vertexArray[(i + 2) * 3];
                float y2 = vertexArray[(i + 2) * 3 + 1];
                float z2 = vertexArray[(i + 2) * 3 + 2];
                if (x0 == x2 && y0 == y2)
                {
                    x2 = vertexArray[(i + 6) * 3];
                    y2 = vertexArray[(i + 6) * 3 + 1];
                }
                float dx = x1 - x0;
                float dy = y1 - y0;
                float r = sqrt(dx*dx + dy*dy);
                float r01 = sqrt(dx*dx + dy*dy);
                float pi = 3.1415926f;
                float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                                   sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

                dx = dx/r01 * (step / cos(angle - pi / 2.0f));
                dy = dy/r01 * (step / cos(angle - pi / 2.0f));


                axisArray.push_back(x0 + dx);
                axisArray.push_back(y0 + dy);
                axisArray.push_back(0.001f);


                i += 2;
            }
            else
            {
                if (i == vertexArray.size() / 3 - 2)
                {
                    float x0 = vertexArray[i * 3];
                    float y0 = vertexArray[i * 3 + 1];
                    float z0 = vertexArray[i * 3 + 2];
                    float x1 = vertexArray[(i + 1) * 3];
                    float y1 = vertexArray[(i + 1) * 3 + 1];
                    float z1 = vertexArray[(i + 1) * 3 + 2];
                    float x2 = vertexArray[(i - 2) * 3];
                    float y2 = vertexArray[(i - 2) * 3 + 1];
                    float z2 = vertexArray[(i - 2) * 3 + 2];
                    if (x0 == x2 && y0 == y2)
                    {
                        x2 = vertexArray[(i - 6) * 3];
                        y2 = vertexArray[(i - 6) * 3 + 1];
                    }
                    float dx = x1 - x0;
                    float dy = y1 - y0;
                    float r01 = sqrt(dx*dx + dy*dy);
                    float pi = 3.1415926f;
                    float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                            sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

                    axisArray.push_back(x0 + dx/r01 * (step / cos(angle - pi / 2.0f)));
                    axisArray.push_back(y0 + dy/r01 * (step / cos(angle - pi / 2.0f)));
                    axisArray.push_back(0.001f);

                    i += 2;

                }
                else
                {
                    float x1 = vertexArray[(i - 2) * 3];
                    float y1 = vertexArray[(i - 2) * 3 + 1];
                    float x2 = vertexArray[i * 3];
                    float y2 = vertexArray[i * 3 + 1];
                    float x3 = vertexArray[(i + 4) * 3];
                    float y3 = vertexArray[(i + 4) * 3 + 1];
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

                    float dx = (step / sin(alpha)) * cos(hamma);
                    float dy = (step / sin(alpha)) * sin(hamma);


                    axisArray.push_back(x2 + dx);
                    axisArray.push_back(y2 + dy);
                    axisArray.push_back(0.001f);

                    i += 4;
                }
            }

        }
    }
    else
    {
        for (int i = 0; i < vertexArray.size() / 3;)
        {
            if (i == 0)
            {
                float x0 = vertexArray[(i + 1) * 3];
                float y0 = vertexArray[(i + 1) * 3 + 1];
                float z0 = vertexArray[(i + 1) * 3 + 2];
                float x1 = vertexArray[i * 3];
                float y1 = vertexArray[i * 3 + 1];
                float z1 = vertexArray[i * 3 + 2];
                float x2 = vertexArray[(i + 3) * 3];
                float y2 = vertexArray[(i + 3) * 3 + 1];
                float z2 = vertexArray[(i + 3) * 3 + 2];
                if (x0 == x2 && y0 == y2)
                {
                    x2 = vertexArray[(i + 7) * 3];
                    y2 = vertexArray[(i + 7) * 3 + 1];
                }
                float dx = x1 - x0;
                float dy = y1 - y0;
                float r01 = sqrt(dx*dx + dy*dy);
                float pi = 3.1415926f;
                float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                                   sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));


                axisArray.push_back(x0 + dx/r01 * (step / cos(angle - pi / 2.0f)));
                axisArray.push_back(y0 + dy/r01 * (step / cos(angle - pi / 2.0f)));
                axisArray.push_back(0.001f);


                i += 2;
            }
            else
            if (i == vertexArray.size() / 3 - 2)
            {
                float x0 = vertexArray[(i + 1) * 3];
                float y0 = vertexArray[(i + 1) * 3 + 1];
                float z0 = vertexArray[(i + 1) * 3 + 2];
                float x1 = vertexArray[i * 3];
                float y1 = vertexArray[i * 3 + 1];
                float z1 = vertexArray[i * 3 + 2];
                float x2 = vertexArray[(i - 1) * 3];
                float y2 = vertexArray[(i - 1) * 3 + 1];
                float z2 = vertexArray[(i - 1) * 3 + 2];
                if (x0 == x2 && y0 == y2)
                {
                    x2 = vertexArray[(i - 5) * 3];
                    y2 = vertexArray[(i - 5) * 3 + 1];
                }
                float dx = x1 - x0;
                float dy = y1 - y0;
                float r01 = sqrt(dx*dx + dy*dy);
                float pi = 3.1415926f;
                float angle = acos((dx * (x2 - x0) + dy * (y2 - y0))/
                        sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0))));

                axisArray.push_back(x0 + dx/r01 * (step / cos(angle - pi / 2.0f)));
                axisArray.push_back(y0 + dy/r01 * (step / cos(angle - pi / 2.0f)));
                axisArray.push_back(0.001f);


                i += 2;
            }
            else
            {
                float x1 = vertexArray[(i - 1) * 3];
                float y1 = vertexArray[(i - 1) * 3 + 1];
                float x2 = vertexArray[(i + 1) * 3];
                float y2 = vertexArray[(i + 1) * 3 + 1];
                float x3 = vertexArray[(i + 5) * 3];
                float y3 = vertexArray[(i + 5) * 3 + 1];
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

                float dx = (step / sin(alpha)) * cos(hamma);
                float dy = (step / sin(alpha)) * sin(hamma);

                axisArray.push_back(x2 - dx);
                axisArray.push_back(y2 - dy);
                axisArray.push_back(0.001f);

                i += 4;
            }
    }
    }

    for (int i = 0; i < axisArray.size() / 3;)
    {
        float x1 = axisArray[i * 3];
        float y1 = axisArray[i * 3 + 1];
        float x2 = axisArray[(i + 1) * 3];
        float y2 = axisArray[(i + 1) * 3 + 1];
        float r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        if (r < beginStep)
        {
            beginStep -= r;
            axisArray.pop_front();
            axisArray.pop_front();
            axisArray.pop_front();
        }
        else
        {
            axisArray[i * 3] = x1 + (x2 - x1) / r * beginStep;
            axisArray[i * 3 + 1] = y1 + (y2 - y1) / r * beginStep;
            break;
        }
    }
    for (int i = axisArray.size() / 3 - 1; i >= 0;)
    {
        float x1 = axisArray[i * 3];
        float y1 = axisArray[i * 3 + 1];
        float x2 = axisArray[(i - 1) * 3];
        float y2 = axisArray[(i - 1) * 3 + 1];
        float r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        if (r < endStep)
        {
            endStep -= r;
            axisArray.pop_back();
            axisArray.pop_back();
            axisArray.pop_back();
            i = axisArray.size() / 3 - 1;
        }
        else
        {
            axisArray[i * 3] = x1 + (x2 - x1) / r * endStep;
            axisArray[i * 3 + 1] = y1 + (y2 - y1) / r * endStep;
            break;
        }
    }


}


GLuint RoadBroken::getTextures(QString source)
{
    QImage image1;

    GLuint ID;
    image1.load(source);
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, &ID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, ID);
    // связываем текстурный объект с изображением
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());

    // задаём линейную фильтрацию вблизи:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // задаём линейную фильтрацию вдали:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    qDebug() << "TEXTURE ID " << ID;
    return ID;
}



void RoadBroken::setIndexArray()
{
    //if (indexArray.size() != vertexArray.size() / 2)
    //    indexArray.resize(vertexArray.size() / 2);
    indexArray.clear();
    for (int i = 0; i < vertexArray.size() / 3 - 2; i += 4)
    {
        /*
        indexArray[i * 3] = i;
        indexArray[i * 3 + 1] = i + 3;
        indexArray[i * 3 + 2] = i + 1;

        indexArray[i * 3 + 3] = i;
        indexArray[i * 3 + 4] = i + 2;
        indexArray[i * 3 + 5] = i + 3;
        */
        indexArray.push_back(i);
        indexArray.push_back(i + 3);
        indexArray.push_back(i + 1);

        indexArray.push_back(i);
        indexArray.push_back(i + 2);
        indexArray.push_back(i + 3);
    }
}

void RoadBroken::addBreak(bool front)
{
    int numberOfVertices = vertexArray.size() / 3;
    if (front)
    {
        GLfloat x1 = vertexArray[0];
        GLfloat y1 = vertexArray[1];
        GLfloat z1 = vertexArray[2];
        GLfloat x2 = vertexArray[3];
        GLfloat y2 = vertexArray[4];
        GLfloat z2 = vertexArray[5];
        vertexArray.push_front(z2);
        vertexArray.push_front(y2);
        vertexArray.push_front(x2);

        vertexArray.push_front(z1);
        vertexArray.push_front(y1);
        vertexArray.push_front(x1);
        ///////////////////////////////////////////////////////
        vertexArray.push_front(z2);
        vertexArray.push_front(y2);
        vertexArray.push_front(x2);

        vertexArray.push_front(z1);
        vertexArray.push_front(y1);
        vertexArray.push_front(x1);
    }
    else
    {
        GLfloat x1 = vertexArray[(numberOfVertices - 2) * 3];
        GLfloat y1 = vertexArray[(numberOfVertices - 2) * 3 + 1];
        GLfloat z1 = vertexArray[(numberOfVertices - 2) * 3 + 2];
        GLfloat x2 = vertexArray[(numberOfVertices - 1) * 3];
        GLfloat y2 = vertexArray[(numberOfVertices - 1) * 3 + 1];
        GLfloat z2 = vertexArray[(numberOfVertices - 1) * 3 + 2];

        vertexArray.push_back(x1);
        vertexArray.push_back(y1);
        vertexArray.push_back(z1);

        vertexArray.push_back(x2);
        vertexArray.push_back(y2);
        vertexArray.push_back(z2);
        ///////////////////////////////////////////////////////////////
        vertexArray.push_back(x1);
        vertexArray.push_back(y1);
        vertexArray.push_back(z1);

        vertexArray.push_back(x2);
        vertexArray.push_back(y2);
        vertexArray.push_back(z2);
    }


    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();
    setTextureArray(texture_1Usize, texture_1Vsize);

    setRightVertexArray();
    setRightTextureArray(texture_2Usize, texture_2Vsize);
    setRightIndexArray();

    setLeftVertexArray();
    setLeftTextureArray(texture_2Usize, texture_2Vsize);
    setLeftIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(1.0f, 1.0f, 1.0f);

    resetLines();

}

void RoadBroken::setIndexArrayForSelectionFrame()
{
    if (indexArrayForSelection.size() != vertexArray.size() / 3)
        indexArrayForSelection.resize(vertexArray.size() / 3);
    int i = 0;
    int count = 0;
    indexArrayForSelection[count++] = i++;


    for (; i < vertexArray.size() / 3; i += 2)
    {
        indexArrayForSelection[count++] = i;
    }

    i = vertexArray.size() / 3 - 2;
    for (; i > 0; i -= 2)
    {
        indexArrayForSelection[count++] = i;
    }
}

void RoadBroken::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    if (colorArrayForSelection.size() != vertexArray.size())
        colorArrayForSelection.resize(vertexArray.size());
    for (int i = 0; i < vertexArray.size()/3; ++i)
    {
        colorArrayForSelection[i * 3] = red;
        colorArrayForSelection[i * 3 + 1] = green;
        colorArrayForSelection[i * 3 + 2] = blue;
    }
}



bool RoadBroken::isSelected()
{
    return selected;
}

void RoadBroken::setSelectedStatus(bool status)
{
    selected = status;
}

void RoadBroken::drawFigure(QGLWidget* render)
{
    if (selected == true)
    {
        if (indexOfSelectedControl >= 0)
        {

            //glLineWidth(2.0);
            //glPointSize(5.0);
            // drawControlElement(indexOfSelectedControl, 2.0, 5.0);
        }
        // Если фигуры выбрана - изменяем цвет заливки
        setColorArray(0.7f, 0.7f, 0.7f, alpha);
        drawSelectionFrame();
        //for (int i = 0; i < getNumberOfControls(); ++i)
        //    drawControlElement(i, 5.0f, 10.0f);
        // glColor3d(0.3, 0.7, 0.1);


    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(red, green, blue, alpha);

    }
    //qDebug() << "vertices " << vertexArrayRight.size()/3;
    //qDebug() << "textures " << textureArrayRight.size()/2;
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    //glColorPointer(4, GL_FLOAT, 0, colorArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    if (showRightBoard)
    {
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArrayRight.begin());
    //glColorPointer(4, GL_FLOAT, 0, colorArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArrayRight.begin());
    glDrawElements(GL_TRIANGLES, indexArrayRight.size(), GL_UNSIGNED_BYTE, indexArrayRight.begin());
    }

    if (showLeftBoard)
    {
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArrayLeft.begin());
    //glColorPointer(4, GL_FLOAT, 0, colorArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArrayLeft.begin());
    glDrawElements(GL_TRIANGLES, indexArrayLeft.size(), GL_UNSIGNED_BYTE, indexArrayLeft.begin());
    }

    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->drawFigure();
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);
}

void RoadBroken::drawSelectionFrame()
{
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0f);
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, colorArrayForSelection.begin());
    glLineWidth(5.0);
    glDrawElements(GL_LINE_LOOP, indexArrayForSelection.size(), GL_UNSIGNED_BYTE, indexArrayForSelection.begin());

    // Угловые точки для изменения размера
    glPointSize(10.0);
    glDrawElements(GL_POINTS, indexArrayForSelection.size(), GL_UNSIGNED_BYTE, indexArrayForSelection.begin());

    if (showRightBoard)
    {
    glLineWidth(5.0f);
    for (int i = 0; i < vertexArrayRight.size() / 3 - 5; i += 10)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayRight[(i + 4) * 3],
                vertexArrayRight[(i + 4) * 3 + 1],
                vertexArrayRight[(i + 4) * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayRight[(i + 9) * 3],
                vertexArrayRight[(i + 9) * 3 + 1],
                vertexArrayRight[(i + 9) * 3 + 2]);
        glEnd();
    }
    }

    if (showLeftBoard)
    {
        glLineWidth(5.0f);
    for (int i = 0; i < vertexArrayLeft.size() / 3 - 5; i += 10)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayLeft[(i + 4) * 3],
                vertexArrayLeft[(i + 4) * 3 + 1],
                vertexArrayLeft[(i + 4) * 3 + 2]);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayLeft[(i + 9) * 3],
                vertexArrayLeft[(i + 9) * 3 + 1],
                vertexArrayLeft[(i + 9) * 3 + 2]);
        glEnd();
    }
    }

}

void RoadBroken::move(float dx, float dy, float dz)
{
    if (fixed)
    {
        return;
    }
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
    for (int i = 0; i < vertexArrayRight.size() / 3; ++i)
    {
        vertexArrayRight[i * 3] += dx;
        vertexArrayRight[i * 3 + 1] += dy;
    }
    for (int i = 0; i < vertexArrayLeft.size() / 3; ++i)
    {
        vertexArrayLeft[i * 3] += dx;
        vertexArrayLeft[i * 3 + 1] += dy;
    }
    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->move(dx, dy, dz);

}

void RoadBroken::drawControlElement(int index, float lineWidth, float pointSize)
{   
    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6)
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[0],
                vertexArray[1],
                vertexArray[2]);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[3],
                vertexArray[4],
                vertexArray[5]);
        glEnd();
    }
    else
    {
        if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + 1)
        {
            int j = vertexArray.size() / 3 - 2;
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(vertexArray[j * 3],
                    vertexArray[j * 3 + 1],
                    vertexArray[j * 3 + 2]);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(vertexArray[(j + 1) * 3],
                    vertexArray[(j + 1) * 3 + 1],
                    vertexArray[(j + 1) * 3 + 2]);
            glEnd();
        }
        else
        {
            if (index >= vertexArray.size() / 3 + vertexArray.size() / 6)
            {
                int j = index - vertexArray.size() / 3 - vertexArray.size() / 6;
                if (j < vertexArray.size() / (3 * 4))
                {
                    glLineWidth(lineWidth);
                    glBegin(GL_LINES);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(vertexArray[(j * 4) * 3],
                                vertexArray[(j * 4) * 3 + 1],
                                vertexArray[(j * 4) * 3 + 2]);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(vertexArray[(j * 4 + 2) * 3],
                                vertexArray[(j * 4 + 2) * 3 + 1],
                                vertexArray[(j * 4 + 2) * 3 + 2]);

                    glEnd();
                }
                else
                {
                    j -= vertexArray.size() / (3 * 4);
                    glLineWidth(lineWidth);
                    glBegin(GL_LINES);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(vertexArray[(j * 4 + 1) * 3],
                                vertexArray[(j * 4 + 1) * 3 + 1],
                                vertexArray[(j * 4 + 1) * 3 + 2]);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(vertexArray[(j * 4 + 3) * 3],
                                vertexArray[(j * 4 + 3) * 3 + 1],
                                vertexArray[(j * 4 + 3) * 3 + 2]);

                    glEnd();
                }
            }
            else
            if (index >= vertexArray.size() / 3)
            {
                int j = index - vertexArray.size() / 3;
                if (j < vertexArray.size() / (3 * 4))
                {
                    if (showRightBoard)
                    {
                    glLineWidth(lineWidth);
                    glBegin(GL_LINES);
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glVertex3f(vertexArrayRight[(j * 10 + 4) * 3],
                                vertexArrayRight[(j * 10 + 4) * 3 + 1],
                                vertexArrayRight[(j * 10 + 4) * 3 + 2]);
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glVertex3f(vertexArrayRight[(j * 10 + 9) * 3],
                                vertexArrayRight[(j * 10 + 9) * 3 + 1],
                                vertexArrayRight[(j * 10 + 9) * 3 + 2]);

                    glEnd();
                    }
                }
                else
                {
                    if (showLeftBoard)
                    {
                    j -= vertexArray.size() / (3 * 4);
                    glLineWidth(lineWidth);
                    glBegin(GL_LINES);
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glVertex3f(vertexArrayLeft[(j * 10 + 4) * 3],
                                vertexArrayLeft[(j * 10 + 4) * 3 + 1],
                                vertexArrayLeft[(j * 10 + 4) * 3 + 2]);
                        glColor3f(0.0f, 1.0f, 0.0f);
                        glVertex3f(vertexArrayLeft[(j * 10 + 9) * 3],
                                vertexArrayLeft[(j * 10 + 9) * 3 + 1],
                                vertexArrayLeft[(j * 10 + 9) * 3 + 2]);

                    glEnd();
                    }
                }

            }


                else
                {                    
                        glPointSize(pointSize);
                        glBegin(GL_POINTS);
                        glColor3f(0.0f, 0.0f, 0.0f);
                        glVertex3f(vertexArray[index * 3],
                                vertexArray[index * 3 + 1],
                                vertexArray[index * 3 + 2]);
                        glEnd();
                }

        }

    }
}

QCursor RoadBroken::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void RoadBroken::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6)
    {
        vertexArray[0] += dx;
        vertexArray[1] += dy;

        vertexArray[3] += dx;
        vertexArray[4] += dy;
    }
    else
    {
        if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + 1)
        {
            int j = vertexArray.size() / 3 - 2;

            vertexArray[j * 3] += dx;
            vertexArray[j * 3 + 1] += dy;

            vertexArray[(j + 1) * 3] += dx;
            vertexArray[(j + 1) * 3 + 1] += dy;

        }
        else
        {
            if (index >= vertexArray.size() / 3 + vertexArray.size() / 6)
            {

            }
            else
            if (index >= vertexArray.size() / 3)
            {
                int j = index - vertexArray.size() / 3;
                if (j < vertexArray.size() / (3 * 4))
                {
                    float x0 = vertexArrayRight[(j * 10 + 4) * 3];
                    float y0 = vertexArrayRight[(j * 10 + 4) * 3 + 1];
                    float x1 = vertexArrayRight[(j * 10 + 9) * 3];
                    float y1 = vertexArrayRight[(j * 10 + 9) * 3 + 1];
                    float pi = 3.1415926f;
                    float angle = acos((dx * (x1 - x0) + dy * (y1 - y0))/
                            sqrt((dx*dx + dy*dy)*((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0))));
                    float dr = sqrt(dx*dx + dy*dy);
                    float sa = dx*(x1-x0) - dy*(x1-x0);
                    float factor = sa < 0 ? -1 : 1;
                    rightBoardWidth += dr * sin(angle) * factor;
                    emit rightBoardWidthChanged(rightBoardWidth);
                }
                else
                {
                    j -= vertexArray.size() / (3 * 4);
                    float x0 = vertexArrayLeft[(j * 10 + 4) * 3];
                    float y0 = vertexArrayLeft[(j * 10 + 4) * 3 + 1];
                    float x1 = vertexArrayLeft[(j * 10 + 9) * 3];
                    float y1 = vertexArrayLeft[(j * 10 + 9) * 3 + 1];
                    float angle = acos((dx * (x1 - x0) + dy * (y1 - y0))/
                            sqrt((dx*dx + dy*dy)*((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0))));
                    float dr = sqrt(dx*dx + dy*dy);
                    float sa = dx*(x1-x0) - dy*(x1-x0);
                    float factor = sa < 0 ? 1 : -1;
                    leftBoardWidth += dr * sin(angle) * factor;
                    emit leftBoardWidthChanged(leftBoardWidth);
                }
            }
        else
        {
            // Изменение узловых точек
            vertexArray[index * 3] += dx;
            vertexArray[index * 3 + 1] += dy;

            if (index != 0 && index != 1 &&
                    (index != vertexArray.size() / 3 - 1) &&
                    (index != vertexArray.size() / 3 - 2))
            {
            if (index % 2 == 0)
            {
                index += index % 4 == 0 ? -2 : 2;
            }
            else
                index += index % 4 == 1 ? -2 : 2;

                vertexArray[index * 3] += dx;
                vertexArray[index * 3 + 1] += dy;
            }
        }
        }
    }
    setTextureArray(texture_1Usize, texture_1Vsize);\
    setRightVertexArray();
    setRightTextureArray(texture_2Usize, texture_2Vsize);
    setLeftVertexArray();
    setLeftTextureArray(texture_2Usize, texture_2Vsize);
    resetLines();

}

int RoadBroken::getNumberOfControls()
{
    //return vertexArray.size() / 3 + 2;
    return vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + 2;
}

int RoadBroken::controlsForPoint()
{
    return 1;
}

void RoadBroken::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}

void RoadBroken::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}


QJsonObject RoadBroken::getJSONInfo()
{
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    {
        QJsonArray temp;
        temp.append(QJsonValue(red));
        temp.append(QJsonValue(green));
        temp.append(QJsonValue(blue));
        temp.append(QJsonValue(alpha));

        element["Color"] = temp;
    }

    QJsonArray temp;

    for (int i = 0; i < vertexArray.size(); ++i)
    {
        temp.append(QJsonValue(vertexArray[i]));
    }
    element["Vertices"] = temp;

    return element;


}


void RoadBroken::getProperties(QFormLayout *layout, QGLWidget* render)
{
    this->layout = layout;
    this->render = render;
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    QCheckBox* showRightBoardCheckBox = new QCheckBox();
    QCheckBox* showLeftBoardCheckBox = new QCheckBox();
    QCheckBox* fixedCheckBox = new QCheckBox();
    QDoubleSpinBox* rightBoardSpinBox = new QDoubleSpinBox();
    QDoubleSpinBox* leftBoardSpinBox = new QDoubleSpinBox();

    showRightBoardCheckBox->setChecked(showRightBoard);
    QObject::connect(showRightBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setRightBoardShowStatus(bool)));

    showLeftBoardCheckBox->setChecked(showLeftBoard);
    QObject::connect(showLeftBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLeftBoardShowStatus(bool)));

    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    rightBoardSpinBox->setMinimum(0.0f);
    rightBoardSpinBox->setValue(rightBoardWidth);
    connect(this, SIGNAL(rightBoardWidthChanged(double)), rightBoardSpinBox, SLOT(setValue(double)));
    connect(rightBoardSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRightBoardWidth(double)));

    leftBoardSpinBox->setMinimum(0.0f);
    leftBoardSpinBox->setValue(leftBoardWidth);
    connect(this, SIGNAL(leftBoardWidthChanged(double)), leftBoardSpinBox, SLOT(setValue(double)));
    connect(leftBoardSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeftBoardWidth(double)));

    if (render)
    {
        connect(rightBoardSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(leftBoardSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(showRightBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        connect(showLeftBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
    }

    QPushButton *addLineButton = new QPushButton("+");
    StepDialog *stepDialog = new StepDialog();
    connect(stepDialog, SIGNAL(lineTypeChanged(int)), this, SLOT(setLineType(int)));
    connect(stepDialog, SIGNAL(rightSideChanged(bool)), this, SLOT(setRightSide(bool)));
    connect(stepDialog, SIGNAL(stepChanged(double)), this, SLOT(setStep(double)));
    connect(stepDialog, SIGNAL(beginStepChanged(double)), this, SLOT(setBeginStep(double)));
    connect(stepDialog, SIGNAL(endStepChanged(double)), this, SLOT(setEndStep(double)));
    connect(stepDialog, SIGNAL(beginSideChanged(bool)), this, SLOT(setBeginSide(bool)));
    connect(stepDialog, SIGNAL(beginRoundingChanged(bool)), this, SLOT(setBeginRounding(bool)));
    connect(stepDialog, SIGNAL(endRoundingChanged(bool)), this, SLOT(setEndRounding(bool)));
    connect(stepDialog, SIGNAL(splitZoneWidthChanged(double)), this, SLOT(setSplitZoneWidth(double)));
    connect(stepDialog, SIGNAL(differentDirectionsChanged(bool)), this, SLOT(setDifferentDirections(bool)));
    connect(addLineButton, SIGNAL(clicked(bool)), stepDialog, SLOT(exec()));
    connect(stepDialog, SIGNAL(accepted()), this, SLOT(addLine()));




    layout->addRow("Ширина правого тротуара", rightBoardSpinBox);
    layout->addRow("Ширина левого тротуара", leftBoardSpinBox);
    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Правый тротуар", showRightBoardCheckBox);
    layout->addRow("Левый тротуар", showLeftBoardCheckBox);

    for (int i = 0; i < lines.size(); ++i)
    {
        QPushButton* b = new QPushButton(QString::number(i + 1));
        connect(b, SIGNAL(clicked(bool)), this, SLOT(deleteLine()));
        layout->addRow("Удалить линию ",b);
    }

    layout->addRow("Добавить линию", addLineButton);
}


bool RoadBroken::isFixed()
{
    return fixed;
}

void RoadBroken::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide)
{

    QVector<float> axisArray;

    getVertexArrayForLineAxis(axisArray,rightSide,step,beginStep,endStep);

    int size = axisArray.size();
    float *lineVertexArray = new float[size];
    for (int i = 0; i < size; ++i)
        lineVertexArray[i] = axisArray[i];

    qDebug() << "Got axis array, size: " << size / 3;
    LineBrokenLinkedToRoadBroken line;
    if (lineType == 6)
    {
        line.line = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                  QString("Линия №") + QString::number(lines.size() + 1));
        line.splitZoneWidth = splitZoneWidth;
        line.beginRounding = beginRounding;
        line.endRounding = endRounding;
    }
    else
    {
        line.line = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                   QString("Линия №") + QString::number(lines.size() + 1));
    }
    qDebug() << "Creted line";
    line.lineWidth = lineWidth;
    line.step = step;
    line.rightSide = rightSide;
    line.lineType = lineType;
    line.textureSource = textureSource;
    line.beginStep = beginStep;
    line.endStep = endStep;
    line.line->setSelectedStatus(false);
    lines.push_back(line);
    delete[] lineVertexArray;
    lineVertexArray = NULL;

    emit linesChanged(layout, render);
}

void RoadBroken::addLine()
{
    qDebug() << "Add line";
    QString textSource;
    float lWidth;
    switch(lineType)
    {
    case 0:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/solid.png";
        lWidth = 0.1f;
        break;
    case 1:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/inter.png";
        lWidth = 0.1f;
        break;
    case 2:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/d_solid.png";
        lWidth = 0.25f;
        break;
    case 3:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/d_inter_l.png";
        lWidth = 0.25f;
        break;
    case 4:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/d_inter_r.png";
        lWidth = 0.25f;
        break;
    case 5:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/d_inter.png";
        lWidth = 0.25f;
        break;
    default:
        break;

    }

    addLine(step, textSource, 6.0f, lWidth, lineType, rightSide);
    qDebug() << "Line added";
}

void RoadBroken::setRightSide(bool status)
{
    rightSide = status;
}

void RoadBroken::setStep(double value)
{
    step = value;
}

void RoadBroken::setLineType(int type)
{
    lineType = type;
}

void RoadBroken::deleteLine()
{
    QPushButton * b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    qDebug() << "delete line " << b->text();
    int i = b->text().toInt() - 1;
    delete lines[i].line;
    lines.remove(i);
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].lineType != 6)
        {
            LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
            line->setDescription(QString("Линия №") + QString::number(i + 1));
        }
        else
        {
            SplitZone* line = dynamic_cast<SplitZone*>(lines[i].line);
            line->setDescription(QString("Линия №") + QString::number(i + 1));
        }
    }
    emit linesChanged(layout, render);
}

void RoadBroken::resetLines()
{
    for (int i = 0; i < lines.size(); ++i)
    {
        delete lines[i].line;

        QVector<float> axisArray;

        getVertexArrayForLineAxis(axisArray,lines[i].rightSide,lines[i].step,lines[i].beginStep, lines[i].endStep);
        int size = axisArray.size();
        float *lineVertexArray = new float[size];
        for (int i = 0; i < size; ++i)
            lineVertexArray[i] = axisArray[i];

        if (lines[i].lineType == 6)
        {
            lines[i].line = new SplitZone(lineVertexArray, size, lines[i].splitZoneWidth, lines[i].beginRounding, lines[i].endRounding,
                                          QString("Линия №") + QString::number(i + 1));
        }
        else
        {
            lines[i].line = new LineBroken(lines[i].lineWidth, lineVertexArray, size, lines[i].textureSource, 6.0f, "LineBroken", 1,
                                           QString("Линия №") + QString::number(i + 1));
        }
        //lines[i].line = new LineBroken(lines[i].lineWidth, lineVertexArray, size, lines[i].textureSource, 6.0f, "LineBroken", 1);
        delete[] lineVertexArray;
        lineVertexArray = NULL;
    }
}

void RoadBroken::setBeginStep(double step)
{
    beginStep = step;
}

void RoadBroken::setEndStep(double step)
{
    endStep = step;
}

void RoadBroken::setSplitZoneWidth(double value)
{
    splitZoneWidth = value;
}

void RoadBroken::setBeginSide(bool status)
{
    beginSide = status;
}

void RoadBroken::setBeginRounding(bool status)
{
    beginRounding = status;
}

void RoadBroken::setEndRounding(bool status)
{
    endRounding = status;
    qDebug() << "endRounding" << endRounding;
}

void RoadBroken::setDifferentDirections(bool status)
{
    differentDirections = status;
}


void RoadBroken::drawMeasurements(QGLWidget *render)
{
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Black);
    float x1, x2, y1, y2;
    float dr;

    glEnable(GL_LINE_STIPPLE);
    for (int i = 0; i < vertexArray.size() / 3 - 1; i += 2)
    {
        x1 = vertexArray[i * 3];
        y1 = vertexArray[i * 3 + 1];
        x2 = vertexArray[(i + 1) * 3];
        y2 = vertexArray[(i + 1) * 3 + 1];

        dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

        if (i >= 2 && i < vertexArray.size() / 3  - 2)
        {
            glLineStipple(1, 0x0FFF);
            glLineWidth(1.5f);

            glBegin(GL_LINES);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(x1, y1, 0.3f);

            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(x2, y2, 0.3f);
            glEnd();
        }
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        glColor3f(1.0f, 1.0f, 1.0f);
        render->renderText(wx, wy, QString("%1").arg(dr), shrift);

    }
    glDisable(GL_LINE_STIPPLE);

    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].lineType != 6)
        {
            LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
            line->drawDescription(render);
        }
        else
        {
            SplitZone* line = dynamic_cast<SplitZone*>(lines[i].line);
            line->drawDescription(render);
        }
    }
}


bool RoadBroken::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void RoadBroken::setRightBoardShowStatus(bool status)
{
    showRightBoard = status;
}

void RoadBroken::setLeftBoardShowStatus(bool status)
{
    showLeftBoard = status;
}

void RoadBroken::setRightBoardWidth(double width)
{
    if (rightBoardWidth == width)
        return;
    rightBoardWidth = width;
    setRightVertexArray();
    setRightTextureArray(texture_2Usize,texture_2Vsize);
    emit rightBoardWidthChanged(width);
}

void RoadBroken::setLeftBoardWidth(double width)
{
    if (leftBoardWidth == width)
        return;
    leftBoardWidth = width;
    setLeftVertexArray();
    setLeftTextureArray(texture_2Usize,texture_2Vsize);
    emit leftBoardWidthChanged(width);
}


int RoadBroken::getLayer()
{
    return layer;
}


std::vector<vec3> RoadBroken::getCoordOfControl(int index)
{
   // vec3 p(0.0f, 0.0f, 0.0f);
   // if (index >= 0 && index < getNumberOfControls())
   // {
   //     p.x = vertexArray[index * 3];
   //     p.y = vertexArray[index * 3 + 1];
   //     p.z = vertexArray[index * 3 + 2];
   // }
    std::vector<vec3> res;
    //res.push_back(p);




    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6)
    {
        vec3 p(vertexArray[0],vertexArray[1],vertexArray[2]);
        vec3 s(vertexArray[3],vertexArray[4],vertexArray[5]);
        res.push_back(p);
        res.push_back(s);
    }
    else
    {
        if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + 1)
        {
            int j = vertexArray.size() / 3 - 2;
            vec3 p(vertexArray[j * 3],
                    vertexArray[j * 3 + 1],
                    vertexArray[j * 3 + 2]);
            vec3 s(vertexArray[(j + 1) * 3],
                    vertexArray[(j + 1) * 3 + 1],
                    vertexArray[(j + 1) * 3 + 1]);
            res.push_back(p);
            res.push_back(s);
        }
        else
        {
            if (index >= vertexArray.size() / 3 + vertexArray.size() / 6)
            {
                int j = index - vertexArray.size() / 3 - vertexArray.size() / 6;
                if (j < vertexArray.size() / (3 * 4))
                {
                    vec3 p(vertexArray[(j * 4) * 3],
                            vertexArray[(j * 4) * 3 + 1],
                            vertexArray[(j * 4) * 3 + 2]);
                    vec3 s(vertexArray[(j * 4 + 2) * 3],
                            vertexArray[(j * 4 + 2) * 3 + 1],
                            vertexArray[(j * 4 + 2) * 3 + 2]);
                    res.push_back(p);
                    res.push_back(s);
                }
                else
                {
                    j -= vertexArray.size() / (3 * 4);
                    vec3 p(vertexArray[(j * 4 + 1) * 3],
                            vertexArray[(j * 4 + 1) * 3 + 1],
                            vertexArray[(j * 4 + 1) * 3 + 2]);
                    vec3 s(vertexArray[(j * 4 + 3) * 3],
                            vertexArray[(j * 4 + 3) * 3 + 1],
                            vertexArray[(j * 4 + 3) * 3 + 2]);
                    res.push_back(p);
                    res.push_back(s);
                }
            }
            else
            if (index >= vertexArray.size() / 3)
            {
                int j = index - vertexArray.size() / 3;
                if (j < vertexArray.size() / (3 * 4))
                {
                    if (showRightBoard)
                    {
                        vec3 p(vertexArrayRight[(j * 10 + 4) * 3],
                                vertexArrayRight[(j * 10 + 4) * 3 + 1],
                                vertexArrayRight[(j * 10 + 4) * 3 + 2]);
                        vec3 s(vertexArrayRight[(j * 10 + 9) * 3],
                                vertexArrayRight[(j * 10 + 9) * 3 + 1],
                                vertexArrayRight[(j * 10 + 9) * 3 + 2]);
                        res.push_back(p);
                        res.push_back(s);
                    }
                }
                else
                {
                    if (showLeftBoard)
                    {
                    j -= vertexArray.size() / (3 * 4);
                    vec3 p(vertexArrayLeft[(j * 10 + 4) * 3],
                            vertexArrayLeft[(j * 10 + 4) * 3 + 1],
                            vertexArrayLeft[(j * 10 + 4) * 3 + 2]);
                    vec3 s(vertexArrayLeft[(j * 10 + 9) * 3],
                            vertexArrayLeft[(j * 10 + 9) * 3 + 1],
                            vertexArrayLeft[(j * 10 + 9) * 3 + 2]);
                    res.push_back(p);
                    res.push_back(s);
                    }
                }

            }


                else
                {
                vec3 p(vertexArray[index * 3],
                        vertexArray[index * 3 + 1],
                        vertexArray[index * 3 + 2]);
                res.push_back(p);
                }

        }

    }
    return res;
}


void RoadBroken::clear()
{
}
