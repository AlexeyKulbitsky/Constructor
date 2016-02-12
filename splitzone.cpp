#include "splitzone.h"
#include <GL/glu.h>
#include <QApplication>

bool SplitZone::log = true;

SplitZone::SplitZone()
{

}

SplitZone::SplitZone(float *pointsArray, int size,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     QString description)
{

    lineAxisArray = NULL;
    this->description = description;
    this->name = "SplitZone";
    line = NULL;
    this->beginRounding = beginRounding;
    this->endRounding = endRounding;
    this->width = width;
    lineWidth = 0.15f;
    selected = fixed = false;
    calculateLine(pointsArray,size,width);
    lineWidth = 0.15f;
    this->layer = 1;
    this->width = width;
    this->type = 0;
    this->height = 0.0f;
    indexOfSelectedControl = -1;
    texture1 = "/models/city_roads/solid.png";
    line = new LineBroken(lineWidth,lineAxisArray,this->size,
                          "/models/city_roads/solid.png", 6.0f,
                          "LineBroken", 1);
    selected = fixed = false;
}

SplitZone::SplitZone(float *pointsArray, int size,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     int type,
                     double height,
                     QString texture_1,
                     float texture1USize, float texture1VSize,
                     QString texture_2,
                     float texture2USize, float texture2VSize,
                     QString description)
{
    this->type = type;
    this->height = height;
    lineAxisArray = NULL;
    indexOfSelectedControl = -1;
    switch (type)
    {
    case 0:
    {
        SplitZone(pointsArray, size,
                  width,
                  beginRounding,
                  endRounding,
                  description);
    }
        break;
    case 1:
    {
        this->description = description;
        this->name = "SplitZone";
        line = NULL;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        selected = fixed = false;
        calculateLine(pointsArray,size,width);
        this->layer = 1;
        this->height = height;
        textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
        textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
        texture1 = texture_1;
        texture2 = texture_2;
        this->texture1USize = texture1USize;
        this->texture1VSize = texture1VSize;
        this->texture2USize = texture2USize;
        this->texture2VSize = texture2VSize;
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    case 2:
    {
        this->description = description;
        this->name = "SplitZone";
        line = NULL;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        selected = fixed = false;
        calculateLine(pointsArray,size,width);
        this->layer = 1;
        this->height = height;
        textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
        textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
        texture1 = texture_1;
        texture2 = texture_2;
        this->texture1USize = texture1USize;
        this->texture1VSize = texture1VSize;
        this->texture2USize = texture2USize;
        this->texture2VSize = texture2VSize;
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    default:
        break;
    }
}

SplitZone::SplitZone(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     QString description)
{
    lineAxisArray = NULL;
    this->type = 0;
    this->height = 0.0f;
    this->description = description;
    this->name = "SplitZone";
    this->layer = 1;
    line = NULL;
    pBegin.x = x1;
    pBegin.y = y1;
    pBegin.z = z1;
    pEnd.x = x2;
    pEnd.y = y2;
    pEnd.z = z2;
    this->beginRounding = beginRounding;
    this->endRounding = endRounding;
    this->width = width;
    calculateLine(pBegin,pEnd,width);
    lineWidth = 0.15f;
    indexOfSelectedControl = -1;
    texture1 = "/models/city_roads/solid.png";
    line = new LineBroken(lineWidth,lineAxisArray,this->size,
                          "/models/city_roads/solid.png", 6.0f,
                          "LineBroken", 1);
    selected = fixed = false;
}

SplitZone::SplitZone(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     int type,
                     double height,
                     QString texture_1,
                     float texture1USize, float texture1VSize,
                     QString texture_2,
                     float texture2USize, float texture2VSize,
                     QString description)
{
    this->type = type;
    this->height = height;
    lineAxisArray = NULL;
    indexOfSelectedControl = -1;
    switch (type)
    {
    case 0:
        SplitZone(x1, y1, z1,
                  x2, y2, z2,
                  width,
                  beginRounding,
                  endRounding,
                  description);
        break;
    case 1:
    {
        this->description = description;
        this->name = "SplitZone";
        line = NULL;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        selected = fixed = false;
        pBegin.x = x1;
        pBegin.y = y1;
        pBegin.z = z1;
        pEnd.x = x2;
        pEnd.y = y2;
        pEnd.z = z2;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        calculateLine(pBegin,pEnd,width);
        this->layer = 1;
        this->height = height;
        textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
        textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
        texture1 = texture_1;
        texture2 = texture_2;
        this->texture1USize = texture1USize;
        this->texture1VSize = texture1VSize;
        this->texture2USize = texture2USize;
        this->texture2VSize = texture2VSize;
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    case 2:
    {
        this->description = description;
        this->name = "SplitZone";
        line = NULL;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        selected = fixed = false;
        pBegin.x = x1;
        pBegin.y = y1;
        pBegin.z = z1;
        pEnd.x = x2;
        pEnd.y = y2;
        pEnd.z = z2;
        this->beginRounding = beginRounding;
        this->endRounding = endRounding;
        this->width = width;
        calculateLine(pBegin,pEnd,width);
        this->layer = 1;
        this->height = height;
        textureID[0] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_1);
        textureID[1] = TextureManager::getInstance()->getID(QApplication::applicationDirPath() + texture_2);
        texture1 = texture_1;
        texture2 = texture_2;
        this->texture1USize = texture1USize;
        this->texture1VSize = texture1VSize;
        this->texture2USize = texture2USize;
        this->texture2VSize = texture2VSize;
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    default:
        break;
    }
}

SplitZone::SplitZone(const SplitZone &source)
{
    if (source.line)
        line = qobject_cast<LineBroken*>(source.line->getCopy());
    selected = source.selected;
    fixed = source.fixed;
    layer = source.layer;

    beginRounding = source.beginRounding;
    endRounding = source.endRounding;
    pBegin = source.pBegin;
    pEnd = source.pEnd;
    pPerpBegin = source.pPerpBegin;
    pPerpEnd = source.pPerpEnd;
    width = source.width;
    numberOfSides = source.numberOfSides;
    texture1 = source.texture1;
    texture2 = source.texture2;
    size = source.size;
    lineAxisArray = new GLfloat[size];
    for (int i = 0; i < size; ++i)
        lineAxisArray[i] = source.lineAxisArray[i];

    axisArray.resize(source.axisArray.size());
    for (int i = 0; i < source.axisArray.size(); ++i)
        axisArray[i] = source.axisArray[i];
    elementX = source.elementX;
    elementY = source.elementY;
    lineWidth = source.lineWidth;
    p1 = source.p1;
    p2 = source.p2;
    p3 = source.p3;
    p4 = source.p1;
    line1_p1 = source.line1_p1;
    line1_p2 = source.line1_p1;
    line2_p1 = source.line1_p1;
    line2_p2 = source.line1_p1;
    description = source.description;
    xCenter = source.xCenter;
    yCenter = source.yCenter;
    type = source.type;
    height = source.height;
    indexOfSelectedControl = source.indexOfSelectedControl;

    boardVertexArray.resize(source.boardVertexArray.size());
    for (int i = 0; i < source.boardVertexArray.size(); ++i)
        boardVertexArray[i] = source.boardVertexArray[i];

    boardTextureArray.resize(source.boardTextureArray.size());
    for (int i = 0; i < source.boardTextureArray.size(); ++i)
        boardTextureArray[i] = source.boardTextureArray[i];

    boardIndexArray.resize(source.boardIndexArray.size());
    for (int i = 0; i < source.boardIndexArray.size(); ++i)
        boardIndexArray[i] = source.boardIndexArray[i];

    vertexArray.resize(source.vertexArray.size());
    for (int i = 0; i < source.vertexArray.size(); ++i)
        vertexArray[i] = source.vertexArray[i];

    textureArray.resize(source.textureArray.size());
    for (int i = 0; i < source.textureArray.size(); ++i)
        textureArray[i] = source.textureArray[i];

    indexArray.resize(source.indexArray.size());
    for (int i = 0; i < source.indexArray.size(); ++i)
        indexArray[i] = source.indexArray[i];


    textureID[0] = source.textureID[0];
    textureID[1] = source.textureID[1];
    texture1USize = source.texture1USize;
    texture1VSize = source.texture1VSize;
    texture2USize = source.texture2USize;
    texture2VSize = source.texture2VSize;
}

SplitZone::~SplitZone()
{
    if (line)
        delete line;
    line = NULL;

    if (lineAxisArray)
        delete[] lineAxisArray;
    lineAxisArray = NULL;
}



bool SplitZone::isSelected()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::isSelected()\n";
    return selected;
}

void SplitZone::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
}

void SplitZone::drawFigure(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::drawFigure(QGLWidget *render)\n";
    switch (type)
    {
    case 0:
    {
        line->drawFigure(render);
    }
        break;
    case 1:   
    case 2:
    {
        glDisableClientState(GL_COLOR_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, textureID[0]);
        glVertexPointer(3, GL_FLOAT, 0, boardVertexArray.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, boardTextureArray.begin());
        glDrawElements(GL_TRIANGLES, boardIndexArray.size(), GL_UNSIGNED_INT, boardIndexArray.begin());

        glBindTexture(GL_TEXTURE_2D, textureID[1]);
        glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
        glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, indexArray.begin());

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        glEnableClientState(GL_COLOR_ARRAY);
    }
        break;
    default:
        break;
    }
    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
    if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
    {
        drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
    }
}

