#include "roundingroad.h"
#include <QApplication>
#include "model.h"

bool RoundingRoad::log = true;

RoundingRoad::RoundingRoad()
{

}

RoundingRoad::RoundingRoad(float, float, float, float, float,
                           float, float, float, float, float,
                           int, QString, int)
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
    this->texture1 = texture_1;
    this->texture2 = texture_2;
    showNearBoard = true;
    showFarBoard = true;
    nearBoardWidth = farBoardWidth = 2.75f;
    this->layer = layer;
    this->name = name;
    this->numberOfSides = numberOfSides;
    numberOfVertices = numberOfSides * 4;
    numberOfPolygones = numberOfSides * 2;



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
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
    textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
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
    this->indexOfSelectedControl = -1;
    layout = NULL;
    render = NULL;
    selected = false;
    fixed = false;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoundingRoad::RoundingRoad(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius, float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius, int numberOfSides, QString name, int layer, float nearBoardWidth, float farBoardWidth, bool showNearBoard, bool showFarBoard, bool fixed, QString texture_1, float texture_1Usize, float texture_1Vsize, QString texture_2, float texture_2Usize, float texture_2Vsize)
{
    this->texture_1Usize = texture_1Usize;
    this->texture_1Vsize = texture_1Vsize;
    this->texture_2Usize = texture_2Usize;
    this->texture_2Vsize = texture_2Vsize;
    this->texture1 = texture_1;
    this->texture2 = texture_2;
    this->showNearBoard = showNearBoard;
    this->showFarBoard = showFarBoard;
    this->nearBoardWidth = nearBoardWidth;
    this->farBoardWidth = farBoardWidth;
    this->layer = layer;
    this->name = name;
    this->numberOfSides = numberOfSides;
    numberOfVertices = numberOfSides * 4;
    numberOfPolygones = numberOfSides * 2;

    indexArrayForSelection = new GLubyte[numberOfVertices * 2];
    colorArrayForSelection = new GLfloat[numberOfVertices * 3];

    setVertexArray(x1, y1, nearRadius, angel1NearRadius, angel2NearRadius,
                   x2, y2, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    setIndexArray();
    setTextureArray(texture_1Usize, texture_1Vsize);
    setNearTextureArray(texture_2Usize, texture_2Vsize);
    setFarTextureArray(texture_2Usize, texture_2Vsize);
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
    textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
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

    this->indexOfSelectedControl = -1;
    selected = false;
    this->fixed = fixed;
    layout = NULL;
    render = NULL;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoundingRoad::RoundingRoad(const RoundingRoad &source)
{
    this->texture_1Usize = source.texture_1Usize;
    this->texture_1Vsize = source.texture_1Vsize;
    this->texture_2Usize = source.texture_2Usize;
    this->texture_2Vsize = source.texture_2Vsize;
    this->showNearBoard = source.showNearBoard;
    this->showFarBoard = source.showFarBoard;
    this->nearBoardWidth = source.nearBoardWidth;
    this->farBoardWidth = source.farBoardWidth;
    this->layer = source.layer;
    this->name = source.name;
    this->numberOfVertices = source.numberOfVertices;
    this->numberOfPolygones = source.numberOfPolygones;
    this->numberOfSides = source.numberOfSides;
    this->xCenterNearRadius = source.xCenterNearRadius;
    this->yCenterNearRadius = source.yCenterNearRadius;
    this->angel1NearRadius = source.angel1NearRadius;
    this->angel2NearRadius = source.angel2NearRadius;
    this->nearRadius = source.nearRadius;

    this->xCenterFarRadius = source.xCenterFarRadius;
    this->yCenterFarRadius = source.yCenterFarRadius;
    this->angel1FarRadius = source.angel1FarRadius;
    this->angel2FarRadius = source.angel2FarRadius;
    this->farRadius = source.farRadius;

    this->numberOfSides = source.numberOfSides;
    this->indexOfSelectedControl = source.indexOfSelectedControl;
    this->selected = source.selected;
    this->fixed = source.fixed;
    this->texture1 = source.texture1;
    this->texture2 = source.texture2;
    this->textureID[0] = source.textureID[0];
    this->textureID[1] = source.textureID[1];
    layout = source.layout;
    render = source.render;
    indexArrayForSelection = new GLubyte[numberOfVertices * 2];
    colorArrayForSelection = new GLfloat[numberOfVertices * 3];


    setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                   xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                   numberOfSides);
    setIndexArray();
    setTextureArray(texture_1Usize, texture_1Vsize);
    setNearTextureArray(texture_2Usize, texture_2Vsize);
    setFarTextureArray(texture_2Usize, texture_2Vsize);
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    lines.resize(source.lines.size());
    for (int i = 0; i < source.lines.size(); ++i)
    {
        lines[i].line = source.lines[i].line->getCopy();
        lines[i].lineType = source.lines[i].lineType;
        lines[i].lineWidth = source.lines[i].lineWidth;
        lines[i].nearSide = source.lines[i].nearSide;
        lines[i].beginSide = source.lines[i].beginSide;
        lines[i].step = source.lines[i].step;
        lines[i].beginStep = source.lines[i].beginStep;
        lines[i].endStep = source.lines[i].endStep;
        lines[i].differentDirections = source.lines[i].differentDirections;
        lines[i].splitZoneWidth = source.lines[i].splitZoneWidth;
    }

    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoundingRoad::~RoundingRoad()
{

    for (int i = 0; i < lines.size(); ++i)
    {
//        for (QList<RoadElement*>::iterator it = model->getGroup(1).begin();
//             it != model->getGroup(1).end(); ++it)
//        {
//            if (lines[i].line == (*it))
//            {
//                model->getGroup(1).erase(it);
//                break;
//            }
//        }
        delete lines[i].line;
    }
    lines.clear();

    if (indexArrayForSelection)
        delete []indexArrayForSelection;
    if (colorArrayForSelection)
        delete []colorArrayForSelection;
    indexArrayForSelection = NULL;
    colorArrayForSelection = NULL;

    layout = NULL;
    render = NULL;

}

void RoundingRoad::setVertexArray(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius,
                                  float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius, int numberOfSides)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoundingRoad::setVertexArray(float x1, float y1, float nearRadius, float angel1NearRadius, float angel2NearRadius, float x2, float y2, float farRadius, float angel1FarRadius, float angel2FarRadius, int numberOfSides)"
                                   << " x1 = " << x1 << " y1 = " << y1
                                   << " nearRadius = " << nearRadius
                                   << " angel1NearRadius = " << angel1NearRadius
                                   << " angel2NearRadius = " << angel2NearRadius
                                   << " x2 = " << x2 << " y2 = " << y2
                                   << " farRadius = " << farRadius
                                   << " angel1FarRadius = " << angel1FarRadius
                                   << " angel2FarRadius = " << angel2FarRadius
                                   << " numberOfSides = " << numberOfSides << "\n";

    this->angel1NearRadius = angel1NearRadius;
    this->angel2NearRadius = angel2NearRadius;
    this->angel1FarRadius = angel1FarRadius;
    this->angel2FarRadius = angel2FarRadius;
    vertexArray.clear();
    vertexArrayFar.clear();
    vertexArrayNear.clear();
    for (int i = 0; i <= numberOfSides; ++i)
    {
        float angle = (angel1NearRadius + (angel2NearRadius - angel1NearRadius) * float(i) / float(numberOfSides)) * 3.14159265f / 180.0f;
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

    elementX = (xCenterFarRadius + xCenterNearRadius) / 2.0f;
    elementY = (yCenterFarRadius + yCenterNearRadius) / 2.0f;
    ////qDebug() << "Number of vertices: " << vertexArray.size() / 3;
}

void RoundingRoad::setColorArray(float red, float green, float blue)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setColorArray(float red, float green, float blue)"
                                       << " red = " << red
                                       << " green = " << green
                                       << " blue = " << blue << "\n";
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArray.push_back(red);
        colorArray.push_back(green);
        colorArray.push_back(blue);
    }
}

