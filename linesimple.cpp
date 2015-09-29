#include "linesimple.h"
#include <GL/glu.h>


LineSimple::LineSimple()
{
    //setVertexArray(0.0, 0.0, 1.0);
    //setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    ///setIndexArray();

    // Рвмка для выбора фигуры
    //setIndexArrayForSelectionFrame();
    //setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    //selected = false;
    fixed = false;
    this->description = '\0';

}
LineSimple::LineSimple(float x1, float y1, float x2, float y2, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    useColor = true;
    //this->size = size;
    width = 1.0f;
    setVertexArray(x1, y1, x2, y2, width);
    setColorArray(0.5f, 0.5f, 0.5f, 1.0f);
    setIndexArray();
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;
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
    this->description = '\0';
}

LineSimple::LineSimple(float x1, float y1, float x2, float y2, float width, float red, float green, float blue, float alpha, QString name,int layer)
{
    this->layer = layer;
    this->name = name;
    useColor = true;
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
    this->description = '\0';
}

LineSimple::LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer)
{
    qDebug() << "LineSimple";
    this->layer = layer;
    this->name = name;
    this->textureSize = textureSize;
    textureSource = source;
    useColor = false;
    this->size = size;
    this->width = width;

    setVertexArray(x1, y1, x2, y2, width);
    //getTextures(source);
    textureID[0] = TextureManager::getInstance()->getID(source);
    setTextureArray();
    setIndexArray();
    qDebug() << "Texture binded";
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    this->description = '\0';
}

LineSimple::LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer, QString description)
{
    qDebug() << "LineSimple";
    this->layer = layer;
    this->name = name;
    this->textureSize = textureSize;
    textureSource = source;
    useColor = false;
    this->size = size;
    this->width = width;

    setVertexArray(x1, y1, x2, y2, width);
    //getTextures(source);
    textureID[0] = TextureManager::getInstance()->getID(source);
    setTextureArray();
    setIndexArray();
    qDebug() << "Texture binded";
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;

    this->description = description;
}


// Индексы вершины для отрисовки
void LineSimple::setVertexArray(float x1, float y1, float x2, float y2, float width)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->width = width;
    this->length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
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
    VertexArray[0][0]=x1 + dx;
    VertexArray[0][1]=y1 - dy;
    VertexArray[0][2]=0.001f;

    VertexArray[1][0]=x1 - dx;
    VertexArray[1][1]=y1 + dy;
    VertexArray[1][2]=0.001f;

    VertexArray[2][0]=x2 - dx;
    VertexArray[2][1]=y2 + dy;
    VertexArray[2][2]=0.001f;

    VertexArray[3][0]=x2 + dx;
    VertexArray[3][1]=y2 - dy;
    VertexArray[3][2]=0.001f;
}

// Индексы каждой вершины
void LineSimple::setColorArray(float red, float green, float blue, float alpha)
{
   for (int i=0; i<4; i++)
   {
      ColorArray[i][0]=red;
      ColorArray[i][1]=green;
      ColorArray[i][2]=blue;
      ColorArray[i][3]=alpha;
   }
}

void LineSimple::setTextureArray()
{
    GLfloat x1 = VertexArray[0][0];
    GLfloat y1 = VertexArray[0][1];
    GLfloat x2 = VertexArray[3][0];
    GLfloat y2 = VertexArray[3][1];
    GLfloat r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));
    GLfloat delta = r / textureSize;

    TextureArray[0][0] = 1.0f;
    TextureArray[0][1] = 0.0f;

    TextureArray[1][0] = 0.0f;
    TextureArray[1][1] = 0.0f;

    TextureArray[2][0] = 0.0f;
    TextureArray[2][1] = delta;

    TextureArray[3][0] = 1.0f;
    TextureArray[3][1] = delta;
}

// Индексы для отрисовки фигуры
void LineSimple::setIndexArray()
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

void LineSimple::getTextures(QString source)
{
    QImage image1;

    image1.load(source);
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
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // задаём: при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // задаём: цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    qDebug() << "Line texture: " << textureID[0];
}

