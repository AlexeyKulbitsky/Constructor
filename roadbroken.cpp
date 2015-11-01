#include "roadbroken.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QApplication>
#include "model.h"

bool RoadBroken::log = true;

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
    this->fixedRightWidth = true;
    this->fixedLeftWidth = true;
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

RoadBroken::~RoadBroken()
{
    for (int i = 0; i < lines.size(); ++i)
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            for (QList<RoadElement*>::iterator it = model->getGroup(1).begin();
                 it != model->getGroup(1).end(); ++it)
            {
                if (lines[i].line == (*it))
                {
                    model->getGroup(1).erase(it);
                    break;
                }
            }
            delete lines[i].line;
        }
        delete lines[i].line;
    }
    lines.clear();

    layout = NULL;
    render = NULL;
}

void RoadBroken::setVertexArray(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat width)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setVertexArray(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat width)"
                                 << " x1 = " << x1 << " y1 = " << y1
                                 << " x2 = " << x2 << " y2 = " << y2
                                 << " width = " << width << "\n";

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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setVertexArray(QVector<GLfloat> &vertexArray)";
    this->vertexArray.resize(vertexArray.size());
    for (int i = 0; i < vertexArray.size(); ++i)
    {
        this->vertexArray[i] = vertexArray[i];
    }
}

void RoadBroken::resetVertexArray(float dx, float dy, bool right)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetVertexArray(float dx, float dy, bool right)"
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " right = " << right << "\n";
    int i = right ? 0 : 1;
    int delta = right ? 1 : -1;
    for (; i < vertexArray.size() / 3; i += 2)
    {
        float x1 = vertexArray[(i + delta) * 3];
        float y1 = vertexArray[(i + delta) * 3 + 1];
        float x2 = vertexArray[i * 3];
        float y2 = vertexArray[i * 3 + 1];
        float dx1 = x2 - x1;
        float dy1 = y2 - y1;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float r = (dx*dx1 + dy*dy1) / r1;
        float dX = dx1 / r1 * r;
        float dY = dy1 / r1 * r;
        vertexArray[i * 3] += dX;
        vertexArray[i * 3 + 1] += dY;
    }
}

void RoadBroken::setColorArray(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setColorArray(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"
                                   << " red = " << red
                                   << " green = " << green
                                   << " blue = " << blue
                                   << " alpha = " << alpha << "\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setTextureArray(float textureUsize, float textureVsize)"
                                 << " textureUsize = " << textureUsize
                                 << " textureVsize = " << textureVsize << "\n";
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
        float r1 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
        float r2 = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
        float t = ((x3 - x1) * (x0 - x1) + (y3 - y1) * (y0 - y1))/
                (r1 * r2);
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle = acos(t);
        textureArray.push_back(r2 * sin(angle)/textureUsize);
        textureArray.push_back(r2 * cos(angle)/textureVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back(0.0f);

        r2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        t = ((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1))/
                (r1 * r2);
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        angle = acos(t);
        textureArray.push_back(r2 * sin(angle)/textureUsize);
        textureArray.push_back(r2 * cos(angle)/textureVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back(r1 / textureVsize);
    }
}

void RoadBroken::setLeftVertexArray()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLeftVertexArray()\n";
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
            float pi = 3.14159265f;
            float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                    sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
            if (t > 1.0f)
                t = 1.0f;
            if (t < -1.0f)
                t = -1.0f;
            float angle = acos(t);
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
                float pi = 3.14159265f;
                float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                        sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float angle = acos(t);

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
                        float pi = 3.14159265f;
                        float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                                sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                        if (t > 1.0f)
                            t = 1.0f;
                        if (t < -1.0f)
                            t = -1.0f;
                        float angle = acos(t);

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
                float t = num / den;
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float alpha = (acos(t))/2.0f;
                float sa = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
                float pi = 3.14159265f;
                if(sa < 0) // Точка находится справа
                {
                    alpha = pi - alpha;
                }

                t = (x3-x2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float beta = acos(t);
                t = (y3-y2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                if (asin(t) < 0)
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

void RoadBroken::resetLeftVertexArray()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetLeftVertexArray()\n";
    for (int i = 1; i < vertexArray.size() / 3; i += 2)
    {
        float x1 = vertexArray[i * 3];
        float y1 = vertexArray[i * 3 + 1];
        float x2 = vertexArrayLeft[i / 2 * 5 * 3];
        float y2 = vertexArrayLeft[i / 2 * 5 * 3 + 1];
        if (x1 != x2 || y1 != y2)
        {
            float dx = x1 - x2;
            float dy = y1 - y2;
            vertexArrayLeft[(i - 1) / 2 * 5 * 3] += dx;
            vertexArrayLeft[(i - 1) / 2 * 5 * 3 + 1] += dy;
            vertexArrayLeft[((i - 1) / 2 * 5 + 1) * 3] += dx;
            vertexArrayLeft[((i - 1) / 2 * 5 + 1) * 3 + 1] += dy;
            vertexArrayLeft[((i - 1) / 2 * 5 + 2) * 3] += dx;
            vertexArrayLeft[((i - 1) / 2 * 5 + 2) * 3 + 1] += dy;
            vertexArrayLeft[((i - 1) / 2 * 5 + 3) * 3] += dx;
            vertexArrayLeft[((i - 1) / 2 * 5 + 3) * 3 + 1] += dy;
            vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3] += dx;
            vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3 + 1] += dy;
        }
    }
}

void RoadBroken::resetLeftVertexArray(float dx, float dy)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetLeftVertexArray(float dx, float dy)"
                                   << " dx = " << dx
                                   << " dy = " << dy << "\n";
    for (int i = 1; i < vertexArray.size() / 3; i += 2)
    {
        float x1 = vertexArray[i * 3];
        float y1 = vertexArray[i * 3 + 1];
        float x2 = vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3];
        float y2 = vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3 + 1];
        float dx1 = x2 - x1;
        float dy1 = y2 - y1;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float r = (dx*dx1 + dy*dy1) / r1;
        float dX = dx1 / r1 * r;
        float dY = dy1 / r1 * r;
        vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3] += dX;
        vertexArrayLeft[((i - 1) / 2 * 5 + 4) * 3 + 1] += dY;
    }
}

