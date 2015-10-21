#include "roadsimple.h"
#include <QPushButton>
#include <QDialog>
#include <QApplication>
#include <assert.h>
#include "splitzone.h"
#include "model.h"

RoadSimple::RoadSimple()
{
    name = "noname";
    //setVertexArray(0.0, 0.0, 1.0);
    //setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    ///setIndexArray();

    // Рвмка для выбора фигуры
    //setIndexArrayForSelectionFrame();
    //setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    //selected = false;

}
RoadSimple::RoadSimple(float x1, float y1, float x2, float y2, QString name, int layer, QString description)
{

    this->description = description;
    this->layer = layer;
    this->name = name;
    if (name == "Crosswalk")
    {
        showRightBoard = showLeftBoard = false;
        z = 0.02f;
    }
    else
    {
        showRightBoard = showLeftBoard = true;
        z = 0.0f;
    }
    useColor = true;
    //this->size = size;
    width = 1.0f;
    rightWidth = leftWidth = width / 2.0f;


    setVertexArray(x1, y1, x2, y2, width);
    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();
    red = 0.7f;
    green = 0.7f;
    blue = 0.7f;
    alpha = 1.0f;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    // Рвмка для выбора фигуры
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    layout = NULL;
    render = NULL;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoadSimple::RoadSimple(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name, int layer, QString description)
{

    this->description = description;
    this->layer = layer;
    this->name = name;
    useColor = true;
    this->size = size;
    this->width = width;
    rightWidth = leftWidth = width / 2.0f;
    if (name == "Crosswalk")
    {
        showRightBoard = showLeftBoard = false;
        z = 0.02f;
    }
    else
    {
        showRightBoard = showLeftBoard = true;
        z = 0.0f;
    }

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
    layout = NULL;
    render = NULL;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoadSimple::RoadSimple(float x1, float y1, float x2, float y2, float width,
                       QString source_1, float textureSize_1_Usize, float textureSize_1_Vsize,
                       QString source_2, float textureSize_2_Usize, float textureSize_2_Vsize,
                       QString name, int layer, QString description)
{

    this->description = description;
    this->layer = layer;
    this->name = name;
    indexOfSelectedControl = -1;
    this->texture1Source = source_1;
    this->texture1USize = textureSize_1_Usize;
    this->texture1VSize = textureSize_1_Vsize;
    this->texture2Source = source_2;
    this->texture2USize = textureSize_2_Usize;
    this->texture2VSize = textureSize_2_Vsize;
    this->rightBoardWidth = this->leftBoardWidth = textureSize_2_Usize;
    if (name == "Crosswalk")
    {
        showRightBoard = showLeftBoard = false;
        z = 0.02f;
    }
    else
    {
        showRightBoard = showLeftBoard = true;
        z = 0.0f;
    }
    useColor = false;
    this->size = size;
    this->width = width;
    rightWidth = leftWidth = width / 2.0f;
    vertexArrayRight.resize(30);
    indexArrayRight.resize(24);
    textureArrayRight.resize(20);

    vertexArrayLeft.resize(30);
    indexArrayLeft.resize(24);
    textureArrayLeft.resize(20);

    setVertexArray(x1, y1, x2, y2, width);
    // textureID[0] = getTextures(source_1);
    // textureID[1] = getTextures(source_2);
    textureID[0] = TextureManager::getInstance()->getID(source_1);
    textureID[1] = TextureManager::getInstance()->getID(source_2);

    setTextureArray();
    setIndexArray();

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(1.0f, 1.0f, 1.0f);

    selected = false;
    fixed = false;
    layout = NULL;
    render = NULL;
    connect(this, SIGNAL(linesChanged(QFormLayout*,QGLWidget*)),SLOT(getProperties(QFormLayout*,QGLWidget*)));
}

RoadSimple::~RoadSimple()
{
    //qDebug() << "RoadSimple destructor";
    for (int i = 0; i < lines.size(); ++i)
    {
        for (std::list<RoadElement*>::iterator it = model->getGroup(1).begin();
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
    lines.clear();



    vertexArrayRight.clear();
    indexArrayRight.clear();
    textureArrayRight.clear();

    vertexArrayLeft.clear();
    indexArrayLeft.clear();
    textureArrayLeft.clear();

    layout = NULL;
    render = NULL;
}

// Индексы вершины для отрисовки
void RoadSimple::setVertexArray(float x1, float y1, float x2, float y2, float width)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->width = width;
    this->length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float r1 = rightWidth;
    float r2 = leftWidth;
    float dx1 = sqrt(r1*r1*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
    float dy1 = sqrt(r1*r1/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
    float dx2 = sqrt(r2*r2*(y2-y1)*(y2-y1)/((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1)));
    float dy2 = sqrt(r2*r2/(1 + (y2-y1)*(y2-y1)/((x2-x1)*(x2-x1))));
    if (x1 >= x2 && y1 >= y2)
    {
        dx1 *= -1.0f;
        dy1 *= -1.0f;
        dx2 *= -1.0f;
        dy2 *= -1.0f;

    }
    if (x1 < x2 && y1 > y2)
    {
        dx1 *= -1.0f;
        dx2 *= -1.0f;

    }
    if (x1 > x2 && y1 < y2)
    {
        dy1 *= -1.0f;
        dy2 *= -1.0f;

    }
    VertexArray[0][0]=x1 + dx1;
    VertexArray[0][1]=y1 - dy1;
    VertexArray[0][2]=z;

    VertexArray[1][0]=x1 - dx2;
    VertexArray[1][1]=y1 + dy2;
    VertexArray[1][2]=z;

    VertexArray[2][0]=x2 - dx2;
    VertexArray[2][1]=y2 + dy2;
    VertexArray[2][2]=z;

    VertexArray[3][0]=x2 + dx1;
    VertexArray[3][1]=y2 - dy1;
    VertexArray[3][2]=z;

    xP1 = (x2 + x1) / 2.0f + dx1;
    yP1 = (y2 + y1) / 2.0f - dy1;
    xP2 = (x2 + x1) / 2.0f - dx2;
    yP2 = (y2 + y1) / 2.0f + dy2;

    //////////////////////////////////////////////////////////////////////////////

    vertexArrayRight[0] = x1 + dx1;
    vertexArrayRight[1] = y1 - dy1;
    vertexArrayRight[2] = z;

    vertexArrayRight[3] = x1 + dx1;
    vertexArrayRight[4] = y1 - dy1;
    vertexArrayRight[5] = 0.08f;

    vertexArrayRight[6] = x1 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * 0.03f;
    vertexArrayRight[7] = y1 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * 0.03f;
    vertexArrayRight[8] = 0.1f;

    vertexArrayRight[9] = x1 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * 0.25f;
    vertexArrayRight[10] = y1 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * 0.25f;
    vertexArrayRight[11] = 0.1f;

    vertexArrayRight[12] = x1 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * rightBoardWidth;
    vertexArrayRight[13] = y1 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * rightBoardWidth;
    vertexArrayRight[14] = 0.1f;

    ////////////////////////////////////

    vertexArrayRight[15] = x2 + dx1;
    vertexArrayRight[16] = y2 - dy1;
    vertexArrayRight[17] = z;

    vertexArrayRight[18] = x2 + dx1;
    vertexArrayRight[19] = y2 - dy1;
    vertexArrayRight[20] = 0.08f;

    vertexArrayRight[21] = x2 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * 0.03f;
    vertexArrayRight[22] = y2 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * 0.03f;
    vertexArrayRight[23] = 0.1f;

    vertexArrayRight[24] = x2 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * 0.25f;
    vertexArrayRight[25] = y2 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * 0.25f;
    vertexArrayRight[26] = 0.1f;

    vertexArrayRight[27] = x2 + dx1 + dx1 / sqrt(dx1*dx1 + dy1*dy1) * rightBoardWidth;
    vertexArrayRight[28] = y2 - dy1 - dy1 / sqrt(dx1*dx1 + dy1*dy1) * rightBoardWidth;
    vertexArrayRight[29] = 0.1f;

    //-----------------------------------------------------------------

    vertexArrayLeft[0] = x1 - dx2;
    vertexArrayLeft[1] = y1 + dy2;
    vertexArrayLeft[2] = z;

    vertexArrayLeft[3] = x1 - dx2;
    vertexArrayLeft[4] = y1 + dy2;
    vertexArrayLeft[5] = 0.08f;

    vertexArrayLeft[6] = x1 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * 0.03f;
    vertexArrayLeft[7] = y1 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * 0.03f;
    vertexArrayLeft[8] = 0.1f;

    vertexArrayLeft[9] = x1 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * 0.25f;
    vertexArrayLeft[10] = y1 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * 0.25f;
    vertexArrayLeft[11] = 0.1f;

    vertexArrayLeft[12] = x1 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * leftBoardWidth;
    vertexArrayLeft[13] = y1 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * leftBoardWidth;
    vertexArrayLeft[14] = 0.1f;

    ////////////////////////////////////

    vertexArrayLeft[15] = x2 - dx2;
    vertexArrayLeft[16] = y2 + dy2;
    vertexArrayLeft[17] = z;

    vertexArrayLeft[18] = x2 - dx2;
    vertexArrayLeft[19] = y2 + dy2;
    vertexArrayLeft[20] = 0.08f;

    vertexArrayLeft[21] = x2 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * 0.03f;
    vertexArrayLeft[22] = y2 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * 0.03f;
    vertexArrayLeft[23] = 0.1f;

    vertexArrayLeft[24] = x2 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * 0.25f;
    vertexArrayLeft[25] = y2 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * 0.25f;
    vertexArrayLeft[26] = 0.1f;

    vertexArrayLeft[27] = x2 - dx2 - dx2 / sqrt(dx2*dx2 + dy2*dy2) * leftBoardWidth;
    vertexArrayLeft[28] = y2 + dy2 + dy2 / sqrt(dx2*dx2 + dy2*dy2) * leftBoardWidth;
    vertexArrayLeft[29] = 0.1f;




}

void RoadSimple::setVertexArray()
{
    setVertexArray(x1, y1, x2, y2, width);
}

// Индексы каждой вершины
void RoadSimple::setColorArray(float red, float green, float blue, float alpha)
{
    for (int i=0; i<4; i++)
    {
        ColorArray[i][0]=red;
        ColorArray[i][1]=green;
        ColorArray[i][2]=blue;
        ColorArray[i][3]=alpha;
    }
}

void RoadSimple::setTextureArray()
{
    GLfloat x1 = VertexArray[0][0];
    GLfloat y1 = VertexArray[0][1];
    GLfloat x2 = VertexArray[3][0];
    GLfloat y2 = VertexArray[3][1];
    GLfloat r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));
    GLfloat delta_1 = r / texture1VSize;

    TextureArray[0][0] = width / texture1USize;
    TextureArray[0][1] = 0.0f;

    TextureArray[1][0] = 0.0f;
    TextureArray[1][1] = 0.0f;

    TextureArray[2][0] = 0.0f;
    TextureArray[2][1] = delta_1;

    TextureArray[3][0] = width / texture1USize;
    TextureArray[3][1] = delta_1;

    //////////////////////////////

    x1 = vertexArrayRight[0];
    y1 = vertexArrayRight[1];
    x2 = vertexArrayRight[12];
    y2 = vertexArrayRight[13];
    GLfloat d = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));
    delta_1 = d / texture2USize;
    GLfloat delta_2 = r / texture2VSize;



    textureArrayRight[0] = 0.0f;
    textureArrayRight[1] = 0.0f;

    textureArrayRight[2] = 0.08f;
    textureArrayRight[3] = 0.0f;

    textureArrayRight[4] = 0.09f;
    textureArrayRight[5] = 0.0f;

    textureArrayRight[6] = 0.16666f;
    textureArrayRight[7] = 0.0f;

    textureArrayRight[8] = 1.0f; //rightBoardWidth / texture2USize;
    textureArrayRight[9] = 0.0f;

    /////////////////////////////////////
    textureArrayRight[10] = 0.0f;
    textureArrayRight[11] = delta_2;

    textureArrayRight[12] = 0.08f;
    textureArrayRight[13] = delta_2;

    textureArrayRight[14] = 0.09f;
    textureArrayRight[15] = delta_2;

    textureArrayRight[16] = 0.16666f;
    textureArrayRight[17] = delta_2;

    textureArrayRight[18] = 1.0f; //rightBoardWidth / texture2USize;
    textureArrayRight[19] = delta_2;
    //-----------------------------------------------------

    textureArrayLeft[0] = 0.0f;
    textureArrayLeft[1] = 0.0f;

    textureArrayLeft[2] = 0.08f;
    textureArrayLeft[3] = 0.0f;

    textureArrayLeft[4] = 0.09f;
    textureArrayLeft[5] = 0.0f;

    textureArrayLeft[6] = 0.16666f;
    textureArrayLeft[7] = 0.0f;

    textureArrayLeft[8] = 1.0f; //leftBoardWidth / texture2USize;
    textureArrayLeft[9] = 0.0f;

    /////////////////////////////////////
    textureArrayLeft[10] = 0.0f;
    textureArrayLeft[11] = delta_2;

    textureArrayLeft[12] = 0.08f;
    textureArrayLeft[13] = delta_2;

    textureArrayLeft[14] = 0.09f;
    textureArrayLeft[15] = delta_2;

    textureArrayLeft[16] = 0.16666f;
    textureArrayLeft[17] = delta_2;

    textureArrayLeft[18] = 1.0f; //leftBoardWidth / texture2USize;
    textureArrayLeft[19] = delta_2;

}

// Индексы для отрисовки фигуры
void RoadSimple::setIndexArray()
{
    // Порядок обхода - по часовой стрелке

    // 1-ый полигон
    IndexArray[0][0]=0;
    IndexArray[0][1]=2;
    IndexArray[0][2]=1;

    // 2-ой полигон
    IndexArray[1][0]=0;
    IndexArray[1][1]=3;
    IndexArray[1][2]=2;


    indexArrayRight[0] = 1;
    indexArrayRight[1] = 5;
    indexArrayRight[2] = 0;

    indexArrayRight[3] = 1;
    indexArrayRight[4] = 6;
    indexArrayRight[5] = 5;

    indexArrayRight[6] = 2;
    indexArrayRight[7] = 6;
    indexArrayRight[8] = 1;

    indexArrayRight[9] = 2;
    indexArrayRight[10] = 7;
    indexArrayRight[11] = 6;

    indexArrayRight[12] = 3;
    indexArrayRight[13] = 7;
    indexArrayRight[14] = 2;

    indexArrayRight[15] = 3;
    indexArrayRight[16] = 8;
    indexArrayRight[17] = 7;

    indexArrayRight[18] = 4;
    indexArrayRight[19] = 8;
    indexArrayRight[20] = 3;

    indexArrayRight[21] = 4;
    indexArrayRight[22] = 9;
    indexArrayRight[23] = 8;

    //----------------------------------------
    indexArrayLeft[0] = 0;
    indexArrayLeft[1] = 5;
    indexArrayLeft[2] = 6;

    indexArrayLeft[3] = 0;
    indexArrayLeft[4] = 6;
    indexArrayLeft[5] = 1;

    indexArrayLeft[6] = 1;
    indexArrayLeft[7] = 6;
    indexArrayLeft[8] = 7;

    indexArrayLeft[9] = 1;
    indexArrayLeft[10] = 7;
    indexArrayLeft[11] = 2;

    indexArrayLeft[12] = 2;
    indexArrayLeft[13] = 7;
    indexArrayLeft[14] = 8;

    indexArrayLeft[15] = 2;
    indexArrayLeft[16] = 8;
    indexArrayLeft[17] = 3;

    indexArrayLeft[18] = 3;
    indexArrayLeft[19] = 8;
    indexArrayLeft[20] = 9;

    indexArrayLeft[21] = 3;
    indexArrayLeft[22] = 9;
    indexArrayLeft[23] = 4;


}

void RoadSimple::setVertexArrayBoard()
{

}

void RoadSimple::setIndexArrayBoard()
{

}

void RoadSimple::setTextureArrayBoard()
{

}

GLuint RoadSimple::getTextures(QString source)
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

    return ID;
}