void RoundingRoad::setIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setIndexArray()\n";
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
    ////qDebug() << "Number of faces: " << indexArray.size() / 3;
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
    ////qDebug() << "Number of NEAR faces: " << indexArrayNear.size() / 3;
    ////qDebug() << "Number of FAR faces: " << indexArrayFar.size() / 3;

}

void RoundingRoad::setTextureArray(float textUsize, float textVsize)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setTextureArray(float textUsize, float textVsize)"
                                       << " textUsize = " << textUsize
                                       << " textVsize = " << textVsize << "\n";
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
    ////qDebug() << "Number of texture vertices: " << textureArray.size()/2;


}

void RoundingRoad::setNearTextureArray(float textUsize, float textVsize)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setNearTextureArray(float textUsize, float textVsize)"
                                       << " textUsize = " << textUsize
                                       << " textVsize = " << textVsize << "\n";
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
    //float s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 1) * 3];
    y2 = vertexArrayNear[(i + 1) * 3 + 1];
    x2 = vertexArrayNear[(i + 2) * 3];
    y2 = vertexArrayNear[(i + 2) * 3 + 1];
    //float s2 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 2) * 3];
    y2 = vertexArrayNear[(i + 2) * 3 + 1];
    x2 = vertexArrayNear[(i + 3) * 3];
    y2 = vertexArrayNear[(i + 3) * 3 + 1];
    //float s3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayNear[(i + 3) * 3];
    y2 = vertexArrayNear[(i + 3) * 3 + 1];
    x2 = vertexArrayNear[(i + 4) * 3];
    y2 = vertexArrayNear[(i + 4) * 3 + 1];
    //float s4 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));

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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setFarTextureArray(float textUsize, float textVsize)"
                                       << " textUsize = " << textUsize
                                       << " textVsize = " << textVsize << "\n";
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
    //float s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 1) * 3];
    y2 = vertexArrayFar[(i + 1) * 3 + 1];
    x2 = vertexArrayFar[(i + 2) * 3];
    y2 = vertexArrayFar[(i + 2) * 3 + 1];
    //float s2 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 2) * 3];
    y2 = vertexArrayFar[(i + 2) * 3 + 1];
    x2 = vertexArrayFar[(i + 3) * 3];
    y2 = vertexArrayFar[(i + 3) * 3 + 1];
    //float s3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayFar[(i + 3) * 3];
    y2 = vertexArrayFar[(i + 3) * 3 + 1];
    x2 = vertexArrayFar[(i + 4) * 3];
    y2 = vertexArrayFar[(i + 4) * 3 + 1];
    //float s4 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));

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