void LineSimple::drawFigure(QGLWidget* render)
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
       // glColor3d(0.3, 0.7, 0.1);


    }
    else
    {
        // Если фигуры не выбрана - цвет заливки по умолчанию
        setColorArray(red, green, blue, alpha);

    }
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


}



//////////////////////////////////////

void LineSimple::setIndexArrayForSelectionFrame()
{
    IndexArrayForSelection[0] = 0;
    IndexArrayForSelection[1] = 1;
    IndexArrayForSelection[2] = 2;
    IndexArrayForSelection[3] = 3;

    IndexArrayForSelection[4] = 4;
    IndexArrayForSelection[5] = 5;
    IndexArrayForSelection[6] = 6;
    IndexArrayForSelection[7] = 7;

}

void LineSimple::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    for (int i = 0; i < 4; ++i)
    {
        ColorArrayForSelection[i][0] = red;
        ColorArrayForSelection[i][1] = green;
        ColorArrayForSelection[i][2] = blue;
    }
}



void LineSimple::drawSelectionFrame()
{
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < 2)
    {
        //qDebug() << "Index " << indexOfSelectedControl;
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
    }
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glColorPointer(3, GL_FLOAT, 0, ColorArrayForSelection);
    glLineWidth(2.0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);

    // Угловые точки для изменения размера
    glPointSize(5.0);
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);

    // Точки для вращения

}


void LineSimple::move(float dx, float dy, float dz)
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
    this->x1 += dx;
    this->y1 += dy;
    this->x2 += dx;
    this->y2 += dy;

}

void LineSimple::drawControlElement(int index, float lineWidth, float pointSize)
{
    switch (index)
    {
    case 0:
    {
        /*
        glLineWidth(lineWidth + 5.0);
        glBegin(GL_LINES);
          glColor3d(1.0, 1.0, 0.1);
           glVertex3d(VertexArray[0][0],
                        VertexArray[0][1],
                         VertexArray[0][2]);
           glColor3d(1.0, 1.0, 0.1);
           glVertex3d(VertexArray[1][0],
                        VertexArray[1][1],
                        VertexArray[1][2]);
        glEnd();
        */
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x1, y1, VertexArray[0][2]);
        glEnd();
    }
        break;
    case 1:
    {
        /*
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
          glColor3d(1.0, 1.0, 0.1);
           glVertex3d(VertexArray[2][0],
                        VertexArray[2][1],
                         VertexArray[2][2]);
           glColor3d(1.0, 1.0, 0.1);
           glVertex3d(VertexArray[3][0],
                        VertexArray[3][1],
                        VertexArray[3][2]);
        glEnd();
        */
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x2, y2, VertexArray[3][2]);
        glEnd();
    }
        break;
    case 2:
    {
        /*
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
          glColor3d(1.0, 1.0, 0.1);
           glVertex3d(x1, y1, VertexArray[2][2]);
           glColor3d(1.0, 1.0, 0.1);
           glVertex3d(x2,y2,VertexArray[3][2]);
        glEnd();
        */
    }
        break;
    default:
        break;
    }

    /*
    if (index == 0)

    else
    {
        if (index == 1)

        else
        {   /*

            if (index == 2)
            {
                glLineWidth(lineWidth);
                glBegin(GL_LINES);
                  glColor3d(1.0, 1.0, 0.1);
                   glVertex3d(VertexArray[0][0],
                                VertexArray[0][1],
                                 VertexArray[0][2]);
                   glColor3d(1.0, 1.0, 0.1);
                   glVertex3d(VertexArray[3][0],
                                VertexArray[3][1],
                                VertexArray[3][2]);
                glEnd();
            }
            else
            {
                if (index == 3)
                {
                    glLineWidth(lineWidth);
                    glBegin(GL_LINES);
                      glColor3d(1.0, 1.0, 0.1);
                       glVertex3d(VertexArray[1][0],
                                    VertexArray[1][1],
                                     VertexArray[1][2]);
                       glColor3d(1.0, 1.0, 0.1);
                       glVertex3d(VertexArray[2][0],
                                    VertexArray[2][1],
                                    VertexArray[2][2]);
                    glEnd();
                }
            }

        */
    //    }
  //  }

}