void RoadSimple::drawFigure(QGLWidget* render)
{

    if (!useColor)
    {
        glDisableClientState(GL_COLOR_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, textureID[0]);
        //getTextures(textureSource);
        glVertexPointer(3, GL_FLOAT, 0, VertexArray);
        glTexCoordPointer(2, GL_FLOAT, 0, TextureArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, IndexArray);



        ///////////////////////////////////

        if (showRightBoard)
        {
            ////qDebug() << "RoadSimple: text1" << textureID[1];
            glBindTexture(GL_TEXTURE_2D, textureID[1]);
            glVertexPointer(3, GL_FLOAT, 0, vertexArrayRight.begin());
            glTexCoordPointer(2, GL_FLOAT, 0, textureArrayRight.begin());
            glDrawElements(GL_TRIANGLES, indexArrayRight.size(), GL_UNSIGNED_BYTE, indexArrayRight.begin());
        }
        if (showLeftBoard)
        {
            ////qDebug() << "RoadSimple: text1" << textureID[1];
            glBindTexture(GL_TEXTURE_2D, textureID[1]);
            glVertexPointer(3, GL_FLOAT, 0, vertexArrayLeft.begin());
            glTexCoordPointer(2, GL_FLOAT, 0, textureArrayLeft.begin());
            glDrawElements(GL_TRIANGLES, indexArrayLeft.size(), GL_UNSIGNED_BYTE, indexArrayLeft.begin());
        }
        for (int i = 0; i < lines.size(); ++i)
            lines[i].line->drawFigure();

        //////////////////////////////////////////

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        glEnableClientState(GL_COLOR_ARRAY);
    }
    else
    {

        glVertexPointer(3, GL_FLOAT, 0, VertexArray);
        glColorPointer(4, GL_FLOAT, 0, ColorArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, IndexArray);
    }

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



//////////////////////////////////////

void RoadSimple::setIndexArrayForSelectionFrame()
{
    IndexArrayForSelection[0] = 0;
    IndexArrayForSelection[1] = 1;
    IndexArrayForSelection[2] = 2;
    IndexArrayForSelection[3] = 3;


}

void RoadSimple::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    for (int i = 0; i < 4; ++i)
    {
        ColorArrayForSelection[i][0] = red;
        ColorArrayForSelection[i][1] = green;
        ColorArrayForSelection[i][2] = blue;
    }
}

