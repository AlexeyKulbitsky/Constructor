#include "roundingroad.h"
#include <QApplication>
#include "roundingcrossroad.h"

RoundingRoad::RoundingRoad()
{

}

RoundingRoad::RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                           float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius,
                           int numberOfSides, QString name, int layer)
{
    /*
    this->layer = layer;
    this->name = name;
    numberOfVertices = numberOfSides * 4;
    numberOfPolygones = numberOfSides * 2;

    vertexArray = new GLfloat[numberOfVertices * 3];
    colorArray = new GLfloat[numberOfVertices * 3];
    indexArray = new GLubyte[numberOfPolygones * 3];

    indexArrayForSelection = new GLubyte[numberOfVertices * 2];
    colorArrayForSelection = new GLfloat[numberOfVertices * 3];

    //vertexArrayNear.resize((numberOfSides * 4 + 4)*3);
    //vertexArrayFar.resize((numberOfSides * 4 + 4)*3);
    //textureArrayNear.resize((numberOfSides * 4 + 4)*2);
    //textureArrayFar.resize((numberOfSides * 4 + 4)*2);
    //indexArrayNear.resize((numberOfSides * 6)*3);
    //indexArrayFar.resize((numberOfSides * 6)*3);

    setVertexArray(x1, y1, nearRadius, angel1NearRadius, angel2NearRadius,
                   x2, y2, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    this->xCenterNearRadius = x1;
    this->yCenterNearRadius = y1;
    this->angel1NearRadius = angel1NearRadius;
    this->angel2NearRadius = angel2NearRadius;
    this->nearRadius = nearRadius;

    this->xCenterFarRadius = x2;
    this->yCenterFarRadius = y2;
    this->angel1FarRadius = angel1FarRadius;
    this->angel2FarRadius = angel2FarRadius;
    this->farRadius = farRadius;

    this->numberOfSides = numberOfSides;

    selected = false;
    fixed = false;
    */
}