void RoundingRoad::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->setSelectedStatus(status);
}

void RoundingRoad::drawFigure(QGLWidget* render)
{

    if (log)
    {
        Logger::getLogger()->infoLog() << "RoundingRoad::drawFigure(QGLWidget* render)\n";
        if (render == NULL)
            Logger::getLogger()->warningLog() << "RoundingRoad::drawFigure(QGLWidget* render) render = NULL\n";
    }
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

    if (selected == true)
    {
        glDisable(GL_DEPTH_TEST);        
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

void RoundingRoad::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::drawSelectionFrame()\n";
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        ////qDebug() << "Index " << indexOfSelectedControl;
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
    }
    for (int i = 0; i < getNumberOfControls(); ++i)
    {
        drawControlElement(i, 2.0f, 5.0f);
    }

}

void RoundingRoad::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setColorArrayForSelectionFrame(float red, float green, float blue)"
                                       << " red = " << red
                                       << " green = " << green
                                       << " blue = " << blue << "\n";
    for (int i = 0; i < numberOfVertices; ++i)
    {
        colorArrayForSelection[i * 3] = red;
        colorArrayForSelection[i * 3 + 1] = green;
        colorArrayForSelection[i * 3 + 2] = blue;
    }
}

void RoundingRoad::setIndexArrayForSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setIndexArrayForSelectionFrame()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setIndexArrayForSelectionFrame()"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
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
    elementX += dx;
    elementY += dy;

}

void RoundingRoad::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    int linesCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesCount += lines[i].line->getNumberOfControls();

    if (index >= linesCount)
    {
        index -= linesCount;

        switch (index)
        {

        case 0:
            // Правая угловая точка внутреннего радиуса
        {
            int i = 0;
            glPointSize(pointSize + 5.0f);
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
            glPointSize(pointSize + 5.0f);
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
            glPointSize(pointSize + 5.0f);
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
            glPointSize(pointSize + 5.0f);
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
            glLineWidth(lineWidth);
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
            glLineWidth(lineWidth);
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
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f, 0.0f);
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
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 1.0f,0.0f);
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
    else
    {
        int i;
        for (i = 0; i < lines.size(); ++i)
        {
            if (index >= lines[i].line->getNumberOfControls())
            {
                index -= lines[i].line->getNumberOfControls();
            }
            else
            {
                break;
            }
        }
        lines[i].line->drawControlElement(index, lineWidth, pointSize);
    }


}

QCursor RoundingRoad::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void RoundingRoad::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx << " dy = " << dy
                                       << " x = " << x << " y = " << y << "\n";
    if (fixed)
    {
        return;
    }
    //int j;
    //float x1, y1, x2, y2;
    float dr;
    //float dR;
    float X, Y;
    //X = xCenter - x;
    //X = yCenter - y;
    float factor;
    float res = (X * dx + Y * dy);
    factor = res < 0 ? 1 : -1;
    dr = sqrt(dx * dx + dy * dy);

    int linesCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesCount += lines[i].line->getNumberOfControls();

    if (index >= linesCount)
    {
        index -= linesCount;

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
            float t = (dx1*dx2 + dy1*dy2) / (r1 * nearRadius);                       
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
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
            float t = (dx1*dx2 + dy1*dy2) / (r1 * nearRadius);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
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
            float t = (dx1*dx2 + dy1*dy2) / (r1 * farRadius);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
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
            float t = (dx1*dx2 + dy1*dy2) / (r1 * farRadius);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
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
            //resetLines();
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
            //resetLines();
            emit farRadiusChanged(farRadius);
            break;
        case 8:
            dr = ((x - xCenterNearRadius)*dx + (y - yCenterNearRadius)*dy)/
                    sqrt((x - xCenterNearRadius)*(x - xCenterNearRadius) + (y - yCenterNearRadius)*(y - yCenterNearRadius));
            nearBoardWidth -= dr;
            if (nearBoardWidth < 0.0f)
                nearBoardWidth = 0.0f;
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
            if (farBoardWidth < 0.0f)
                farBoardWidth = 0.0f;
            setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                           xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                           numberOfSides);
            setFarTextureArray(texture_2Usize,texture_2Vsize);
            emit farBoardWidthChanged(farBoardWidth);
            break;

        case 10:
        {
            float pi = 3.14159265f;
            float x1 = x + dx;
            float y1 = y + dy;
            float x2 = xCenterNearRadius;
            float y2 = yCenterNearRadius;
            float x3 = x;
            float y3 = y;
            float dx1 = x1 - x2;
            float dy1 = y1 - y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = x3 - x2;
            float dy2 = y3 - y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
            float res = dx2*dy1 - dx1*dy2;
            float factor = res > 0 ? 1.0f : -1.0f;
            angle = factor * angle * 180.0f / pi;
            setAngel_1_NearRadius(angel1NearRadius + angle);

            x1 = x + dx;
            y1 = y + dy;
            x2 = xCenterFarRadius;
            y2 = yCenterFarRadius;
            x3 = x;
            y3 = y;
            dx1 = x1 - x2;
            dy1 = y1 - y2;
            r1 = sqrt(dx1*dx1 + dy1*dy1);
            dx2 = x3 - x2;
            dy2 = y3 - y2;
            r2 = sqrt(dx2*dx2 + dy2*dy2);
            t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            angle = acos(t);
            res = dx2*dy1 - dx1*dy2;
            factor = res > 0 ? 1.0f : -1.0f;
            angle = factor * angle * 180.0f / pi;
            setAngel_1_FarRadius(angel1FarRadius + angle);

        }
            break;
        case 11:
        {
            float pi = 3.14159265f;
            float x1 = x + dx;
            float y1 = y + dy;
            float x2 = xCenterNearRadius;
            float y2 = yCenterNearRadius;
            float x3 = x;
            float y3 = y;
            float dx1 = x1 - x2;
            float dy1 = y1 - y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = x3 - x2;
            float dy2 = y3 - y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float angle = acos(t);
            float res = dx2*dy1 - dx1*dy2;
            float factor = res > 0 ? 1.0f : -1.0f;
            angle = factor * angle * 180.0f / pi;
            setAngel_2_NearRadius(angel2NearRadius + angle);

            x1 = x + dx;
            y1 = y + dy;
            x2 = xCenterFarRadius;
            y2 = yCenterFarRadius;
            x3 = x;
            y3 = y;
            dx1 = x1 - x2;
            dy1 = y1 - y2;
            r1 = sqrt(dx1*dx1 + dy1*dy1);
            dx2 = x3 - x2;
            dy2 = y3 - y2;
            r2 = sqrt(dx2*dx2 + dy2*dy2);
            t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            angle = acos(t);
            res = dx2*dy1 - dx1*dy2;
            factor = res > 0 ? 1.0f : -1.0f;
            angle = factor * angle * 180.0f / pi;
            setAngel_2_FarRadius(angel2FarRadius + angle);
        }
            break;

        default:
            break;
        }
    }
    else
    {
        int i;
        for (i = 0; i < lines.size(); ++i)
        {
            if (index >= lines[i].line->getNumberOfControls())
            {
                index -= lines[i].line->getNumberOfControls();
            }
            else
            {
                break;
            }
        }
        lines[i].line->resizeByControl(index, dx, dy, x, y);
    }

    //resetLines();
}