void SplitZone::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::drawSelectionFrame()\n";
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 2.0f, 5.0f);
}

void SplitZone::drawMeasurements(QGLWidget *render)
{
    if (!showMeasurements)
        return;
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::drawMeasurements(QGLWidget *render)\n";
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Black);
    float x1, x2, y1, y2;

    int count = axisArray.size() / 3;
    // Ширина полосы
    x1 = axisArray[(count / 2 - 1) * 3];
    y1 = axisArray[(count / 2 - 1) * 3 + 1];
    x2 = axisArray[(count / 2) * 3];
    y2 = axisArray[(count / 2) * 3 + 1];
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    glColor3f(0.0f, 0.0f, 0.0f);
    getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx, wy, "W=" + QString("%1").arg(width, 0, 'f', 2), shrift);

}

void SplitZone::move(float dx, float dy, float dz)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::move(float dx, float dy, float dz)"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
    for (int i = 0; i < size / 3; ++i)
    {
        this->lineAxisArray[i * 3] += dx;
        this->lineAxisArray[i * 3 + 1] += dy;
    }
    for (int i = 0; i < axisArray.size() / 3; ++i)
    {
        axisArray[i * 3] += dx;
        axisArray[i * 3 + 1] += dy;
    }
    p1.x += dx;
    p2.x += dx;
    p3.x += dx;
    p4.x += dx;
    pBegin.x += dx;
    pEnd.x += dx;

    p1.y += dy;
    p2.y += dy;
    p3.y += dy;
    p4.y += dy;
    pBegin.y += dy;
    pEnd.y += dy;

    switch (type)
    {
    case 0:
        line->move(dx, dy, dz);
        break;
    case 1:
    {
        for (int i = 0; i < boardVertexArray.size() / 3; ++i)
        {
            boardVertexArray[i * 3] += dx;
            boardVertexArray[i * 3 + 1] += dy;
        }
        for (int i = 0; i < vertexArray.size() / 3; ++i)
        {
            vertexArray[i * 3] += dx;
            vertexArray[i * 3 + 1] += dy;
        }
    }
        break;
    case 2:
    {
        for (int i = 0; i < boardVertexArray.size() / 3; ++i)
        {
            boardVertexArray[i * 3] += dx;
            boardVertexArray[i * 3 + 1] += dy;
        }
        for (int i = 0; i < vertexArray.size() / 3; ++i)
        {
            vertexArray[i * 3] += dx;
            vertexArray[i * 3 + 1] += dy;
        }
    }
        break;
    default:
        break;
    }
    elementX += dx;
    elementY += dy;
}

void SplitZone::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    glPointSize(pointSize + 5.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(axisArray[index * 3],
                axisArray[index * 3 + 1],
                axisArray[index * 3 + 2]);
    glEnd();

}

QCursor SplitZone::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void SplitZone::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " x = " << x
                                       << " y = " << y << "\n";
        axisArray[index * 3] += dx;
        axisArray[index * 3 + 1] += dy;
        calculateLine(axisArray, width);

        switch (type)
        {
        case 0:
            line->setVertexArray(lineWidth, lineAxisArray, size);
            break;
        case 1:
        {
            setBoardVertexArray();
            setBoardTextureArray(texture1USize, texture1VSize);
            setVertexArray();
            setTextureArray(texture2USize, texture2VSize);

        }
            break;
        case 2:
        {
            setBoardVertexArray();
            setBoardTextureArray(texture1USize, texture1VSize);
            setVertexArray();
            setTextureArray(texture2USize, texture2VSize);

        }
            break;
        default:
            break;
        }

}

int SplitZone::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getNumberOfControls()\n";
    return axisArray.size() / 3;
}

int SplitZone::controlsForPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::controlsForPoint()\n";
    return 1;
}

void SplitZone::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    if (index < 0 || index >= getNumberOfControls())
    {
        QMessageBox::critical(0, "Ошибка", "SplitZone::changeColorOfSelectedControl(int index), index is out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->errorLog() << "SplitZone::changeColorOfSelectedControl(int index), index is out of range\n";
        QApplication::exit(0);
    }
    indexOfSelectedControl = index;
}

void SplitZone::getProperties(QVBoxLayout *layout, QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getProperties(QFormLayout *layout, QGLWidget *render)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "SplitZone::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "SplitZone::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL\n";
        QApplication::exit(0);
    }
    clearProperties(layout);

    QFormLayout *l = new QFormLayout();

    QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox();
    widthSpinBox->setKeyboardTracking(false);

    QCheckBox* fixedCheckBox = new QCheckBox();

    widthSpinBox->setValue(width);
    connect(this, SIGNAL(widthChanged(double)), widthSpinBox, SLOT(setValue(double)));
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));

    fixedCheckBox->setChecked(fixed);
    QObject::connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    connect(widthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
    l->addRow("Зафиксировать", fixedCheckBox);

    QCheckBox *showMeasurementsCheckBox = new QCheckBox();
    showMeasurementsCheckBox->setChecked(showMeasurements);
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
    l->addRow("Размеры", showMeasurementsCheckBox);

    l->addRow("Ширина", widthSpinBox);

    switch (type)
    {
    case 0:
    {
        QDoubleSpinBox* lineWidthSpinBox = new QDoubleSpinBox();
        lineWidthSpinBox->setKeyboardTracking(false);
        lineWidthSpinBox->setValue(lineWidth);
        connect(this, SIGNAL(lineWidthChanged(double)), lineWidthSpinBox, SLOT(setValue(double)));
        connect(lineWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLineWidth(double)));
        connect(lineWidthSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        l->addRow("Толщина линии", lineWidthSpinBox);
    }
        break;
    case 1:
    case 2:
    {
        QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox();
        heightSpinBox->setKeyboardTracking(false);
        heightSpinBox->setValue(height);
        connect(this, SIGNAL(heightChanged(double)), heightSpinBox, SLOT(setValue(double)));
        connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setHeight(double)));
        connect(heightSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        l->addRow("Высота", heightSpinBox);
    }
        break;
    default:
        break;
    }

    layout->addLayout(l);

}

bool SplitZone::isFixed()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::isFixed()\n";
    return fixed;
}

int SplitZone::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getLayer()\n";
    return layer;
}

void SplitZone::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::clear()\n";
}

vec3 SplitZone::getAxisPoint(int index)
{
    if (index < 0 || index > getNumberOfControls())
        return vec3(0.0f, 0.0f, 0.0f);
    return vec3(axisArray[index * 3],
                axisArray[index * 3 + 1],
                axisArray[index * 3 + 2]);
}