vec2 RoadSimple::getAxisPoint_1()
{
    vec2 res;
    res.x = x1;
    res.y = y1;
    return res;
}

vec2 RoadSimple::getAxisPoint_2()
{
    vec2 res;
    res.x = x2;
    res.y = y2;
    return res;
}

void RoadSimple::setCoordForAxisPoint(int index, float x, float y)
{
    if (index == 0)
    {
        x1 = x;
        y1 = y;
    }
    else
        if(index == 1)
        {
            x2 = x;
            y2 = y;
        }
}

void RoadSimple::setCoordForPoint(int index, float newX, float newY, float newZ)
{
    if (index < 0 || index > 3)
        return;
    VertexArray[index][0] = newX;
    VertexArray[index][1] = newY;
    VertexArray[index][2] = newZ;

    //x1 = (VertexArray[0][0] + VertexArray[1][0]) / 2.0f;
    //y1 = (VertexArray[0][1] + VertexArray[1][1]) / 2.0f;

    // x2 = (VertexArray[2][0] + VertexArray[3][0]) / 2.0f;
    //y2 = (VertexArray[2][1] + VertexArray[3][1]) / 2.0f;
}

vec3 RoadSimple::getCoordOfPoint(int index)
{
    assert(index >= 0 && index <= 3);
    //if (index < 0 || index > 3);
    vec3 res(0.0f, 0.0f, 0.0f);
    res.x = VertexArray[index][0];
    res.y = VertexArray[index][1];
    res.z = VertexArray[index][2];
    return res;
}

void RoadSimple::setDescription(const QString &description)
{
    this->description = description;
}

void RoadSimple::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    if (render && description[0] != '\0')
    {
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        x = x2;
        y = y2;
        z = 0.0f;
        QFont shrift = QFont("Times", 8, QFont::Black);
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, description, shrift);
    }
}





void RoadSimple::drawSelectionFrame()
{
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        //qDebug() << "Index " << indexOfSelectedControl;
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
    }
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glColorPointer(3, GL_FLOAT, 0, ColorArrayForSelection);
    glLineWidth(5.0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);
    glPointSize(10.0);
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x1, y1, VertexArray[1][2]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x2, y2, VertexArray[2][2]);
    glEnd();

    if(showRightBoard)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayRight[12],vertexArrayRight[13],vertexArrayRight[14]);
        glVertex3f(vertexArrayRight[27],vertexArrayRight[28],vertexArrayRight[29]);
        glEnd();
    }
    if(showLeftBoard)
    {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(vertexArrayLeft[12],vertexArrayLeft[13],vertexArrayLeft[14]);
        glVertex3f(vertexArrayLeft[27],vertexArrayLeft[28],vertexArrayLeft[29]);
        glEnd();
    }
    // Угловые точки для изменения размера


    // Точки для вращения



}


void RoadSimple::move(float dx, float dy, float dz)
{
    if (fixed)
    {
        return;
    }
    for (int i = 0; i < 4; ++i)
    {
        VertexArray[i][0] += dx; // X
        VertexArray[i][1] += dy; // Y

    }
    for (int i = 0; i < vertexArrayRight.size() / 3; ++i)
    {
        vertexArrayRight[i * 3] += dx;
        vertexArrayRight[i * 3 + 1] += dy;

        vertexArrayLeft[i * 3] += dx;
        vertexArrayLeft[i * 3 + 1] += dy;
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        lines[i].line->move(dx,dy,dz);
    }
    this->x1 += dx;
    this->y1 += dy;
    this->x2 += dx;
    this->y2 += dy;
}