RoundingRoad::RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius, float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius, int numberOfSides, QString name, int layer, QString texture_1, float texture_1Usize, float texture_1Vsize, QString texture_2, float texture_2Usize, float texture_2Vsize)
{
    this->texture_1Usize = texture_1Usize;
    this->texture_1Vsize = texture_1Vsize;
    this->texture_2Usize = texture_2Usize;
    this->texture_2Vsize = texture_2Vsize;
    showNearBoard = true;
    showFarBoard = true;
    nearBoardWidth = farBoardWidth = 2.75f;
    this->layer = layer;
    this->name = name;
    numberOfVertices = numberOfSides * 4;
    numberOfPolygones = numberOfSides * 2;
    this->numberOfSides = numberOfSides;


    indexArrayForSelection = new GLubyte[numberOfVertices * 2];
    colorArrayForSelection = new GLfloat[numberOfVertices * 3];

    //vertexArrayNear.resize((numberOfSides * 4 + 4)*3);
    //vertexArrayFar.resize((numberOfSides * 4 + 4)*3);
    //textureArrayNear.resize((numberOfSides * 4 + 4)*2);
    //textureArrayFar.resize((numberOfSides * 4 + 4)*2);
    //indexArrayNear.resize((numberOfSides * 6)*3);
    //indexArrayFar.resize((numberOfSides * 6)*3);

    setVertexArray(x1, y1, nearRadius, angel1NearRadius, angel2NearRadius,
                   x2, y2, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    //setColorArray(0.5f, 0.5f, 0.5f);
    setIndexArray();
    setTextureArray(texture_1Usize, texture_1Vsize);
    setNearTextureArray(texture_2Usize, texture_2Vsize);
    setFarTextureArray(texture_2Usize, texture_2Vsize);
    //textureID[0] = getTextures(texture_1);
    //textureID[1] = getTextures(texture_2);
    textureID[0] = TextureManager::getInstance()->getID(texture_1);
    textureID[1] = TextureManager::getInstance()->getID(texture_2);
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    this->xCenterNearRadius = x1;
    this->yCenterNearRadius = y1;
    this->angel1NearRadius = angel1NearRadius;
    this->angel2NearRadius = angel2NearRadius;
    this->nearRadius = nearRadius;

    this->xCenterFarRadius = x2;
    this->yCenterFarRadius = y2;
    this->angel1FarRadius = angel1FarRadius;
    this->angel2FarRadius = angel2FarRadius;
    this->farRadius = farRadius;

    this->numberOfSides = numberOfSides;

    selected = false;
    fixed = false;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoundingRoad::~RoundingRoad()
{

    delete []indexArrayForSelection;
    delete []colorArrayForSelection;



    indexArrayForSelection = NULL;
    colorArrayForSelection = NULL;

}

void RoundingRoad::setVertexArray(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                                  float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius, int numberOfSides)
{
    this->angel1NearRadius = angel1NearRadius;
    this->angel2NearRadius = angel2NearRadius;
    this->angel1FarRadius = angel1FarRadius;
    this->angel2FarRadius = angel2FarRadius;
    vertexArray.clear();
    vertexArrayFar.clear();
    vertexArrayNear.clear();
    for (int i = 0; i <= numberOfSides; ++i)
    {
        //float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
        float angle = (angel1NearRadius + (angel2NearRadius - angel1NearRadius) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
        float dx = nearRadius * cosf(angle);
        float dy = nearRadius * sinf(angle);
        vertexArray.push_back(x1 + dx);
        vertexArray.push_back(y1 + dy);
        vertexArray.push_back(0.0f);

        vertexArrayNear.push_back(x1 + dx);
        vertexArrayNear.push_back(y1 + dy);
        vertexArrayNear.push_back(0.0f);

        vertexArrayNear.push_back(x1 + dx);
        vertexArrayNear.push_back(y1 + dy);
        vertexArrayNear.push_back(0.08f);

        vertexArrayNear.push_back(x1 + dx - 0.03f * cosf(angle));
        vertexArrayNear.push_back(y1 + dy - 0.03f * sinf(angle));
        vertexArrayNear.push_back(0.1f);

        vertexArrayNear.push_back(x1 + dx - 0.25f * cosf(angle));
        vertexArrayNear.push_back(y1 + dy - 0.25f * sinf(angle));
        vertexArrayNear.push_back(0.1f);

        vertexArrayNear.push_back(x1 + dx - nearBoardWidth * cosf(angle));
        vertexArrayNear.push_back(y1 + dy - nearBoardWidth * sinf(angle));
        vertexArrayNear.push_back(0.1f);

        angle = (angel1FarRadius + (angel2FarRadius - angel1FarRadius) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
        dx = farRadius * cosf(angle);
        dy = farRadius * sinf(angle);
        vertexArray.push_back(x2 + dx);
        vertexArray.push_back(y2 + dy);
        vertexArray.push_back(0.0f);

        vertexArrayFar.push_back(x2 + dx);
        vertexArrayFar.push_back(y2 + dy);
        vertexArrayFar.push_back(0.0f);

        vertexArrayFar.push_back(x2 + dx);
        vertexArrayFar.push_back(y2 + dy);
        vertexArrayFar.push_back(0.08f);

        vertexArrayFar.push_back(x2 + dx + 0.03f * cosf(angle));
        vertexArrayFar.push_back(y2 + dy + 0.03f * sinf(angle));
        vertexArrayFar.push_back(0.1f);

        vertexArrayFar.push_back(x2 + dx + 0.25f * cosf(angle));
        vertexArrayFar.push_back(y2 + dy + 0.25f * sinf(angle));
        vertexArrayFar.push_back(0.1f);

        vertexArrayFar.push_back(x2 + dx + farBoardWidth * cosf(angle));
        vertexArrayFar.push_back(y2 + dy + farBoardWidth * sinf(angle));
        vertexArrayFar.push_back(0.1f);

        if (i != 0 && i != numberOfSides)
        {

            float angle = (angel1NearRadius + (angel2NearRadius - angel1NearRadius) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
            float dx = nearRadius * cosf(angle);
            float dy = nearRadius * sinf(angle);
            vertexArray.push_back(x1 + dx);
            vertexArray.push_back(y1 + dy);
            vertexArray.push_back(0.0f);

            vertexArrayNear.push_back(x1 + dx);
            vertexArrayNear.push_back(y1 + dy);
            vertexArrayNear.push_back(0.0f);

            vertexArrayNear.push_back(x1 + dx);
            vertexArrayNear.push_back(y1 + dy);
            vertexArrayNear.push_back(0.08f);

            vertexArrayNear.push_back(x1 + dx - 0.03f * cosf(angle));
            vertexArrayNear.push_back(y1 + dy - 0.03f * sinf(angle));
            vertexArrayNear.push_back(0.1f);

            vertexArrayNear.push_back(x1 + dx - 0.25f * cosf(angle));
            vertexArrayNear.push_back(y1 + dy - 0.25f * sinf(angle));
            vertexArrayNear.push_back(0.1f);

            vertexArrayNear.push_back(x1 + dx - nearBoardWidth * cosf(angle));
            vertexArrayNear.push_back(y1 + dy - nearBoardWidth * sinf(angle));
            vertexArrayNear.push_back(0.1f);

            angle = (angel1FarRadius + (angel2FarRadius - angel1FarRadius) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
            dx = farRadius * cosf(angle);
            dy = farRadius * sinf(angle);
            vertexArray.push_back(x2 + dx);
            vertexArray.push_back(y2 + dy);
            vertexArray.push_back(0.0f);

            vertexArrayFar.push_back(x2 + dx);
            vertexArrayFar.push_back(y2 + dy);
            vertexArrayFar.push_back(0.0f);

            vertexArrayFar.push_back(x2 + dx);
            vertexArrayFar.push_back(y2 + dy);
            vertexArrayFar.push_back(0.08f);

            vertexArrayFar.push_back(x2 + dx + 0.03f * cosf(angle));
            vertexArrayFar.push_back(y2 + dy + 0.03f * sinf(angle));
            vertexArrayFar.push_back(0.1f);

            vertexArrayFar.push_back(x2 + dx + 0.25f * cosf(angle));
            vertexArrayFar.push_back(y2 + dy + 0.25f * sinf(angle));
            vertexArrayFar.push_back(0.1f);

            vertexArrayFar.push_back(x2 + dx + farBoardWidth * cosf(angle));
            vertexArrayFar.push_back(y2 + dy + farBoardWidth * sinf(angle));
            vertexArrayFar.push_back(0.1f);

        }

    }
    this->angel1NearRadius = angel1NearRadius;
    this->angel2NearRadius = angel2NearRadius;
    this->angel1FarRadius = angel1FarRadius;
    this->angel2FarRadius = angel2FarRadius;
    //qDebug() << "Number of vertices: " << vertexArray.size() / 3;
}

void RoundingRoad::setColorArray(float red, float green, float blue)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArray.push_back(red);
        colorArray.push_back(green);
        colorArray.push_back(blue);
    }
}

void RoundingRoad::setIndexArray()
{
    int i;
    for (i = 0; i < vertexArray.size() / 3 - 2; i += 4)
    {
        indexArray.push_back(i);
        indexArray.push_back(i + 1);
        indexArray.push_back(i + 3);

        indexArray.push_back(i);
        indexArray.push_back(i + 3);
        indexArray.push_back(i + 2);
    }
    //qDebug() << "Number of faces: " << indexArray.size() / 3;
    for (int i = 0; i < vertexArrayNear.size() / 3 - 5; i += 10)
    {
        indexArrayNear.push_back(i + 1);
        indexArrayNear.push_back(i);
        indexArrayNear.push_back(i + 5);


        indexArrayNear.push_back(i + 1);
        indexArrayNear.push_back(i + 5);
        indexArrayNear.push_back(i + 6);

        indexArrayNear.push_back(i + 2);
        indexArrayNear.push_back(i + 1);
        indexArrayNear.push_back(i + 6);

        indexArrayNear.push_back(i + 2);
        indexArrayNear.push_back(i + 6);
        indexArrayNear.push_back(i + 7);

        indexArrayNear.push_back(i + 3);
        indexArrayNear.push_back(i + 2);
        indexArrayNear.push_back(i + 7);

        indexArrayNear.push_back(i + 3);
        indexArrayNear.push_back(i + 7);
        indexArrayNear.push_back(i + 8);

        indexArrayNear.push_back(i + 4);
        indexArrayNear.push_back(i + 3);
        indexArrayNear.push_back(i + 8);

        indexArrayNear.push_back(i + 4);
        indexArrayNear.push_back(i + 8);
        indexArrayNear.push_back(i + 9);

        //////////////////////////////////////

        indexArrayFar.push_back(i);
        indexArrayFar.push_back(i + 6);
        indexArrayFar.push_back(i + 5);

        indexArrayFar.push_back(i);
        indexArrayFar.push_back(i + 1);
        indexArrayFar.push_back(i + 6);

        indexArrayFar.push_back(i + 1);
        indexArrayFar.push_back(i + 7);
        indexArrayFar.push_back(i + 6);

        indexArrayFar.push_back(i + 1);
        indexArrayFar.push_back(i + 2);
        indexArrayFar.push_back(i + 7);

        indexArrayFar.push_back(i + 2);
        indexArrayFar.push_back(i + 8);
        indexArrayFar.push_back(i + 7);

        indexArrayFar.push_back(i + 2);
        indexArrayFar.push_back(i + 3);
        indexArrayFar.push_back(i + 8);

        indexArrayFar.push_back(i + 3);
        indexArrayFar.push_back(i + 9);
        indexArrayFar.push_back(i + 8);

        indexArrayFar.push_back(i + 3);
        indexArrayFar.push_back(i + 4);
        indexArrayFar.push_back(i + 9);
    }
    //qDebug() << "Number of NEAR faces: " << indexArrayNear.size() / 3;
    //qDebug() << "Number of FAR faces: " << indexArrayFar.size() / 3;

}

void RoundingRoad::setTextureArray(float textUsize, float textVsize)
{
    textureArray.clear();
    float x1, y1, x2, y2, r1 = 0.0f, r2 = 0.0f, r3;
    int i = 0;
    x1 = vertexArray[i * 3];
    y1 = vertexArray[i * 3 + 1];
    x2 = vertexArray[(i + 2) * 3];
    y2 = vertexArray[(i + 2) * 3 + 1];
    float r1Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));


    x1 = vertexArray[(i + 1) * 3];
    y1 = vertexArray[(i + 1) * 3 + 1];
    x2 = vertexArray[(i + 3) * 3];
    y2 = vertexArray[(i + 3) * 3 + 1];
    float r2Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));


    x1 = vertexArray[(i + 1) * 3];
    y1 = vertexArray[(i + 1) * 3 + 1];
    x2 = vertexArray[i * 3];
    y2 = vertexArray[i * 3 + 1];
    r3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    float uTemp = sqrt(r3 * r3 - ((r2Temp - r1Temp) / 2.0f) * ((r2Temp - r1Temp) / 2.0f));

    for (int j = 0; j < numberOfSides; ++j)
    {
        textureArray.push_back(uTemp / textUsize);
        textureArray.push_back(((r2 + r2Temp - r1 - r1Temp) / 2.0) / textVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back(r2 / textVsize);

        textureArray.push_back(uTemp / textUsize);
        textureArray.push_back(((r2 + r2Temp - r1 - r1Temp) / 2.0 + r1Temp) / textVsize);

        textureArray.push_back(0.0f);
        textureArray.push_back((r2 + r2Temp) / textVsize);

        r1 += r2Temp;
        r2 += r2Temp;
    }
    //qDebug() << "Number of texture vertices: " << textureArray.size()/2;


}

void RoundingRoad::setNearTextureArray(float textUsize, float textVsize)
{
    textureArrayNear.clear();
    float x1, y1, x2, y2;
    float r1 = 0.0f, r2 = 0.0f, r3 = 0.0f, r4 = 0.0f, r5 = 0.0f;
    float r1Temp, r2Temp, r3Temp, r4Temp, r5Temp;
    int i = 0;
    x1 = vertexArrayNear[i * 3];
    y1 = vertexArrayNear[i * 3 + 1];
    x2 = vertexArrayNear[(i + 5) * 3];
    y2 = vertexArrayNear[(i + 5) * 3 + 1];
    r1Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayNear[(i + 1) * 3];
    y1 = vertexArrayNear[(i + 1) * 3 + 1];
    x2 = vertexArrayNear[(i + 6) * 3];
    y2 = vertexArrayNear[(i + 6) * 3 + 1];
    r2Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayNear[(i + 2) * 3];
    y1 = vertexArrayNear[(i + 2) * 3 + 1];
    x2 = vertexArrayNear[(i + 7) * 3];
    y2 = vertexArrayNear[(i + 7) * 3 + 1];
    r3Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayNear[(i + 3) * 3];
    y1 = vertexArrayNear[(i + 3) * 3 + 1];
    x2 = vertexArrayNear[(i + 8) * 3];
    y2 = vertexArrayNear[(i + 8) * 3 + 1];
    r4Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayNear[(i + 4) * 3];
    y1 = vertexArrayNear[(i + 4) * 3 + 1];
    x2 = vertexArrayNear[(i + 9) * 3];
    y2 = vertexArrayNear[(i + 9) * 3 + 1];
    r5Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    ////////////////////////////////////////////

    x1 = vertexArrayNear[i * 3];
    y1 = vertexArrayNear[i * 3 + 1];
    x2 = vertexArrayNear[(i + 1) * 3];
    y2 = vertexArrayNear[(i + 1) * 3 + 1];
    float s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 1) * 3];
    y2 = vertexArrayNear[(i + 1) * 3 + 1];
    x2 = vertexArrayNear[(i + 2) * 3];
    y2 = vertexArrayNear[(i + 2) * 3 + 1];
    float s2 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 2) * 3];
    y2 = vertexArrayNear[(i + 2) * 3 + 1];
    x2 = vertexArrayNear[(i + 3) * 3];
    y2 = vertexArrayNear[(i + 3) * 3 + 1];
    float s3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 3) * 3];
    y2 = vertexArrayNear[(i + 3) * 3 + 1];
    x2 = vertexArrayNear[(i + 4) * 3];
    y2 = vertexArrayNear[(i + 4) * 3 + 1];
    float s4 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));

    for (int i = 0; i < numberOfSides; ++i)
    {
        textureArrayNear.push_back(0.0f);
        textureArrayNear.push_back(r1/textVsize);

        textureArrayNear.push_back(0.08f);
        textureArrayNear.push_back(r2/textVsize);

        textureArrayNear.push_back(0.09f);
        textureArrayNear.push_back((r3 + (r2Temp - r3Temp)/2.0f)/textVsize);

        textureArrayNear.push_back(0.16666f);
        textureArrayNear.push_back((r4 + (r2Temp - r4Temp)/2.0f)/textVsize);

        //textureArrayNear.push_back((s2 + s3 + s4)/textUsize);
        textureArrayNear.push_back(1.0f);
        textureArrayNear.push_back((r5 + (r2Temp - r5Temp)/2.0f)/textVsize);

        /////////////////////////////////////////////////////////

        textureArrayNear.push_back(0.0f);
        textureArrayNear.push_back((r1 + r1Temp)/textVsize);

        textureArrayNear.push_back(0.08f);
        textureArrayNear.push_back((r2 + r2Temp)/textVsize);

        textureArrayNear.push_back(0.09f);
        textureArrayNear.push_back((r3 + (r2Temp - r3Temp)/2.0f + r3Temp)/textVsize);

        textureArrayNear.push_back(0.16666f);
        textureArrayNear.push_back((r4 + (r2Temp - r4Temp)/2.0f + r4Temp)/textVsize);

        //textureArrayNear.push_back((s2 + s3 + s4)/textUsize);
        textureArrayNear.push_back(1.0f);
        textureArrayNear.push_back((r5 + (r2Temp - r5Temp)/2.0f + r5Temp)/textVsize);

        r1 += r1Temp;
        r2 += r2Temp;
        r3 += r1Temp;
        r4 += r1Temp;
        r5 += r1Temp;
    }


}