bool SplitZone::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
    return true;
}

void SplitZone::setHeight(double height)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setHeight(double height)"
                                       << " height = " << height << "\n";
    if (this->height == height)
        return;
    this->height = height;

    switch (type)
    {
    case 0:
        break;
    case 1:
    {
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);

    }
        break;
    case 2:
    {
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);

    }
        break;
    default:
        break;
    }

    emit heightChanged(width);
}

void SplitZone::setLineWidth(double value)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setLineWidth(double value)"
                                       << " value = " << value << "\n";
    if (this->lineWidth == value)
        return;
    this->lineWidth = value;

    switch (type)
    {
    case 0:
        line->setVertexArray(lineWidth, lineAxisArray, size);
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
    emit lineWidthChanged(value);
}

void SplitZone::setWidth(double width)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setWidth(double width)"
                                       << " width = " << width << "\n";
    if (this->width == width)
        return;
    this->width = width;

    calculateLine(axisArray, width);

    switch (type)
    {
    case 0:
        line->setVertexArray(lineWidth, lineAxisArray, size);
        break;
    case 1:
    {
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);

    }
        break;
    case 2:
    {
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);

    }
        break;
    default:
        break;
    }

    emit widthChanged(width);
}

void SplitZone::calculateLine(vec3 p1, vec3 p2, float width)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::calculateLine(vec3 p1, vec3 p2, float width)"
                                       << " p1.x = " << p1.x << " p1.y = " << p1.y << " p1.z = " << p1.z
                                       << " p2.x = " << p2.x << " p2.y = " << p2.y << " p2.z = " << p2.z
                                       << " width = " << width << "\n";
    if (axisArray.size() != 6)
        axisArray.resize(6);
    axisArray[0] = p1.x;
    axisArray[1] = p1.y;
    axisArray[2] = p1.z;
    axisArray[3] = p2.x;
    axisArray[4] = p2.y;
    axisArray[5] = p2.z;
    xCenter = (p1.x + p2.x) / 2.0f;
    yCenter = (p1.y + p2.y) / 2.0f;
    float r = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    float xR1, yR1, xR2, yR2;
    float pi = 3.1415926f;
    float t = (p1.x - p2.x) / r;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float alpha = acos(t);

    if ((p1.y - p2.y) < 0)
        alpha = 2.0f * pi - alpha;
    float alpha1 = alpha - pi / 2.0f;
    if (alpha1 < 0)
        alpha1 = 2.0f * pi + alpha1;
    float alpha2 = alpha1 + pi;

    numberOfSides = 10;
    float r1 = width / 2.0f;

    pPerpBegin.x = (p2.x + p1.x) / 2.0f + r1 * cos(alpha1);
    pPerpBegin.y = (p2.y + p1.y) / 2.0f + r1 * sin(alpha1);
    pPerpEnd.x = (p2.x + p1.x) / 2.0f + r1 * cos(alpha2);
    pPerpEnd.y = (p2.y + p1.y) / 2.0f + r1 * sin(alpha2);



    QVector<GLfloat> lineAxis;

    if (beginRounding == true && endRounding == true)
    {

        xR1 = p1.x + ((p2.x - p1.x) / r) * (width / 2.0f);
        yR1 = p1.y + ((p2.y - p1.y) / r) * (width / 2.0f);
        xR2 = p2.x + ((p1.x - p2.x) / r) * (width / 2.0f);
        yR2 = p2.y + ((p1.y - p2.y) / r) * (width / 2.0f);


        for (int i = 0; i <= numberOfSides; ++i)
        {
            float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
            float dx = r1 * cos(angle);
            float dy = r1 * sin(angle);
            lineAxis.push_back(xR1  + dx);
            lineAxis.push_back(yR1 + dy);
            lineAxis.push_back(0.02f);

            if (i == 0)
            {
                this->p1.x = p1.x + dx;
                this->p1.y = p1.y + dy;
                this->p1.z = 0.02f;
            }
            if (i == numberOfSides)
            {
                this->p2.x = p1.x + dx;
                this->p2.y = p1.y + dy;
                this->p2.z = 0.02f;
            }
        }

        alpha1 = alpha2;
        alpha2 = alpha1 + pi;

        for (int i = 0; i <= numberOfSides; ++i)
        {
            float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
            float dx = r1 * cos(angle);
            float dy = r1 * sin(angle);
            lineAxis.push_back(xR2  + dx);
            lineAxis.push_back(yR2 + dy);
            lineAxis.push_back(0.02f);

            if (i == 0)
            {
                this->p3.x = p2.x + dx;
                this->p3.y = p2.y + dy;
                this->p3.z = 0.02f;
            }
            if (i == numberOfSides)
            {
                this->p4.x = p2.x + dx;
                this->p4.y = p2.y + dy;
                this->p4.z = 0.02f;
            }
        }

        float x = lineAxis[0];
        float y = lineAxis[1];
        float z = lineAxis[2];

        lineAxis.push_back(x);
        lineAxis.push_back(y);
        lineAxis.push_back(z);
    }
    else
        if (beginRounding == true && endRounding == false)
        {
            xR1 = p1.x + ((p2.x - p1.x) / r) * (width / 2.0f);
            yR1 = p1.y + ((p2.y - p1.y) / r) * (width / 2.0f);
            xR2 = p2.x;
            yR2 = p2.y;

            float dx = r1 * cos(alpha1);
            float dy = r1 * sin(alpha1);
            lineAxis.push_back(xR2  + dx);
            lineAxis.push_back(yR2 + dy);
            lineAxis.push_back(0.02f);

            this->p4.x = xR2  + dx;
            this->p4.y = yR2 + dy;
            this->p4.z = 0.02f;

            for (int i = 0; i <= numberOfSides; ++i)
            {
                float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                float dx = r1 * cos(angle);
                float dy = r1 * sin(angle);
                lineAxis.push_back(xR1  + dx);
                lineAxis.push_back(yR1 + dy);
                lineAxis.push_back(0.02f);

                if (i == 0)
                {
                    this->p1.x = p1.x + dx;
                    this->p1.y = p1.y + dy;
                    this->p1.z = 0.02f;
                }
                if (i == numberOfSides)
                {
                    this->p2.x = p1.x + dx;
                    this->p2.y = p1.y + dy;
                    this->p2.z = 0.02f;
                }
            }
            dx = r1 * cos(alpha2);
            dy = r1 * sin(alpha2);
            lineAxis.push_back(xR2  + dx);
            lineAxis.push_back(yR2 + dy);
            lineAxis.push_back(0.02f);
            this->p3.x = xR2 + dx;
            this->p3.y = yR2 + dy;
            this->p3.z = 0.02f;
        }
        else
            if (beginRounding == false && endRounding == true)
            {
                xR1 = p1.x;
                yR1 = p1.y;
                xR2 = p2.x + ((p1.x - p2.x) / r) * (width / 2.0f);
                yR2 = p2.y + ((p1.y - p2.y) / r) * (width / 2.0f);

                float dx = r1 * cos(alpha2);
                float dy = r1 * sin(alpha2);
                lineAxis.push_back(xR1  + dx);
                lineAxis.push_back(yR1 + dy);
                lineAxis.push_back(0.02f);
                this->p2.x = xR1 + dx;
                this->p2.y = yR1 + dy;
                this->p2.z = 0.02f;

                alpha1 = alpha2;
                alpha2 += pi;

                for (int i = 0; i <= numberOfSides; ++i)
                {
                    float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                    float dx = r1 * cos(angle);
                    float dy = r1 * sin(angle);
                    lineAxis.push_back(xR2  + dx);
                    lineAxis.push_back(yR2 + dy);
                    lineAxis.push_back(0.02f);

                    if (i == 0)
                    {
                        this->p3.x = p2.x + dx;
                        this->p3.y = p2.y + dy;
                        this->p3.z = 0.02f;
                    }
                    if (i == numberOfSides)
                    {
                        this->p4.x = p2.x + dx;
                        this->p4.y = p2.y + dy;
                        this->p4.z = 0.02f;
                    }
                }
                dx = r1 * cos(alpha2);
                dy = r1 * sin(alpha2);
                lineAxis.push_back(xR1  + dx);
                lineAxis.push_back(yR1 + dy);
                lineAxis.push_back(0.02f);
                this->p1.x = xR1 + dx;
                this->p1.y = yR1 + dy;
                this->p1.z = 0.02f;

            }

    this->size = lineAxis.size();
    if (lineAxisArray != NULL)
    {
        delete lineAxisArray;
        lineAxisArray = NULL;
    }
    lineAxisArray = new GLfloat[this->size];
    for (int i = 0; i < this->size; ++i)
        lineAxisArray[i] = lineAxis[i];

    if (line != NULL)
    {
        line->setVertexArrayForAxis(lineAxisArray, this->size);
        line->setVertexArray(lineWidth,lineAxisArray, this->size);
        line->setTextureArray();
    }
}