int RoundingRoad::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getNumberOfControls()\n";
    //return vertexArray.size() / 3 + 10 + 2;
    int linesCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesCount += lines[i].line->getNumberOfControls();
    return 12 + linesCount;
}


void RoundingRoad::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    indexOfSelectedControl = index;
}


QJsonObject RoundingRoad::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getJSONInfo()\n";
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;

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
    element["NumberOfVertices"] = numberOfVertices;
    element["NumberOfPolygones"] = numberOfPolygones;

    element["ShowNearBoard"] = showNearBoard;
    element["ShowFarBoard"] = showFarBoard;
    element["NearBoardWidth"] = nearBoardWidth;
    element["FarBoardWidth"] = farBoardWidth;

    element["Texture1Source"] = texture1;
    element["Texture1USize"] = texture_1Usize;
    element["Texture1VSize"] = texture_1Vsize;
    element["Texture2Source"] = texture2;
    element["Texture2USize"] = texture_2Usize;
    element["Texture2VSize"] = texture_2Vsize;

    element["Fixed"] = fixed;
    element["Id"] = Id;

    QJsonArray linesArray;
    for (int i = 0; i < lines.size(); ++i)
    {
        QJsonObject line;
        line["Line"] = lines[i].line->getJSONInfo();
        line["LineType"] = lines[i].lineType;
        line["LineWidth"] = lines[i].lineWidth;
        line["NearSide"] = lines[i].nearSide;
        line["Step"] = lines[i].step;
        line["BeginSide"] = lines[i].beginSide;
        line["BeginStep"] = lines[i].beginStep;
        line["EndStep"] = lines[i].endStep;
        line["DifferentDirections"] = lines[i].differentDirections;
        line["SplitZoneWidth"] = lines[i].splitZoneWidth;
        linesArray.append(line);
    }
    element["Lines"] = linesArray;
    return element;    
}