void RoadSimple::drawControlElement(int index, float lineWidth, float pointSize)
{
    int lineControls = getNumberOfControls() - 12;
    if (index >= lineControls)
    {
        index -= lineControls;

        switch(index)
        {
        case 0:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[0][0],VertexArray[0][1],VertexArray[0][2]);
            glEnd();
            ////qDebug() << "Point 0";
        }
            break;
        case 1:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[1][0],VertexArray[1][1],VertexArray[1][2]);
            glEnd();
            ////qDebug() << "Point 1";
        }
            break;
        case 2:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[2][0],VertexArray[2][1],VertexArray[2][2]);
            glEnd();
            ////qDebug() << "Point 2";
        }
            break;
        case 3:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[3][0],VertexArray[3][1],VertexArray[3][2]);
            glEnd();
            ////qDebug() << "Point 3";
        }
            break;
        case 4:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(x1, y1, VertexArray[1][2]);
            glEnd();
        }
            break;
        case 5:
        {
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(x2, y2, VertexArray[2][2]);
            glEnd();
        }
            break;
        case 6:
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[0][0],
                    VertexArray[0][1],
                    VertexArray[0][2]);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[1][0],
                    VertexArray[1][1],
                    VertexArray[1][2]);
            glEnd();
        }
            break;
        case 7:
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[2][0],
                    VertexArray[2][1],
                    VertexArray[2][2]);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[3][0],
                    VertexArray[3][1],
                    VertexArray[3][2]);
            glEnd();
        }
            break;
        case 8:
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[0][0],
                    VertexArray[0][1],
                    VertexArray[0][2]);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[3][0],
                    VertexArray[3][1],
                    VertexArray[3][2]);
            glEnd();
        }
            break;
        case 9:
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[1][0],
                    VertexArray[1][1],
                    VertexArray[1][2]);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex3f(VertexArray[2][0],
                    VertexArray[2][1],
                    VertexArray[2][2]);
            glEnd();
        }
            break;
        case 10:
        {
            if (showRightBoard)
            {
                glLineWidth(lineWidth);
                glBegin(GL_LINES);
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex3f(vertexArrayRight[12],vertexArrayRight[13],vertexArrayRight[14]);
                glVertex3f(vertexArrayRight[27],vertexArrayRight[28],vertexArrayRight[29]);
                glEnd();
            }
        }
            break;
        case 11:
        {
            if (showLeftBoard)
            {
                glLineWidth(lineWidth);
                glBegin(GL_LINES);
                glColor3f(0.0f, 0.0f, 0.0f);
                glVertex3f(vertexArrayLeft[12],vertexArrayLeft[13],vertexArrayLeft[14]);
                glVertex3f(vertexArrayLeft[27],vertexArrayLeft[28],vertexArrayLeft[29]);
                glEnd();
            }
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
        lines[i].line->drawControlElement(index, lineWidth, pointSize);
    }


}


void RoadSimple::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }

    int lineControls = getNumberOfControls() - 12;
    if (index >= lineControls)
    {
        index -= lineControls;

        switch (index)
        {
        case 0:
        {
            float X1 = VertexArray[0][0];
            float Y1 = VertexArray[0][1];
            float X2 = VertexArray[3][0];
            float Y2 = VertexArray[3][1];
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            //float angle = acos((dx1*dx2 + dy1*dy2) / (r1 * r2));
            //float res = dx1*dy2 - dx2*dy1;
            //if (res < 0)
            //    angle *= -1.0f;
            x1 -= X2;
            y1 -= Y2;
            x2 -= X2;
            y2 -= Y2;
            float tx = x1, ty = y1;
            x1 = tx * cos(angle) - ty * sin(angle);
            y1 = tx * sin(angle) + ty * cos(angle);
            tx = x2;
            ty = y2;
            x2 = tx * cos(angle) - ty * sin(angle);
            y2 = tx * sin(angle) + ty * cos(angle);
            x1 += X2;
            y1 += Y2;
            x2 += X2;
            y2 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
            //emit lengthChanged(length);
        }
            break;
        case 1:
        {
            float X1 = VertexArray[1][0];
            float Y1 = VertexArray[1][1];
            float X2 = VertexArray[2][0];
            float Y2 = VertexArray[2][1];
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            x1 -= X2;
            y1 -= Y2;
            x2 -= X2;
            y2 -= Y2;
            float tx = x1, ty = y1;
            x1 = tx * cos(angle) - ty * sin(angle);
            y1 = tx * sin(angle) + ty * cos(angle);
            tx = x2;
            ty = y2;
            x2 = tx * cos(angle) - ty * sin(angle);
            y2 = tx * sin(angle) + ty * cos(angle);
            x1 += X2;
            y1 += Y2;
            x2 += X2;
            y2 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
        }
            break;
        case 2:
        {
            float X1 = VertexArray[2][0];
            float Y1 = VertexArray[2][1];
            float X2 = VertexArray[1][0];
            float Y2 = VertexArray[1][1];
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            x1 -= X2;
            y1 -= Y2;
            x2 -= X2;
            y2 -= Y2;
            float tx = x1, ty = y1;
            x1 = tx * cos(angle) - ty * sin(angle);
            y1 = tx * sin(angle) + ty * cos(angle);
            tx = x2;
            ty = y2;
            x2 = tx * cos(angle) - ty * sin(angle);
            y2 = tx * sin(angle) + ty * cos(angle);
            x1 += X2;
            y1 += Y2;
            x2 += X2;
            y2 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
        }
            break;
        case 3:
        {
            float X1 = VertexArray[3][0];
            float Y1 = VertexArray[3][1];
            float X2 = VertexArray[0][0];
            float Y2 = VertexArray[0][1];
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            x1 -= X2;
            y1 -= Y2;
            x2 -= X2;
            y2 -= Y2;
            float tx = x1, ty = y1;
            x1 = tx * cos(angle) - ty * sin(angle);
            y1 = tx * sin(angle) + ty * cos(angle);
            tx = x2;
            ty = y2;
            x2 = tx * cos(angle) - ty * sin(angle);
            y2 = tx * sin(angle) + ty * cos(angle);
            x1 += X2;
            y1 += Y2;
            x2 += X2;
            y2 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
        }
            break;
        case 4:
        {
            float X1 = x1;
            float Y1 = y1;
            float X2 = x2;
            float Y2 = y2;
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            x1 -= X2;
            y1 -= Y2;
            float tx = x1, ty = y1;
            x1 = tx * cos(angle) - ty * sin(angle);
            y1 = tx * sin(angle) + ty * cos(angle);
            x1 += X2;
            y1 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
        }
            break;
        case 5:
        {
            float X1 = x2;
            float Y1 = y2;
            float X2 = x1;
            float Y2 = y1;
            float X3 = X1 + dx;
            float Y3 = Y1 + dy;
            float dx1 = X1 - X2;
            float dy1 = Y1 - Y2;
            float r1 = sqrt(dx1*dx1 + dy1*dy1);
            float dx2 = X3 - X2;
            float dy2 = Y3 - Y2;
            float r2 = sqrt(dx2*dx2 + dy2*dy2);
            float pi = 3.14159265f;
            float angle1 = acos(dx1 / r1);
            if (dy1 < 0)
                angle1 = 2.0f * pi - angle1;
            float angle2 = acos(dx2 / r2);
            if (dy2 < 0)
                angle2 = 2.0f * pi - angle2;
            float angle = angle2 - angle1;
            x2 -= X2;
            y2 -= Y2;
            float tx = x2, ty = y2;
            x2 = tx * cos(angle) - ty * sin(angle);
            y2 = tx * sin(angle) + ty * cos(angle);
            x2 += X2;
            y2 += Y2;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->rotate(angle, X2, Y2, 0.0f);
        }
            break;
        case 6:
        {
            // Первый торец
            float dx1 = x1 - x2;
            float dy1 = y1 - y2;
            float dr = (dx * dx1 + dy * dy1) / length;
            x1 += dx1 / length * dr;
            y1 += dy1 / length * dr;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->resizeByControl(0,dx1 / length * dr,dy1 / length * dr,x,y);
            emit lengthChanged(length);
        }
            break;

        case 7:
        {
            // Второй торец
            float dx1 = x2 - x1;
            float dy1 = y2 - y1;
            float dr = (dx * dx1 + dy * dy1) / length;
            x2 += dx1 / length * dr;
            y2 += dy1 / length * dr;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            for (int i = 0; i < lines.size(); ++i)
                lines[i].line->resizeByControl(1,dx1 / length * dr,dy1 / length * dr,x,y);
            emit lengthChanged(length);
        }
            break;

        case 8:
        {
            float dr = ((xP1 - xP2)*dx + (yP1 - yP2)*dy)/
                    sqrt((xP1 - xP2)*(xP1 - xP2) + (yP1 - yP2)*(yP1 - yP2));
            float widthResult = width + dr * 2.0f > 0 ?
                        width + dr * 2.0f :
                        0.1f;
            this->rightWidth += dr;
            this->width += dr;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            //resetLines();
            emit rightWidthChanged(rightWidth);
            emit widthChanged(width);
        }
            break;
        case 9:
        {
            float dr = ((xP2 - xP1)*dx + (yP2 - yP1)*dy)/
                    sqrt((xP2 - xP1)*(xP2 - xP1) + (yP2 - yP1)*(yP2 - yP1));
            float widthResult = width + dr * 2.0f > 0 ?
                        width + dr * 2.0f :
                        0.1f;
            this->leftWidth += dr;
            this->width += dr;
            setVertexArray(x1, y1, x2, y2, width);
            setTextureArray();
            //resetLines();
            emit leftWidthChanged(leftWidth);
            emit widthChanged(width);
        }
            break;
        case 10:
            // Правый
        {
            float dr = ((xP1 - xP2)*dx + (yP1 - yP2)*dy)/
                    sqrt((xP1 - xP2)*(xP1 - xP2) + (yP1 - yP2)*(yP1 - yP2));
            rightBoardWidth += dr;
            setVertexArray(x1,y1,x2,y2,width);
            emit rightBoardWidthChanged(rightBoardWidth);
        }
            break;
        case 11:
        {
            float dr = ((xP2 - xP1)*dx + (yP2 - yP1)*dy)/
                    sqrt((xP2 - xP1)*(xP2 - xP1) + (yP2 - yP1)*(yP2 - yP1));
            leftBoardWidth += dr;
            setVertexArray(x1,y1,x2,y2,width);
            emit leftBoardWidthChanged(leftBoardWidth);
        }
            break;

        default:
            break;
        }
        setTextureArray();
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

}

