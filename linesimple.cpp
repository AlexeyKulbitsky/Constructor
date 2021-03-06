#include "linesimple.h"
#include <GL/glu.h>

bool LineSimple::log = true;

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
    indexOfSelectedControl = -1;
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
    indexOfSelectedControl = -1;
    this->description = '\0';
}

LineSimple::LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer)
{
    this->layer = layer;
    this->name = name;
    this->textureSize = textureSize;
    textureSource = source;
    useColor = false;
    this->size = size;
    this->width = width;

    setVertexArray(x1, y1, x2, y2, width);    
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + source);
    setTextureArray();
    setIndexArray();
    //qDebug() << "Texture binded";
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = false;
    fixed = false;
    indexOfSelectedControl = -1;
    this->description = '\0';
}

LineSimple::LineSimple(float x1, float y1, float x2, float y2, float width, QString source, float textureSize, QString name, int layer, QString description)
{
    //qDebug() << "LineSimple";
    this->layer = layer;
    this->name = name;
    this->textureSize = textureSize;
    textureSource = source;
    useColor = false;
    this->size = size;
    this->width = width;

    setVertexArray(x1, y1, x2, y2, width);
    textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + source);
    setTextureArray();
    setIndexArray();
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);
    indexOfSelectedControl = -1;
    selected = false;
    fixed = false;

    this->description = description;
}

LineSimple::LineSimple(const LineSimple &source)
{
    layer = source.layer;
    name = source.name;
    textureSize = source.textureSize;
    textureSource = source.textureSource;
    useColor = source.useColor;
    size = source.size;
    width = source.width;

    x1 = source.x1;
    y1 = source.y1;
    x2 = source.x2;
    y2 = source.y2;

    textureID[0] = source.textureID[0];

    setVertexArray(x1, y1, x2, y2, width);
    setTextureArray();
    setIndexArray();

    setIndexArrayForSelectionFrame();
    setColorArrayForSelectionFrame(0.0f, 0.0f, 0.0f);

    selected = source.selected;
    fixed = source.fixed;
    description = source.description;
    indexOfSelectedControl = -1;
}


// Индексы вершины для отрисовки
void LineSimple::setVertexArray(float x1, float y1, float x2, float y2, float width)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setVertexArray(float x1, float y1, float x2, float y2, float width)"
                                       << " x1 = " << x1 << " y1 = " << y1
                                       << " x2 = " << x2 << " y2 = " << y2
                                       << " width = " << width << "\n";
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
    if (x1 <= x2 && y1 >= y2)
    {
        dx *= -1.0f;

    }
    if (x1 >= x2 && y1 <= y2)
    {
        dy *= -1.0f;

    }
    VertexArray[0][0]=x1 + dx;
    VertexArray[0][1]=y1 - dy;
    VertexArray[0][2]=0.002f;

    VertexArray[1][0]=x1 - dx;
    VertexArray[1][1]=y1 + dy;
    VertexArray[1][2]=0.002f;

    VertexArray[2][0]=x2 - dx;
    VertexArray[2][1]=y2 + dy;
    VertexArray[2][2]=0.002f;

    VertexArray[3][0]=x2 + dx;
    VertexArray[3][1]=y2 - dy;
    VertexArray[3][2]=0.002f;

    elementX = (x1 + x2) / 2.0f;
    elementY = (y1 + y2) / 2.0f;
}

// Индексы каждой вершины
void LineSimple::setColorArray(float red, float green, float blue, float alpha)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setColorArray(float red, float green, float blue, float alpha)"
                                       << " red = " << red << " green = " << green
                                       << " blue = " << blue << " alpha = " << alpha << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setTextureArray()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setIndexArray()\n";
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


void LineSimple::drawFigure(QGLWidget* )
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::drawFigure(QGLWidget* render)\n";

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

    if (selected == true)
    {
        glDisable(GL_DEPTH_TEST);
        setColorArray(0.7f, 0.7f, 0.7f, alpha);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);

    }
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < 2)
    {
        glDisable(GL_DEPTH_TEST);
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
        glEnable(GL_DEPTH_TEST);
    }
}