void RoundingRoad::getProperties(QFormLayout *layout, QGLWidget* render)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getProperties(QFormLayout *layout, QGLWidget* render)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "RoundingRoad::getProperties(QFormLayout *layout, QGLWidget* render) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->fatalLog() << "RoundingRoad::getProperties(QFormLayout *layout, QGLWidget* render) layout = NULL\n";
        QApplication::exit(0);
    }
    clearProperties(layout);
    this->layout = layout;
    this->render = render;

    QDoubleSpinBox* nearRadiusSpinBox = new QDoubleSpinBox();
    nearRadiusSpinBox->setKeyboardTracking(false);
    nearRadiusSpinBox->setMaximum(10000.0);
    QDoubleSpinBox* farRadiusSpinBox = new QDoubleSpinBox();
    farRadiusSpinBox->setKeyboardTracking(false);
    farRadiusSpinBox->setMaximum(10000.0);

    QDoubleSpinBox* angel_1_NearRadiusSpinBox = new QDoubleSpinBox();
    angel_1_NearRadiusSpinBox->setKeyboardTracking(false);
    angel_1_NearRadiusSpinBox->setMaximum(10000.0);
    angel_1_NearRadiusSpinBox->setMinimum(-10000.0);
    QDoubleSpinBox* angel_2_NearRadiusSpinBox = new QDoubleSpinBox();
    angel_2_NearRadiusSpinBox->setKeyboardTracking(false);
    angel_2_NearRadiusSpinBox->setMaximum(10000.0);
    angel_2_NearRadiusSpinBox->setMinimum(-10000.0);

    QDoubleSpinBox* angel_1_FarRadiusSpinBox = new QDoubleSpinBox();
    angel_1_FarRadiusSpinBox->setKeyboardTracking(false);
    angel_1_FarRadiusSpinBox->setMaximum(10000.0);
    angel_1_FarRadiusSpinBox->setMinimum(-10000.0);
    QDoubleSpinBox* angel_2_FarRadiusSpinBox = new QDoubleSpinBox();
    angel_2_FarRadiusSpinBox->setKeyboardTracking(false);
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
    nearBoardWidthSpinBox->setKeyboardTracking(false);
    nearBoardWidthSpinBox->setMinimum(0.0f);
    nearBoardWidthSpinBox->setValue(nearBoardWidth);

    connect(this, SIGNAL(nearBoardWidthChanged(double)), nearBoardWidthSpinBox, SLOT(setValue(double)));
    connect(nearBoardWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNearBoardWidth(double)));

    QDoubleSpinBox* farBoardWidthSpinBox = new QDoubleSpinBox();
    farBoardWidthSpinBox->setKeyboardTracking(false);
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
    connect(stepDialog, SIGNAL(singleWayChanged(bool)), this, SLOT(setSingleWay(bool)));
    connect(stepDialog, SIGNAL(axisStepChanged(double)), this, SLOT(setAxisStep(double)));
    connect(stepDialog, SIGNAL(splitZoneTypeChanged(int)), this, SLOT(setSplitZoneType(int)));
    connect(stepDialog, SIGNAL(splitZoneHeightChanged(double)), this, SLOT(setSplitZoneHeight(double)));
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

    QCheckBox *showMeasurementsCheckBox = new QCheckBox();
    showMeasurementsCheckBox->setChecked(showMeasurements);
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
    layout->addRow("Размеры", showMeasurementsCheckBox);

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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setNearRadius(double nearRadius)"
                                       << " nearRadius = " << nearRadius << "\n";
    if (this->nearRadius != nearRadius)
    {
        this->nearRadius = nearRadius;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit nearRadiusChanged(nearRadius);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_1_NearRadius(double angel1)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setAngel_1_NearRadius(double angel1)"
                                       << " angel1 = " << angel1 << "\n";
    if (angel1NearRadius != angel1)
    {
        angel1NearRadius = angel1;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit angel_1_NearRadiusChanged(angel1);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_2_NearRadius(double angel2)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setAngel_2_NearRadius(double angel2)"
                                       << " angel2 = " << angel2 << "\n";
    if (angel2NearRadius != angel2)
    {
        angel2NearRadius = angel2;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit angel_2_NearRadiusChanged(angel2);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setFarRadius(double farRadius)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setFarRadius(double farRadius)"
                                       << " farRadius = " << farRadius << "\n";
    if (this->farRadius != farRadius)
    {
        this->farRadius = farRadius;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit farRadiusChanged(farRadius);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_1_FarRadius(double angel1)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setAngel_1_FarRadius(double angel1)"
                                       << " angel1 = " << angel1 << "\n";
    if (angel1FarRadius != angel1)
    {
        angel1FarRadius = angel1;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit angel_1_FarRadiusChanged(angel1);
    }
    else
    {
        return;
    }
}

void RoundingRoad::setAngel_2_FarRadius(double angel2)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setAngel_2_FarRadius(double angel2)"
                                       << " angel2 = " << angel2 << "\n";
    if (angel2FarRadius != angel2)
    {
        angel2FarRadius = angel2;
        setVertexArray(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                       xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                       numberOfSides);
        setTextureArray(texture_1Usize,texture_1Vsize);
        setNearTextureArray(texture_2Usize,texture_2Vsize);
        setFarTextureArray(texture_2Usize,texture_2Vsize);
        //resetLines();
        emit angel_2_FarRadiusChanged(angel2);
    }
    else
    {
        return;
    }
}

void RoundingRoad::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool nearSide)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool nearSide)"
                                       << " step = " << step
                                       << " textureSource = " << textureSource
                                       << " textureSize = " << textureSize
                                       << " lineWidth = " << lineWidth
                                       << " lineType = " << lineType
                                       << " nearSide = " << nearSide << "\n";
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
    bool beginStepReleased = false;
    bool endStepReleased = false;
    for (int i = 0; i <= numberOfSides; ++i)
    {
        //float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
        float angle = (angel_1 + (angel_2 - angel_1) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        //float dr;

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
                //qDebug() << "Begin step...";
            }
        }
        else
            if ((angel_2 * 3.1415926 / 180.0f - angle)*radius > endStep)
            {
                vertices.push_back(x + dx);
                vertices.push_back(y + dy);
                vertices.push_back(0.001f);
                //qDebug() << "Middle part...";
            }

            else
            {
                dx = radius * cosf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                dy = radius * sinf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                vertices.push_back(x + dx);
                vertices.push_back(y + dy);
                vertices.push_back(0.001f);
                //qDebug() << "End step...";
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
        switch (splitZoneType)
        {
        case 0:
        {
            line.line = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        case 1:
        {
            line.line = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                      splitZoneType,
                                      splitZoneHeight,
                                      "/models/city_roads/board.jpg",
                                      0.25f, 6.0f,
                                      "/models/city_roads/grass.jpg",
                                      3.0f, 3.0f,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        case 2:
        {
            line.line = new SplitZone(lineVertexArray, size, splitZoneWidth, beginRounding, endRounding,
                                      splitZoneType,
                                      splitZoneHeight,
                                      "/models/city_roads/board.jpg",
                                      0.25f, 6.0f,
                                      "/models/city_roads/nr_07S.jpg",
                                      6.0f, 6.0f,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        default:
            break;
        }


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
    //model->getGroup(1).push_back(line.line);
    line.line->setSelectedStatus(false);
    RoadElement::undoStack->push(new AddLineCommand(this, line, render));
    //lines.push_back(line);
    delete[] lineVertexArray;
    lineVertexArray = NULL;
    //emit linesChanged(layout, render);
}

void RoundingRoad::addLine()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::addLine()\n";
    QString textSource;
    float lWidth = 0.0f;
    switch(lineType)
    {
    case 0:
        textSource = "/models/city_roads/solid.png";
        lWidth = 0.1f;
        break;
    case 1:
        textSource = "/models/city_roads/inter.png";
        lWidth = 0.1f;
        break;
    case 2:
        textSource = "/models/city_roads/d_solid.png";
        lWidth = 0.25f;
        break;
    case 3:
        textSource = "/models/city_roads/d_inter_l.png";
        lWidth = 0.25f;
        break;
    case 4:
        textSource = "/models/city_roads/d_inter_r.png";
        lWidth = 0.25f;
        break;
    case 5:
        textSource = "/models/city_roads/d_inter.png";
        lWidth = 0.25f;
        break;
    case 8:
    {
        textSource = QString("/models/city_roads/tramways.png");
        lWidth = 1.5f;
        if (singleWay)
        {
            addLine(step, textSource, 1.5f, lWidth, lineType, nearSide);
        }
        else
        {
            addLine(step + axisStep / 2.0, textSource, 1.5f, lWidth, lineType, nearSide);
            addLine(step - axisStep / 2.0, textSource, 1.5f, lWidth, lineType, nearSide);
        }
        return;

    }

        break;
    default:
        break;

    }

    addLine(step, textSource, 6.0f, lWidth, lineType, nearSide);
}

void RoundingRoad::addLine(LineBrokenLinked line)
{
    lines.push_back(line);
    if (layout && render)
        emit linesChanged(layout, render);
}



void RoundingRoad::setNearSide(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setNearSide(bool status)"
                                       << " status = " << status << "\n";
    this->nearSide = status;
}

void RoundingRoad::setStep(double value)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setStep(double value)"
                                       << " value = " << value << "\n";
    this->step = value;
}

void RoundingRoad::setLineType(int type)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setLineType(int type)"
                                       << " type = " << type << "\n";
    this->lineType = type;
}

void RoundingRoad::deleteLine()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::deleteLine()\n";
    QPushButton * b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    ////qDebug() << "delete line " << b->text();
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

void RoundingRoad::deleteLine(LineBrokenLinked line)
{
    int i;
    for (i = 0; i < lines.size(); ++i)
    {
        if (lines[i].line == line.line)
            break;
    }
    lines.removeAt(i);
    emit linesChanged(layout, render);
}

void RoundingRoad::resetLines()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::resetLines()\n";
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


        QVector<float> vertices;
        bool beginStepReleased = false;
        //bool endStepReleased = false;
        for (int j = 0; j <= numberOfSides; ++j)
        {
            //float angle = 2.0 * 3.1415926 * float(i) / float(numberOfSides);
            float angle = (angel_1 + (angel_2 - angel_1) * float(i) / float(numberOfSides)) * 3.1415926 / 180.0f;
            float dx = radius * cosf(angle);
            float dy = radius * sinf(angle);
            //float dr;

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
                    ////qDebug() << "Begin step...";
                }
            }
            else
                if ((angel_2 * 3.1415926 / 180.0f - angle)*radius > endStep)
                {
                    vertices.push_back(x + dx);
                    vertices.push_back(y + dy);
                    vertices.push_back(0.001f);
                    ////qDebug() << "Middle part...";
                }

                else
                {
                    dx = radius * cosf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                    dy = radius * sinf(angel_2 * 3.1415926 / 180.0f - endStep / radius);
                    vertices.push_back(x + dx);
                    vertices.push_back(y + dy);
                    vertices.push_back(0.001f);
                    //.//qDebug() << "End step...";
                    break;
                }

        }


        int size = vertices.size();
        float *lineVertexArray = new float[size];
        for (int i = 0; i < vertices.size(); ++i)
            lineVertexArray[i] = vertices[i];

        if (lines[i].lineType == 6)
        {
            SplitZone *splitZone = dynamic_cast<SplitZone*>(lines[i].line);
            splitZone->calculateLine(lineVertexArray, size);
            splitZone->reset();
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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setBeginStep(double step)"
                                       << " step = " << step << "\n";
    beginStep = step;
    //qDebug() << "RoundingRoad::beginStep = " << beginStep;
}