void RoundingRoad::setFarTextureArray(float textUsize, float textVsize)
{
    textureArrayFar.clear();
    float x1, y1, x2, y2;
    float r1 = 0.0f, r2 = 0.0f, r3 = 0.0f, r4 = 0.0f, r5 = 0.0f;
    float r1Temp, r2Temp, r3Temp, r4Temp, r5Temp;
    int i = 0;
    x1 = vertexArrayFar[i * 3];
    y1 = vertexArrayFar[i * 3 + 1];
    x2 = vertexArrayFar[(i + 5) * 3];
    y2 = vertexArrayFar[(i + 5) * 3 + 1];
    r1Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayFar[(i + 1) * 3];
    y1 = vertexArrayFar[(i + 1) * 3 + 1];
    x2 = vertexArrayFar[(i + 6) * 3];
    y2 = vertexArrayFar[(i + 6) * 3 + 1];
    r2Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayFar[(i + 2) * 3];
    y1 = vertexArrayFar[(i + 2) * 3 + 1];
    x2 = vertexArrayFar[(i + 7) * 3];
    y2 = vertexArrayFar[(i + 7) * 3 + 1];
    r3Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayFar[(i + 3) * 3];
    y1 = vertexArrayFar[(i + 3) * 3 + 1];
    x2 = vertexArrayFar[(i + 8) * 3];
    y2 = vertexArrayFar[(i + 8) * 3 + 1];
    r4Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayFar[(i + 4) * 3];
    y1 = vertexArrayFar[(i + 4) * 3 + 1];
    x2 = vertexArrayFar[(i + 9) * 3];
    y2 = vertexArrayFar[(i + 9) * 3 + 1];
    r5Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    ////////////////////////////////////////////

    x1 = vertexArrayFar[i * 3];
    y1 = vertexArrayFar[i * 3 + 1];
    x2 = vertexArrayFar[(i + 1) * 3];
    y2 = vertexArrayFar[(i + 1) * 3 + 1];
    float s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 1) * 3];
    y2 = vertexArrayFar[(i + 1) * 3 + 1];
    x2 = vertexArrayFar[(i + 2) * 3];
    y2 = vertexArrayFar[(i + 2) * 3 + 1];
    float s2 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 2) * 3];
    y2 = vertexArrayFar[(i + 2) * 3 + 1];
    x2 = vertexArrayFar[(i + 3) * 3];
    y2 = vertexArrayFar[(i + 3) * 3 + 1];
    float s3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 3) * 3];
    y2 = vertexArrayFar[(i + 3) * 3 + 1];
    x2 = vertexArrayFar[(i + 4) * 3];
    y2 = vertexArrayFar[(i + 4) * 3 + 1];
    float s4 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));

    for (int i = 0; i < numberOfSides; ++i)
    {
        textureArrayFar.push_back(0.0f);
        textureArrayFar.push_back((r1 + (r5Temp - r1Temp)/2.0f)/textVsize);

        textureArrayFar.push_back(0.08f);
        textureArrayFar.push_back((r2 + (r5Temp - r2Temp)/2.0f)/textVsize);

        textureArrayFar.push_back(0.09f);
        textureArrayFar.push_back((r3 + (r5Temp - r3Temp)/2.0f)/textVsize);

        textureArrayFar.push_back(0.16666f);
        textureArrayFar.push_back((r4 + (r5Temp - r4Temp)/2.0f)/textVsize);

        //textureArrayFar.push_back((s2 + s3 + s4)/textUsize);
        textureArrayFar.push_back(1.0f);
        textureArrayFar.push_back(r5/textVsize);

        /////////////////////////////////////////////////////////

        textureArrayFar.push_back(0.0f);
        textureArrayFar.push_back((r1 + (r5Temp - r1Temp)/2.0f + r1Temp)/textVsize);

        textureArrayFar.push_back(0.08f);
        textureArrayFar.push_back((r2 + (r5Temp - r2Temp)/2.0f + r2Temp)/textVsize);

        textureArrayFar.push_back(0.09f);
        textureArrayFar.push_back((r3 + (r5Temp - r3Temp)/2.0f + r3Temp)/textVsize);

        textureArrayFar.push_back(0.16666f);
        textureArrayFar.push_back((r4 + (r5Temp - r4Temp)/2.0f + r4Temp)/textVsize);

        //textureArrayFar.push_back((s2 + s3 + s4)/textUsize);
        textureArrayFar.push_back(1.0f);
        textureArrayFar.push_back((r5 + r5Temp)/textVsize);

        r1 += r5Temp;
        r2 += r5Temp;
        r3 += r5Temp;
        r4 += r5Temp;
        r5 += r5Temp;
    }
}