int RoadSimple::getNumberOfControls()
{
    int roadControls = 12;
    int lineControls = 0;
    for (int i = 0; i < lines.size(); ++i)
        lineControls += lines[i].line->getNumberOfControls();
    return roadControls + lineControls;
}

void RoadSimple::changeColorOfSelectedControl(int index)
{

    indexOfSelectedControl = index;
    //qDebug() << "ROAD CONTROL COLOR CHANGED";
}

QCursor RoadSimple::getCursorForControlElement(int index)
{

        return Qt::CrossCursor;

}

bool RoadSimple::hasPoint(GLfloat x, GLfloat y)
{
    if (x >= VertexArray[0][0] &&
            x <= VertexArray[2][0] &&
            y >= VertexArray[0][1] &&
            y <= VertexArray[2][1])
    {
        return true;
    }
    else
    {
        return false;
    }
}

QPoint RoadSimple::getCoorninateOfPointControl(int index)
{
    QPoint p;
    p.setX(VertexArray[index][0]);
    p.setY(VertexArray[index][1]);

    return p;
}

void RoadSimple::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}


QJsonObject RoadSimple::getJSONInfo()
{
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
        //element["TextureSource"] = textureSource;
        // element["TextureSize"] = textureSize_1;
    }
    element["Width"] = width;

    QJsonArray temp;
    temp.append(QJsonValue(x1));
    temp.append(QJsonValue(y1));
    temp.append(QJsonValue(x2));
    temp.append(QJsonValue(y2));

    element["Vertices"] = temp;

    return element;

}

void RoadSimple::setSelectedStatus(bool status)
{
    selected = status;
    for (int i = 0; i < lines.size(); ++i)
        lines[i].line->setSelectedStatus(status);
}