void SplitZone::calculateLine(float *pointsArray, int size, float width)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::calculateLine(float *pointsArray, int size, float width)"
                                       << " size = " << size
                                       << " width = " << width << "\n";
    if (pointsArray == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "SplitZone::calculateLine(float *pointsArray, int size, float width) pointsArray = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "SplitZone::calculateLine(float *pointsArray, int size, float width) pointsArray = NULL\n";
        QApplication::exit(0);
    }
    QVector<GLfloat> lineAxis;
    if (axisArray.size() != size)
        axisArray.resize(size);
    for (int i = 0; i < size; ++i)
    {
        axisArray[i] = pointsArray[i];
    }
    xCenter = pointsArray[size / 6 * 3];
    yCenter = pointsArray[size / 6 * 3 + 1];
    for (int i = 0; i < size / 3; ++i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            float x1 = pointsArray[i * 3];
            float y1 = pointsArray[i * 3 + 1];
            float x2 = pointsArray[(i + 1) * 3];
            float y2 = pointsArray[(i + 1) * 3 + 1];

            float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            float xR1, yR1;
            //float xR2, yR2;
            float pi = 3.14159265f;
            float t = (x1 - x2) / r;
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float alpha = acos(t);

            if ((y1 - y2) < 0)
                alpha = 2.0f * pi - alpha;

            float alpha1 = alpha - pi / 2.0f;
            if (alpha1 < 0)
                alpha1 = 2.0f * pi + alpha1;
            float alpha2 = alpha1 + pi;
            float r1 = width / 2.0f;

            if (beginRounding)
            {
                xR1 = x1 + ((x2 - x1) / r) * (width / 2.0f);
                yR1 = y1 + ((y2 - y1) / r) * (width / 2.0f);
                numberOfSides = 10;
                for (int i = 0; i <= numberOfSides; ++i)
                {
                    float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                    float dx = r1 * cos(angle);
                    float dy = r1 * sin(angle);
                    lineAxis.push_back(xR1  + dx);
                    lineAxis.push_back(yR1 + dy);
                    lineAxis.push_back(0.02f);
                }
            }
            else
            {
                lineAxis.push_back(x1  + r1 * cos(alpha2));
                lineAxis.push_back(y1 + r1 * sin(alpha2));
                lineAxis.push_back(0.02f);
            }
        }
        else
        {   // Если теукщий индекс - конец осевой линии,
            // то строим перпендикуляр
            if (i == size / 3 - 1)
            {
                float x2 = pointsArray[i * 3];
                float y2 = pointsArray[i * 3 + 1];
                float x1 = pointsArray[(i - 1) * 3];
                float y1 = pointsArray[(i - 1) * 3 + 1];
                float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
                float xR1, yR1;
                //float xR2, yR2;
                float pi = 3.14159265f;
                float t = (x1 - x2) / r;
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float alpha = acos(t);

                if ((y1 - y2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                float alpha2 = alpha1 + pi;
                float r1 = width / 2.0f;
                alpha1 = alpha2;
                alpha2 += pi;
                if (endRounding)
                {
                    xR1 = x2 + ((x1 - x2) / r) * (width / 2.0f);
                    yR1 = y2 + ((y1 - y2) / r) * (width / 2.0f);
                    numberOfSides = 10;
                    for (int i = 0; i <= numberOfSides; ++i)
                    {
                        float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                        float dx = r1 * cos(angle);
                        float dy = r1 * sin(angle);
                        lineAxis.push_back(xR1  + dx);
                        lineAxis.push_back(yR1 + dy);
                        lineAxis.push_back(0.02f);
                    }
                }
                else
                {
                    lineAxis.push_back(x2  + r1 * cos(alpha1));
                    lineAxis.push_back(y2 + r1 * sin(alpha1));
                    lineAxis.push_back(0.02f);
                    lineAxis.insert(0,0.02f);
                    lineAxis.insert(0,y2 + r1 * sin(alpha2));
                    lineAxis.insert(0,x2  + r1 * cos(alpha2));
                    //lineAxis.push_back(x2  + r1 * cos(alpha2));
                    //lineAxis.push_back(y2 + r1 * sin(alpha2));
                    //lineAxis.push_back(0.02f);
                }
            }
            else
            {

                float x1 = pointsArray[(i - 1) * 3];
                float y1 = pointsArray[(i - 1) * 3 + 1];
                float x2 = pointsArray[i * 3];
                float y2 = pointsArray[i * 3 + 1];
                float x3 = pointsArray[(i + 1) * 3];
                float y3 = pointsArray[(i + 1) * 3 + 1];
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
                float r1 = width / 2.0f;
                float hamma = alpha + beta;
                float dx = (r1 / sin(alpha)) * cos(hamma);
                float dy = (r1 / sin(alpha)) * sin(hamma);

                lineAxis.push_back(x2 - dx);
                lineAxis.push_back(y2 - dy);
                lineAxis.push_back(0.02f);


            }
        }
    }

    ////////////////////////////////////////////////////////
    int counter = 3;
    for (int i = size / 3 - 2; i >= 0; --i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            if (!beginRounding)
            {
                float x1 = pointsArray[i * 3];
                float y1 = pointsArray[i * 3 + 1];
                float x2 = pointsArray[(i + 1) * 3];
                float y2 = pointsArray[(i + 1) * 3 + 1];

                float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
                //float xR1, yR1, xR2, yR2;
                float pi = 3.14159265f;
                float t = (x1 - x2) / r;
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float alpha = acos(t);

                if ((x1 - x2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                //float alpha2 = alpha1 + pi;
                float r1 = width / 2.0f;

                lineAxis.push_back(x1 - r1 * cos(alpha1));
                lineAxis.push_back(y1 - r1 * sin(alpha1));
                lineAxis.push_back(0.02f);
            }
            else
            {
                if (endRounding)
                {
                float x1 = lineAxis[0];
                float y1 = lineAxis[1];
                float z1 = lineAxis[2];
                lineAxis.push_back(x1);
                lineAxis.push_back(y1);
                lineAxis.push_back(z1);
                }
            }

        }
        // Если теукщий индекс - конец осевой линии,
        // то строим перпендикуляр

        else
        {
            float x1 = pointsArray[(i - 1) * 3];
            float y1 = pointsArray[(i - 1) * 3 + 1];
            float x2 = pointsArray[i * 3];
            float y2 = pointsArray[i * 3 + 1];
            float x3 = pointsArray[(i + 1) * 3];
            float y3 = pointsArray[(i + 1) * 3 + 1];
            float num = (x1-x2)*(x3-x2)+(y1-y2)*(y3-y2);
            float den = sqrt(((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))*((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
            float t = num / den;
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float alpha = (acos(t))/2.0f;
            float sa = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);
            float pi = 3.14159265f;
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
            float r1 = width / 2.0f;
            float hamma = alpha + beta;
            float dx = (r1 / sin(alpha)) * cos(hamma);
            float dy = (r1 / sin(alpha)) * sin(hamma);

            if (!endRounding)
            {
                lineAxis.insert(counter,0.02f);
                lineAxis.insert(counter,y2 + dy);
                lineAxis.insert(counter,x2 + dx);
                counter += 3;
            }
            else
            {
                lineAxis.push_back(x2 + dx);
                lineAxis.push_back(y2 + dy);
                lineAxis.push_back(0.02f);
            }

        }

    }

    this->size = lineAxis.size();
    if (lineAxisArray != NULL)
    {
        delete lineAxisArray;
        lineAxisArray = NULL;
    }
    lineAxisArray = new GLfloat[this->size];
    for (int i = 0; i < this->size; ++i)
        lineAxisArray[i] = lineAxis[i];

    if (line != NULL)
    {
        line->setVertexArrayForAxis(lineAxisArray, this->size);
        line->setVertexArray(lineWidth,lineAxisArray, this->size);
        line->setTextureArray();
    }

}

void SplitZone::calculateLine(float *pointsArray, int size)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::calculateLine(float *pointsArray, int size)\n";
    calculateLine(pointsArray, size, this->width);
}

void SplitZone::calculateLine(QVector<GLfloat> &pointsArray, float width)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::calculateLine(QVector<GLfloat> &pointsArray, float width)\n";
    QVector<GLfloat> lineAxis;
    int size = pointsArray.size();
    if (axisArray.size() != size)
        axisArray.resize(size);
    for (int i = 0; i < size; ++i)
    {
        axisArray[i] = pointsArray[i];
    }
    xCenter = pointsArray[size / 6 * 3];
    yCenter = pointsArray[size / 6 * 3 + 1];
    for (int i = 0; i < size / 3; ++i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            float x1 = pointsArray[i * 3];
            float y1 = pointsArray[i * 3 + 1];
            float x2 = pointsArray[(i + 1) * 3];
            float y2 = pointsArray[(i + 1) * 3 + 1];

            float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            float xR1, yR1;
            //float xR2, yR2;
            float pi = 3.14159265f;
            float t = (x1 - x2) / r;
            if (t > 1)
                t = 1.0f;
            if (t < -1)
                t = -1.0f;
            float alpha = acos(t);

            if ((y1 - y2) < 0)
                alpha = 2.0f * pi - alpha;

            float alpha1 = alpha - pi / 2.0f;
            if (alpha1 < 0)
                alpha1 = 2.0f * pi + alpha1;
            float alpha2 = alpha1 + pi;
            float r1 = width / 2.0f;

            if (beginRounding)
            {
                xR1 = x1 + ((x2 - x1) / r) * (width / 2.0f);
                yR1 = y1 + ((y2 - y1) / r) * (width / 2.0f);
                numberOfSides = 10;
                for (int i = 0; i <= numberOfSides; ++i)
                {
                    float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                    float dx = r1 * cos(angle);
                    float dy = r1 * sin(angle);
                    lineAxis.push_back(xR1  + dx);
                    lineAxis.push_back(yR1 + dy);
                    lineAxis.push_back(0.02f);
                }
            }
            else
            {
                lineAxis.push_back(x1  + r1 * cos(alpha2));
                lineAxis.push_back(y1 + r1 * sin(alpha2));
                lineAxis.push_back(0.02f);
            }
        }
        else
        {   // Если теукщий индекс - конец осевой линии,
            // то строим перпендикуляр
            if (i == size / 3 - 1)
            {
                float x2 = pointsArray[i * 3];
                float y2 = pointsArray[i * 3 + 1];
                float x1 = pointsArray[(i - 1) * 3];
                float y1 = pointsArray[(i - 1) * 3 + 1];
                float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
                float xR1, yR1;
                //float xR2, yR2;
                float pi = 3.14159265f;
                float t = (x1 - x2) / r;
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float alpha = acos(t);

                if ((y1 - y2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                float alpha2 = alpha1 + pi;
                float r1 = width / 2.0f;
                alpha1 = alpha2;
                alpha2 += pi;
                if (endRounding)
                {
                    xR1 = x2 + ((x1 - x2) / r) * (width / 2.0f);
                    yR1 = y2 + ((y1 - y2) / r) * (width / 2.0f);
                    numberOfSides = 10;
                    for (int i = 0; i <= numberOfSides; ++i)
                    {
                        float angle = alpha1 + (alpha2 - alpha1) / numberOfSides * float(i);
                        float dx = r1 * cos(angle);
                        float dy = r1 * sin(angle);
                        lineAxis.push_back(xR1  + dx);
                        lineAxis.push_back(yR1 + dy);
                        lineAxis.push_back(0.02f);
                    }
                }
                else
                {
                    lineAxis.push_back(x2  + r1 * cos(alpha1));
                    lineAxis.push_back(y2 + r1 * sin(alpha1));
                    lineAxis.push_back(0.02f);
                    lineAxis.insert(0,0.02f);
                    lineAxis.insert(0,y2 + r1 * sin(alpha2));
                    lineAxis.insert(0,x2  + r1 * cos(alpha2));
                    //lineAxis.push_back(x2  + r1 * cos(alpha2));
                    //lineAxis.push_back(y2 + r1 * sin(alpha2));
                    //lineAxis.push_back(0.02f);
                }
            }
            else
            {

                float x1 = pointsArray[(i - 1) * 3];
                float y1 = pointsArray[(i - 1) * 3 + 1];
                float x2 = pointsArray[i * 3];
                float y2 = pointsArray[i * 3 + 1];
                float x3 = pointsArray[(i + 1) * 3];
                float y3 = pointsArray[(i + 1) * 3 + 1];
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
                float r1 = width / 2.0f;
                float hamma = alpha + beta;
                float dx = (r1 / sin(alpha)) * cos(hamma);
                float dy = (r1 / sin(alpha)) * sin(hamma);

                lineAxis.push_back(x2 - dx);
                lineAxis.push_back(y2 - dy);
                lineAxis.push_back(0.02f);


            }
        }
    }

    ////////////////////////////////////////////////////////
    int counter = 3;
    for (int i = size / 3 - 2; i >= 0; --i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            if (!beginRounding)
            {
                float x1 = pointsArray[i * 3];
                float y1 = pointsArray[i * 3 + 1];
                float x2 = pointsArray[(i + 1) * 3];
                float y2 = pointsArray[(i + 1) * 3 + 1];

                float r = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
                //float xR1, yR1, xR2, yR2;
                float pi = 3.14159265f;
                float t = (x1 - x2) / r;
                if (t > 1)
                    t = 1.0f;
                if (t < -1)
                    t = -1.0f;
                float alpha = acos(t);

                if ((x1 - x2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                //float alpha2 = alpha1 + pi;
                float r1 = width / 2.0f;

                lineAxis.push_back(x1 + r1 * cos(alpha1));
                lineAxis.push_back(y1 + r1 * sin(alpha1));
                lineAxis.push_back(0.02f);
            }
            else
            {
                if (endRounding)
                {
                float x1 = lineAxis[0];
                float y1 = lineAxis[1];
                float z1 = lineAxis[2];
                lineAxis.push_back(x1);
                lineAxis.push_back(y1);
                lineAxis.push_back(z1);
                }
            }

        }
        // Если теукщий индекс - конец осевой линии,
        // то строим перпендикуляр

        else
        {
            float x1 = pointsArray[(i - 1) * 3];
            float y1 = pointsArray[(i - 1) * 3 + 1];
            float x2 = pointsArray[i * 3];
            float y2 = pointsArray[i * 3 + 1];
            float x3 = pointsArray[(i + 1) * 3];
            float y3 = pointsArray[(i + 1) * 3 + 1];
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
            float r1 = width / 2.0f;
            float hamma = alpha + beta;
            float dx = (r1 / sin(alpha)) * cos(hamma);
            float dy = (r1 / sin(alpha)) * sin(hamma);

            if (!endRounding)
            {
                lineAxis.insert(counter,0.02f);
                lineAxis.insert(counter,y2 + dy);
                lineAxis.insert(counter,x2 + dx);
                counter += 3;
            }
            else
            {
                lineAxis.push_back(x2 + dx);
                lineAxis.push_back(y2 + dy);
                lineAxis.push_back(0.02f);
            }

        }

    }

    this->size = lineAxis.size();
    if (lineAxisArray != NULL)
    {
        delete lineAxisArray;
        lineAxisArray = NULL;
    }
    lineAxisArray = new GLfloat[this->size];
    for (int i = 0; i < this->size; ++i)
        lineAxisArray[i] = lineAxis[i];

    if (line != NULL)
    {
        line->setVertexArrayForAxis(lineAxisArray, this->size);
        line->setVertexArray(lineWidth,lineAxisArray, this->size);
        line->setTextureArray();
    }
    float sumX = 0.0f, sumY = 0.0f;
    for (int i = 0; i < this->axisArray.size() / 3; ++i)
    {
        sumX += this->axisArray[i * 3];
        sumY += this->axisArray[i * 3 + 1];
    }
    elementX = sumX / float(this->axisArray.size() / 3);
    elementY = sumY / float(this->axisArray.size() / 3);
}

void SplitZone::reset()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::reset()\n";
    switch (type)
    {
    case 0:
        line->setVertexArrayForAxis(lineAxisArray, this->size);
        line->setVertexArray(lineWidth,lineAxisArray, this->size);
        line->setTextureArray();
        break;
    case 1:
        qDebug() << "Grass";
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
        break;
    case 2:
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
        break;
    default:
        break;
    }
}

void SplitZone::setBoardVertexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setBoardVertexArray()\n";
    boardVertexArray.clear();
    for (int i = 0; i < size / 3; ++i)
    {
        if (i == 0)
        {
            float r = 0.25f;
            float x1 = lineAxisArray[i * 3];
            float y1 = lineAxisArray[i * 3 + 1];
            float x2 = lineAxisArray[(i + 1) * 3];
            float y2 = lineAxisArray[(i + 1) * 3 + 1];
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

            boardVertexArray.push_back(x1);
            boardVertexArray.push_back(y1);
            boardVertexArray.push_back(0.0f);

            boardVertexArray.push_back(x1);
            boardVertexArray.push_back(y1);
            boardVertexArray.push_back(height - 0.02f);

            boardVertexArray.push_back(x1 - dx / r * 0.03f);
            boardVertexArray.push_back(y1 + dy / r * 0.03f);
            boardVertexArray.push_back(height);

            boardVertexArray.push_back(x1 - dx);
            boardVertexArray.push_back(y1 + dy);
            boardVertexArray.push_back(height);
        }
        else
        if (i == size / 3 - 1)
        {
            float r = 0.25f;
            float x1 = lineAxisArray[i * 3];
            float y1 = lineAxisArray[i * 3 + 1];
            float x2 = lineAxisArray[(i - 1) * 3];
            float y2 = lineAxisArray[(i - 1) * 3 + 1];
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

            boardVertexArray.push_back(x1);
            boardVertexArray.push_back(y1);
            boardVertexArray.push_back(0.0f);

            boardVertexArray.push_back(x1);
            boardVertexArray.push_back(y1);
            boardVertexArray.push_back(height - 0.02f);

            boardVertexArray.push_back(x1 + dx / r * 0.03f);
            boardVertexArray.push_back(y1 - dy / r * 0.03f);
            boardVertexArray.push_back(height);

            boardVertexArray.push_back(x1 + dx);
            boardVertexArray.push_back(y1 - dy);
            boardVertexArray.push_back(height);
        }
        else
        {
            float r = 0.25f;
            float x1 = lineAxisArray[(i - 1) * 3];
            float y1 = lineAxisArray[(i - 1) * 3 + 1];
            float x2 = lineAxisArray[i * 3];
            float y2 = lineAxisArray[i * 3 + 1];
            float x3 = lineAxisArray[(i + 1) * 3];
            float y3 = lineAxisArray[(i + 1) * 3 + 1];
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
            float dx = (r / sin(alpha)) * cos(hamma);
            float dy = (r / sin(alpha)) * sin(hamma);

            boardVertexArray.push_back(x2);
            boardVertexArray.push_back(y2);
            boardVertexArray.push_back(0.0f);

            boardVertexArray.push_back(x2);
            boardVertexArray.push_back(y2);
            boardVertexArray.push_back(height - 0.02f);

            boardVertexArray.push_back(x2 + dx / r * 0.03f);
            boardVertexArray.push_back(y2 + dy / r * 0.03f);
            boardVertexArray.push_back(height);

            boardVertexArray.push_back(x2 + dx);
            boardVertexArray.push_back(y2 + dy);
            boardVertexArray.push_back(height);

            boardVertexArray.push_back(x2);
            boardVertexArray.push_back(y2);
            boardVertexArray.push_back(0.0f);

            boardVertexArray.push_back(x2);
            boardVertexArray.push_back(y2);
            boardVertexArray.push_back(height - 0.02f);

            boardVertexArray.push_back(x2 + dx / r * 0.03f);
            boardVertexArray.push_back(y2 + dy / r * 0.03f);
            boardVertexArray.push_back(height);

            boardVertexArray.push_back(x2 + dx);
            boardVertexArray.push_back(y2 + dy);
            boardVertexArray.push_back(height);
        }
    }
}

void SplitZone::setBoardTextureArray(float textureUsize, float textureVsize)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setBoardTextureArray(float textureUsize, float textureVsize)"
                                       << " textureUsize = " << textureUsize
                                       << " textureVsize" << textureVsize << "\n";
    boardTextureArray.clear();
    for (int i = 0; i < boardVertexArray.size() / 3 - 4; i += 8)
    {
        float x0 = boardVertexArray[i * 3];
        float y0 = boardVertexArray[i * 3 + 1];
        float x1 = boardVertexArray[(i + 1) * 3];
        float y1 = boardVertexArray[(i + 1) * 3 + 1];
        float x2 = boardVertexArray[(i + 2) * 3];
        float y2 = boardVertexArray[(i + 2) * 3 + 1];
        float x3 = boardVertexArray[(i + 3) * 3];
        float y3 = boardVertexArray[(i + 3) * 3 + 1];
        float x4 = boardVertexArray[(i + 4) * 3];
        float y4 = boardVertexArray[(i + 4) * 3 + 1];
        float x5 = boardVertexArray[(i + 5) * 3];
        float y5 = boardVertexArray[(i + 5) * 3 + 1];
        float x6 = boardVertexArray[(i + 6) * 3];
        float y6 = boardVertexArray[(i + 6) * 3 + 1];
        float x7 = boardVertexArray[(i + 7) * 3];
        float y7 = boardVertexArray[(i + 7) * 3 + 1];
        //float pi = 3.14159265f;

        boardTextureArray.push_back(0.0f);
        boardTextureArray.push_back(0.0f);

        float r1 = sqrt((x4 - x0) * (x4 - x0) + (y4 - y0) * (y4 - y0));

        float factor;

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x1 - x0) + (y4 - y0) * (y1 - y0))/r1;

        boardTextureArray.push_back(0.08f);
        boardTextureArray.push_back(factor/textureVsize);

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x2 - x0) + (y4 - y0) * (y2 - y0))/r1;

        boardTextureArray.push_back(0.09f);
        boardTextureArray.push_back(factor/textureVsize);

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x3 - x0) + (y4 - y0) * (y3 - y0))/r1;

        boardTextureArray.push_back(1.0f);
        boardTextureArray.push_back(factor/textureVsize);


        //////////////////////////////////////////////////////////////

        boardTextureArray.push_back(0.0f);
        boardTextureArray.push_back(r1 / textureVsize);

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x5 - x0) + (y4 - y0) * (y5 - y0))/r1;

        boardTextureArray.push_back(0.08f);
        boardTextureArray.push_back(r1 / textureVsize);

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x6 - x0) + (y4 - y0) * (y6 - y0))/r1;

        boardTextureArray.push_back(0.09f);
        boardTextureArray.push_back(factor / textureVsize);

        if (x4 == x0 && y4 == y0)
            factor = 0.0f;
        else
            factor = ((x4 - x0) * (x7 - x0) + (y4 - y0) * (y7 - y0))/r1;

        boardTextureArray.push_back(1.0f);
        boardTextureArray.push_back(factor / textureVsize);
    }
}