void RoadBroken::setRightTextureArray(float textureUsize, float textureVsize)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightTextureArray(float textureUsize, float textureVsize)"
                                   << " textureUsize = " << textureUsize
                                   << " textureVsize = " << textureVsize << "\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightIndexArray()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightVertexArray()\n";
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
            float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                    sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
            if (t > 1.0f)
                t = 1.0f;
            if (t < -1.0f)
                t = -1.0f;
            float angle = acos(t);

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
                float pi = 3.14159265f;
                float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                        sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float angle = acos(t);

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
                        float pi = 3.14159265f;
                        float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                                sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                        if (t > 1.0f)
                            t = 1.0f;
                        if (t < -1.0f)
                            t = -1.0f;
                        float angle = acos(t);

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
                float t = num / den;
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float alpha = (acos(t))/2.0f;
                float sa = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
                float pi = 3.14159265f;
                if(sa < 0) // Точка находится справа
                {
                    alpha = pi - alpha;
                }

                t = (x3-x2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                float beta = acos(t);
                t = (y3-y2)/(sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
                if (t > 1.0f)
                    t = 1.0f;
                if (t < -1.0f)
                    t = -1.0f;
                if (asin(t) < 0)
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

void RoadBroken::resetRightVertexArray()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetRightVertexArray()\n";
    for (int i = 0; i < vertexArray.size() / 3; i += 2)
    {
        float x1 = vertexArray[i * 3];
        float y1 = vertexArray[i * 3 + 1];
        float x2 = vertexArrayRight[i / 2 * 5 * 3];
        float y2 = vertexArrayRight[i / 2 * 5 * 3 + 1];
        if (x1 != x2 || y1 != y2)
        {
            float dx = x1 - x2;
            float dy = y1 - y2;
            vertexArrayRight[i / 2 * 5 * 3] += dx;
            vertexArrayRight[i / 2 * 5 * 3 + 1] += dy;
            vertexArrayRight[(i / 2 * 5 + 1) * 3] += dx;
            vertexArrayRight[(i / 2 * 5 + 1) * 3 + 1] += dy;
            vertexArrayRight[(i / 2 * 5 + 2) * 3] += dx;
            vertexArrayRight[(i / 2 * 5 + 2) * 3 + 1] += dy;
            vertexArrayRight[(i / 2 * 5 + 3) * 3] += dx;
            vertexArrayRight[(i / 2 * 5 + 3) * 3 + 1] += dy;
            vertexArrayRight[(i / 2 * 5 + 4) * 3] += dx;
            vertexArrayRight[(i / 2 * 5 + 4) * 3 + 1] += dy;
        }
    }
}

void RoadBroken::resetRightVertexArray(float dx, float dy)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetRightVertexArray(float dx, float dy)"
                                   << " dx = " << dx
                                   << " dy = " << dy << "\n";
    for (int i = 0; i < vertexArray.size() / 3; i += 2)
    {
        float x1 = vertexArray[i * 3];
        float y1 = vertexArray[i * 3 + 1];
        float x2 = vertexArrayRight[(i / 2 * 5 + 4) * 3];
        float y2 = vertexArrayRight[(i / 2 * 5 + 4) * 3 + 1];
        float dx1 = x2 - x1;
        float dy1 = y2 - y1;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float r = (dx*dx1 + dy*dy1) / r1;
        float dX = dx1 / r1 * r;
        float dY = dy1 / r1 * r;
        vertexArrayRight[(i / 2 * 5 + 4) * 3] += dX;
        vertexArrayRight[(i / 2 * 5 + 4) * 3 + 1] += dY;
    }
}

void RoadBroken::setLeftTextureArray(float textureUsize, float textureVsize)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLeftTextureArray(float textureUsize, float textureVsize)"
                                   << " textureUsize = " << textureUsize
                                   << " textureVsize = " << textureVsize << "\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLeftIndexArray()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getVertexArrayForLineAxis(QVector<float> &axisArray, bool rightSide, float step, float beginStep, float endStep)"
                                   << " rightSide = " << rightSide
                                   << " step = " << step
                                   << " beginStep = " << beginStep
                                   << " endStep = " << endStep << "\n";
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
                float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                        sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float angle = acos(t);

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
                    float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                            sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                    if (t > 1)
                        t = 1.0f;
                    if (t < -1)
                        t = -1.0f;
                    float angle = acos(t);

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
                float pi = 3.14159265f;
                float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                        sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float angle = acos(t);


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
                    float pi = 3.14159265f;
                    float t = (dx * (x2 - x0) + dy * (y2 - y0))/
                            sqrt((dx*dx + dy*dy)*((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)));
                    if (t > 1)
                        t = 1.0f;
                    if (t < -1)
                        t = -1.0f;
                    float angle = acos(t);

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




void RoadBroken::setIndexArray()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setIndexArray()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::addBreak(bool front)"
                                   << " front = " << front << "\n";
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

        int size = 15;
        float tempRight[size];
        float tempLeft[size];
        for (int i = 0; i < size; ++i)
        {
            tempRight[i] = vertexArrayRight[i];
            tempLeft[i] = vertexArrayLeft[i];
        }
        for (int i = size - 1; i >= 0; --i)
        {
            vertexArrayRight.push_front(tempRight[i]);
            vertexArrayLeft.push_front(tempLeft[i]);
        }
        for (int i = size - 1; i >= 0; --i)
        {
            vertexArrayRight.push_front(tempRight[i]);
            vertexArrayLeft.push_front(tempLeft[i]);
        }


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

        int size = 15;
        float tempRight[size];
        float tempLeft[size];
        int arraySize = vertexArrayRight.size();
        for (int i = arraySize - size; i < arraySize; ++i)
        {
            tempRight[i - (arraySize - size)] = vertexArrayRight[i];
            tempLeft[i - (arraySize - size)] = vertexArrayLeft[i];
        }
        for (int i = 0; i < size; ++i)
        {
            vertexArrayRight.push_back(tempRight[i]);
            vertexArrayLeft.push_back(tempLeft[i]);
        }
        for (int i = 0; i < size; ++i)
        {
            vertexArrayRight.push_back(tempRight[i]);
            vertexArrayLeft.push_back(tempLeft[i]);
        }
    }


    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();
    setTextureArray(texture_1Usize, texture_1Vsize);

    //setRightVertexArray();
    setRightTextureArray(texture_2Usize, texture_2Vsize);
    setRightIndexArray();

    //setLeftVertexArray();
    setLeftTextureArray(texture_2Usize, texture_2Vsize);
    setLeftIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].lineType == 6)
        {
            SplitZone* splitZone = dynamic_cast<SplitZone*>(lines[i].line);
            splitZone->addBreak(front);
        }
        else
        {
            LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
            line->addBreak(front);
        }

    }
    //resetLines();

}

void RoadBroken::setIndexArrayForSelectionFrame()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setIndexArrayForSelectionFrame()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setColorArrayForSelectionFrame(float red, float green, float blue)"
                                   << " red = " << red
                                   << " green = " << green
                                   << " blue = " << blue << "\n";
    if (colorArrayForSelection.size() != vertexArray.size())
        colorArrayForSelection.resize(vertexArray.size());
    for (int i = 0; i < vertexArray.size()/3; ++i)
    {
        colorArrayForSelection[i * 3] = red;
        colorArrayForSelection[i * 3 + 1] = green;
        colorArrayForSelection[i * 3 + 2] = blue;
    }
}

void RoadBroken::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "RoadBroken::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}



bool RoadBroken::isSelected()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::isSelected()\n";
    return selected;
}

void RoadBroken::setSelectedStatus(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setSelectedStatus(bool status)"
                                   << " status = " << status << "\n";
    selected = status;
    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->setSelectedStatus(status);
}

void RoadBroken::drawFigure(QGLWidget* render)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::drawFigure(QGLWidget* render)\n";
    if (render == NULL)
        Logger::getLogger()->warningLog() << "RoadBroken::drawFigure(QGLWidget* render), render = NULL\n";
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
        lines[i].line->drawFigure(render);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);

    if (selected == true)
    {
        glDisable(GL_DEPTH_TEST);
        // Если фигуры выбрана - изменяем цвет заливки
        setColorArray(0.7f, 0.7f, 0.7f, alpha);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);

    }
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        glDisable(GL_DEPTH_TEST);
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0f);
        glEnable(GL_DEPTH_TEST);
    }
}