void RoundingRoad::drawFigure(QGLWidget* render)
{

    if (selected == true)
    {

        setColorArray(0.7f, 0.7f, 0.7f);
        drawSelectionFrame();
        if (render)
        {
            //drawMeasurements(render);
        }

    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(0.5f, 0.5f, 0.5f);

    }
    //qDebug() << "Texture coord: " << textureArray.size() / 2;
    //qDebug() << "Vertex coord: " << vertexArray.size() / 3;
    //qDebug() << "Index coord: " << indexArray.size() / 3;
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    //glColorPointer(3, GL_FLOAT, 0, colorArray);
    //glDrawElements(GL_TRIANGLES, numberOfPolygones * 3, GL_UNSIGNED_BYTE, indexArray);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());

    if (showNearBoard)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[1]);
        glVertexPointer(3, GL_FLOAT, 0, vertexArrayNear.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, textureArrayNear.begin());
        glDrawElements(GL_TRIANGLES, indexArrayNear.size(), GL_UNSIGNED_BYTE, indexArrayNear.begin());
    }

    if (showFarBoard)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[1]);
        glVertexPointer(3, GL_FLOAT, 0, vertexArrayFar.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, textureArrayFar.begin());
        glDrawElements(GL_TRIANGLES, indexArrayFar.size(), GL_UNSIGNED_BYTE, indexArrayFar.begin());
    }



    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->drawFigure();


    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);


}

void RoundingRoad::drawSelectionFrame()
{
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        //qDebug() << "Index " << indexOfSelectedControl;
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
    }
    // Боковые грани для изменения размера

    /*
    ///////////////////////////////////////////////////////
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glColorPointer(3, GL_FLOAT, 0, colorArrayForSelection);
    glLineWidth(2.0);
    glDrawElements(GL_LINES, numberOfVertices * 2, GL_UNSIGNED_BYTE, indexArrayForSelection);
    */
    for (int i = 0; i < 10; ++i)
    {
        drawControlElement(i, 2.0f, 5.0f);
    }

}

void RoundingRoad::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArrayForSelection[i * 3] = red;
        colorArrayForSelection[i * 3 + 1] = green;
        colorArrayForSelection[i * 3 + 2] = blue;
    }
}

void RoundingRoad::setIndexArrayForSelectionFrame()
{
    int count = 0;
    // Внутренний радиус
    int i = 0;
    while (i < numberOfVertices - 2)
    {
        indexArrayForSelection[count] = i;
        indexArrayForSelection[count + 1] = i + 2;
        i += 2;
        count += 2;
    }

    // Внешний радиус
    i = 1;
    while (i < numberOfVertices - 1)
    {
        indexArrayForSelection[count] = i;
        indexArrayForSelection[count + 1] = i + 2;
        i += 2;
        count += 2;
    }

    // Первая боковая линия
    i = 0;
    indexArrayForSelection[count] = i;
    indexArrayForSelection[count + 1] = i + 1;
    count += 2;

    // Вторая боковая линия
    i = numberOfVertices - 2;
    indexArrayForSelection[count] = i;
    indexArrayForSelection[count + 1] = i + 1;


}

GLuint RoundingRoad::getTextures(QString source)
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
    //qDebug() << "TEXTURE ID " << ID;
    return ID;
}

void RoundingRoad::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

void RoundingRoad::move(float dx, float dy, float dz)
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
    for (int i = 0; i < vertexArrayNear.size() / 3; ++i)
    {
        vertexArrayNear[i * 3] += dx;
        vertexArrayNear[i * 3 + 1] += dy;
    }
    for (int i = 0; i < vertexArrayFar.size() / 3; ++i)
    {
        vertexArrayFar[i * 3] += dx;
        vertexArrayFar[i * 3 + 1] += dy;
    }
    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->move(dx, dy);
    this->xCenterNearRadius += dx;
    this->yCenterNearRadius += dy;

    this->xCenterFarRadius += dx;
    this->yCenterFarRadius += dy;

}