void RoundingRoad::setEndStep(double step)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setEndStep(double step)"
                                       << " step = " << step << "\n";
    endStep = step;
    //qDebug() << "RoundingRoad::endStep = " << endStep;
}

void RoundingRoad::setSplitZoneWidth(double value)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setSplitZoneWidth(double value)"
                                       << " value = " << value << "\n";
    splitZoneWidth = value;
}

void RoundingRoad::setBeginSide(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setBeginSide(bool status)"
                                       << " status = " << status << "\n";
    beginSide = status;
}

void RoundingRoad::setBeginRounding(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setBeginRounding(bool status)"
                                       << " status = " << status << "\n";
    beginRounding = status;
}

void RoundingRoad::setEndRounding(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setEndRounding(bool status)"
                                       << " status = " << status << "\n";
    endRounding = status;
}

void RoundingRoad::setDifferentDirections(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setDifferentDirections(bool status)"
                                       << " status = " << status << "\n";
    differentDirections = status;
}

void RoundingRoad::setSingleWay(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setSingleWay(bool status)"
                                       << " status = " << status << "\n";
    singleWay = status;
}

void RoundingRoad::setAxisStep(double step)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setAxisStep(double step)"
                                       << " step = " << step << "\n";
    axisStep = step;
}

void RoundingRoad::setSplitZoneType(int type)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setSplitZoneType(int type)"
                                       << " type = " << type << "\n";
    splitZoneType = type;
}