void RoadBroken::drawSelectionFrame()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::drawSelectionFrame()";
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, colorArrayForSelection.begin());
    glLineWidth(5.0);
    glDrawElements(GL_LINE_LOOP, indexArrayForSelection.size(), GL_UNSIGNED_BYTE, indexArrayForSelection.begin());

    // Угловые точки для изменения размера
    glPointSize(10.0);
    glDrawElements(GL_POINTS, indexArrayForSelection.size(), GL_UNSIGNED_BYTE, indexArrayForSelection.begin());

    int linesControlsCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesControlsCount += lines[i].line->getNumberOfControls();

    drawControlElement(vertexArray.size() / 3 + linesControlsCount, 5.0f, 10.0f);
    drawControlElement(vertexArray.size() / 3 + linesControlsCount + 1, 5.0f, 10.0f);

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
        glPointSize(10.0f);
        for (int i = 0; i < vertexArrayRight.size() / 3; i += 5)
        {
            glBegin(GL_POINTS);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayRight[(i + 4) * 3],
                    vertexArrayRight[(i + 4) * 3 + 1],
                    vertexArrayRight[(i + 4) * 3 + 2]);
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

        glPointSize(10.0f);
        for (int i = 0; i < vertexArrayLeft.size() / 3; i += 5)
        {
            glBegin(GL_POINTS);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayLeft[(i + 4) * 3],
                    vertexArrayLeft[(i + 4) * 3 + 1],
                    vertexArrayLeft[(i + 4) * 3 + 2]);
            glEnd();
        }
    }

}