void LineSimple::setIndexArrayForSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setIndexArrayForSelectionFrame()\n";
    IndexArrayForSelection[0] = 0;
    IndexArrayForSelection[1] = 1;
    IndexArrayForSelection[2] = 2;
    IndexArrayForSelection[3] = 3;

    //IndexArrayForSelection[4] = 4;
    //IndexArrayForSelection[5] = 5;
    //IndexArrayForSelection[6] = 6;
    //IndexArrayForSelection[7] = 7;

}

void LineSimple::setColorArrayForSelectionFrame(float red, float green, float blue)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setColorArrayForSelectionFrame(float red, float green, float blue)"
                                       << " red = " << red
                                       << " green = " << green
                                       << " blue = " << blue << "\n";
    for (int i = 0; i < 4; ++i)
    {
        ColorArrayForSelection[i][0] = red;
        ColorArrayForSelection[i][1] = green;
        ColorArrayForSelection[i][2] = blue;
    }
}



void LineSimple::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::drawSelectionFrame()\n";
    // Боковые грани для изменения размера
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glColorPointer(3, GL_FLOAT, 0, ColorArrayForSelection);
    glLineWidth(2.0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);

    // Угловые точки для изменения размера
    glPointSize(5.0);
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_BYTE, IndexArrayForSelection);
}


void LineSimple::move(float dx, float dy, float dz)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::move(float dx, float dy, float dz)"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
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
    elementX += dx;
    elementY += dy;

}

void LineSimple::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    switch (index)
    {
    case 0:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x1, y1, VertexArray[0][2]);
        glEnd();
    }
        break;
    case 1:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x2, y2, VertexArray[3][2]);
        glEnd();
    }
        break;
    default:
        break;
    }

}


void LineSimple::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx << " dy = " << dy
                                       << " x = " << x << " y = " << y << "\n";
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
    default:
        break;
    }
    setTextureArray();
}

void LineSimple::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    indexOfSelectedControl = index;
}

QCursor LineSimple::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::hasPoint(GLfloat x, GLfloat y)"
                                       << " x = " << x
                                       << " y = " << y << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getCoorninateOfPointControl(int index)"
                                       << " index = " << index << "\n";
    if (index < 0 || index > 3)
    {
        QMessageBox::critical(0, "Ошибка", "LineSimple::getCoorninateOfPointControl(int index) index out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "LineSimple::getCoorninateOfPointControl(int index) index out of range\n";
        QApplication::exit(0);
    }
    QPoint p;
    p.setX(VertexArray[index][0]);
    p.setY(VertexArray[index][1]);

    return p;
}



void LineSimple::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::drawDescription(QGLWidget *render, float red, float green, float blue)"
                                       << " red = " << red
                                       << " green = " << green
                                       << " blue = " << blue << "\n";
    if (render == NULL)
    {
        QMessageBox::warning(0, "Внимание", "LineSimple::drawDescription(QGLWidget *render, float red, float green, float blue) render = NULL",
                             QMessageBox::Yes);
        if (log)
            Logger::getLogger()->warningLog() << "LineSimple::drawDescription(QGLWidget *render, float red, float green, float blue) render = NULL\n";
    }
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

void LineSimple::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "LineSimple::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


QJsonObject LineSimple::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getJSONInfo()\n";
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    element["UseColor"] = useColor;
    element["TextureSource"] = textureSource;
    element["TextureSize"] = textureSize;
    element["Width"] = width;
    element["Fixed"] = fixed;
    element["Id"] = Id;

    QJsonArray temp;
    temp.append(QJsonValue(x1));
    temp.append(QJsonValue(y1));
    temp.append(QJsonValue(x2));
    temp.append(QJsonValue(y2));
    element["Vertices"] = temp;

    return element;

}


void LineSimple::getProperties(QVBoxLayout *layout, QGLWidget* render)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getProperties(QFormLayout *layout, QGLWidget* render)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "LineSimple::getProperties(QFormLayout *layout, QGLWidget* render) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "LineSimple::getProperties(QFormLayout *layout, QGLWidget* render) layout = NULL\n";
        QApplication::exit(0);
    }