void RoundingRoad::setSplitZoneHeight(double height)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setSplitZoneHeight(double height)"
                                       << " height = " << height << "\n";
    splitZoneHeight = height;
}

void RoundingRoad::setShowNearBoard(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setShowNearBoard(bool status)"
                                       << " status = " << status << "\n";
    showNearBoard = status;
}

void RoundingRoad::setShowFarBoard(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setShowFarBoard(bool status)"
                                       << " status = " << status << "\n";
    showFarBoard = status;
}

void RoundingRoad::setNearBoardWidth(double width)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setNearBoardWidth(double width)"
                                       << " width = " << width << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setFarBoardWidth(double width)"
                                       << " width = " << width << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::isFixed()\n";
    return fixed;
}


void RoundingRoad::drawMeasurements(QGLWidget *render)
{
    if (!showMeasurements)
        return;
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::drawMeasurements(QGLWidget *render)\n";
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);

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
        render->renderText(wx, wy, QString("%1").arg(nearRadius, 0, 'f', 2), shrift);



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
        render->renderText(wx, wy, QString("%1").arg(farRadius, 0, 'f', 2), shrift);


        if (showNearBoard)
        {
            glColor3f(0.0f, 0.0f, 0.0f);
            int count = vertexArrayNear.size() / 6;
            x = (vertexArrayNear[(count - 2) * 3] + vertexArrayNear[(count - 1) * 3]) / 2.0f;
            y = (vertexArrayNear[(count - 2) * 3 + 1] + vertexArrayNear[(count - 1) * 3 + 1]) / 2.0f;
            z = (vertexArrayNear[(count - 2) * 3 + 2] + vertexArrayNear[(count - 1) * 3 + 2]) / 2.0f;
            getWindowCoord(x, y, z, wx, wy, wz);

            render->renderText(wx, wy,"Правый тротуар: " +  QString("%1").arg(nearBoardWidth, 0, 'f', 2), shrift);
        }

        if (showFarBoard)
        {
            glColor3f(0.0f, 0.0f, 0.0f);
            int count = vertexArrayFar.size() / 3;
            x = (vertexArrayFar[(count - 2) * 3] + vertexArrayFar[(count - 1) * 3]) / 2.0f;
            y = (vertexArrayFar[(count - 2) * 3 + 1] + vertexArrayFar[(count - 1) * 3 + 1]) / 2.0f;
            z = (vertexArrayFar[(count - 2) * 3 + 2] + vertexArrayFar[(count - 1) * 3 + 2]) / 2.0f;
            getWindowCoord(x, y, z, wx, wy, wz);

            render->renderText(wx, wy,"Левый тротуар: " +  QString("%1").arg(farBoardWidth, 0, 'f', 2), shrift);
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
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
    return true;
}


int RoundingRoad::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getLayer()\n";
    return layer;
}


void RoundingRoad::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::clear()\n";
}