void RoadBroken::move(float dx, float dy, float dz)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::move(float dx, float dy, float dz)"
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " dz = " << dz << "\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::drawControlElement(int index, float lineWidth, float pointSize)"
                                   << " index = " << index
                                   << " lineWidth = " << lineWidth
                                   << " pointSize = " << pointSize << "\n";
    int linesControlsCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesControlsCount += lines[i].line->getNumberOfControls();

    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 2 + linesControlsCount)
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
        return;
    }
    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 1 + 2 + linesControlsCount)
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
        return;
    }
    if (index >= vertexArray.size() / 3 + vertexArray.size() / 3 + vertexArray.size() / 6 + 2 + linesControlsCount)
    {
        int j = index - vertexArray.size() / 3 - vertexArray.size() / 3 - vertexArray.size() / 6 - 2 - linesControlsCount;
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
            return;
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
            return;
        }
        return;
    }

    if (index >= vertexArray.size() / 3 + vertexArray.size() / 3  + 2 + linesControlsCount)
    {

        int j = index - vertexArray.size() / 3 - vertexArray.size() / 3 - 2 - linesControlsCount;
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
                return;
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
                return;
            }
        }

    }



    if (index >= vertexArray.size() / 3 + 2 + linesControlsCount)
    {
        int j = index - vertexArray.size() / 3 - 2 - linesControlsCount;
        if (j % 2 == 0)
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayRight[(j / 2 * 5 + 4) * 3],
                    vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1],
                    vertexArrayRight[(j / 2 * 5 + 4) * 3 + 2]);
            glEnd();
        }
        else
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3],
                    vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1],
                    vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 2]);
            glEnd();
        }
        return;
    }
    else
        if (index >= vertexArray.size() / 3 + linesControlsCount)
        {
            int j = index - vertexArray.size() / 3 - linesControlsCount;
            switch (j)
            {
            case 0:
            {
                int k = 0;
                glPointSize(pointSize);
                glBegin(GL_POINTS);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f((vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f,
                        (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f,
                        (vertexArray[k * 3 + 2] + vertexArray[(k + 1) * 3 + 2]) / 2.0f);
                glEnd();
            }
                break;
            case 1:
            {
                int k = vertexArray.size() / 3 - 2;
                glPointSize(pointSize);
                glBegin(GL_POINTS);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f((vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f,
                        (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f,
                        (vertexArray[k * 3 + 2] + vertexArray[(k + 1) * 3 + 2]) / 2.0f);
                glEnd();
            }
                break;
            default:
                break;
            }

        }
        else
            if (index >= vertexArray.size() / 3)
            {
                int count = index - vertexArray.size() / 3;
                int j = -1;
                for (int i = 0; i < lines.size(); ++i)
                {
                    if (count >= lines[i].line->getNumberOfControls())
                    {
                        count -= lines[i].line->getNumberOfControls();
                    }
                    else
                    {
                        j = i;
                        break;
                    }
                }
                if (j >=0)
                {
                    lines[j].line->drawControlElement(count, lineWidth, pointSize);
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
                return;
            }




}

QCursor RoadBroken::getCursorForControlElement(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getCursorForControlElement(int index)"
                                   << " index = " << index << "\n";
    int linesControlsCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesControlsCount += lines[i].line->getNumberOfControls();

    if (index >= vertexArray.size() / 3)
        return Qt::CrossCursor;
    else
        return Qt::CrossCursor;
}

void RoadBroken::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resizeByControl(int index, float dx, float dy, float x, float y)"
                                   << " index = " << index
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " x = " << x
                                   << " y = " << y << "\n";
    int linesControlsCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesControlsCount += lines[i].line->getNumberOfControls();

    if (fixed)
    {
        return;
    }
    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 2 + linesControlsCount)
    {
        float x1 = vertexArray[0];
        float y1 = vertexArray[1];
        float x2 = vertexArray[3];
        float y2 = vertexArray[4];
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float dx2 = 5.0f;
        float dy2 = (-1.0f) * (dx1*dx2) / dy1;
        float r = sqrt(dx2*dx2 + dy2*dy2);
        float dr = (dx*dx2 + dy*dy2) / r;
        float dxRes = dx2 / r * dr;
        float dyRes = dy2 / r * dr;
        vertexArray[0] += dxRes;
        vertexArray[1] += dyRes;
        vertexArray[3] += dxRes;
        vertexArray[4] += dyRes;
        for (int i = 0; i < lines.size(); ++i)
        {
            if (lines[i].lineType == 6)
            {
                SplitZone* splitZone = dynamic_cast<SplitZone*>(lines[i].line);
                splitZone->resizeByControl(0, dxRes, dyRes, x, y);
            }
            else
            {
                LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
                line->resizeByControl(0, dxRes, dyRes, x, y);
            }

        }
    }
    else
    {
        if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 1 + 2 + linesControlsCount)
        {
            int j = vertexArray.size() / 3 - 2;
            float x1 = vertexArray[j * 3];
            float y1 = vertexArray[j * 3 + 1];
            float x2 = vertexArray[(j + 1) * 3];
            float y2 = vertexArray[(j + 1) * 3 + 1];
            float dx1 = x1 - x2;
            float dy1 = y1 - y2;
            float dx2 = 5.0f;
            float dy2 = (-1.0f) * (dx1*dx2) / dy1;
            float r = sqrt(dx2*dx2 + dy2*dy2);
            float dr = (dx*dx2 + dy*dy2) / r;
            float dxRes = dx2 / r * dr;
            float dyRes = dy2 / r * dr;
            vertexArray[j * 3] += dxRes;
            vertexArray[j * 3 + 1] += dyRes;
            vertexArray[(j + 1) * 3] += dxRes;
            vertexArray[(j + 1) * 3 + 1] += dyRes;
            for (int i = 0; i < lines.size(); ++i)
            {
                if (lines[i].lineType == 6)
                {
                    SplitZone* splitZone = dynamic_cast<SplitZone*>(lines[i].line);
                    splitZone->resizeByControl(splitZone->getNumberOfControls() - 1, dxRes, dyRes, x, y);
                }
                else
                {
                    LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
                    line->resizeByControl(line->getNumberOfControls() - 1, dxRes, dyRes, x, y);
                }

            }
        }

        else
            if (index >= vertexArray.size() / 3 + vertexArray.size() / 3 + vertexArray.size() / 6 + 2 + linesControlsCount)
            {
                // Изменение линий дороги
                int j = index - vertexArray.size() / 3 - vertexArray.size() / 3 - vertexArray.size() / 6 - 2 - linesControlsCount;
                if (j < vertexArray.size() / (3 * 4))
                {
                    resetVertexArray(dx, dy, true);
                }
                else
                {
                    resetVertexArray(dx, dy, false);
                }
            }
            else
            {
                if (index >= vertexArray.size() / 3 + vertexArray.size() / 3 + 2 + linesControlsCount)
                {
                    int j = index - vertexArray.size() / 3 - vertexArray.size() / 3 - 2 - linesControlsCount;
                    if (j < vertexArray.size() / (3 * 4))
                    {
                        resetRightVertexArray(dx, dy);
                    }
                    else
                    {
                        j -= vertexArray.size() / (3 * 4);
                        resetLeftVertexArray(dx, dy);
                    }
                }
                else

                    if (index >= vertexArray.size() / 3 + 2 + linesControlsCount)
                    {
                        // Изменение точек тротуара
                        int j = index - vertexArray.size() / 3 - 2 - linesControlsCount;
                        if (j % 2 == 0)
                        {
                            float x1 = vertexArrayRight[(j / 2 * 5 + 4) * 3];
                            float y1 = vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1];
                            float x2 = vertexArray[j * 3];
                            float y2 = vertexArray[j * 3 + 1];
                            float dx1 = x1 - x2;
                            float dy1 = y1 - y2;
                            float r = sqrt(dx1*dx1 + dy1*dy1);
                            float dr = (dx*dx1 + dy*dy1) / r;
                            float dxRes = dx1 / r * dr;
                            float dyRes = dy1 / r * dr;

                            vertexArrayRight[(j / 2 * 5 + 4) * 3] += dxRes;
                            vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1] += dyRes;
                            x1 = vertexArrayRight[(j / 2 * 5 + 4) * 3];
                            y1 = vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1];
                            x2 = vertexArrayRight[j / 2 * 5 * 3];
                            y2 = vertexArrayRight[j / 2 * 5 * 3 + 1];
                            r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

                            vertexArrayRight[(j / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                            vertexArrayRight[(j / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                            vertexArrayRight[(j / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                            vertexArrayRight[(j / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;

                            if (j != 0 && j != (vertexArray.size() / 3 - 2))
                            {
                                j = (j % 4) == 0 ? j - 2 : j + 2;
                                vertexArrayRight[(j / 2 * 5 + 4) * 3] += dxRes;
                                vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1] += dyRes;

                                vertexArrayRight[(j / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                vertexArrayRight[(j / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                vertexArrayRight[(j / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                vertexArrayRight[(j / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                            }


                            fixedRightWidth = false;

                        }
                        else
                        {
                            float x1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3];
                            float y1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1];
                            float x2 = vertexArray[j * 3];
                            float y2 = vertexArray[j * 3 + 1];
                            float dx1 = x1 - x2;
                            float dy1 = y1 - y2;
                            float r = sqrt(dx1*dx1 + dy1*dy1);
                            float dr = (dx*dx1 + dy*dy1) / r;
                            float dxRes = dx1 / r * dr;
                            float dyRes = dy1 / r * dr;

                            vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3] += dxRes;
                            vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1] += dyRes;
                            x1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3];
                            y1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1];
                            x2 = vertexArrayLeft[(j - 1) / 2 * 5 * 3];
                            y2 = vertexArrayLeft[(j - 1) / 2 * 5 * 3 + 1];
                            r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

                            vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                            vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                            vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                            vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                            if (j != 1 && j != (vertexArray.size() / 3 - 1))
                            {
                                j = ((j - 1) % 4) == 0 ? j - 2 : j + 2;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3] += dxRes;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1] += dyRes;

                                vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                            }

                            fixedLeftWidth = false;
                        }

                    }
                    else
                        if (index >= vertexArray.size() / 3 + linesControlsCount)
                        {
                            int j = index - vertexArray.size() / 3 - linesControlsCount;
                            float X1 = 0.0f, X2 = 0.0f, X3 = 0.0f,
                                    Y1 = 0.0f, Y2 = 0.0f, Y3 = 0.0f;
                            switch (j)
                            {
                            case 0:
                            {
                                int k = 0;
                                X1 = (vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f;
                                Y1 = (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f;
                                k = vertexArray.size() / 3 - 2;
                                X2 = (vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f;
                                Y2 = (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f;
                            }
                                break;
                            case 1:
                            {
                                int k = vertexArray.size() / 3 - 2;
                                X1 = (vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f;
                                Y1 = (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f;
                                k = 0;
                                X2 = (vertexArray[k * 3] + vertexArray[(k + 1) * 3]) / 2.0f;
                                Y2 = (vertexArray[k * 3 + 1] + vertexArray[(k + 1) * 3 + 1]) / 2.0f;
                            }
                                break;
                            default:
                                break;
                            }

                            X3 = X1 + dx;
                            Y3 = Y1 + dy;
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
                            for (int i = 0; i < vertexArray.size() / 3; ++i)
                            {
                                vertexArray[i * 3] -= X2;
                                vertexArray[i * 3 + 1] -= Y2;
                                float tx = vertexArray[i * 3];
                                float ty = vertexArray[i * 3 + 1];
                                vertexArray[i * 3] = tx * cos(angle) - ty * sin(angle);
                                vertexArray[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
                                vertexArray[i * 3] += X2;
                                vertexArray[i * 3 + 1] += Y2;
                            }
                            for (int i = 0; i < vertexArrayRight.size() / 3; ++i)
                            {
                                vertexArrayRight[i * 3] -= X2;
                                vertexArrayRight[i * 3 + 1] -= Y2;
                                float tx = vertexArrayRight[i * 3];
                                float ty = vertexArrayRight[i * 3 + 1];
                                vertexArrayRight[i * 3] = tx * cos(angle) - ty * sin(angle);
                                vertexArrayRight[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
                                vertexArrayRight[i * 3] += X2;
                                vertexArrayRight[i * 3 + 1] += Y2;
                            }
                            for (int i = 0; i < vertexArrayLeft.size() / 3; ++i)
                            {
                                vertexArrayLeft[i * 3] -= X2;
                                vertexArrayLeft[i * 3 + 1] -= Y2;
                                float tx = vertexArrayLeft[i * 3];
                                float ty = vertexArrayLeft[i * 3 + 1];
                                vertexArrayLeft[i * 3] = tx * cos(angle) - ty * sin(angle);
                                vertexArrayLeft[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
                                vertexArrayLeft[i * 3] += X2;
                                vertexArrayLeft[i * 3 + 1] += Y2;
                            }
                            for (int i = 0; i < lines.size(); ++i)
                            {
                                if (lines[i].lineType == 6)
                                {
                                    SplitZone* splitZone = dynamic_cast<SplitZone*>(lines[i].line);
                                    splitZone->rotate(angle, X2, Y2, 0.0f);
                                }
                                else
                                {
                                    LineBroken* line = dynamic_cast<LineBroken*>(lines[i].line);
                                    line->rotate(angle, X2, Y2, 0.0f);
                                }

                            }
                        }
                        else
                            if (index >= vertexArray.size() / 3)
                            {
                                int count = index - vertexArray.size() / 3;
                                int j = -1;
                                for (int i = 0; i < lines.size(); ++i)
                                {
                                    if (count >= lines[i].line->getNumberOfControls())
                                    {
                                        count -= lines[i].line->getNumberOfControls();
                                    }
                                    else
                                    {
                                        j = i;
                                        break;
                                    }
                                }
                                if (j >=0)
                                {
                                    lines[j].line->resizeByControl(count, dx, dy, x, y);
                                }
                            }
                            else

                            {
                                float x1 = vertexArray[index * 3];
                                float y1 = vertexArray[index * 3 + 1];
                                int j = index % 2 ? index - 1 : index + 1;
                                float x2 = vertexArray[j * 3];
                                float y2 = vertexArray[j * 3 + 1];
                                float dx1 = x1 - x2;
                                float dy1 = y1 - y2;
                                float r = sqrt(dx1*dx1 + dy1*dy1);
                                float dr = (dx*dx1 + dy*dy1) / r;
                                float dxRes = dx1 / r * dr;
                                float dyRes = dy1 / r * dr;
                                // Изменение узловых точек
                                vertexArray[index * 3] += dxRes;
                                vertexArray[index * 3 + 1] += dyRes;

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

                                    vertexArray[index * 3] += dxRes;
                                    vertexArray[index * 3 + 1] += dyRes;
                                }
                            }
            }
    }


    setTextureArray(texture_1Usize, texture_1Vsize);
    //if (fixedRightWidth)
    //    setRightVertexArray();
    //else
    resetRightVertexArray();
    setRightTextureArray(texture_2Usize, texture_2Vsize);
    //if (fixedLeftWidth)
    //    setLeftVertexArray();
    //else
    resetLeftVertexArray();
    setLeftTextureArray(texture_2Usize, texture_2Vsize);
    //resetLines();

}

void RoadBroken::resizeByControl(int index, float dx, float dy, float x, float y, int keyBoardKey)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resizeByControl(int index, float dx, float dy, float x, float y, int keyBoardKey)"
                                   << " index = " << index
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " x = " << x
                                   << " y = " << y
                                   << " keyBoardKey = " << keyBoardKey << "\n";
    if (fixed)
    {
        return;
    }
    if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 2)
    {
        switch (keyBoardKey)
        {
        case Qt::Key_Shift:
        {
            // Первый торец
            vertexArray[0] += dx;
            vertexArray[1] += dy;

            vertexArray[3] += dx;
            vertexArray[4] += dy;
        }
            break;
        default:
            break;
        }
    }
    else
    {
        if (index == vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 1 + 2)
        {
            int j = vertexArray.size() / 3 - 2;
            switch (keyBoardKey)
            {
            case Qt::Key_Shift:
            {
                // Второй торец
                int j = vertexArray.size() / 3 - 2;

                vertexArray[j * 3] += dx;
                vertexArray[j * 3 + 1] += dy;

                vertexArray[(j + 1) * 3] += dx;
                vertexArray[(j + 1) * 3 + 1] += dy;
            }
                break;
            default:
                break;
            }
        }

        else
            if (index >= vertexArray.size() / 3 + vertexArray.size() / 3 + vertexArray.size() / 6 + 2)
            {
                /*
                // Изменение линий дороги
                int j = index - vertexArray.size() / 3 - vertexArray.size() / 3 - vertexArray.size() / 6;
                if (j < vertexArray.size() / (3 * 4))
                {
                    resetVertexArray(dx, dy, true);
                }
                else
                {
                    resetVertexArray(dx, dy, false);
                }
                */
            }
            else
            {
                if (index >= vertexArray.size() / 3 + vertexArray.size() / 3 + 2)
                {
                    /*
                    int j = index - vertexArray.size() / 3 - vertexArray.size() / 3;
                    if (j < vertexArray.size() / (3 * 4))
                    {
                        resetRightVertexArray(dx, dy);
                    }
                    else
                    {
                        j -= vertexArray.size() / (3 * 4);

                        resetLeftVertexArray(dx, dy);
                    }
                    */
                }
                else

                    if (index >= vertexArray.size() / 3 + 2)
                    {
                        // Изменение точек тротуара
                        int j = index - vertexArray.size() / 3 - 2;
                        if (j % 2 == 0)
                        {
                            switch (keyBoardKey)
                            {
                            case Qt::Key_Shift:
                            {
                                vertexArrayRight[(j / 2 * 5 + 4) * 3] += dx;
                                vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1] += dy;
                                float x1 = vertexArrayRight[(j / 2 * 5 + 4) * 3];
                                float y1 = vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1];
                                float x2 = vertexArrayRight[j / 2 * 5 * 3];
                                float y2 = vertexArrayRight[j / 2 * 5 * 3 + 1];
                                float r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

                                vertexArrayRight[(j / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                vertexArrayRight[(j / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                vertexArrayRight[(j / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                vertexArrayRight[(j / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;

                                if (j != 0 && j != (vertexArray.size() / 3 - 2))
                                {
                                    j = (j % 4) == 0 ? j - 2 : j + 2;
                                    vertexArrayRight[(j / 2 * 5 + 4) * 3] += dx;
                                    vertexArrayRight[(j / 2 * 5 + 4) * 3 + 1] += dy;

                                    vertexArrayRight[(j / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                    vertexArrayRight[(j / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                    vertexArrayRight[(j / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                    vertexArrayRight[(j / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                                }


                                fixedRightWidth = false;
                            }
                                break;
                            default:
                                break;
                            }

                        }
                        else
                        {
                            switch (keyBoardKey)
                            {
                            case Qt::Key_Shift:
                            {
                                // Изменение узловых точек
                                vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3] += dx;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1] += dy;
                                float x1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3];
                                float y1 = vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1];
                                float x2 = vertexArrayLeft[(j - 1) / 2 * 5 * 3];
                                float y2 = vertexArrayLeft[(j - 1) / 2 * 5 * 3 + 1];
                                float r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

                                vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                                if (j != 1 && j != (vertexArray.size() / 3 - 1))
                                {
                                    j = ((j - 1) % 4) == 0 ? j - 2 : j + 2;
                                    vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3] += dx;
                                    vertexArrayLeft[((j - 1) / 2 * 5 + 4) * 3 + 1] += dy;

                                    vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3] = x2 + (x1 - x2) / r * 0.03f;
                                    vertexArrayLeft[((j - 1) / 2 * 5 + 2) * 3 + 1] = y2 + (y1 - y2) / r * 0.03f;

                                    vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3] = x2 + (x1 - x2) / r * 0.25f;
                                    vertexArrayLeft[((j - 1) / 2 * 5 + 3) * 3 + 1] = y2 + (y1 - y2) / r * 0.25f;
                                }

                                fixedLeftWidth = false;
                            }
                                break;
                            default:
                                break;
                            }
                        }

                    }
                    else
                        if (index >= vertexArray.size() / 3)
                        {
                            // Точки для поворота
                            resizeByControl(index, dx, dy, x, y);
                        }
                        else

                        {
                            switch (keyBoardKey)
                            {
                            case Qt::Key_Shift:
                            {
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
                                break;
                            default:
                                break;
                            }


                        }
            }
    }
    setTextureArray(texture_1Usize, texture_1Vsize);
    // if (fixedRightWidth)
    //     setRightVertexArray();
    // else
    resetRightVertexArray();
    setRightTextureArray(texture_2Usize, texture_2Vsize);
    // if (fixedLeftWidth)
    //     setLeftVertexArray();
    // else
    resetLeftVertexArray();
    setLeftTextureArray(texture_2Usize, texture_2Vsize);
    resetLines();

}

int RoadBroken::getNumberOfControls()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getNumberOfControls()\n";
    int roadPoints = vertexArray.size() / 3;
    int roadLines = roadPoints / 2 + 2;
    int boardPoints = roadPoints;
    int boardLines = roadPoints / 2;
    int result = roadPoints + roadLines + boardPoints + boardLines + 2;
    int linesControlsCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesControlsCount += lines[i].line->getNumberOfControls();
    return result + linesControlsCount;
    //return vertexArray.size() / 3 + vertexArray.size() / 6 + vertexArray.size() / 6 + vertexArray.size() / 3 + 2;
}

int RoadBroken::controlsForPoint()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::controlsForPoint()\n";
    return 1;
}

void RoadBroken::changeColorOfSelectedControl(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::changeColorOfSelectedControl(int index)"
                                   << " index = " << index << "\n";
    indexOfSelectedControl = index;
}

void RoadBroken::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)"
                                   << " x = " << x
                                   << " y = " << y
                                   << " z = " << z << "\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getJSONInfo()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getProperties(QFormLayout *layout, QGLWidget* render)\n";
    clearProperties(layout);
    this->layout = layout;
    this->render = render;

    QCheckBox* showRightBoardCheckBox = new QCheckBox();
    QCheckBox* showLeftBoardCheckBox = new QCheckBox();
    QCheckBox* fixedCheckBox = new QCheckBox();
    QDoubleSpinBox* rightBoardSpinBox = new QDoubleSpinBox();
    rightBoardSpinBox->setKeyboardTracking(false);
    QDoubleSpinBox* leftBoardSpinBox = new QDoubleSpinBox();
    leftBoardSpinBox->setKeyboardTracking(false);
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
    connect(stepDialog, SIGNAL(singleWayChanged(bool)), this, SLOT(setSingleWay(bool)));
    connect(stepDialog, SIGNAL(axisStepChanged(double)), this, SLOT(setAxisStep(double)));
    connect(stepDialog, SIGNAL(splitZoneTypeChanged(int)), this, SLOT(setSplitZoneType(int)));
    connect(stepDialog, SIGNAL(splitZoneHeightChanged(double)), this, SLOT(setSplitZoneHeight(double)));
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::isFixed()\n";
    return fixed;
}

void RoadBroken::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide)"
                                   << " step = " << step
                                   << " textureSource = " << textureSource
                                   << " textureSize = " << textureSize
                                   << " lineWidth = " << lineWidth
                                   << " lineType = " << lineType
                                   << " rightSide = " << rightSide << "\n";
    //qDebug() << "Got axis array, size: " << size / 3;
    LineBrokenLinkedToRoadBroken line;
    if (lineType == 8)
    {
        if (singleWay)
        {
            QVector<float> axisArray;
            getVertexArrayForLineAxis(axisArray,rightSide,step,beginStep,endStep);
            int size = axisArray.size();
            float *lineVertexArray = new float[size];
            for (int i = 0; i < size; ++i)
                lineVertexArray[i] = axisArray[i];

            LineBroken* lineBroken = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                                    QString("Линия №") + QString::number(lines.size() + 1));
            line.line = lineBroken;
            model->getGroup(1).push_back(lineBroken);
            delete[] lineVertexArray;
            lineVertexArray = NULL;
        }
        else
        {
            QVector<float> axisArray;
            getVertexArrayForLineAxis(axisArray,rightSide,step + axisStep / 2.0,beginStep,endStep);
            int size = axisArray.size();
            float *lineVertexArray = new float[size];
            for (int i = 0; i < size; ++i)
                lineVertexArray[i] = axisArray[i];

            //LineBrokenLinkedToRoadBroken line1;

            LineBroken* lineBroken = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                                    QString("Линия №") + QString::number(lines.size() + 1));
            line.line = lineBroken;
            model->getGroup(1).push_back(lineBroken);

            line.lineWidth = lineWidth;
            line.step = step;
            line.rightSide = rightSide;
            line.lineType = lineType;
            line.textureSource = textureSource;
            line.beginStep = beginStep;
            line.endStep = endStep;
            line.line->setSelectedStatus(false);
            lines.push_back(line);

            lineBroken = NULL;
            axisArray.clear();
            getVertexArrayForLineAxis(axisArray,rightSide,step - axisStep / 2.0,beginStep,endStep);
            for (int i = 0; i < size; ++i)
                lineVertexArray[i] = axisArray[i];
            lineBroken = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                        QString("Линия №") + QString::number(lines.size() + 1));
            line.line = lineBroken;
            model->getGroup(1).push_back(lineBroken);

            delete[] lineVertexArray;
            lineVertexArray = NULL;
        }

    }
    else
        if (lineType == 6)
        {
            QVector<float> axisArray;
            getVertexArrayForLineAxis(axisArray,rightSide,step,beginStep,endStep);
            int size = axisArray.size();
            float *lineVertexArray = new float[size];
            for (int i = 0; i < size; ++i)
                lineVertexArray[i] = axisArray[i];

            SplitZone* splitZone = NULL;
            switch (splitZoneType)
            {
            case 0:
            {
                splitZone = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                          QString("Линия №") + QString::number(lines.size() + 1));
            }
                break;
            case 1:
            {
                splitZone = new SplitZone(lineVertexArray, size,
                                          splitZoneWidth, beginRounding, endRounding,
                                          splitZoneType,
                                          splitZoneHeight,
                                          QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                          0.25f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/grass.jpg",
                                          3.0f, 3.0f,
                                          QString("Линия №") + QString::number(lines.size() + 1));
            }
                break;
            case 2:
            {
                splitZone = new SplitZone(lineVertexArray, size,
                                          splitZoneWidth, beginRounding, endRounding,
                                          splitZoneType,
                                          splitZoneHeight,
                                          QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                          0.25f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07S.jpg",
                                          6.0f, 6.0f,
                                          QString("Линия №") + QString::number(lines.size() + 1));
            }
                break;
            default:
                break;
            }


            line.line = splitZone;
            model->getGroup(1).push_back(splitZone);
            line.splitZoneWidth = splitZoneWidth;
            line.beginRounding = beginRounding;
            line.endRounding = endRounding;

            delete[] lineVertexArray;
            lineVertexArray = NULL;
        }
        else
        {
            QVector<float> axisArray;
            getVertexArrayForLineAxis(axisArray,rightSide,step,beginStep,endStep);
            int size = axisArray.size();
            float *lineVertexArray = new float[size];
            for (int i = 0; i < size; ++i)
                lineVertexArray[i] = axisArray[i];

            LineBroken* lineBroken = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                                    QString("Линия №") + QString::number(lines.size() + 1));
            line.line = lineBroken;
            model->getGroup(1).push_back(lineBroken);

            delete[] lineVertexArray;
            lineVertexArray = NULL;

        }
    //qDebug() << "Created line";
    line.lineWidth = lineWidth;
    line.step = step;
    line.rightSide = rightSide;
    line.lineType = lineType;
    line.textureSource = textureSource;
    line.beginStep = beginStep;
    line.endStep = endStep;
    line.line->setSelectedStatus(false);
    lines.push_back(line);

    emit linesChanged(layout, render);
}

void RoadBroken::addLine()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::addLine()\n";
    //qDebug() << "Add line";
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
    case 8:
        textSource = QString(":/textures/tramways.png");
        lWidth = 1.5f;
        addLine(step, textSource, 1.5f, lWidth, lineType, rightSide);
        return;
        break;
    default:
        break;

    }

    addLine(step, textSource, 6.0f, lWidth, lineType, rightSide);
    //qDebug() << "Line added";
}

void RoadBroken::setRightSide(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightSide(bool status)"
                                   << " status = " << status << "\n";
    rightSide = status;
}

void RoadBroken::setStep(double value)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setStep(double value)"
                                   << " value = " << value << "\n";
    step = value;
}

void RoadBroken::setLineType(int type)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLineType(int type)"
                                   << " type = " << type << "\n";
    lineType = type;
}

void RoadBroken::deleteLine()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::deleteLine()\n";
    QPushButton * b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    //qDebug() << "delete line " << b->text();
    int i = b->text().toInt() - 1;
    for (QList<RoadElement*>::iterator it = model->getGroup(1).begin();
         it != model->getGroup(1).end(); ++it)
    {
        if (lines[i].line == (*it))
        {
            model->getGroup(1).erase(it);
            break;
        }
    }
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::resetLines()\n";
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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setBeginStep(double step)"
                                   << " step = " << step << "\n";
    beginStep = step;
}

void RoadBroken::setEndStep(double step)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setEndStep(double step)"
                                   << " step = " << step << "\n";
    endStep = step;
}

void RoadBroken::setSplitZoneWidth(double value)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setSplitZoneWidth(double value)"
                                   << " value = " << value << "\n";
    splitZoneWidth = value;
}

void RoadBroken::setBeginSide(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setBeginSide(bool status)"
                                   << " status = " << status << "\n";
    beginSide = status;
}

void RoadBroken::setBeginRounding(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setBeginRounding(bool status)"
                                   << " status = " << status << "\n";
    beginRounding = status;
}

void RoadBroken::setEndRounding(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setEndRounding(bool status)"
                                   << " status = " << status << "\n";
    endRounding = status;
    //qDebug() << "endRounding" << endRounding;
}

void RoadBroken::setDifferentDirections(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setDifferentDirections(bool status)"
                                   << " status = " << status << "\n";
    differentDirections = status;
}

void RoadBroken::setSingleWay(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setSingleWay(bool status)"
                                   << " status = " << status << "\n";
    singleWay = status;
}

void RoadBroken::setAxisStep(double step)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setAxisStep(double step)"
                                   << " step = " << step << "\n";
    axisStep = step;
}