//    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
//    widthSpinBox->setKeyboardTracking(false);
//    QDoubleSpinBox* lengthSpinBox = new QDoubleSpinBox();
//    lengthSpinBox->setKeyboardTracking(false);
//    QCheckBox* fixedCheckBox = new QCheckBox();

//    widthSpinBox->setValue(width);
//    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));
//    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));

//    lengthSpinBox->setValue(length);
//    connect(this, SIGNAL(lengthChanged(double)), lengthSpinBox, SLOT(setValue(double)));
//    connect(lengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLength(double)));

//    fixedCheckBox->setChecked(fixed);
//    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));
//    if (render)
//    {
//        connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//        connect(lengthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
//    }

//    QCheckBox *showMeasurementsCheckBox = new QCheckBox();
//    showMeasurementsCheckBox->setChecked(showMeasurements);
//    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));
//    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
//    layout->addRow("Размеры", showMeasurementsCheckBox);

//    layout->addRow("Длина", lengthSpinBox);
//    layout->addRow("Ширина", widthSpinBox);
//    layout->addRow("Зафиксировать", fixedCheckBox);
}

void LineSimple::setWidth(double width)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setWidth(double width)"
                                       << " width = " << width << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setLength(double length)"
                                       << " length = " << length << "\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setDescription(QString description)"
                                       << " description = " << description << "\n";
    this->description = description;
}


bool LineSimple::isFixed()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::isFixed()\n";
    return fixed;
}

void LineSimple::setLength(float length, bool front)
{
    if (front)
    {
        x1 = x2 + (x1 - x2) / this->length * length;
        y1 = y2 + (y1 - y2) / this->length * length;
    }
    else
    {
        x2 = x1 + (x2 - x1) / this->length * length;
        y2 = y1 + (y2 - y1) / this->length * length;
    }
    this->length = length;
    setVertexArray(x1, y1, x2, y2, width);
    setTextureArray();
}


void LineSimple::drawMeasurements(QGLWidget *)
{
    if (!showMeasurements)
        return;
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::drawMeasurements(QGLWidget *render)\n";
}


bool LineSimple::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
    return true;
}


int LineSimple::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getLayer()\n";
    return layer;
}


void LineSimple::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::clear()\n";
}


std::vector<vec3> LineSimple::getCoordOfControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getCoordOfControl(int index)"
                                       << " index = " << index << "\n";
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

void LineSimple::rotate(float angle, float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::rotate(float angle, float x, float y, float z)"
                                       << " angle = " << angle
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    float tx = 0.0f, ty = 0.0f;
    x1 -= x;
    y1 -= y;
    tx = x1;
    ty = y1;
    x1 = tx * cos(angle) - ty * sin(angle);
    y1 = tx * sin(angle) + ty * cos(angle);
    x1 += x;
    y1 += y;

    x2 -= x;
    y2 -= y;
    tx = x2;
    ty = y2;
    x2 = tx * cos(angle) - ty * sin(angle);
    y2 = tx * sin(angle) + ty * cos(angle);
    x2 += x;
    y2 += y;

    setVertexArray(x1, y1, x2, y2, width);
}


RoadElement *LineSimple::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::getCopy()\n";
    LineSimple* copyElement = new LineSimple(*this);
    return copyElement;
}


void LineSimple::setCoordForControl(int index, std::vector<vec3> &controls)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::setCoordForControl(int index, std::vector<vec3> &controls)"
                                       << " index = " << index << "\n";
    switch (index)
    {
    case 0:
    {
        float x, y;
        x = x1;
        y = y1;
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 1:
    {
        float x, y;
        x = x2;
        y = y2;
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    default:
        break;
    }
}


void LineSimple::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "LineSimple::clearProperties(QLayout *layout)\n";
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        if (item->layout() != NULL)
        {
            clearProperties(item->layout());
            delete item->layout();
        }
        if (item->widget() != NULL)
        {
            delete item->widget();
        }
    }
}