void SplitZone::setBoardIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setBoardIndexArray()\n";
    boardIndexArray.clear();
    for (int i = 0; i < boardVertexArray.size() / 3; i += 8)
    {
        boardIndexArray.push_back(i);
        boardIndexArray.push_back(i + 4);
        boardIndexArray.push_back(i + 5);

        boardIndexArray.push_back(i);
        boardIndexArray.push_back(i + 5);
        boardIndexArray.push_back(i + 1);

        boardIndexArray.push_back(i + 1);
        boardIndexArray.push_back(i + 5);
        boardIndexArray.push_back(i + 6);

        boardIndexArray.push_back(i + 1);
        boardIndexArray.push_back(i + 6);
        boardIndexArray.push_back(i + 2);

        boardIndexArray.push_back(i + 2);
        boardIndexArray.push_back(i + 6);
        boardIndexArray.push_back(i + 7);

        boardIndexArray.push_back(i + 2);
        boardIndexArray.push_back(i + 7);
        boardIndexArray.push_back(i + 3);
    }
}

void SplitZone::setVertexArray()
{    
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setVertexArray()\n";
    vertexArray.clear();
    if ((!beginRounding && endRounding) || (beginRounding && !endRounding))
    {
        int count = size / 3;
        int i = count / 2;
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3]);
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 2]);
        vertexArray.push_back(boardVertexArray[((i + 1) * 8 + 3) * 3]);
        vertexArray.push_back(boardVertexArray[((i + 1) * 8 + 3) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[((i + 1) * 8 + 3) * 3 + 2]);
        vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3]);
        vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 2]);

        for (i = count / 2 - 1; i > 0; --i)
        {
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 2]);

            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[((count - i - 1) * 8 - 5) * 3]);
            vertexArray.push_back(boardVertexArray[((count - i - 1) * 8 - 5) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[((count - i - 1) * 8 - 5) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[((count - i) * 8 - 5) * 3 + 2]);
        }
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3]);
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 2]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 3) * 3]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 3) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 3) * 3 + 2]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 7) * 3]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 7) * 3 + 1]);
        vertexArray.push_back(boardVertexArray[((count - 2) * 8 + 7) * 3 + 2]);
    }
    else
    if (beginRounding && endRounding)
    {

        for (int i = 2; i <= numberOfSides; ++i)
        {
            vertexArray.push_back(boardVertexArray[(i * 0 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[(i * 0 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i * 0 + 3) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[((i - 1) * 8 + 3) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i * 8 + 3) * 3 + 2]);

        }

        int count = boardVertexArray.size() / 3;
        for (int i = numberOfSides * 8; i < boardVertexArray.size() / 6 + numberOfSides * 4; i += 8)
        {
            vertexArray.push_back(boardVertexArray[(i + 3) * 3]);
            vertexArray.push_back(boardVertexArray[(i + 3) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i + 3) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3]);
            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3 + 2]);

            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3]);
            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(count - i - 1 + numberOfSides * 8) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(i + 7) * 3 + 2]);
            vertexArray.push_back(boardVertexArray[(count - i - 5 + numberOfSides * 8) * 3]);
            vertexArray.push_back(boardVertexArray[(count - i - 5 + numberOfSides * 8) * 3 + 1]);
            vertexArray.push_back(boardVertexArray[(count - i - 5 + numberOfSides * 8) * 3 + 2]);

        }
    }

}