void RoadBroken::setSplitZoneType(int type)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setSplitZoneType(int type)"
                                   << " type = " << type << "\n";
    splitZoneType = type;
}

void RoadBroken::setSplitZoneHeight(double height)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setSplitZoneHeight(double height)"
                                   << " height = " << height << "\n";
    splitZoneHeight = height;
}


void RoadBroken::drawMeasurements(QGLWidget *render)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::drawMeasurements(QGLWidget *render)\n";
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);
    float x1, x2, x3, x4, xR, xL, xL1, xR1,
            y1, y2, y3, y4, yR, yL, yR1, yL1;
    float dr, dr1, dr2, drR, drL, drR1, drL1;

    glEnable(GL_LINE_STIPPLE);
    for (int i = 0; i < vertexArray.size() / 3 - 1; i += 4)
    {
        if (i % 4 != 0)
            continue;
        x1 = vertexArray[i * 3];
        y1 = vertexArray[i * 3 + 1];
        x2 = vertexArray[(i + 1) * 3];
        y2 = vertexArray[(i + 1) * 3 + 1];
        x3 = vertexArray[(i + 2) * 3];
        y3 = vertexArray[(i + 2) * 3 + 1];
        x4 = vertexArray[(i + 3) * 3];
        y4 = vertexArray[(i + 3) * 3 + 1];
        xR = vertexArrayRight[(i / 2 * 5 + 4) * 3];
        yR = vertexArrayRight[(i / 2 * 5 + 4) * 3 + 1];
        xR1 = vertexArrayRight[((i + 2) / 2 * 5 + 4) * 3];
        yR1 = vertexArrayRight[((i + 2) / 2 * 5 + 4) * 3 + 1];
        xL = vertexArrayLeft[(i / 2 * 5 + 4) * 3];
        yL = vertexArrayLeft[(i / 2 * 5 + 4) * 3 + 1];
        xL1 = vertexArrayLeft[((i + 2) / 2 * 5 + 4) * 3];
        yL1 = vertexArrayLeft[((i + 2) / 2 * 5 + 4) * 3 + 1];
        dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
        dr1 = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
        dr2 = sqrt((x4 - x2)*(x4 - x2) + (y4 - y2)*(y4 - y2));
        drR = sqrt((x1 - xR)*(x1 - xR) + (y1 - yR)*(y1 - yR));
        drL = sqrt((x2 - xL)*(x2 - xL) + (y2 - yL)*(y2 - yL));
        drR1 = sqrt((xR1 - xR)*(xR1 - xR) + (yR1 - yR)*(yR1 - yR));
        drL1 = sqrt((xL1 - xL)*(xL1 - xL) + (yL1 - yL)*(yL1 - yL));
        if (i >= 2 && i < vertexArray.size() / 3  - 2)
        {
            glLineStipple(1, 0x0FFF);
            glLineWidth(1.5f);

            glBegin(GL_LINES);

            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(x1, y1, 0.3f);
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(x2, y2, 0.3f);
            if (showRightBoard)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(x1, y1, 0.3f);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(xR, yR, 0.3f);
            }

            if (showLeftBoard)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(x2, y2, 0.3f);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(xL, yL, 0.3f);
            }


            glEnd();


        }
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        glColor3f(1.0f, 1.0f, 1.0f);
        render->renderText(wx, wy, QString("%1").arg(dr, 0, 'f', 2), shrift);


        x = (x1 + x3) / 2.0f;
        y = (y1 + y3) / 2.0f;
        z = 0.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        glColor3f(1.0f, 1.0f, 1.0f);
        render->renderText(wx, wy, QString("%1").arg(dr1, 0, 'f', 2), shrift);

        x = (x2 + x4) / 2.0f;
        y = (y2 + y4) / 2.0f;
        z = 0.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        glColor3f(1.0f, 1.0f, 1.0f);
        render->renderText(wx, wy, QString("%1").arg(dr2, 0, 'f', 2), shrift);

        if (showRightBoard)
        {
            x = (x1 + xR) / 2.0f;
            y = (y1 + yR) / 2.0f;
            z = 0.0f;
            getWindowCoord(x, y, z, wx, wy, wz);
            glColor3f(0.0f, 0.0f, 0.0f);
            render->renderText(wx, wy, QString("%1").arg(drR, 0, 'f', 2), shrift);

            x = (xR1 + xR) / 2.0f;
            y = (yR1 + yR) / 2.0f;
            z = 0.0f;
            getWindowCoord(x, y, z, wx, wy, wz);
            glColor3f(0.0f, 0.0f, 0.0f);
            render->renderText(wx, wy, QString("%1").arg(drR1, 0, 'f', 2), shrift);
        }

        if (showLeftBoard)
        {
            x = (x2 + xL) / 2.0f;
            y = (y2 + yL) / 2.0f;
            z = 0.0f;
            getWindowCoord(x, y, z, wx, wy, wz);
            glColor3f(0.0f, 0.0f, 0.0f);
            render->renderText(wx, wy, QString("%1").arg(drL, 0, 'f', 2), shrift);

            x = (xL1 + xL) / 2.0f;
            y = (yL1 + yL) / 2.0f;
            z = 0.0f;
            getWindowCoord(x, y, z, wx, wy, wz);
            glColor3f(0.0f, 0.0f, 0.0f);
            render->renderText(wx, wy, QString("%1").arg(drL1, 0, 'f', 2), shrift);
        }




        if (i == vertexArray.size() / 3 - 4)
        {
            x1 = vertexArray[(i + 2) * 3];
            y1 = vertexArray[(i + 2) * 3 + 1];
            x2 = vertexArray[(i + 3) * 3];
            y2 = vertexArray[(i + 3) * 3 + 1];
            xR = vertexArrayRight[((i + 2) / 2 * 5 + 4) * 3];
            yR = vertexArrayRight[((i + 2) / 2 * 5 + 4) * 3 + 1];
            xL = vertexArrayLeft[((i + 2) / 2 * 5 + 4) * 3];
            yL = vertexArrayLeft[((i + 2) / 2 * 5 + 4) * 3 + 1];
            drR = sqrt((x1 - xR)*(x1 - xR) + (y1 - yR)*(y1 - yR));
            drL = sqrt((x2 - xL)*(x2 - xL) + (y2 - yL)*(y2 - yL));
            dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

            if (showLeftBoard)
            {
                x = (x2 + xL) / 2.0f;
                y = (y2 + yL) / 2.0f;
                z = 0.0f;
                getWindowCoord(x, y, z, wx, wy, wz);
                glColor3f(0.0f, 0.0f, 0.0f);
                render->renderText(wx, wy,QString("%1").arg(drL, 0, 'f', 2) + " Левый тротуар", shrift);
            }

            if (showRightBoard)
            {
                x = (x1 + xR) / 2.0f;
                y = (y1 + yR) / 2.0f;
                z = 0.0f;
                getWindowCoord(x, y, z, wx, wy, wz);
                glColor3f(0.0f, 0.0f, 0.0f);
                render->renderText(wx, wy, QString("%1").arg(drR, 0, 'f', 2) + " Правый тротуар", shrift);
            }


            x = (x1 + x2) / 2.0f;
            y = (y1 + y2) / 2.0f;
            z = 0.0f;
            getWindowCoord(x, y, z, wx, wy, wz);
            glColor3f(1.0f, 1.0f, 1.0f);
            render->renderText(wx, wy, QString("%1").arg(dr, 0, 'f', 2), shrift);
        }

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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setFixed(bool fixed)"
                                   << " fixed = " << fixed << "\n";
    this->fixed = fixed;
}