void LineSimple::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    switch (index)
    {
    case 0:

       x1 += dx;
       y1 += dy;
        setVertexArray(x1, y1, x2, y2, width);
        emit lengthChanged(length);
        break;

    case 1:
        x2 += dx;
        y2 += dy;
         setVertexArray(x1, y1, x2, y2, width);
         emit lengthChanged(length);
        break;

    case 2:
    {
        /*
        float dr = sqrt(dx * dx + dy * dy);
        float res = dx * (x2 - x1) + dy * (y2 - y1);
        float factor = res < 0 ? -1 : 1;
        setVertexArray(x1, y1, x2, y2, width + dr * factor);
        */
    }
        break;
    case 3:
    {
        float dr = sqrt(dx * dx + dy * dy);
        float res = dx * (x2 - x1) + dy * (y2 - y1);
        float factor = res < 0 ? -1 : 1;
        setVertexArray(x1, y1, x2, y2, width + dr * factor);
    }
        break;
   default:
       break;
    }
     setTextureArray();
}

void LineSimple::changeColorOfSelectedControl(int index)
{

    indexOfSelectedControl = index;
    qDebug() << "ROAD CONTROL COLOR CHANGED";
}

QCursor LineSimple::getCursorForControlElement(int index)
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
    default:
        return Qt::ArrowCursor;
    }
}

bool LineSimple::hasPoint(GLfloat x, GLfloat y)
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

QPoint LineSimple::getCoorninateOfPointControl(int index)
{
    QPoint p;
    p.setX(VertexArray[index][0]);
    p.setY(VertexArray[index][1]);

    return p;
}

void LineSimple::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

void LineSimple::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    if (render && description[0] != '\0')
    {
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        x = (x1 + x2) / 2.0f;
        y = (y1 + y2) / 2.0f;
        z = 0.0f;
        QFont shrift = QFont("Times", 8, QFont::Black);
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, description, shrift);
    }
}


QJsonObject LineSimple::getJSONInfo()
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
        element["TextureSource"] = textureSource;
        element["TextureSize"] = textureSize;
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


void LineSimple::getProperties(QFormLayout *layout, QGLWidget* render)
{

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
    if (render)
    {
        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(lengthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    }

    layout->addRow("Длина", lengthSpinBox);
    layout->addRow("Ширина", widthSpinBox);
    layout->addRow("Зафиксировать", fixedCheckBox);
}

void LineSimple::setWidth(double width)
{
    if (this->width != width)
    {
        this->width = width;
        setVertexArray(x1, y1, x2, y2, width);
        emit widthChanged((double)width);
    }
    else
    {
        return;
    }
}

void LineSimple::setLength(double length)
{
    if (this->length != length)
    {
        x2 = x1 + (x2 - x1) * (length / this->length);
        y2 = y1 + (y2 - y1) * (length / this->length);
        setVertexArray(x1, y1, x2, y2, width);
        this->length = length;
        emit lengthChanged(length);
    }
    else
    {
        return;
    }
}

void LineSimple::setDescription(QString description)
{
    this->description = description;
}


bool LineSimple::isFixed()
{
    return fixed;
}


void LineSimple::drawMeasurements(QGLWidget *render)
{
}


bool LineSimple::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int LineSimple::getLayer()
{
    return layer;
}


void LineSimple::clear()
{
}


std::vector<vec3> LineSimple::getCoordOfControl(int index)
{
    std::vector<vec3> res;

    switch (index)
    {
    case 0:
    {
        vec3 p(x1,y1,VertexArray[0][2]);
        res.push_back(p);
    }
        break;
    case 1:
    {
        vec3 p(x2,y2,VertexArray[2][2]);
        res.push_back(p);
    }
        break;
    case 2:
    {
        /*
        vec3 p(x1, y1, VertexArray[2][2]);
        vec3 s(x2,y2,VertexArray[3][2]);
        res.push_back(p);
        res.push_back(s);
        */
    }

        break;
    default:
        break;
    }
    return res;
}
