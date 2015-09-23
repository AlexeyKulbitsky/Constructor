#include "road.h"
#include <math.h>
#include <QtGui>

Road::Road()
{
    setVertexArray(0.0, 0.0, 1.0);
    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();

    // Рaмка для выбора фигуры
    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    getTextures();
    fixed = false;

}
Road::Road(float x, float y, float size)
{
    this->size = size;
    this->red = 0.5f;
    this->green = 0.5f;
    this->blue = 0.5f;
    this->alpha = 1.0f;

    setVertexArray(x, y, size);
    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    getTextures();
    fixed = false;
}

Road::Road(float x, float y, float size, float red, float green, float blue, float alpha)
{
    this->size = size;
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;

    setVertexArray(x, y, size);
    setColorArray(red, green, blue, alpha);
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    getTextures();
    fixed = false;
}

// Индексы вершины для отрисовки
void Road::setVertexArray(float x, float y, float size)
{
    this->getTextures();

    VertexArray[0][0]=x-size;
    VertexArray[0][1]=y-size;
    VertexArray[0][2]=0.01f;
    TextureArray[0][0] = 0.0f;
    TextureArray[0][1] = 0.0f;

    VertexArray[1][0]=x-size;
    VertexArray[1][1]=y+size;
    VertexArray[1][2]=0.01f;
    TextureArray[1][0] = 0.0f;
    TextureArray[1][1] = 1.0f;

    VertexArray[2][0]=x+size;
    VertexArray[2][1]=y+size;
    VertexArray[2][2]=0.01f;
    TextureArray[2][0] = 1.0f;
    TextureArray[2][1] = 1.0f;

    VertexArray[3][0]=x+size;
    VertexArray[3][1]=y-size;
    VertexArray[3][2]=0.01f;
    TextureArray[3][0] = 1.0f;
    TextureArray[3][1] = 0.0f;
}

// Индексы каждой вершины
void Road::setColorArray(float red, float green, float blue, float alpha)
{
   for (int i=0; i<4; i++)
   {
      ColorArray[i][0]=red;
      ColorArray[i][1]=green;
      ColorArray[i][2]=blue;
      ColorArray[i][3]=alpha;
   }
}

// Индексы для отрисовки фигуры
void Road::setIndexArray()
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


}

void Road::drawFigure(QGLWidget* render)
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
    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(red, green, blue, alpha);

    }
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    //glColorPointer(4, GL_FLOAT, 0, ColorArray);
    glTexCoordPointer(2, GL_FLOAT, 0, TextureArray);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, IndexArray);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);

}





void Road::setIndexArrayForSelectionFrame()
{
    IndexArrayForSelection[0] = 0;
    IndexArrayForSelection[1] = 1;
    IndexArrayForSelection[2] = 2;
    IndexArrayForSelection[3] = 3;
}

void Road::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    for (int i = 0; i < 4; ++i)
    {
        ColorArrayForSelection[i][0] = red;
        ColorArrayForSelection[i][1] = green;
        ColorArrayForSelection[i][2] = blue;
    }
}



void Road::drawSelectionFrame()
{
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glColorPointer(3, GL_FLOAT, 0, ColorArrayForSelection);
    glLineWidth(2.0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);

    // Угловые точки для изменения размера
    glPointSize(7.0);
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);
}


void Road::move(float dx, float dy, float dz)
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
}