void RoadBroken::setRightBoardShowStatus(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightBoardShowStatus(bool status)"
                                   << " status = " << status << "\n";
    showRightBoard = status;
}

void RoadBroken::setLeftBoardShowStatus(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLeftBoardShowStatus(bool status)"
                                   << " status = " << status << "\n";
    showLeftBoard = status;
}

void RoadBroken::setRightBoardWidth(double width)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setRightBoardWidth(double width)"
                                   << " width = " << width << "\n";
    if (rightBoardWidth == width)
        return;
    rightBoardWidth = width;
    setRightVertexArray();
    setRightTextureArray(texture_2Usize,texture_2Vsize);
    emit rightBoardWidthChanged(width);
}

void RoadBroken::setLeftBoardWidth(double width)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::setLeftBoardWidth(double width)"
                                   << " width = " << width << "\n";
    if (leftBoardWidth == width)
        return;
    leftBoardWidth = width;
    setLeftVertexArray();
    setLeftTextureArray(texture_2Usize,texture_2Vsize);
    emit leftBoardWidthChanged(width);
}


int RoadBroken::getLayer()
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getLayer()\n";
    return layer;
}


std::vector<vec3> RoadBroken::getCoordOfControl(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::getCoordOfControl(int index)"
                                   << " index = " << index << "\n";
    std::vector<vec3> res;

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
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::clear()\n";
}