void RoadSimple::getProperties(QFormLayout *layout, QGLWidget* render)
{
    clearProperties(layout);
    //qDebug() << "Road Simple Properties";
    this->layout = layout;
    this->render = render;
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
    QDoubleSpinBox* lengthSpinBox = new QDoubleSpinBox();
    QCheckBox* fixedCheckBox = new QCheckBox();


    widthSpinBox->setValue(width);
    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));

    lengthSpinBox->setValue(length);
    connect(this, SIGNAL(lengthChanged(double)), lengthSpinBox, SLOT(setValue(double)));
    connect(lengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLength(double)));

    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    layout->addRow("Длина", lengthSpinBox);
    layout->addRow("Ширина", widthSpinBox);
    layout->addRow("Зафиксировать", fixedCheckBox);

    if (render)
    {
        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(lengthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    if (name != "Crosswalk")
    {
        QDoubleSpinBox* rightBoardSpinBox = new QDoubleSpinBox();
        QDoubleSpinBox* leftBoardSpinBox = new QDoubleSpinBox();

        QCheckBox* showRightBoardCheckBox = new QCheckBox();
        QCheckBox* showLeftBoardCheckBox = new QCheckBox();

        rightBoardSpinBox->setValue(rightBoardWidth);
        connect(this, SIGNAL(rightBoardWidthChanged(double)), rightBoardSpinBox, SLOT(setValue(double)));
        connect(rightBoardSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRightBoardWidth(double)));

        leftBoardSpinBox->setValue(leftBoardWidth);
        connect(this, SIGNAL(leftBoardWidthChanged(double)), leftBoardSpinBox, SLOT(setValue(double)));
        connect(leftBoardSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeftBoardWidth(double)));

        showRightBoardCheckBox->setChecked(showRightBoard);
        QObject::connect(showRightBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setRightBoardShowStatus(bool)));

        showLeftBoardCheckBox->setChecked(showLeftBoard);
        QObject::connect(showLeftBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setLeftBoardShowStatus(bool)));

        if (render)
        {
            connect(rightBoardSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(leftBoardSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
            connect(showRightBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
            connect(showLeftBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        }
        layout->addRow("Ширина правого тротуара", rightBoardSpinBox);
        layout->addRow("Ширина левого тротуара", leftBoardSpinBox);

        layout->addRow("Правый тротуар", showRightBoardCheckBox);
        layout->addRow("Левый тротуар", showLeftBoardCheckBox);

        QPushButton *addLineButton = new QPushButton("+");

        //StepDialog *stepDialog = new StepDialog();
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

        ///////////////////////////////////////////////////////////////////////////


        for (int i = 0; i < lines.size(); ++i)
        {
            QPushButton* b = new QPushButton(QString::number(i + 1));
            connect(b, SIGNAL(clicked(bool)), this, SLOT(deleteLine()));
            layout->addRow("Удалить линию ",b);
        }

        layout->addRow("Добавить линию", addLineButton);
    }





}

void RoadSimple::resetLines()
{
    /*
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
    float line_x1, line_x2, line_y1, line_y2;

    */
    // Отсчет от правой стороны

    for (int i = 0; i < lines.size(); ++i)
    {
        /*
        float factor = lines[i].rightSide ? 1 : -1;
        if(lines[i].step < r)
        {
            line_x1 = x1 + factor * dx * (r - lines[i].step)/r;
            line_y1 = y1 - factor * dy * (r - lines[i].step)/r;

            line_x2 = x2 + factor * dx * (r - lines[i].step)/r;
            line_y2 = y2 - factor * dy * (r - lines[i].step)/r;

        }
        else
        {
            line_x1 = x1 - factor * dx * (lines[i].step - r)/r;
            line_y1 = y1 + factor * dy * (lines[i].step - r)/r;

            line_x2 = x2 - factor * dx * (lines[i].step - r)/r;
            line_y2 = y2 + factor * dy * (lines[i].step - r)/r;
        }
        */
        float line_x1, line_x2, line_y1, line_y2;
        float r1;
        if (!lines[i].rightSide)
            r1 = width - lines[i].step;
        else
            r1 = lines[i].step;

        float x0 = VertexArray[0][0];
        float y0 = VertexArray[0][1];
        float x1 = VertexArray[1][0];
        float y1 = VertexArray[1][1];
        float x2 = VertexArray[2][0];
        float y2 = VertexArray[2][1];
        float x3 = VertexArray[3][0];
        float y3 = VertexArray[3][1];


        float x = x0 + (x1 - x0) * r1 / width;
        float y = y0 + (y1 - y0) * r1 / width;

        float R = sqrt((x3 - x0) * (x3 - x0) + (y3 - y0) * (y3 - y0));
        line_x1 = x + (x3 - x0) / R * lines[i].beginStep;
        line_y1 = y + (y3 - y0) / R * lines[i].beginStep;


        x = x3 + (x2 - x3) * r1 / width;
        y = y3 + (y2 - y3) * r1 / width;

        line_x2 = x + (x0 - x3) / R * lines[i].endStep;
        line_y2 = y + (y0 - y3) / R * lines[i].endStep;

        switch (lines[i].lineType)
        {
        case 6:
        {
            SplitZone *splitZone = dynamic_cast<SplitZone*>(lines[i].line);
            float width = splitZone->getWidth();
            vec3 p1(line_x1, line_y1, 0.02f);
            vec3 p2(line_x2, line_y2, 0.02f);
            splitZone->calculateLine(p1,p2,width);
        }
            break;
        case 7:
        {
            float X1, Y1, X2, Y2, X3, Y3, X4, Y4;
            if (!lines[i].rightSide)
            {
                X1 = x1;
                Y1 = y1;
                X2 = x2;
                Y2 = y2;
                X3 = x0;
                Y3 = y0;
                X4 = x3;
                Y4 = y3;
            }
            else
            {
                X1 = x0;
                Y1 = y0;
                X2 = x3;
                Y2 = y3;
                X3 = x1;
                Y3 = y1;
                X4 = x2;
                Y4 = y2;
            }
            float r = sqrt((X2 - X1)*(X2 - X1) + (Y2 - Y1)*(Y2 - Y1));
            float d;
            if (!lines[i].beginSide)
                d = r - lines[i].step;
            else
                d = lines[i].step;
            line_x1 = X1 + (X2 - X1) / r * d;
            line_y1 = Y1 + (Y2 - Y1) / r * d;

            float dr = ((X3 - X1)*(line_x1 - X1) + (Y3 - Y1)*(line_y1 - Y1)) / d;
            dr = d - dr;
            r = sqrt((X4 - X3)*(X4 - X3) + (Y4 - Y3)*(Y4 - Y3));
            line_x2 = X3 + (X4 - X3) / r * dr;
            line_y2 = Y3 + (Y4 - Y3) / r * dr;

            int index = -1;
            for (int i = 0; i < lines.size(); ++i)
            {
                if (lines[i].line->getName() == "SplitZone")
                {
                    index = i;
                    break;
                }
                else
                if (lines[i].differentDirections)
                {
                    index = i;
                    break;
                }
            }

            if (index >= 0)
            {
                float r1;
                if (lines[index].line->getName() == "SplitZone")
                {
                    if (!lines[index].beginSide)
                    {
                        if (!lines[index].rightSide)
                            r1 = width - lines[index].step + lines[index].splitZoneWidth / 2.0f;
                        else
                            r1 = lines[index].step + lines[index].splitZoneWidth / 2.0f;
                    }
                    else
                    {
                        if (!lines[index].rightSide)
                            r1 = width - lines[index].step - lines[index].splitZoneWidth / 2.0f;
                        else
                            r1 = lines[index].step - lines[index].splitZoneWidth / 2.0f;
                    }
                    //qDebug() << "SplitZone founded";

                }
                else
                if (!lines[index].rightSide)
                    r1 = width - lines[index].step;
                else
                    r1 = lines[index].step;
                line_x1 = line_x1 + (line_x2 - line_x1) / width * r1;
                line_y1 = line_y1 + (line_y2 - line_y1) / width * r1;

                for (int i = 0; i < lines.size(); ++i)
                {
                    if (i == index)
                        continue;
                    float r;
                    if (!lines[i].rightSide)
                        r = width - lines[i].step;
                    else
                        r = lines[i].step;
                    if (r >= r1)
                    {
                        float x = line_x1 + (line_x2 - line_x1) / (width - r1) * (r - r1);
                        float y = line_y1 + (line_y2 - line_y1) / (width - r1) * (r - r1);
                        LineSimple *line = dynamic_cast<LineSimple*>(lines[i].line);
                        vec2 p = line->getAxisPoint_2();
                        line->setVertexArray(x, y, p.x, p.y, lines[i].lineWidth);
                        line->setTextureArray();
                    }
                }
            }
            LineSimple *lineSimple = dynamic_cast<LineSimple*>(lines[i].line);
            lineSimple->setVertexArray(line_x1, line_y1, line_x2, line_y2, lines[i].lineWidth);
            lineSimple->setTextureArray();
        }
            break;
        default:
        {
            LineSimple *lineSimple = dynamic_cast<LineSimple*>(lines[i].line);
            lineSimple->setVertexArray(line_x1, line_y1, line_x2, line_y2, lines[i].lineWidth);
            lineSimple->setTextureArray();
        }
            break;
        }



    }

}

void RoadSimple::setWidth(double width)
{
    if (this->width != width)
    {
        float temp = this->width;
        float delta = (width - temp) / 2.0f;
        this->rightWidth += delta;
        this->leftWidth += delta;
        this->width = width;
        setVertexArray(x1, y1, x2, y2, width);
        //resetLines();
        emit widthChanged((double)width);
        emit rightWidthChanged((double)rightWidth);
        emit leftWidthChanged((double)leftWidth);
    }
    else
    {
        return;
    }
}

void RoadSimple::setRightWidth(double width)
{
    if (this->rightWidth != width)
    {
        float temp = this->rightWidth;
        float delta = (width - temp);
        this->width += delta;
        this->rightWidth = width;
        setVertexArray(x1, y1, x2, y2, width);
        //resetLines();
        emit rightWidthChanged((double)rightWidth);
        emit widthChanged((double)width);
    }
    else
    {
        return;
    }
}

void RoadSimple::setLeftWidth(double width)
{
    if (this->leftWidth != width)
    {
        float temp = this->leftWidth;
        float delta = (width - temp);
        this->width += delta;
        this->leftWidth = width;
        setVertexArray(x1, y1, x2, y2, width);
        //resetLines();
        emit leftWidthChanged((double)leftWidth);
        emit widthChanged((double)width);
    }
    else
    {
        return;
    }
}

void RoadSimple::setLength(double length)
{
    if (this->length != length)
    {
        x2 = x1 + (x2 - x1) * (length / this->length);
        y2 = y1 + (y2 - y1) * (length / this->length);
        setVertexArray(x1, y1, x2, y2, width);
        this->length = length;
        //resetLines();
        emit lengthChanged(length);
    }
    else
    {
        return;
    }
}

void RoadSimple::setRightBoardShowStatus(bool status)
{
    showRightBoard = status;
}

void RoadSimple::setLeftBoardShowStatus(bool status)
{
    showLeftBoard = status;
}

void RoadSimple::setRightBoardWidth(double width)
{
    if (rightBoardWidth == width)
        return;
    rightBoardWidth = width;
    setVertexArray(x1,y1,x2,y2,this->width);
    emit rightBoardWidthChanged(width);
}

void RoadSimple::setLeftBoardWidth(double width)
{
    if (leftBoardWidth == width)
        return;
    leftBoardWidth = width;
    setVertexArray(x1,y1,x2,y2,this->width);
    emit leftBoardWidthChanged(width);
}

void RoadSimple::addLine(float step, QString textureSource, float textureSize, float lineWidth, int lineType, bool rightSide, float beginStep, float endStep)
{

    float line_x1, line_x2, line_y1, line_y2;
    float r1;
    if (!rightSide)
        r1 = width - step;
    else
        r1 = step;

    float x0 = VertexArray[0][0];
    float y0 = VertexArray[0][1];
    float x1 = VertexArray[1][0];
    float y1 = VertexArray[1][1];
    float x2 = VertexArray[2][0];
    float y2 = VertexArray[2][1];
    float x3 = VertexArray[3][0];
    float y3 = VertexArray[3][1];


    float x = x0 + (x1 - x0) * r1 / width;
    float y = y0 + (y1 - y0) * r1 / width;

    float R = sqrt((x3 - x0) * (x3 - x0) + (y3 - y0) * (y3 - y0));
    line_x1 = x + (x3 - x0) / R * beginStep;
    line_y1 = y + (y3 - y0) / R * beginStep;


    x = x3 + (x2 - x3) * r1 / width;
    y = y3 + (y2 - y3) * r1 / width;

    line_x2 = x + (x0 - x3) / R * endStep;
    line_y2 = y + (y0 - y3) / R * endStep;

    LineLinked line;

    switch (lineType)
    {
    case 6:
        switch (splitZoneType)
        {
        case 0:
        {
            line.line = new SplitZone(line_x1, line_y1, 0.02f, line_x2, line_y2, 0.02f, splitZoneWidth, beginRounding, endRounding,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            //qDebug() << "SplitZone width:" << splitZoneWidth;
            //qDebug() << "Begin rounding:" << beginRounding;
            //qDebug() << "End rounding:" << endRounding;
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        case 1:
        {
            line.line = new SplitZone(line_x1, line_y1, 0.02f,
                                      line_x2, line_y2, 0.02f,
                                      splitZoneWidth,
                                      beginRounding,
                                      endRounding,
                                      splitZoneType,
                                      splitZoneHeight,
                                      QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                      0.25f, 6.0f,
                                      QApplication::applicationDirPath() + "/models/city_roads/grass.jpg",
                                      3.0f, 3.0f,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        case 2:
        {
            line.line = new SplitZone(line_x1, line_y1, 0.02f,
                                      line_x2, line_y2, 0.02f,
                                      splitZoneWidth,
                                      beginRounding,
                                      endRounding,
                                      splitZoneType,
                                      splitZoneHeight,
                                      QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                      0.25f, 6.0f,
                                      QApplication::applicationDirPath() + "/models/city_roads/nr_07S.jpg",
                                      6.0f, 6.0f,
                                      QString("Линия №") + QString::number(lines.size() + 1));
            line.splitZoneWidth = splitZoneWidth;
        }
            break;
        default:
            break;
        }
        line.splitZoneWidth = splitZoneWidth;

        break;
    case 7:
    {
        float X1, Y1, X2, Y2, X3, Y3, X4, Y4;
        if (!rightSide)
        {
            X1 = x1;
            Y1 = y1;
            X2 = x2;
            Y2 = y2;
            X3 = x0;
            Y3 = y0;
            X4 = x3;
            Y4 = y3;
        }
        else
        {
            X1 = x0;
            Y1 = y0;
            X2 = x3;
            Y2 = y3;
            X3 = x1;
            Y3 = y1;
            X4 = x2;
            Y4 = y2;
        }
        float r = sqrt((X2 - X1)*(X2 - X1) + (Y2 - Y1)*(Y2 - Y1));
        float d;
        if (!beginSide)
            d = r - step;
        else
            d = step;
        line_x1 = X1 + (X2 - X1) / r * d;
        line_y1 = Y1 + (Y2 - Y1) / r * d;

        float dr = ((X3 - X1)*(line_x1 - X1) + (Y3 - Y1)*(line_y1 - Y1)) / d;
        dr = d - dr;
        r = sqrt((X4 - X3)*(X4 - X3) + (Y4 - Y3)*(Y4 - Y3));
        line_x2 = X3 + (X4 - X3) / r * dr;
        line_y2 = Y3 + (Y4 - Y3) / r * dr;

        int index = -1;
        for (int i = 0; i < lines.size(); ++i)
        {

            if (lines[i].line->getName() == "SplitZone")
            {
                index = i;
                break;
            }
            else
            if (lines[i].differentDirections)
            {
                index = i;
                break;
            }
        }

        if (index >= 0)
        {
            float r1;
            if (lines[index].line->getName() == "SplitZone")
            {
                if (beginSide)
                {
                    if (!lines[index].rightSide)
                        r1 = width - lines[index].step + lines[index].splitZoneWidth / 2.0f;
                    else
                        r1 = lines[index].step + lines[index].splitZoneWidth / 2.0f;
                }
                else
                {
                    if (!lines[index].rightSide)
                        r1 = width - lines[index].step - lines[index].splitZoneWidth / 2.0f;
                    else
                        r1 = lines[index].step - lines[index].splitZoneWidth / 2.0f;
                }
                //qDebug() << "SplitZone founded";

            }
            else
            if (!lines[index].rightSide)
                r1 = width - lines[index].step;
            else
                r1 = lines[index].step;
            line_x1 = line_x1 + (line_x2 - line_x1) / width * r1;
            line_y1 = line_y1 + (line_y2 - line_y1) / width * r1;

            for (int i = 0; i < lines.size(); ++i)
            {
                if (i == index)
                    continue;
                float r;
                if (!lines[i].rightSide)
                    r = width - lines[i].step;
                else
                    r = lines[i].step;
                if (r >= r1)
                {
                    float x = line_x1 + (line_x2 - line_x1) / (width - r1) * (r - r1);
                    float y = line_y1 + (line_y2 - line_y1) / (width - r1) * (r - r1);
                    LineSimple *line = dynamic_cast<LineSimple*>(lines[i].line);
                    vec2 p = line->getAxisPoint_2();
                    line->setVertexArray(x, y, p.x, p.y, lines[i].lineWidth);
                    line->setTextureArray();
                }
            }
        }
        line.line = new LineSimple(line_x1, line_y1, line_x2, line_y2, lineWidth, textureSource, textureSize, "LineSimple", 1,
                                   QString("Линия №") + QString::number(lines.size() + 1));
    }
        break;
        /*
    case 8:
    {
        line.line = new LineSimple(line_x1, line_y1, line_x2, line_y2, lineWidth, textureSource, textureSize, "LineSimple", 1,
                                   QString("Линия №") + QString::number(lines.size() + 1));
    }
        break;
        */
    default:
        line.line = new LineSimple(line_x1, line_y1, line_x2, line_y2, lineWidth, textureSource, textureSize, "LineSimple", 1,
                                   QString("Линия №") + QString::number(lines.size() + 1));
        break;
    }
    line.lineWidth = lineWidth;
    line.step = step;
    line.rightSide = rightSide;
    line.lineType = lineType;
    line.beginStep = beginStep;
    line.endStep = endStep;
    line.beginSide = beginSide;
    line.differentDirections = differentDirections;
    line.line->setSelectedStatus(false);
    lines.push_back(line);
    model->getGroup(1).push_back(line.line);
    if (this->layout && this->render)
        emit linesChanged(layout, render);
}

void RoadSimple::addLine()
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
    case 7:
        textSource = QApplication::applicationDirPath() + "/models/city_roads/solid.png";
        lWidth = 0.4f;
        break;
    case 8:
    {
        textSource = QString(":/textures/tramways.png");
        lWidth = 1.5f;
        if (!singleWay)
        {
            addLine(step + axisStep / 2.0, textSource, 1.5f, lWidth, lineType, rightSide, beginStep, endStep);
            addLine(step - axisStep / 2.0, textSource, 1.5f, lWidth, lineType, rightSide, beginStep, endStep);
            return;
        }
        else
        {
            addLine(step, textSource, 1.5f, lWidth, lineType, rightSide, beginStep, endStep);
            return;
        }
    }
        break;
    default:
        break;

    }

    addLine(step, textSource, 6.0f, lWidth, lineType, rightSide, beginStep, endStep);
    //LineSimple *line = new LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer)
}



void RoadSimple::setRightSide(bool status)
{
    rightSide = status;
    ////qDebug() << "Road right side " << status;
}

void RoadSimple::setBeginSide(bool status)
{
    beginSide = status;
}

void RoadSimple::setBeginRounding(bool status)
{
    beginRounding = status;
    //qDebug() << "Begin rounding" << beginRounding;
}

void RoadSimple::setEndRounding(bool status)
{
    endRounding = status;
    //qDebug() << "End rounding" << endRounding;
}

void RoadSimple::setDifferentDirections(bool status)
{
    differentDirections = status;
    //qDebug() << "RoadSimple::setDifferentDirections";
}

void RoadSimple::setStep(double value)
{
    step = value;
    ////qDebug() << "Road step " << step;
}

void RoadSimple::setLineType(int type)
{
    lineType = type;
    ////qDebug() << "Road type " << type;
}

void RoadSimple::deleteLine()
{
    QPushButton * b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    ////qDebug() << "delete line " << b->text();
    int i = b->text().toInt() - 1;
    for (std::list<RoadElement*>::iterator it = model->getGroup(1).begin();
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
            LineSimple* line = dynamic_cast<LineSimple*>(lines[i].line);
            line->setDescription(QString("Линия №") + QString::number(i + 1));
        }
        else
        {
            SplitZone* line = dynamic_cast<SplitZone*>(lines[i].line);
            line->setDescription(QString("Линия №") + QString::number(i + 1));
        }
    }
    if (this->layout && this->render)
        emit linesChanged(layout, render);
}

void RoadSimple::setBeginStep(double step)
{
    beginStep = step;
    //qDebug() << "Begin step";
}

void RoadSimple::setEndStep(double step)
{
    endStep = step;
    //qDebug() << "End step";
}

void RoadSimple::setSplitZoneWidth(double value)
{
    splitZoneWidth = value;
}

void RoadSimple::setSingleWay(bool status)
{
    singleWay = status;
}

void RoadSimple::setAxisStep(double step)
{
    axisStep = step;
}

void RoadSimple::setSplitZoneType(int type)
{
    splitZoneType = type;
}

void RoadSimple::setSplitZoneHeight(double height)
{
    splitZoneHeight = height;
}


bool RoadSimple::isFixed()
{
    return fixed;
}


void RoadSimple::drawMeasurements(QGLWidget *render)
{
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Bold);
    float x1, x2, y1, y2;

    // Ширина полосы
    x1 = VertexArray[2][0];
    y1 = VertexArray[2][1];
    x2 = VertexArray[3][0];
    y2 = VertexArray[3][1];
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    glColor3f(0.0f, 0.0f, 0.0f);
    float dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, "W=" + QString("%1").arg(dr, 0, 'f', 2), shrift);

    // Длина полосы
    x1 = VertexArray[0][0];
    y1 = VertexArray[0][1];
    x2 = VertexArray[3][0];
    y2 = VertexArray[3][1];
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    glColor3f(0.0f, 0.0f, 0.0f);
    dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, "L=" + QString("%1").arg(dr, 0, 'f', 2), shrift);

    if (description != "\0")
    {
        x1 = VertexArray[1][0];
        y1 = VertexArray[1][1];
        x2 = VertexArray[2][0];
        y2 = VertexArray[2][1];
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        glColor3f(0.0f, 0.0f, 0.0f);
        dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, "L=" + QString("%1").arg(dr, 0, 'f', 2), shrift);
    }

    if (showRightBoard && description == "\0")
    {
        // Ширина правого тротуара
        x1 = vertexArrayRight[0];
        y1 = vertexArrayRight[1];
        x2 = vertexArrayRight[12];
        y2 = vertexArrayRight[13];
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        glColor3f(0.0f, 0.0f, 0.0f);
        dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy, "Правый тротуар: W=" + QString("%1").arg(dr, 0, 'f', 2), shrift);
    }

    if (showLeftBoard && description == "\0")
    {
        // Ширина левого тротуара
        x1 = vertexArrayLeft[0];
        y1 = vertexArrayLeft[1];
        x2 = vertexArrayLeft[12];
        y2 = vertexArrayLeft[13];
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        glColor3f(0.0f, 0.0f, 0.0f);
        dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy, "Левый тротуар: W=" + QString("%1").arg(dr, 0, 'f', 2), shrift);
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        if (lines[i].lineType != 6)
        {
            LineSimple* line = dynamic_cast<LineSimple*>(lines[i].line);
            line->drawDescription(render);
        }
        else
        {
            SplitZone* line = dynamic_cast<SplitZone*>(lines[i].line);
            line->drawDescription(render);
        }
    }
}