void RoundingRoad::drawControlElement(int index, float lineWidth, float pointSize)
{
    switch (index)
    {



    case 0:
        // Правая угловая точка внутреннего радиуса
    {
        int i = 0;
        glPointSize(pointSize + 10.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 1:
        // Левая угловая точка внутреннего радиуса
    {
        int i = numberOfVertices - 2;
        glPointSize(pointSize + 10.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 2:
        // Правая угловая точка наружного радиуса
    {
        int i = 1;
        glPointSize(pointSize + 10.0f);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 3:
        // Левая угловая точка наружного радиуса
    {
        int i = numberOfVertices - 1;
        glPointSize(pointSize + 10.0f);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 4:
        // Дуга внутреннего радиуса

    {
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < vertexArray.size() / 3; i += 2)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
        }
        glEnd();

    }

        break;

    case 5:
        // Дуга наружного радиуса

    {
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 1; i < vertexArray.size() / 3; i += 2)
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
        }
        glEnd();
    }

        break;
    case 6:
        // Центр окружности внутреннего радиуса
    {
        int i = 0;
        glPointSize(pointSize + 5.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(xCenterNearRadius,
                   yCenterNearRadius,
                   vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 7:
        // Центр окружности наружного радиуса
    {
        int i = 0;
        glPointSize(pointSize + 5.0f);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(xCenterFarRadius,
                   yCenterFarRadius,
                   vertexArray[i * 3 + 2]);
        glEnd();
    }
        break;
    case 8:
    {
        if (showNearBoard)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            for (int i = 0; i < vertexArrayNear.size() / 3; i += 10)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayNear[(i + 4)*3], vertexArrayNear[(i + 4)*3 + 1], vertexArrayNear[(i + 4)*3 + 2]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayNear[(i + 9)*3], vertexArrayNear[(i + 9)*3 + 1], vertexArrayNear[(i + 9)*3 + 2]);
            }
            glEnd();
        }
    }
        break;
    case 9:
    {
        if (showFarBoard)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            for (int i = 0; i < vertexArrayFar.size() / 3; i += 10)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayFar[(i + 4)*3], vertexArrayFar[(i + 4)*3 + 1], vertexArrayFar[(i + 4)*3 + 2]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayFar[(i + 9)*3], vertexArrayFar[(i + 9)*3 + 1], vertexArrayFar[(i + 9)*3 + 2]);
            }
            glEnd();
        }
    }
        break;

    case 10:
    {
        int i = 0;
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(vertexArray[(i + 1) * 3],
                vertexArray[(i + 1) * 3 + 1],
                vertexArray[(i + 1) * 3 + 2]);

        glEnd();
    }
        break;
    case 11:
    {
        int i = numberOfVertices - 2;
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(vertexArray[(i + 1) * 3],
                vertexArray[(i + 1) * 3 + 1],
                vertexArray[(i + 1) * 3 + 2]);

        glEnd();
    }
        break;

    default:
    {
        /*
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[(index - 10) * 3],
                vertexArray[(index - 10) * 3 + 1],
                vertexArray[(index - 10) * 3] + 2);
        glEnd();
        */
    }
        break;
    }

}

QCursor RoundingRoad::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void RoundingRoad::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    int j;
    float x1, y1, x2, y2;
    float dr, dR;
    float X, Y;
    //X = xCenter - x;
    //X = yCenter - y;
    float factor;
    float res = (X * dx + Y * dy);
    factor = res < 0 ? 1 : -1;
    dr = sqrt(dx * dx + dy * dy);

    switch (index)
    {

    case 0:
    {
        int i = 0;
        float pi = 3.14159265f;
        float x1 = vertexArray[i * 3] + dx;
        float y1 = vertexArray[i * 3 + 1] + dy;
        float x2 = xCenterNearRadius;
        float y2 = yCenterNearRadius;
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * nearRadius));
        float res = dx2*dy1 - dx1*dy2;
        float factor = res > 0 ? 1.0f : -1.0f;
        angle = factor * angle * 180.0f / pi;
        setAngel_1_NearRadius(angel1NearRadius + angle);
    }
        break;
    case 1:
    {
        int i = numberOfVertices - 2;
        float pi = 3.14159265f;
        float x1 = vertexArray[i * 3] + dx;
        float y1 = vertexArray[i * 3 + 1] + dy;
        float x2 = xCenterNearRadius;
        float y2 = yCenterNearRadius;
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * nearRadius));
        float res = dx2*dy1 - dx1*dy2;
        float factor = res > 0 ? 1.0f : -1.0f;
        angle = factor * angle * 180.0f / pi;
        setAngel_2_NearRadius(angel2NearRadius + angle);
    }
        break;
    case 2:
    {
        int i = 1;
        float pi = 3.14159265f;
        float x1 = vertexArray[i * 3] + dx;
        float y1 = vertexArray[i * 3 + 1] + dy;
        float x2 = xCenterFarRadius;
        float y2 = yCenterFarRadius;
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * farRadius));
        float res = dx2*dy1 - dx1*dy2;
        float factor = res > 0 ? 1.0f : -1.0f;
        angle = factor * angle * 180.0f / pi;
        setAngel_1_FarRadius(angel1FarRadius + angle);
    }
        break;
    case 3:
    {
        int i = numberOfVertices - 1;
        float pi = 3.14159265f;
        float x1 = vertexArray[i * 3] + dx;
        float y1 = vertexArray[i * 3 + 1] + dy;
        float x2 = xCenterFarRadius;
        float y2 = yCenterFarRadius;
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * farRadius));
        float res = dx2*dy1 - dx1*dy2;
        float factor = res > 0 ? 1.0f : -1.0f;
        angle = factor * angle * 180.0f / pi;
        setAngel_2_FarRadius(angel2FarRadius + angle);
    }
        break;
    case 4:
        dr = ((x - xCenterNearRadius)*dx + (y - yCenterNearRadius)*dy)/
                sqrt((x - xCenterNearRadius)*(x - xCenterNearRadius) + (y - yCenterNearRadius)*(y - yCenterNearRadius));
        //this->nearRadius += factor * dr;
        this->nearRadius += dr;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        emit nearRadiusChanged(nearRadius);

        break;
    case 5:
        dr = ((x - xCenterFarRadius)*dx + (y - yCenterFarRadius)*dy)/
                sqrt((x - xCenterFarRadius)*(x - xCenterFarRadius) + (y - yCenterFarRadius)*(y - yCenterFarRadius));
        //this->nearRadius += factor * dr;
        this->farRadius += dr;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        emit farRadiusChanged(farRadius);

        break;
    case 6:
        xCenterNearRadius += dx;
        yCenterNearRadius += dy;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit nearRadiusChanged(nearRadius);
        break;
    case 7:
        xCenterFarRadius += dx;
        yCenterFarRadius += dy;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit farRadiusChanged(farRadius);
        break;
    case 8:
        dr = ((x - xCenterNearRadius)*dx + (y - yCenterNearRadius)*dy)/
                sqrt((x - xCenterNearRadius)*(x - xCenterNearRadius) + (y - yCenterNearRadius)*(y - yCenterNearRadius));
        nearBoardWidth -= dr;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        emit nearBoardWidthChanged(nearBoardWidth);
        break;
    case 9:
        dr = ((x - xCenterFarRadius)*dx + (y - yCenterFarRadius)*dy)/
                sqrt((x - xCenterFarRadius)*(x - xCenterFarRadius) + (y - yCenterFarRadius)*(y - yCenterFarRadius));
        farBoardWidth += dr;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        emit farBoardWidthChanged(farBoardWidth);
        break;

    case 10:
    {
        int i = 0;
        float x1 = vertexArray[i * 3];
        float y1 = vertexArray[i * 3 + 1];
        float x2 = xCenterNearRadius;
        float y2 = yCenterNearRadius;
        float x3 = x1 + dx;
        float y3 = y1 + dy;
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = nearRadius;
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float pi = 3.14159265f;
        float angle1 = angel1NearRadius;
        float angle2 = acos(dx2 / r2);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        float angle = angle2 - angle1;
        angle = angle * 180.0f / pi;
        setAngel_1_NearRadius(angel1NearRadius + angle);

        i = 1;
        x1 = vertexArray[i * 3];
        y1 = vertexArray[i * 3 + 1];
        x2 = xCenterFarRadius;
        y2 = yCenterFarRadius;
        x3 = y1 + dx;
        y3 = y1 + dy;
        dx1 = x1 - x2;
        dy1 = y1 - y2;
        r1 = farRadius;
        dx2 = x3 - x2;
        dy2 = y3 - y2;
        r2 = sqrt(dx2*dx2 + dy2*dy2);
        angle1 = angel1FarRadius;
        angle2 = acos(dx2 / r2);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        angle = angle2 - angle1;
        angle = angle * 180.0f / pi;
        setAngel_1_FarRadius(angel1FarRadius + angle);

    }
        break;
    case 11:
    {
        int i = numberOfVertices - 2;
        float pi = 3.14159265f;
        float x1 = x;
        float y1 = y;
        float x2 = xCenterNearRadius;
        float y2 = yCenterNearRadius;
        float x3 = x + dx;
        float y3 = y + dy;
        float dx1 = x1 - x2;
        float dy1 = y1 - y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float angle1 = acos(dx1 / r1);
        if (dy1 < 0)
            angle1 = 2.0f * pi - angle1;
        float angle2 = acos(dx2 / r2);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        float angle = (angle2 - angle1) * 180.0f / pi;
        qDebug() << "Near angle" << angle;
        setAngel_2_NearRadius(angel2NearRadius + angle);


        i = numberOfVertices - 1;
        x2 = xCenterFarRadius;
        y2 = yCenterFarRadius;
        dx1 = x1 - x2;
        dy1 = y1 - y2;
        r1 = sqrt(dx1*dx1 + dy1*dy1);
        dx2 = x3 - x2;
        dy2 = y3 - y2;
        r2 = sqrt(dx2*dx2 + dy2*dy2);
        angle1 = acos(dx1 / r1);
        if (dy1 < 0)
            angle1 = 2.0f * pi - angle1;
        angle2 = acos(dx2 / r2);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        angle = (angle2 - angle1) * 180.0f / pi;
        qDebug() << "Far angle" << angle;
        setAngel_2_FarRadius(angel2FarRadius + angle);
    }
        break;

    default:
        break;
    }

}