void RoadBroken::clearProperties(QLayout *layout)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::clearProperties(QLayout *layout)\n";
    disconnect(stepDialog, SIGNAL(lineTypeChanged(int)), this, SLOT(setLineType(int)));
    disconnect(stepDialog, SIGNAL(rightSideChanged(bool)), this, SLOT(setRightSide(bool)));
    disconnect(stepDialog, SIGNAL(stepChanged(double)), this, SLOT(setStep(double)));
    disconnect(stepDialog, SIGNAL(beginStepChanged(double)), this, SLOT(setBeginStep(double)));
    disconnect(stepDialog, SIGNAL(endStepChanged(double)), this, SLOT(setEndStep(double)));
    disconnect(stepDialog, SIGNAL(beginSideChanged(bool)), this, SLOT(setBeginSide(bool)));
    disconnect(stepDialog, SIGNAL(beginRoundingChanged(bool)), this, SLOT(setBeginRounding(bool)));
    disconnect(stepDialog, SIGNAL(endRoundingChanged(bool)), this, SLOT(setEndRounding(bool)));
    disconnect(stepDialog, SIGNAL(splitZoneWidthChanged(double)), this, SLOT(setSplitZoneWidth(double)));
    disconnect(stepDialog, SIGNAL(accepted()), this, SLOT(addLine()));
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


void RoadBroken::deleteLine(RoadElement *line)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadBroken::deleteLine(RoadElement *line)\n";
    int index = -1;
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].line == line)
        {
            index = i;
            break;
        }
    }
    if (index >=0)
        lines.remove(index);
}