void Road::drawControlElement(int index, float lineWidth, float pointSize)
{
    // Рисуем первые 4 точки для изменения размера
    if (index < 4)
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
         glColor3d(1.0, 1.0, 0.1);
           glVertex3d(VertexArray[index][0],
           VertexArray[index][1],
           VertexArray[index][2]);

          glEnd();
    }
    else
    {
        if (index < 7)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
              glColor3d(1.0, 1.0, 0.1);
               glVertex3d(VertexArray[index - 4][0],
                            VertexArray[index - 4][1],
                             VertexArray[index - 4][2]);
               glColor3d(1.0, 1.0, 0.1);
               glVertex3d(VertexArray[index - 3][0],
                            VertexArray[index - 3][1],
                            VertexArray[index - 3][2]);
            glEnd();
        }
        else
        {
            if (index == 7)
            {
                glLineWidth(lineWidth);
                glBegin(GL_LINES);
                  glColor3d(1.0, 1.0, 0.1);
                   glVertex3d(VertexArray[index - 4][0],
                                VertexArray[index - 4][1],
                                 VertexArray[index - 4][2]);
                   glColor3d(1.0, 1.0, 0.1);
                   glVertex3d(VertexArray[0][0],
                                VertexArray[0][1],
                                VertexArray[0][2]);
                glEnd();
            }

        }
    }
}


void Road::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    switch (index)
    {
    case 0:

        VertexArray[0][0] += dx;
        VertexArray[0][1] += dy;

       // VertexArray[1][0] += dx;

       // VertexArray[3][1] += dy;
        break;

    case 1:
        VertexArray[1][0] += dx;
        VertexArray[1][1] += dy;

       // VertexArray[0][0] += dx;

       // VertexArray[2][1] += dy;
        break;
    case 2:
        VertexArray[2][0] += dx;
        VertexArray[2][1] += dy;

       // VertexArray[3][0] += dx;

       // VertexArray[1][1] += dy;
        break;
    case 3:
        VertexArray[3][0] += dx;
        VertexArray[3][1] += dy;

       // VertexArray[2][0] += dx;

       // VertexArray[0][1] += dy;
        break;


    case 4:
        VertexArray[0][0] += dx;

        VertexArray[1][0] += dx;
        break;
    case 5:
        VertexArray[1][1] += dy;

        VertexArray[2][1] += dy;
        break;
    case 6:
        VertexArray[2][0] += dx;

        VertexArray[3][0] += dx;
        break;
    case 7:
        VertexArray[3][1] += dy;

        VertexArray[0][1] += dy;
        break;
    default:
        break;
    }
}

void Road::changeColorOfSelectedControl(int index)
{

    indexOfSelectedControl = index;
    qDebug() << "ROAD CONTROL COLOR CHANGED";
}

QCursor Road::getCursorForControlElement(int index)
{
    switch (index)
    {
    // Углы для изменения размера
    case 0:
        return Qt::CrossCursor;
    case 1:
        return Qt::CrossCursor;
    case 2:
        return Qt::CrossCursor;
    case 3:
        return Qt::CrossCursor;

    // Стороны для изменения размера
    case 4:
        return Qt::SizeHorCursor;
    case 5:
        return Qt::SizeVerCursor;
    case 6:
        return Qt::SizeHorCursor;
    case 7:
        return Qt::SizeVerCursor;
    default:
        return Qt::ArrowCursor;
    }
}

bool Road::hasPoint(GLfloat x, GLfloat y)
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

QPoint Road::getCoorninateOfPointControl(int index)
{
    QPoint p;
    p.setX(VertexArray[index][0]);
    p.setY(VertexArray[index][1]);

    return p;
}

void Road::getTextures()
{
    QImage image1;

    image1.load("D://texture.png");
    image1 = QGLWidget::convertToGLFormat(image1);
    glGenTextures(1, textureID);
    // создаём и связываем 1-ый текстурный объект с последующим состоянием текстуры
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    // связываем текстурный объект с изображением
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());

    // задаём линейную фильтрацию вблизи:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // задаём линейную фильтрацию вдали:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}




void Road::getProperties(QFormLayout *layout, QGLWidget* render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}


bool Road::isFixed()
{
    return fixed;
}


void Road::drawMeasurements(QGLWidget *render)
{
}


bool Road::setFixed(bool fixed)
{
    this->fixed = fixed;
    return true;
}


int Road::getLayer()
{
    return layer;
}


void Road::clear()
{
}