void SplitZone::setTextureArray(float textureUSize, float textureVSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setTextureArray(float textureUSize, float textureVSize)"
                                       << " textureUSize = " << textureUSize
                                       << " textureVSize = " << textureVSize << "\n";
    textureArray.clear();

    textureArray.push_back(0.0f);
    textureArray.push_back(0.0f);

    float x1, y1;
    float x2, y2;
    float r1;
    x1 = vertexArray[0];
    y1 = vertexArray[1];
    x2 = vertexArray[3];
    y2 = vertexArray[4];
    r1 = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

    textureArray.push_back(r1 / textureUSize);
    textureArray.push_back(0.0f);

    for (int i = 2; i < vertexArray.size() / 3; ++i)
    {
        float x3 = vertexArray[i * 3];
        float y3 = vertexArray[i * 3 + 1];
        float r2 = sqrt((x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1));
        float angle = acos(((x3 - x1)*(x2 - x1) + (y3 - y1)*(y2 - y1))/(r1 * r2));
        float dU = r2 * cos(angle);
        float dV = r2 * sin(angle);
        textureArray.push_back(dU / textureUSize);
        textureArray.push_back(dV / textureVSize);
    }
}

void SplitZone::setIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setIndexArray()\n";
    indexArray.clear();
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        indexArray.push_back(i);
    }
}