std::vector<vec3> RoundingRoad::getCoordOfControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getCoordOfControl(int index)"
                                       << " index = " << index << "\n";
    std::vector<vec3> res;

    int linesCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesCount += lines[i].line->getNumberOfControls();

    if (index >= linesCount)
    {
        index -= linesCount;

        switch (index)
        {

        case 0:
            // Правая угловая точка внутреннего радиуса
        {
            int i = 0;
            vec3 p(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
            res.push_back(p);
        }
            break;
        case 1:
            // Левая угловая точка внутреннего радиуса
        {
            int i = numberOfVertices - 2;
            vec3 p(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
            res.push_back(p);
        }
            break;
        case 2:
            // Правая угловая точка наружного радиуса
        {
            int i = 1;
            vec3 p(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
            res.push_back(p);
        }
            break;
        case 3:
            // Левая угловая точка наружного радиуса
        {
            int i = numberOfVertices - 1;
            vec3 p(vertexArray[i * 3],
                    vertexArray[i * 3 + 1],
                    vertexArray[i * 3 + 2]);
            res.push_back(p);
        }
            break;
        case 4:
            // Дуга внутреннего радиуса

        {
            for (int i = 0; i < vertexArray.size() / 3; i += 2)
            {
                vec3 p;
                p.x = vertexArray[i * 3];
                p.y = vertexArray[i * 3 + 1];
                p.z = vertexArray[i * 3 + 2];
                res.push_back(p);
            }
        }
            break;

        case 5:
            // Дуга наружного радиуса

        {
            for (int i = 1; i < vertexArray.size() / 3; i += 2)
            {
                vec3 p;
                p.x = vertexArray[i * 3];
                p.y = vertexArray[i * 3 + 1];
                p.z = vertexArray[i * 3 + 2];
                res.push_back(p);
            }
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
            for (int i = 0 ; i < vertexArrayNear.size() / 3; i += 10)
            {
                vec3 p;
                p.x = vertexArrayNear[(i + 4) * 3];
                p.y = vertexArrayNear[(i + 4) * 3 + 1];
                p.z = vertexArrayNear[(i + 4) * 3 + 2];
                res.push_back(p);
            }
        }
            break;
        case 9:
        {
            for (int i = 0 ; i < vertexArrayFar.size() / 3; i += 10)
            {
                vec3 p;
                p.x = vertexArrayFar[(i + 4) * 3];
                p.y = vertexArrayFar[(i + 4) * 3 + 1];
                p.z = vertexArrayFar[(i + 4) * 3 + 2];
                res.push_back(p);
            }
        }
            break;

        case 10:
        {
            int i = 0;
            vec3 p(vertexArray[i * 3],vertexArray[i * 3 + 1],vertexArray[i * 3 + 2]);
            vec3 s(vertexArray[(i + 1) * 3],vertexArray[(i + 1) * 3 + 1],vertexArray[(i + 1) * 3 + 2]);
            res.push_back(p);
            res.push_back(s);
        }
            break;
        case 11:
        {
            int i = numberOfVertices - 2;
            vec3 p(vertexArray[i * 3],vertexArray[i * 3 + 1],vertexArray[i * 3 + 2]);
            vec3 s(vertexArray[(i + 1) * 3],vertexArray[(i + 1) * 3 + 1],vertexArray[(i + 1) * 3 + 2]);
            res.push_back(p);
            res.push_back(s);
        }
            break;

        default:
            break;
        }
    }
    else
    {
        int i;
        for (i = 0; i < lines.size(); ++i)
        {
            if (index >= lines[i].line->getNumberOfControls())
            {
                index -= lines[i].line->getNumberOfControls();
            }
            else
            {
                break;
            }
        }
        for (unsigned j = 0; j < lines[i].line->getCoordOfControl(index).size(); ++j)
        {
            res.push_back(lines[i].line->getCoordOfControl(index)[j]);
        }
    }
    return res;
}


void RoundingRoad::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::clearProperties(QLayout *layout)\n";
    disconnect(stepDialog, 0, this, 0);
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


void RoundingRoad::deleteLine(RoadElement *line)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::deleteLine(RoadElement *line)\n";
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

void RoundingRoad::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "RoundingRoad::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


RoadElement *RoundingRoad::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::getCopy()\n";
    RoundingRoad* copyElement = new RoundingRoad(*this);
    return copyElement;
}


void RoundingRoad::setCoordForControl(int index, std::vector<vec3> &controls)
{
    if (log)
        Logger::getLogger()->infoLog() << "RoundingRoad::setCoordForControl(int index, std::vector<vec3> &controls)"
                                       << " index = " << index << "\n";

    int linesCount = 0;
    for (int i = 0; i < lines.size(); ++i)
        linesCount += lines[i].line->getNumberOfControls();

    if (index >= linesCount)
    {
        index -= linesCount;

        switch (index)
        {

        case 0:
            // Правая угловая точка внутреннего радиуса
        {
            int i = 0;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 1:
            // Левая угловая точка внутреннего радиуса
        {
            int i = numberOfVertices - 2;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 2:
            // Правая угловая точка наружного радиуса
        {
            int i = 1;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 3:
            // Левая угловая точка наружного радиуса
        {
            int i = numberOfVertices - 1;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 4:
            // Дуга внутреннего радиуса
        {
            float x, y;
            x = vertexArray[0];
            y = vertexArray[1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;

        case 5:
            // Дуга наружного радиуса

        {
            float x, y;
            x = vertexArray[3];
            y = vertexArray[4];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }

            break;
        case 6:
            // Центр окружности внутреннего радиуса
        {
            float x, y;
            x = xCenterNearRadius;
            y = yCenterNearRadius;
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 7:
            // Центр окружности наружного радиуса
        {
            float x, y;
            x = xCenterFarRadius;
            y = yCenterFarRadius;
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 8:
        {
            int i = 0;
            float x, y;
            x = vertexArrayNear[(i + 4) * 3];
            y = vertexArrayNear[(i + 4) * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 9:
        {
            int i = 0;
            float x, y;
            x = vertexArrayFar[(i + 4) * 3];
            y = vertexArrayFar[(i + 4) * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;

        case 10:
        {
            int i = 0;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;
        case 11:
        {
            int i = numberOfVertices - 2;
            float x, y;
            x = vertexArray[i * 3];
            y = vertexArray[i * 3 + 1];
            resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
        }
            break;

        default:
            break;
        }
    }
    else
    {
        int i;
        for (i = 0; i < lines.size(); ++i)
        {
            if (index >= lines[i].line->getNumberOfControls())
            {
                index -= lines[i].line->getNumberOfControls();
            }
            else
            {
                break;
            }
        }
        lines[i].line->setCoordForControl(index, controls);
    }

}