bool RoadSimple::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int RoadSimple::getLayer()
{
    return layer;
}


void RoadSimple::clear()
{
    //qDebug() << "RoadSimple::clear()";
}

float RoadSimple::getRightBoardWidth()
{
    return rightBoardWidth;
}

float RoadSimple::getLeftBoardWidth()
{
    return leftBoardWidth;
}


std::vector<vec3> RoadSimple::getCoordOfControl(int index)
{
    std::vector<vec3> res;
    switch(index)
    {
    case 4:
    {
        vec3 p(VertexArray[0][0],
                VertexArray[0][1],
                VertexArray[0][2]);
        vec3 s(VertexArray[1][0],
                VertexArray[1][1],
                VertexArray[1][2]);
        res.push_back(p);
        res.push_back(s);

    }
        break;
    case 5:
    {
        vec3 p(VertexArray[2][0],
                VertexArray[2][1],
                VertexArray[2][2]);
        vec3 s(VertexArray[3][0],
                VertexArray[3][1],
                VertexArray[3][2]);
        res.push_back(p);
        res.push_back(s);
    }
        break;
    case 6:
    {
        vec3 p(VertexArray[0][0],
                VertexArray[0][1],
                VertexArray[0][2]);
        vec3 s(VertexArray[3][0],
                VertexArray[3][1],
                VertexArray[3][2]);
        res.push_back(p);
        res.push_back(s);
    }
        break;
    case 7:
    {
        vec3 p(VertexArray[1][0],
                VertexArray[1][1],
                VertexArray[1][2]);
        vec3 s(VertexArray[2][0],
                VertexArray[2][1],
                VertexArray[2][2]);
        res.push_back(p);
        res.push_back(s);
    }
        break;
    case 8:
    {
        if (showRightBoard)
        {
            vec3 p(vertexArrayRight[12],vertexArrayRight[13],vertexArrayRight[14]);
            vec3 s(vertexArrayRight[27],vertexArrayRight[28],vertexArrayRight[29]);
            res.push_back(p);
            res.push_back(s);
        }
    }
        break;
    case 9:
    {
        if (showLeftBoard)
        {
            vec3 p(vertexArrayLeft[12],vertexArrayLeft[13],vertexArrayLeft[14]);
            vec3 s(vertexArrayLeft[27],vertexArrayLeft[28],vertexArrayLeft[29]);
            res.push_back(p);
            res.push_back(s);
        }
    }
        break;
    case 0:
    {
        vec3 p(VertexArray[0][0],VertexArray[0][1],VertexArray[0][2]);
        res.push_back(p);
    }
        break;
    case 1:
    {
        vec3 p(VertexArray[1][0],VertexArray[1][1],VertexArray[1][2]);
        res.push_back(p);
    }
        break;
    case 2:
    {
        vec3 p(VertexArray[2][0],VertexArray[2][1],VertexArray[2][2]);
        res.push_back(p);
    }
        break;
    case 3:
    {
        vec3 p(VertexArray[3][0],VertexArray[3][1],VertexArray[3][2]);
        res.push_back(p);
    }
        break;
    default:
        break;
    }
    return res;
}


void RoadSimple::clearProperties(QLayout *layout)
{
    /*
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
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
    */
    disconnect(stepDialog, 0, this, 0);
}


void RoadSimple::deleteLine(RoadElement *line)
{
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