float SplitZone::getWidth()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getWidth()\n";
    return width;
}

void SplitZone::setDescription(const QString& description)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setDescription(const QString& description)"
                                       << " description = " << description << "\n";
    this->description = description;
}

void SplitZone::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    if (render && description[0] != '\0')
    {
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
//        x = xCenter;
//        y = yCenter;
        x = elementX;
        y = elementY;
        z = 0.0f;
        QFont shrift = QFont("Times", 8, QFont::Black);
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, description, shrift);
    }
}

void SplitZone::rotate(float angle, float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::rotate(float angle, float x, float y, float z)"
                                       << " angle = " << angle
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    float tx = 0.0f, ty = 0.0f;
    for (int i = 0; i < axisArray.size() / 3; ++i)
    {
        axisArray[i * 3] -= x;
        axisArray[i * 3 + 1] -= y;
        tx = axisArray[i * 3];
        ty = axisArray[i * 3 + 1];
        axisArray[i * 3] = tx * cos(angle) - ty * sin(angle);
        axisArray[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
        axisArray[i * 3] += x;
        axisArray[i * 3 + 1] += y;
    }
    p1.x -= x;
    p1.y -= y;
    tx = p1.x;
    ty = p1.y;
    p1.x = tx * cos(angle) - ty * sin(angle);
    p1.y = tx * sin(angle) + ty * cos(angle);
    p1.x += x;
    p1.y += y;

    p2.x -= x;
    p2.y -= y;
    tx = p2.x;
    ty = p2.y;
    p2.x = tx * cos(angle) - ty * sin(angle);
    p2.y = tx * sin(angle) + ty * cos(angle);
    p2.x += x;
    p2.y += y;

    p3.x -= x;
    p3.y -= y;
    tx = p3.x;
    ty = p3.y;
    p3.x = tx * cos(angle) - ty * sin(angle);
    p3.y = tx * sin(angle) + ty * cos(angle);
    p3.x += x;
    p3.y += y;

    p4.x -= x;
    p4.y -= y;
    tx = p4.x;
    ty = p4.y;
    p4.x = tx * cos(angle) - ty * sin(angle);
    p4.y = tx * sin(angle) + ty * cos(angle);
    p4.x += x;
    p4.y += y;

    line1_p1.x -= x;
    line1_p1.y -= y;
    tx = line1_p1.x;
    ty = line1_p1.y;
    line1_p1.x = tx * cos(angle) - ty * sin(angle);
    line1_p1.y = tx * sin(angle) + ty * cos(angle);
    line1_p1.x += x;
    line1_p1.y += y;

    line1_p2.x -= x;
    line1_p2.y -= y;
    tx = line1_p2.x;
    ty = line1_p2.y;
    line1_p2.x = tx * cos(angle) - ty * sin(angle);
    line1_p2.y = tx * sin(angle) + ty * cos(angle);
    line1_p2.x += x;
    line1_p2.y += y;

    line2_p1.x -= x;
    line2_p1.y -= y;
    tx = line2_p1.x;
    ty = line2_p1.y;
    line2_p1.x = tx * cos(angle) - ty * sin(angle);
    line2_p1.y = tx * sin(angle) + ty * cos(angle);
    line2_p1.x += x;
    line2_p1.y += y;

    line2_p2.x -= x;
    line2_p2.y -= y;
    tx = line2_p2.x;
    ty = line2_p2.y;
    line2_p2.x = tx * cos(angle) - ty * sin(angle);
    line2_p2.y = tx * sin(angle) + ty * cos(angle);
    line2_p2.x += x;
    line2_p2.y += y;

    xCenter -= x;
    yCenter -= y;
    tx = xCenter;
    ty = yCenter;
    xCenter = tx * cos(angle) - ty * sin(angle);
    yCenter = tx * sin(angle) + ty * cos(angle);
    xCenter += x;
    yCenter += y;

    elementX -= x;
    elementY -= y;
    tx = elementX;
    ty = elementY;
    elementX = tx * cos(angle) - ty * sin(angle);
    elementY = tx * sin(angle) + ty * cos(angle);
    elementX += x;
    elementY += y;

    switch (type)
    {
    case 0:
        line->rotate(angle, x, y, z);
        break;
    case 1:
    {
        for (int i = 0; i < boardVertexArray.size() / 3; ++i)
        {
            boardVertexArray[i * 3] -= x;
            boardVertexArray[i * 3 + 1] -= y;
            tx = boardVertexArray[i * 3];
            ty = boardVertexArray[i * 3 + 1];
            boardVertexArray[i * 3] = tx * cos(angle) - ty * sin(angle);
            boardVertexArray[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
            boardVertexArray[i * 3] += x;
            boardVertexArray[i * 3 + 1] += y;
        }
        for (int i = 0; i < vertexArray.size() / 3; ++i)
        {
            vertexArray[i * 3] -= x;
            vertexArray[i * 3 + 1] -= y;
            tx = vertexArray[i * 3];
            ty = vertexArray[i * 3 + 1];
            vertexArray[i * 3] = tx * cos(angle) - ty * sin(angle);
            vertexArray[i * 3 + 1] = tx * sin(angle) + ty * cos(angle);
            vertexArray[i * 3] += x;
            vertexArray[i * 3 + 1] += y;
        }
    }
        break;
    case 2:
        break;
    default:
        break;
    }
}

void SplitZone::addBreak(bool front)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::addBreak(bool front)"
                                       << " front = " << front << "\n";
    switch (type)
    {
    case 0:
    {
        float x, y, z;
        if (front)
        {
            x = axisArray[0];
            y = axisArray[1];
            z = axisArray[2];
            axisArray.push_front(z);
            axisArray.push_front(y);
            axisArray.push_front(x);

        }
        else
        {
            int size = axisArray.size();
            x = axisArray[size - 3];
            y = axisArray[size - 2];
            z = axisArray[size - 1];
            axisArray.push_back(x);
            axisArray.push_back(y);
            axisArray.push_back(z);
        }
        calculateLine(axisArray, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
        line->setTextureArray();
        line->setIndexArray();
    }
        break;
    case 1:
    {
        float x, y, z;
        if (front)
        {
            x = axisArray[0];
            y = axisArray[1];
            z = axisArray[2];
            axisArray.push_front(z);
            axisArray.push_front(y);
            axisArray.push_front(x);

        }
        else
        {
            int size = axisArray.size();
            x = axisArray[size - 3];
            y = axisArray[size - 2];
            z = axisArray[size - 1];
            axisArray.push_back(x);
            axisArray.push_back(y);
            axisArray.push_back(z);
        }
        calculateLine(axisArray, width);
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    case 2:
    {
        float x, y, z;
        if (front)
        {
            x = axisArray[0];
            y = axisArray[1];
            z = axisArray[2];
            axisArray.push_front(z);
            axisArray.push_front(y);
            axisArray.push_front(x);

        }
        else
        {
            int size = axisArray.size();
            x = axisArray[size - 3];
            y = axisArray[size - 2];
            z = axisArray[size - 1];
            axisArray.push_back(x);
            axisArray.push_back(y);
            axisArray.push_back(z);
        }
        calculateLine(axisArray, width);
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    default:
        break;
    }


}

void SplitZone::deleteBreak(bool front)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::deleteBreak(bool front)"
                                       << " front = " << front << "\n";
    switch (type)
    {
    case 0:
    {
        if (front)
        {
            axisArray.pop_front();
            axisArray.pop_front();
            axisArray.pop_front();

        }
        else
        {
            axisArray.pop_back();
            axisArray.pop_back();
            axisArray.pop_back();
        }
        calculateLine(axisArray, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
        line->setTextureArray();
        line->setIndexArray();
    }
        break;
    case 1:
    {
        if (front)
        {
            axisArray.pop_front();
            axisArray.pop_front();
            axisArray.pop_front();

        }
        else
        {
            axisArray.pop_back();
            axisArray.pop_back();
            axisArray.pop_back();
        }
        calculateLine(axisArray, width);
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    case 2:
    {
        if (front)
        {
            axisArray.pop_front();
            axisArray.pop_front();
            axisArray.pop_front();

        }
        else
        {
            axisArray.pop_back();
            axisArray.pop_back();
            axisArray.pop_back();
        }
        calculateLine(axisArray, width);
        setBoardVertexArray();
        setBoardTextureArray(texture1USize, texture1VSize);
        setBoardIndexArray();
        setVertexArray();
        setTextureArray(texture2USize, texture2VSize);
        setIndexArray();
    }
        break;
    default:
        break;
    }
}

void SplitZone::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "SplitZone::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


RoadElement *SplitZone::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getCopy()\n";
    SplitZone* copyElement = new SplitZone(*this);
    return copyElement;
}


std::vector<vec3> SplitZone::getCoordOfControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::getCoordOfControl(int index)"
                                   << " index = " << index << "\n";

    switch (type)
    {
    case 0:
    case 1:
    case 2:
    {
        std::vector<vec3> res;
        vec3 p(axisArray[index * 3],
                axisArray[index * 3 + 1],
                axisArray[index * 3 + 2]);
        res.push_back(p);
        return res;
    }
        break;
    default:
        std::vector<vec3> res;
        return res;
        break;
    }

}

void SplitZone::setCoordForControl(int index, std::vector<vec3> &controls)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::setCoordForControl(int index, std::vector<vec3> &controls)"
                                   << " index = " << index << "\n";
    switch (type)
    {
    case 0:
    case 1:
    case 2:
    {
        float x, y;
        x = axisArray[index * 3];
        y = axisArray[index * 3 + 1];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    default:
        break;
    }
}


void SplitZone::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "SplitZone::clearProperties(QLayout *layout)\n";
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


QJsonObject SplitZone::getJSONInfo()
{
    if (log)
    Logger::getLogger()->infoLog() << "LineBroken::getJSONInfo()\n";
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    element["Width"] = width;
    element["Fixed"] = fixed;
    element["Type"] = type;
    element["BeginRounding"] = beginRounding;
    element["EndRounding"] = endRounding;
    element["Description"] = description;
    element["Id"] = Id;

    QJsonArray temp;

    for (int i = 0; i < axisArray.size(); ++i)
    {
        temp.append(QJsonValue(axisArray[i]));
    }
    element["AxisArray"] = temp;

    switch (type)
    {
    case 0:
        break;
    case 1:
    case 2:
    {
        element["Height"] = height;
        element["Texture1Source"] = texture1;
        element["Texture1USize"] = texture1USize;
        element["Texture1VSize"] = texture1VSize;
        element["Texture2Source"] = texture2;
        element["Texture2USize"] = texture2USize;
        element["Texture2VSize"] = texture2VSize;
    }
        break;
    default:
        break;
    }

    return element;
}