int RoundingRoad::getNumberOfControls()
{
    return vertexArray.size() / 3 + 10 + 2;
}


void RoundingRoad::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}


QJsonObject RoundingRoad::getJSONInfo()
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

    element["xCenterNearRadius"] = xCenterNearRadius;
    element["yCenterNearRadius"] = yCenterNearRadius;
    element["Angel1NearRadius"] = angel1NearRadius;
    element["Angel2NearRadius"] = angel2NearRadius;
    element["NearRadius"] = nearRadius;

    element["xCenterFarRadius"] = xCenterFarRadius;
    element["yCenterFarRadius"] = yCenterFarRadius;
    element["Angel1FarRadius"] = angel1FarRadius;
    element["Angel2FarRadius"] = angel2FarRadius;
    element["FarRadius"] = farRadius;

    element["NumberOfSides"] = numberOfSides;

    return element;
}


void RoundingRoad::getProperties(QFormLayout *layout, QGLWidget* render)
{
    this->layout = layout;
    this->render = render;
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    QDoubleSpinBox* nearRadiusSpinBox = new QDoubleSpinBox();
    nearRadiusSpinBox->setMaximum(10000.0);
    QDoubleSpinBox* farRadiusSpinBox = new QDoubleSpinBox();
    farRadiusSpinBox->setMaximum(10000.0);

    QDoubleSpinBox* angel_1_NearRadiusSpinBox = new QDoubleSpinBox();
    angel_1_NearRadiusSpinBox->setMaximum(10000.0);
    angel_1_NearRadiusSpinBox->setMinimum(-10000.0);
    QDoubleSpinBox* angel_2_NearRadiusSpinBox = new QDoubleSpinBox();
    angel_2_NearRadiusSpinBox->setMaximum(10000.0);
    angel_1_NearRadiusSpinBox->setMinimum(-10000.0);

    QDoubleSpinBox* angel_1_FarRadiusSpinBox = new QDoubleSpinBox();
    angel_1_FarRadiusSpinBox->setMaximum(10000.0);
    angel_1_FarRadiusSpinBox->setMinimum(-10000.0);
    QDoubleSpinBox* angel_2_FarRadiusSpinBox = new QDoubleSpinBox();
    angel_2_FarRadiusSpinBox->setMaximum(10000.0);
    angel_2_FarRadiusSpinBox->setMinimum(-10000.0);


    nearRadiusSpinBox->setValue(nearRadius);
    connect(this, SIGNAL(nearRadiusChanged(double)), nearRadiusSpinBox, SLOT(setValue(double)));
    connect(nearRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNearRadius(double)));

    farRadiusSpinBox->setValue(farRadius);
    connect(this, SIGNAL(farRadiusChanged(double)), farRadiusSpinBox, SLOT(setValue(double)));
    connect(farRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setFarRadius(double)));

    angel_1_NearRadiusSpinBox->setValue(angel1NearRadius);
    connect(this, SIGNAL(angel_1_NearRadiusChanged(double)), angel_1_NearRadiusSpinBox, SLOT(setValue(double)));
    connect(angel_1_NearRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngel_1_NearRadius(double)));

    angel_2_NearRadiusSpinBox->setValue(angel2NearRadius);
    connect(this, SIGNAL(angel_2_NearRadiusChanged(double)), angel_2_NearRadiusSpinBox, SLOT(setValue(double)));
    connect(angel_2_NearRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngel_2_NearRadius(double)));

    angel_1_FarRadiusSpinBox->setValue(angel1FarRadius);
    connect(this, SIGNAL(angel_1_FarRadiusChanged(double)), angel_1_FarRadiusSpinBox, SLOT(setValue(double)));
    connect(angel_1_FarRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngel_1_FarRadius(double)));

    angel_2_FarRadiusSpinBox->setValue(angel2FarRadius);
    connect(this, SIGNAL(angel_2_FarRadiusChanged(double)), angel_2_FarRadiusSpinBox, SLOT(setValue(double)));
    connect(angel_2_FarRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngel_2_FarRadius(double)));

    QCheckBox* fixedCheckBox = new QCheckBox();

    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    QCheckBox* showNearBoardCheckBox = new QCheckBox();
    QCheckBox* showFarBoardCheckBox = new QCheckBox();

    showNearBoardCheckBox->setChecked(showNearBoard);
    QObject::connect(showNearBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowNearBoard(bool)));

    showFarBoardCheckBox->setChecked(showFarBoard);
    QObject::connect(showFarBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowFarBoard(bool)));

    QDoubleSpinBox* nearBoardWidthSpinBox = new QDoubleSpinBox();
    nearBoardWidthSpinBox->setMinimum(0.0f);
    nearBoardWidthSpinBox->setValue(nearBoardWidth);

    connect(this, SIGNAL(nearBoardWidthChanged(double)), nearBoardWidthSpinBox, SLOT(setValue(double)));
    connect(nearBoardWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNearBoardWidth(double)));

    QDoubleSpinBox* farBoardWidthSpinBox = new QDoubleSpinBox();
    farBoardWidthSpinBox->setMinimum(0.0f);
    farBoardWidthSpinBox->setValue(farBoardWidth);

    connect(this, SIGNAL(farBoardWidthChanged(double)), farBoardWidthSpinBox, SLOT(setValue(double)));
    connect(farBoardWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setFarBoardWidth(double)));

    if (render)
    {
        connect(nearRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(farRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(angel_1_NearRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(angel_2_NearRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(angel_1_FarRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(angel_2_FarRadiusSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(showNearBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        connect(showFarBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        connect(nearBoardWidthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(farBoardWidthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    QPushButton *addLineButton = new QPushButton("+");

    connect(stepDialog, SIGNAL(lineTypeChanged(int)), this, SLOT(setLineType(int)));
    connect(stepDialog, SIGNAL(rightSideChanged(bool)), this, SLOT(setNearSide(bool)));
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


    layout->addRow("Дуга:", new QLabel("внутренняя"));
    layout->addRow("Радиус", nearRadiusSpinBox);
    layout->addRow("Угол 1", angel_1_NearRadiusSpinBox);
    layout->addRow("Угол 2", angel_2_NearRadiusSpinBox);

    layout->addRow("Дуга:", new QLabel("наружная"));
    layout->addRow("Радиус", farRadiusSpinBox);
    layout->addRow("Угол 1", angel_1_FarRadiusSpinBox);
    layout->addRow("Угол 2", angel_2_FarRadiusSpinBox);

    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Ближний тротуар",showNearBoardCheckBox);
    layout->addRow("Ширина", nearBoardWidthSpinBox);
    layout->addRow("Дальний тротуар",showFarBoardCheckBox);
    layout->addRow("Ширина", farBoardWidthSpinBox);
    for (int i = 0; i < lines.size(); ++i)
    {
        QPushButton* b = new QPushButton(QString::number(i + 1));
        connect(b, SIGNAL(clicked(bool)), this, SLOT(deleteLine()));
        layout->addRow("Удалить линию ",b);
    }


    layout->addRow("Добавить линию", addLineButton);
}

void RoundingRoad::setNearRadius(double nearRadius)
{
    if (this->nearRadius != nearRadius)
    {
        this->nearRadius = nearRadius;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit nearRadiusChanged(nearRadius);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_1_NearRadius(double angel1)
{
    if (angel1NearRadius != angel1)
    {
        angel1NearRadius = angel1;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit angel_1_NearRadiusChanged(angel1);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_2_NearRadius(double angel2)
{
    if (angel2NearRadius != angel2)
    {
        angel2NearRadius = angel2;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit angel_2_NearRadiusChanged(angel2);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setFarRadius(double farRadius)
{
    if (this->farRadius != farRadius)
    {
        this->farRadius = farRadius;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit farRadiusChanged(farRadius);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_1_FarRadius(double angel1)
{
    if (angel1FarRadius != angel1)
    {
        angel1FarRadius = angel1;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit angel_1_FarRadiusChanged(angel1);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_2_FarRadius(double angel2)
{
    if (angel2FarRadius != angel2)
    {
        angel2FarRadius = angel2;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        resetLines();
        emit angel_2_FarRadiusChanged(angel2);
    }
    else
    {
        return;
    }
}

void RoundingRoad::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool nearSide)
{
    float angel_1, angel_2;
    float x, y, radius;
    float factor;
    if (nearSide)
    {
        angel_1 = angel1NearRadius;
        angel_2 = angel2NearRadius;
        x = xCenterNearRadius;
        y = yCenterNearRadius;
        radius = nearRadius;
        factor = 1.0f;
    }
    else
    {
        angel_1 = angel1FarRadius;
        angel_2 = angel2FarRadius;
        x = xCenterFarRadius;
        y = yCenterFarRadius;
        radius = farRadius;
        factor = -1.0f;
    }
    radius += factor * step;

    QVector<float> vertices;
    bool beginStepReleased = false, endStepReleased = false;
    for (int i = 0; i <= numberOfSides; ++i)
    {
        //float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
        float angle = (angel_1 + (angel_2 - angel_1) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        float dr;

        if (!beginStepReleased)
        {
            if ((angle - angel_1 * 3.1415926 / 180.0f)*radius > beginStep)
            {
                dx = radius * cosf(angel_1 * 3.1415926 / 180.0f + beginStep / radius);
                dy = radius * sinf(angel_1 * 3.1415926 / 180.0f + beginStep / radius);
                vertices.push_back(x + dx);
                vertices.push_back(y + dy);
                vertices.push_back(0.001f);
                beginStepReleased = true;
                qDebug() << "Begin step...";
            }
        }
        else
            if ((angel_2 * 3.1415926 / 180.0f - angle)*radius > endStep)
            {
                vertices.push_back(x + dx);
                vertices.push_back(y + dy);
                vertices.push_back(0.001f);
                qDebug() << "Middle part...";
            }

            else
            {
                dx = radius * cosf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                dy = radius * sinf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                vertices.push_back(x + dx);
                vertices.push_back(y + dy);
                vertices.push_back(0.001f);
                qDebug() << "End step...";
                break;
            }

    }
    int size = vertices.size();
    float *lineVertexArray = new float[size];
    for (int i = 0; i < vertices.size(); ++i)
        lineVertexArray[i] = vertices[i];

    LineBrokenLinked line;

    if (lineType == 6)
    {
        line.line = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                  QString("Линия №") + QString::number(lines.size() + 1));
        line.splitZoneWidth = splitZoneWidth;
    }
    else
    {
        line.line = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1,
                                   QString("Линия №") + QString::number(lines.size() + 1));
    }

    //line.line = new LineBroken(lineWidth, lineVertexArray, size, textureSource, textureSize, "LineBroken", 1);
    line.lineWidth = lineWidth;
    line.step = step;
    line.nearSide = nearSide;
    line.lineType = lineType;
    line.beginStep = beginStep;
    line.endStep = endStep;
    line.beginSide = beginSide;
    line.differentDirections = differentDirections;
    line.line->setSelectedStatus(false);
    lines.push_back(line);
    delete[] lineVertexArray;
    lineVertexArray = NULL;
    emit linesChanged(layout, render);
}

void RoundingRoad::addLine()
{
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
    default:
        break;

    }

    addLine(step, textSource, 6.0f, lWidth, lineType, nearSide);
}



void RoundingRoad::setNearSide(bool status)
{
    this->nearSide = status;
}

void RoundingRoad::setStep(double value)
{
    this->step = value;
}

void RoundingRoad::setLineType(int type)
{
    this->lineType = type;
}

void RoundingRoad::deleteLine()
{
    QPushButton * b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    //qDebug() << "delete line " << b->text();
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

void RoundingRoad::resetLines()
{
    float angel_1, angel_2;
    float x, y, radius;
    float factor;

    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].nearSide)
        {
            angel_1 = angel1NearRadius;
            angel_2 = angel2NearRadius;
            x = xCenterNearRadius;
            y = yCenterNearRadius;
            radius = nearRadius;
            factor = 1.0f;
        }
        else
        {
            angel_1 = angel1FarRadius;
            angel_2 = angel2FarRadius;
            x = xCenterFarRadius;
            y = yCenterFarRadius;
            radius = farRadius;
            factor = -1.0f;
        }
        radius += factor * lines[i].step;
        int size = (numberOfSides + 1) * 3;
        float *lineVertexArray = new float[size];

        for (int i = 0; i <= numberOfSides; ++i)
        {
            //float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
            float angle = (angel_1 + (angel_2 - angel_1) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
            float dx = radius * cosf(angle);
            float dy = radius * sinf(angle);
            lineVertexArray[i * 3] = x + dx;
            lineVertexArray[i * 3 + 1] = y + dy;
            lineVertexArray[i * 3 + 2] = 0.001f;
        }
        if (lines[i].lineType == 6)
        {
            SplitZone *splitZone = dynamic_cast<SplitZone*>(lines[i].line);
            splitZone->calculateLine(lineVertexArray, size, 1.0f);
            //lines[i].line = new SplitZone(lineVertexArray, size, 1.0f, true, true);
        }
        else
        {
            LineBroken *lineBroken = dynamic_cast<LineBroken*>(lines[i].line);
            lineBroken->setVertexArrayForAxis(lineVertexArray, size);
            lineBroken->setVertexArray(lines[i].lineWidth,lineVertexArray, size);
            lineBroken->setTextureArray();
        }

        delete[] lineVertexArray;
        lineVertexArray = NULL;
    }

    //emit linesChanged(layout, render);
}

void RoundingRoad::setBeginStep(double step)
{
    beginStep = step;
    qDebug() << "RoundingRoad::beginStep = " << beginStep;
}

void RoundingRoad::setEndStep(double step)
{
    endStep = step;
    qDebug() << "RoundingRoad::endStep = " << endStep;
}

void RoundingRoad::setSplitZoneWidth(double value)
{
    splitZoneWidth = value;
}

void RoundingRoad::setBeginSide(bool status)
{
    beginSide = status;
}

void RoundingRoad::setBeginRounding(bool status)
{
    beginRounding = status;
}

void RoundingRoad::setEndRounding(bool status)
{
    endRounding = status;
}

void RoundingRoad::setDifferentDirections(bool status)
{
    differentDirections = status;
}

void RoundingRoad::setShowNearBoard(bool status)
{
    showNearBoard = status;
}

void RoundingRoad::setShowFarBoard(bool status)
{
    showFarBoard = status;
}

void RoundingRoad::setNearBoardWidth(double width)
{
    if (nearBoardWidth == width)
        return;
    nearBoardWidth = width;
    setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                   xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    setNearTextureArray(texture_2Usize,texture_2Vsize);
    emit nearBoardWidthChanged(width);
}

void RoundingRoad::setFarBoardWidth(double width)
{
    if (farBoardWidth == width)
        return;
    farBoardWidth = width;
    setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                   xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    setFarTextureArray(texture_2Usize,texture_2Vsize);
    emit farBoardWidthChanged(width);
}






bool RoundingRoad::isFixed()
{
    return fixed;
}


void RoundingRoad::drawMeasurements(QGLWidget *render)
{
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Black);

    // renderText (ptrMousePosition.x(), ptrMousePosition.y(), "HELLO", shrift);
    if (render)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0FFF);
        int i = numberOfVertices;
        glLineWidth(1.5f);
        x = (vertexArray[(i - 2) * 3] + xCenterNearRadius) / 2.0f;
        y = (vertexArray[(i - 2) * 3 + 1] + yCenterNearRadius) / 2.0f;
        z = (vertexArray[(i - 2) * 3 + 2] + 0.3f) / 2.0f;
        getWindowCoord(x, y, z, wx, wy, wz);

        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(xCenterNearRadius, yCenterNearRadius, 0.3f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[(i - 2) * 3],
                vertexArray[(i - 2) * 3 + 1],
                0.3f);
        glEnd();
        render->renderText(wx, wy, QString("%1").arg(nearRadius), shrift);



        x = (vertexArray[3] + xCenterFarRadius) / 2.0f;
        y = (vertexArray[4] + yCenterFarRadius) / 2.0f;
        z = (vertexArray[5] + 0.3f) / 2.0f;
        getWindowCoord(x, y, z, wx, wy, wz);

        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(xCenterFarRadius, yCenterFarRadius, 0.3f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(vertexArray[3],
                vertexArray[4],
                0.3f);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        render->renderText(wx, wy, QString("%1").arg(farRadius), shrift);


        if (showNearBoard)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            x = (vertexArrayNear[0] + vertexArrayNear[12]) / 2.0f;
            y = (vertexArrayNear[1] + vertexArrayNear[13]) / 2.0f;
            z = (vertexArrayNear[2] + vertexArrayNear[14]) / 2.0f;
            getWindowCoord(x, y, z, wx, wy, wz);

            render->renderText(wx, wy, QString("%1").arg(nearBoardWidth), shrift);
        }

        if (showFarBoard)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            x = (vertexArrayFar[0] + vertexArrayFar[12]) / 2.0f;
            y = (vertexArrayFar[1] + vertexArrayFar[13]) / 2.0f;
            z = (vertexArrayFar[2] + vertexArrayFar[14]) / 2.0f;
            getWindowCoord(x, y, z, wx, wy, wz);

            render->renderText(wx, wy, QString("%1").arg(farBoardWidth), shrift);
        }
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
    else
    {
        return;
    }
}


bool RoundingRoad::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int RoundingRoad::getLayer()
{
    return layer;
}


void RoundingRoad::clear()
{
}


std::vector<vec3> RoundingRoad::getCoordOfControl(int index)
{
    std::vector<vec3> res;

    switch (index)
    {

    case 0:
        // Дуга внутреннего радиуса
        /*
    {
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < vertexArray.size() / 3; i += 2)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(vertexArray[i * 3],
                        vertexArray[i * 3 + 1],
                        vertexArray[i * 3 + 2]);
        }
        glEnd();

    }
    */
        break;

    case 1:
        // Дуга наружного радиуса
        /*
    {
        glLineWidth(lineWidth + 5.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 1; i < vertexArray.size() / 3; i += 2)
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(vertexArray[i * 3],
                        vertexArray[i * 3 + 1],
                        vertexArray[i * 3 + 2]);
        }
        glEnd();
    }
    */
        break;

    case 2:
        // Правая угловая точка внутреннего радиуса
    {
        int i = 0;
        vec3 p(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 3:
        // Левая угловая точка внутреннего радиуса
    {
        int i = numberOfVertices - 2;
        vec3 p(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 4:
        // Правая угловая точка наружного радиуса
    {
        int i = 1;
        vec3 p(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 5:
        // Левая угловая точка наружного радиуса
    {
        int i = numberOfVertices - 1;
        vec3 p(vertexArray[i * 3],
                vertexArray[i * 3 + 1],
                vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 6:
        // Центр окружности внутреннего радиуса
    {
        int i = 0;
        vec3 p(xCenterNearRadius,
               yCenterNearRadius,
               vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 7:
        // Центр окружности наружного радиуса
    {
        int i = 0;
        vec3 p(xCenterFarRadius,
               yCenterFarRadius,
               vertexArray[i * 3 + 2]);
        res.push_back(p);
    }
        break;
    case 8:
    {
        /*
        if (showNearBoard)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            for (int i = 0; i < vertexArrayNear.size() / 3; i += 10)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayNear[(i + 4)*3], vertexArrayNear[(i + 4)*3 + 1], vertexArrayNear[(i + 4)*3 + 2]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayNear[(i + 9)*3], vertexArrayNear[(i + 9)*3 + 1], vertexArrayNear[(i + 9)*3 + 2]);
            }
            glEnd();
        }
        */
    }
        break;
    case 9:
    {
        /*
        if (showFarBoard)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            for (int i = 0; i < vertexArrayFar.size() / 3; i += 10)
            {
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayFar[(i + 4)*3], vertexArrayFar[(i + 4)*3 + 1], vertexArrayFar[(i + 4)*3 + 2]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayFar[(i + 9)*3], vertexArrayFar[(i + 9)*3 + 1], vertexArrayFar[(i + 9)*3 + 2]);
            }
            glEnd();
        }
        */
    }
        break;
    default:
    {
        vec3 p(vertexArray[(index - 10) * 3],
                vertexArray[(index - 10) * 3 + 1],
                vertexArray[(index - 10) * 3] + 2);
        res.push_back(p);
    }
        break;
    }
    return res;
}


void RoundingRoad::clearProperties(QLayout *layout)
{

    disconnect(stepDialog, 0, this, 0);
}
