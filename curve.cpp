#include "curve.h"
#include <GL/glu.h>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <assert.h>

bool Curve::log = true;

Curve::Curve(float xCenter, float yCenter, float zCenter,
             float xLeft, float yLeft, float zLeft,
             float xRight, float yRight, float zRight,
             QString texture_1, float texture_1Usize, float texture_1Vsize,
             QString texture_2, float texture_2Usize, float texture_2Vsize,
             int numberOfSides)
{

    this->numberOfSides = numberOfSides;
    controlPoints[0] = xCenter;
    controlPoints[1] = yCenter;
    controlPoints[2] = zCenter;

    controlPoints[3] = xLeft;
    controlPoints[4] = yLeft;
    controlPoints[5] = zLeft;

    controlPoints[6] = xRight;
    controlPoints[7] = yRight;
    controlPoints[8] = zRight;

    //leftLength = sqrt((xLeft - xCenter)*(xLeft - xCenter) + (yLeft - yCenter)*(yLeft - yCenter));
    //rightLength = sqrt((xRight - xCenter)*(xRight - xCenter) + (yRight - yCenter)*(yRight - yCenter));
    texture1 = texture_1;
    texture2 = texture_2;
    textureID[0] = TextureManager::getInstance()->getID(texture_1);
    //textureID[0] = getTextures(texture_1);
    this->texture1USize = texture_1Usize;
    this->texture1VSize = texture_1Vsize;

    textureID[1] = TextureManager::getInstance()->getID(texture_2);
    //textureID[1] = getTextures(texture_2);
    this->texture2USize = texture_2Usize;
    this->texture2VSize = texture_2Vsize;
    angleRounding = 0.0f;
    this->boardWidth = 2.75f;
    setVertexArray();
    setTextureArray();
    setIndexArray();

    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    setIndexArrayBoard();

    calculateAngle();
    setAngleVertexArray();
    setAngleColorArray(1.0f, 0.0f, 0.0f);
    setAngleIndexArray();
    //calculateAngle();
    fixed = selected = false;
    showBoard = true;
    layer = 0;
    name = "Curve";
}

Curve::Curve(float *controls, int size, QString texture_1, float texture_1Usize, float texture_1Vsize, QString texture_2, float texture_2Usize, float texture_2Vsize, int numberOfSides, float angleRounding, float boardWidth, bool fixed, bool showBoard, int layer, QString name)
{
    this->numberOfSides = numberOfSides;
    this->name = name;

    if (size == 9)
    {
        for (int i = 0; i < size; ++i)
            controlPoints[i] = controls[i];
    }

    texture1 = texture_1;
    texture2 = texture_2;
    textureID[0] = TextureManager::getInstance()->getID(texture_1);
    this->texture1USize = texture_1Usize;
    this->texture1VSize = texture_1Vsize;

    textureID[1] = TextureManager::getInstance()->getID(texture_2);
    this->texture2USize = texture_2Usize;
    this->texture2VSize = texture_2Vsize;
    this->angleRounding = angleRounding;
    this->boardWidth = boardWidth;
    setVertexArray();
    setTextureArray();
    setIndexArray();

    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    setIndexArrayBoard();

    calculateAngle();
    setAngleVertexArray();
    setAngleColorArray(1.0f, 0.0f, 0.0f);
    setAngleIndexArray();
    this->fixed = fixed;
    selected = false;
    this->showBoard = showBoard;
    this->layer = layer;
}

Curve::Curve(const Curve &source)
{
    this->numberOfSides = source.numberOfSides;
    controlPoints[0] = source.controlPoints[0];
    controlPoints[1] = source.controlPoints[1];
    controlPoints[2] = source.controlPoints[2];

    controlPoints[3] = source.controlPoints[3];
    controlPoints[4] = source.controlPoints[4];
    controlPoints[5] = source.controlPoints[5];

    controlPoints[6] = source.controlPoints[6];
    controlPoints[7] = source.controlPoints[7];
    controlPoints[8] = source.controlPoints[8];

    texture1 = source.texture1;
    texture2 = source.texture2;

    textureID[0] = source.textureID[0];
    this->texture1USize = source.texture1USize;
    this->texture1VSize = source.texture1VSize;

    textureID[1] = source.textureID[1];
    this->texture2USize = source.texture2USize;
    this->texture2VSize = source.texture2VSize;
    angleRounding = source.angleRounding;
    this->boardWidth = source.boardWidth;
    setVertexArray();
    setTextureArray();
    setIndexArray();

    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    setIndexArrayBoard();

    calculateAngle();
    setAngleVertexArray();
    setAngleColorArray(1.0f, 0.0f, 0.0f);
    setAngleIndexArray();
    name = source.name;
    fixed = source.fixed;
    selected = source.selected;
    showBoard = source.showBoard;
    layer = source.layer;
}

Curve::~Curve()
{
    layout = NULL;
    render = NULL;
    model = NULL;
}



bool Curve::isSelected()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::isSelected()\n";
    return selected;
}

void Curve::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
}

void Curve::drawFigure(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::drawFigure(QGLWidget *render)\n";
    if (selected)
    {
        if (indexOfSelectedControl >= 0 && indexOfSelectedControl < getNumberOfControls())
        {
            ////qDebug() << "Index " << indexOfSelectedControl;
            drawControlElement(indexOfSelectedControl, 5.0f, 10.0);
        }
        for (int i = 0; i < 4; ++i)
            drawControlElement(i, 5.0f, 10.f);       
    }
    ////qDebug() << "Vertices: " << vertexArray.size() / 3;
    ////qDebug() << "Textures: " << textureArray.size() / 2;
    ////qDebug() << "Indices: " << indexArray.size() / 3;
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if (angleRounding < 180)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[0]);
        glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
        glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_BYTE, indexArray.begin());
    }


    if (showBoard)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[1]);
        glVertexPointer(3, GL_FLOAT, 0, vertexArrayBoard.begin());
        glTexCoordPointer(2, GL_FLOAT, 0, textureArrayBoard.begin());
        glDrawElements(GL_TRIANGLES, indexArrayBoard.size(), GL_UNSIGNED_BYTE, indexArrayBoard.begin());
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);


}

void Curve::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::drawSelectionFrame()\n";
    if (showBoard)
    {
        glLineWidth(5.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < vertexArrayBoard.size() / 3 - 5; i += 10)
        {

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayBoard[(i + 4) * 3],
                    vertexArrayBoard[(i + 4) * 3 + 1],
                    vertexArrayBoard[(i + 4) * 3 + 2]);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(vertexArrayBoard[(i + 9) * 3],
                    vertexArrayBoard[(i + 9) * 3 + 1],
                    vertexArrayBoard[(i + 9) * 3 + 2]);

        }
        glEnd();
    }
}

void Curve::drawMeasurements(QGLWidget *render)
{
    if (!showMeasurements)
        return;
    if (log)
        Logger::getLogger()->infoLog() << "Curve::drawMeasurements(QGLWidget *render)\n";

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0FFF);
    glLineWidth(1.5f);
    glVertexPointer(3, GL_FLOAT, 0, angleVertexArray.begin());
    glColorPointer(3, GL_FLOAT, 0, angleColorArray.begin());
    glDrawElements(GL_LINE_STRIP,angleIndexArray.size(),GL_UNSIGNED_BYTE, angleIndexArray.begin());
    glDisable(GL_LINE_STIPPLE);

    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Black);
    if (render)
    {

        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(xRadius, yRadius, 0.3f);
        glEnd();

        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0FFF);
        glLineWidth(1.5f);
        //x = (xRadius + controlPoints[3]) / 2.0f;
        //y = (yRadius + controlPoints[4]) / 2.0f;
        x = xRadius;
        y = yRadius;
        z = 0.3f;
        getWindowCoord(x, y, z, wx, wy, wz);

        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(xRadius, yRadius, 0.3f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[3],
                controlPoints[4],
                0.3f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(xRadius, yRadius, 0.3f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[6],
                controlPoints[7],
                0.3f);
        glEnd();
        render->renderText(wx, wy, "R=" + QString("%1").arg(radius, 0, 'f', 2), shrift);

        x = (controlPoints[3] + controlPoints[0]) / 2.0f;
        y = (controlPoints[4] + controlPoints[1]) / 2.0f;
        z = (controlPoints[5] + controlPoints[2]) / 2.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx, wy, "L=" + QString("%1").arg(leftLength, 0, 'f', 2), shrift);

        x = (controlPoints[6] + controlPoints[0]) / 2.0f;
        y = (controlPoints[7] + controlPoints[1]) / 2.0f;
        z = (controlPoints[8] + controlPoints[2]) / 2.0f;
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx, wy, "L=" + QString("%1").arg(rightLength, 0, 'f', 2), shrift);
        glDisable(GL_LINE_STIPPLE);

        float i = angleVertexArray.size() / 6;
        x = angleVertexArray[i * 3];
        y = angleVertexArray[i * 3 + 1];
        z = angleVertexArray[i * 3 + 2];
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx, wy, "a=" + QString("%1").arg(angleRounding, 0, 'f', 2), shrift);
    }
    else
    {
        return;
    }

}

void Curve::move(float dx, float dy, float dz)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::move(float dx, float dy, float dz)"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
    if (fixed)
        return;

    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
    for (int i = 0; i < 3; ++i)
    {
        controlPoints[i * 3] += dx;
        controlPoints[i * 3 + 1] += dy;
    }
    for (int i = 0; i < vertexArrayBoard.size() / 3; ++i)
    {
        vertexArrayBoard[i * 3] += dx;
        vertexArrayBoard[i * 3 + 1] += dy;
    }
    for (int i = 0; i < angleVertexArray.size() / 3; ++i)
    {
        angleVertexArray[i * 3] += dx;
        angleVertexArray[i * 3 + 1] += dy;
    }
    xRadius += dx;
    yRadius += dy;
}

void Curve::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    switch (index)
    {
    case 0:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        glEnd();
    }
        break;
    case 1:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(controlPoints[3], controlPoints[4], controlPoints[5] + 0.001f);
        glEnd();
    }
        break;
    case 2:
    {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(controlPoints[6], controlPoints[7], controlPoints[8] + 0.001f);
        glEnd();
    }
        break;
    case 3:
    {
        if (showBoard)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            for (int i = 0; i < vertexArrayBoard.size() / 3 - 5; i += 10)
            {

                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayBoard[(i + 4) * 3],
                        vertexArrayBoard[(i + 4) * 3 + 1],
                        vertexArrayBoard[(i + 4) * 3 + 2]);
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(vertexArrayBoard[(i + 9) * 3],
                        vertexArrayBoard[(i + 9) * 3 + 1],
                        vertexArrayBoard[(i + 9) * 3 + 2]);

            }
            glEnd();
        }
    }
        break;
    case 4:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[3], controlPoints[4], controlPoints[5] + 0.001f);
        glEnd();
    }
        break;
    case 5:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(controlPoints[6], controlPoints[7], controlPoints[8] + 0.001f);
        glEnd();
    }
        break;
    default:
        break;
    }
}

QCursor Curve::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void Curve::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " x = " << x
                                       << " y = " << y << "\n";
    if (fixed)
        return;

    switch (index)
    {
    case 0:
    {
        controlPoints[0] += dx;
        controlPoints[1] += dy;
        calculateControlsForAngle(index);
        setVertexArray();
        setTextureArray();
        setVertexArrayBoard();
        setTextureArrayBoard(texture2USize, texture2VSize);
        setAngleVertexArray();
        calculateAngle();
        emit leftLengthChanged(leftLength);
        emit rightLengthChanged(rightLength);
        //emit angleChanged(angleRounding);
    }
        break;
    case 1:
    {
        controlPoints[3] += dx;
        controlPoints[4] += dy;
        calculateControlsForAngle(index);
        setVertexArray();
        setTextureArray();
        setVertexArrayBoard();
        setTextureArrayBoard(texture2USize, texture2VSize);
        setAngleVertexArray();
        calculateAngle();
        emit leftLengthChanged(leftLength);
        //emit angleChanged(angleRounding);
    }
        break;
    case 2:
    {
        controlPoints[6] += dx;
        controlPoints[7] += dy;
        calculateControlsForAngle(index);
        setVertexArray();
        setTextureArray();
        setVertexArrayBoard();
        setTextureArrayBoard(texture2USize, texture2VSize);
        setAngleVertexArray();
        calculateAngle();
        emit rightLengthChanged(rightLength);
        //emit angleChanged(angleRounding);
    }
        break;
    case 3:
    {
        float dr = ((x - xRadius)*dx + (y - yRadius)*dy)/
                sqrt((x - xRadius)*(x - xRadius) + (y - yRadius)*(y - yRadius));
        boardWidth -= dr;
        setVertexArrayBoard();
        setTextureArrayBoard(texture2USize, texture2VSize);
    }
        break;
    default:
        break;
    }

}

int Curve::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getNumberOfControls()\n";
    return 4;
}

int Curve::controlsForPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::controlsForPoint()\n";
    return 1;
}

void Curve::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    indexOfSelectedControl = index;
}

void Curve::getProperties(QFormLayout *layout, QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getProperties(QFormLayout *layout, QGLWidget *render)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Curve::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "Curve::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL\n";
        QApplication::exit(0);
    }
    this->layout = layout;
    this->render = render;

    QCheckBox *fixedCheckBox = new QCheckBox();
    fixedCheckBox->setChecked(fixed);
    connect(fixedCheckBox, SIGNAL(toggled(bool)), this, SLOT(setFixed(bool)));

    QDoubleSpinBox *leftLengthDoubleSpinBox = new QDoubleSpinBox();
    leftLengthDoubleSpinBox->setKeyboardTracking(false);
    leftLengthDoubleSpinBox->setMinimum(0.0);
    leftLengthDoubleSpinBox->setValue(leftLength);
    connect(this, SIGNAL(leftLengthChanged(double)), leftLengthDoubleSpinBox, SLOT(setValue(double)));
    connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLeftLength(double)));

    QDoubleSpinBox *rightLengthDoubleSpinBox = new QDoubleSpinBox();
    rightLengthDoubleSpinBox->setKeyboardTracking(false);
    rightLengthDoubleSpinBox->setMinimum(0.0);
    rightLengthDoubleSpinBox->setValue(rightLength);
    connect(this, SIGNAL(rightLengthChanged(double)), rightLengthDoubleSpinBox, SLOT(setValue(double)));
    connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRightLength(double)));

    QCheckBox *showBoardCheckBox = new QCheckBox();
    showBoardCheckBox->setChecked(showBoard);
    connect(showBoardCheckBox, SIGNAL(toggled(bool)), this, SLOT(setBoardShowStatus(bool)));

    QDoubleSpinBox *boardWidthDoubleSpinBox = new QDoubleSpinBox();
    boardWidthDoubleSpinBox->setKeyboardTracking(false);
    boardWidthDoubleSpinBox->setMinimum(0.0);
    boardWidthDoubleSpinBox->setValue(boardWidth);
    connect(this, SIGNAL(boardWidthChanged(double)), boardWidthDoubleSpinBox, SLOT(setValue(double)));
    connect(boardWidthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setBoardWidth(double)));

    QDoubleSpinBox *angleDoubleSpinBox = new QDoubleSpinBox();
    angleDoubleSpinBox->setKeyboardTracking(false);
    angleDoubleSpinBox->setMinimum(0.0);
    angleDoubleSpinBox->setMaximum(720.0);
    angleDoubleSpinBox->setValue(angleRounding);
    connect(this, SIGNAL(angleChanged(double)), angleDoubleSpinBox, SLOT(setValue(double)));
    connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));

    QCheckBox *showMeasurementsCheckBox = new QCheckBox();
    showMeasurementsCheckBox->setChecked(showMeasurements);
    connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), this, SLOT(setShowMeasurements(bool)));

    if (render)
    {
        connect(leftLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(rightLengthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(showBoardCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
        connect(boardWidthDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(angleDoubleSpinBox, SIGNAL(valueChanged(double)), render, SLOT(updateGL()));
        connect(showMeasurementsCheckBox, SIGNAL(toggled(bool)), render, SLOT(updateGL()));
    }
    layout->addRow("Зафиксировать", fixedCheckBox);
    layout->addRow("Левая сторона", leftLengthDoubleSpinBox);
    layout->addRow("Правая сторона", rightLengthDoubleSpinBox);
    layout->addRow("Угол", angleDoubleSpinBox);
    layout->addRow("Тротуар", showBoardCheckBox);
    layout->addRow("Ширина", boardWidthDoubleSpinBox);
    layout->addRow("Размеры", showMeasurementsCheckBox);
}

bool Curve::isFixed()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::isFixed()\n";
    return fixed;
}

int Curve::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getLayer()\n";
    return layer;
}

void Curve::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::clear()\n";
}

void Curve::setVertexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setVertexArray()\n";
    vertexArray.clear();
    float pi = 3.14159265f;

    float xCenter = controlPoints[0];
    float yCenter = controlPoints[1];
    float zCenter = controlPoints[2];
    float xLeft = controlPoints[3];
    float yLeft = controlPoints[4];
    float zLeft = controlPoints[5];
    float xRight = controlPoints[6];
    float yRight = controlPoints[7];
    float zRight = controlPoints[8];

    if (angleRounding >= 180.0f)
    {

        //vertexArray.push_back(xCenter);
        //vertexArray.push_back(yCenter);
        //vertexArray.push_back(zCenter);
        vertexArray.push_back(xLeft);
        vertexArray.push_back(yLeft);
        vertexArray.push_back(zLeft);
        vertexArray.push_back(xRight);
        vertexArray.push_back(yRight);
        vertexArray.push_back(zRight);
        leftLength = sqrt((xLeft - xCenter)*(xLeft - xCenter) + (yLeft - yCenter)*(yLeft - yCenter));
        rightLength = sqrt((xRight - xCenter)*(xRight - xCenter) + (yRight - yCenter)*(yRight - yCenter));
        //this->angle1 = angle1;
        //this->angle2 = angle2;
        //this->radius = 0.0f;
        //this->xRadius = 0.0f;
        //this->yRadius = 0.0f;
        return;
    }
    float aLeft = xLeft - xCenter;
    float bLeft = yLeft - yCenter;
    //float cLeft = (-1.0f)*(xLeft * aLeft + yLeft * bLeft);
    float cLeft = (-1.0f)*(xLeft * (xLeft - xCenter) + yLeft * (yLeft - yCenter));

    float aRight = xRight - xCenter;
    float bRight = yRight - yCenter;
    float cRight = (-1.0f)*(xRight * (xRight - xCenter) + yRight * (yRight - yCenter));

    float a = xRight - xLeft;
    float b = yRight - yLeft;
    float c = (-1.0f)*((xRight + xLeft) / 2.0f * (xRight - xLeft) +
                       (yRight + yLeft) / 2.0f * (yRight - yLeft));
    float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

    if (a != 0.0f)
    {
        y1 = (c * aLeft - cLeft * a) / (bLeft * a - b * aLeft);
        x1 = (-1.0f) * (b * y1 + c) / a;

        y2 = (c * aRight - cRight * a) / (bRight * a - b * aRight);
        x2 = (-1.0f) * (b * y2 + c) / a;
    }
    else
    {
        y1 = y2 = (yRight + yLeft) / 2.0f;
        x1 = (-1.0f) * (bLeft * y1 + cLeft) / aLeft;
        x2 = (-1.0f) * (bRight * y2 + cRight) / aRight;
    }


    float r1 = sqrt((x1 - xLeft)*(x1 - xLeft) + (y1 - yLeft)*(y1 - yLeft));
    float r2 = sqrt((x2 - xRight)*(x2 - xRight) + (y2 - yRight)*(y2 - yRight));

    float x, y;

    x = r1 > r2 ? x1 : x2;
    y = r1 > r2 ? y1 : y2;

    float dx = xRight - x;
    float dy = yRight - y;
    float r = sqrt(dx * dx + dy * dy);
    float t = dx / r;
    if (t > 1.0f)
        t = 1.0f;
    if (t < -1.0f)
        t = -1.0f;
    float angle2 = acos(t);
    if (dy < 0)
        angle2 = 2.0f * pi - angle2;

    dx = xLeft - x;
    dy = yLeft - y;
    r = sqrt(dx * dx + dy * dy);
    t = dx / r;
    if (t > 1.0f)
        t = 1.0f;
    if (t < -1.0f)
        t = -1.0f;
    float angle1 = acos(t);
    if (dy < 0)
        angle1 = 2.0f * pi - angle1;

    if (angle1 > angle2)
    {
        angle2 += 2.0f * pi;
    }
    this->angle1 = angle1;
    this->angle2 = angle2;
    this->radius = r;
    this->xRadius = x;
    this->yRadius = y;
    leftLength = sqrt((xLeft - xCenter)*(xLeft - xCenter) + (yLeft - yCenter)*(yLeft - yCenter));
    //leftLength = floor(leftLength / 0.01 + 0.5) * 0.01;
    rightLength = sqrt((xRight - xCenter)*(xRight - xCenter) + (yRight - yCenter)*(yRight - yCenter));
    //rightLength = floor(rightLength / 0.01 + 0.5) * 0.01;
    vertexArray.push_back(xCenter);
    vertexArray.push_back(yCenter);
    vertexArray.push_back(zCenter);
    ////qDebug() << "---------------";
    ////qDebug() << "A1 = " << angle1 * 180.0f / 3.14159265f;
    ////qDebug() << "A2 = " << angle2 * 180.0f / 3.14159265f;

    for (int i = 0; i <= numberOfSides; ++i)
    {

        float angle = (angle1 + (angle2 - angle1) * float(i) / float(numberOfSides));
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        vertexArray.push_back(x + dx);
        vertexArray.push_back(y + dy);
        vertexArray.push_back(0.0f);
    }
}

void Curve::setTextureArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setTextureArray()\n";
    textureArray.clear();
    float x = controlPoints[0];
    float y = controlPoints[1];
    float x1 = vertexArray[3];
    float y1 = vertexArray[4];
    float xTotal = 0.0f;
    float yTotal = 0.0f;

    float dx = x1 - x;
    float dy = y1 - y;
    float R = sqrt(dx*dx + dy*dy);

    textureArray.push_back(0.0f);
    textureArray.push_back(0.0f);

    textureArray.push_back(0.0f);
    textureArray.push_back(R / texture1VSize);

    for (int i = 2; i < vertexArray.size() / 3; ++i)
    {
        float x2 = vertexArray[i * 3];
        float y2 = vertexArray[i * 3 + 1];
        float dx1 = x2 - x;
        float dy1 = y2 - y;
        float r = sqrt(dx1*dx1 + dy1*dy1);
        float cosAngle = (dx*dx1 + dy*dy1)/(R*r);
        if (cosAngle > 1.0f)
            cosAngle = 1.0f;
        if (cosAngle < -1.0f)
            cosAngle = -1.0f;
        float sinAngle = sqrt(1 - cosAngle * cosAngle);
        if (sinAngle > 1.0f)
            sinAngle = 1.0f;
        if (sinAngle < -1.0f)
            sinAngle = -1.0f;
        textureArray.push_back(r * sinAngle);
        textureArray.push_back(r * cosAngle);
    }
}

void Curve::setIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setIndexArray()\n";
    indexArray.clear();
    for (int i = 1; i < vertexArray.size() / 3 - 1; ++i)
    {

        indexArray.push_back(i + 1);
        indexArray.push_back(i);
        indexArray.push_back(0);
    }

}

void Curve::setVertexArrayBoard()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setVertexArrayBoard()\n";
    vertexArrayBoard.clear();

    if (angleRounding == 180)
    {
        float r = boardWidth;

        float x1 = controlPoints[3];
        float y1 = controlPoints[4];
        float x2 = controlPoints[6];
        float y2 = controlPoints[7];
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
        vertexArrayBoard.push_back(x1);
        vertexArrayBoard.push_back(y1);
        vertexArrayBoard.push_back(0.0f);

        vertexArrayBoard.push_back(x1);
        vertexArrayBoard.push_back(y1);
        vertexArrayBoard.push_back(0.08f);

        vertexArrayBoard.push_back(x1 - dx / r * 0.03f);
        vertexArrayBoard.push_back(y1 + dy / r * 0.03f);
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(x1 - dx / r * 0.25f);
        vertexArrayBoard.push_back(y1 + dy / r * 0.25f);
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(x1 - dx);
        vertexArrayBoard.push_back(y1 + dy);
        vertexArrayBoard.push_back(0.1f);

        //////////////////////////////////////////

        vertexArrayBoard.push_back(x2);
        vertexArrayBoard.push_back(y2);
        vertexArrayBoard.push_back(0.0f);

        vertexArrayBoard.push_back(x2);
        vertexArrayBoard.push_back(y2);
        vertexArrayBoard.push_back(0.08f);

        vertexArrayBoard.push_back(x2 - dx / r * 0.03f);
        vertexArrayBoard.push_back(y2 + dy / r * 0.03f);
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(x2 - dx / r * 0.25f);
        vertexArrayBoard.push_back(y2 + dy / r * 0.25f);
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(x2 - dx);
        vertexArrayBoard.push_back(y2 + dy);
        vertexArrayBoard.push_back(0.1f);
        return;

    }
    for (int i = 0; i <= numberOfSides; ++i)
    {

        float angle = (angle1 + (angle2 - angle1) * float(i) / float(numberOfSides));
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);

        vertexArrayBoard.push_back(xRadius + dx);
        vertexArrayBoard.push_back(yRadius + dy);
        vertexArrayBoard.push_back(0.0f);

        vertexArrayBoard.push_back(xRadius + dx);
        vertexArrayBoard.push_back(yRadius + dy);
        vertexArrayBoard.push_back(0.08f);

        vertexArrayBoard.push_back(xRadius + dx - 0.03f * cosf(angle));
        vertexArrayBoard.push_back(yRadius + dy - 0.03f * sinf(angle));
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(xRadius + dx - 0.25f * cosf(angle));
        vertexArrayBoard.push_back(yRadius + dy - 0.25f * sinf(angle));
        vertexArrayBoard.push_back(0.1f);

        vertexArrayBoard.push_back(xRadius + dx - boardWidth * cosf(angle));
        vertexArrayBoard.push_back(yRadius + dy - boardWidth * sinf(angle));
        vertexArrayBoard.push_back(0.1f);

        if (i != 0 && i != numberOfSides)
        {
            vertexArrayBoard.push_back(xRadius + dx);
            vertexArrayBoard.push_back(yRadius + dy);
            vertexArrayBoard.push_back(0.0f);

            vertexArrayBoard.push_back(xRadius + dx);
            vertexArrayBoard.push_back(yRadius + dy);
            vertexArrayBoard.push_back(0.08f);

            vertexArrayBoard.push_back(xRadius + dx - 0.03f * cosf(angle));
            vertexArrayBoard.push_back(yRadius + dy - 0.03f * sinf(angle));
            vertexArrayBoard.push_back(0.1f);

            vertexArrayBoard.push_back(xRadius + dx - 0.25f * cosf(angle));
            vertexArrayBoard.push_back(yRadius + dy - 0.25f * sinf(angle));
            vertexArrayBoard.push_back(0.1f);

            vertexArrayBoard.push_back(xRadius + dx - boardWidth * cosf(angle));
            vertexArrayBoard.push_back(yRadius + dy - boardWidth * sinf(angle));
            vertexArrayBoard.push_back(0.1f);
        }
    }
}

void Curve::setTextureArrayBoard(float textureUSize, float textureVSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setTextureArrayBoard(float textureUSize, float textureVSize)"
                                       << " textureUSize = " << textureUSize
                                       << " textureVSize = " << textureVSize << "\n";
    textureArrayBoard.clear();
    float x1, y1, x2, y2;
    float r1 = 0.0f, r2 = 0.0f, r3 = 0.0f, r4 = 0.0f, r5 = 0.0f;
    float r1Temp, r2Temp, r3Temp, r4Temp, r5Temp;
    int i = 0;
    x1 = vertexArrayBoard[i * 3];
    y1 = vertexArrayBoard[i * 3 + 1];
    x2 = vertexArrayBoard[(i + 5) * 3];
    y2 = vertexArrayBoard[(i + 5) * 3 + 1];
    r1Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayBoard[(i + 1) * 3];
    y1 = vertexArrayBoard[(i + 1) * 3 + 1];
    x2 = vertexArrayBoard[(i + 6) * 3];
    y2 = vertexArrayBoard[(i + 6) * 3 + 1];
    r2Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayBoard[(i + 2) * 3];
    y1 = vertexArrayBoard[(i + 2) * 3 + 1];
    x2 = vertexArrayBoard[(i + 7) * 3];
    y2 = vertexArrayBoard[(i + 7) * 3 + 1];
    r3Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayBoard[(i + 3) * 3];
    y1 = vertexArrayBoard[(i + 3) * 3 + 1];
    x2 = vertexArrayBoard[(i + 8) * 3];
    y2 = vertexArrayBoard[(i + 8) * 3 + 1];
    r4Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    x1 = vertexArrayBoard[(i + 4) * 3];
    y1 = vertexArrayBoard[(i + 4) * 3 + 1];
    x2 = vertexArrayBoard[(i + 9) * 3];
    y2 = vertexArrayBoard[(i + 9) * 3 + 1];
    r5Temp = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2));

    ////////////////////////////////////////////

    x1 = vertexArrayBoard[i * 3];
    y1 = vertexArrayBoard[i * 3 + 1];
    x2 = vertexArrayBoard[(i + 1) * 3];
    y2 = vertexArrayBoard[(i + 1) * 3 + 1];
    float s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayBoard[(i + 1) * 3];
    y2 = vertexArrayBoard[(i + 1) * 3 + 1];
    x2 = vertexArrayBoard[(i + 2) * 3];
    y2 = vertexArrayBoard[(i + 2) * 3 + 1];
    float s2 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayBoard[(i + 2) * 3];
    y2 = vertexArrayBoard[(i + 2) * 3 + 1];
    x2 = vertexArrayBoard[(i + 3) * 3];
    y2 = vertexArrayBoard[(i + 3) * 3 + 1];
    float s3 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));
    x2 = vertexArrayBoard[(i + 3) * 3];
    y2 = vertexArrayBoard[(i + 3) * 3 + 1];
    x2 = vertexArrayBoard[(i + 4) * 3];
    y2 = vertexArrayBoard[(i + 4) * 3 + 1];
    float s4 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) - ((r1 - r2)/2.0f)*((r1 - r2)/2.0f));

    for (int i = 0; i < numberOfSides; ++i)
    {
        textureArrayBoard.push_back(0.0f);
        textureArrayBoard.push_back(r1/textureVSize);

        textureArrayBoard.push_back(0.08f);
        textureArrayBoard.push_back(r2/textureVSize);

        textureArrayBoard.push_back(0.09f);
        textureArrayBoard.push_back((r3 + (r2Temp - r3Temp)/2.0f)/textureVSize);

        textureArrayBoard.push_back(0.16666f);
        textureArrayBoard.push_back((r4 + (r2Temp - r4Temp)/2.0f)/textureVSize);


        textureArrayBoard.push_back(1.0f);
        textureArrayBoard.push_back((r5 + (r2Temp - r5Temp)/2.0f)/textureVSize);

        /////////////////////////////////////////////////////////

        textureArrayBoard.push_back(0.0f);
        textureArrayBoard.push_back((r1 + r1Temp)/textureVSize);

        textureArrayBoard.push_back(0.08f);
        textureArrayBoard.push_back((r2 + r2Temp)/textureVSize);

        textureArrayBoard.push_back(0.09f);
        textureArrayBoard.push_back((r3 + (r2Temp - r3Temp)/2.0f + r3Temp)/textureVSize);

        textureArrayBoard.push_back(0.16666f);
        textureArrayBoard.push_back((r4 + (r2Temp - r4Temp)/2.0f + r4Temp)/textureVSize);


        textureArrayBoard.push_back(1.0f);
        textureArrayBoard.push_back((r5 + (r2Temp - r5Temp)/2.0f + r5Temp)/textureVSize);

        r1 += r1Temp;
        r2 += r2Temp;
        r3 += r1Temp;
        r4 += r1Temp;
        r5 += r1Temp;
    }
}

void Curve::setIndexArrayBoard()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setIndexArrayBoard()\n";
    indexArrayBoard.clear();
    for (int i = 0; i < vertexArrayBoard.size() / 3 - 5; i += 10)
    {
        indexArrayBoard.push_back(i + 1);
        indexArrayBoard.push_back(i);
        indexArrayBoard.push_back(i + 5);


        indexArrayBoard.push_back(i + 1);
        indexArrayBoard.push_back(i + 5);
        indexArrayBoard.push_back(i + 6);

        indexArrayBoard.push_back(i + 2);
        indexArrayBoard.push_back(i + 1);
        indexArrayBoard.push_back(i + 6);

        indexArrayBoard.push_back(i + 2);
        indexArrayBoard.push_back(i + 6);
        indexArrayBoard.push_back(i + 7);

        indexArrayBoard.push_back(i + 3);
        indexArrayBoard.push_back(i + 2);
        indexArrayBoard.push_back(i + 7);

        indexArrayBoard.push_back(i + 3);
        indexArrayBoard.push_back(i + 7);
        indexArrayBoard.push_back(i + 8);

        indexArrayBoard.push_back(i + 4);
        indexArrayBoard.push_back(i + 3);
        indexArrayBoard.push_back(i + 8);

        indexArrayBoard.push_back(i + 4);
        indexArrayBoard.push_back(i + 8);
        indexArrayBoard.push_back(i + 9);
    }
}



void Curve::setCoordForPoint(int index, float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setCoordForPoint(int index, float x, float y, float z)"
                                       << " index = " << index << " x = " << x
                                       << " y = " << y << " z = " << z << "\n";

    if (index < 0 || index > 2)
    {
        QMessageBox::critical(0, "Ошибка", "Curve::setCoordForPoint(int index, float x, float y, float z) index out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "Curve::setCoordForPoint(int index, float x, float y, float z) index out of range\n";
        QApplication::exit(0);
    }
    controlPoints[index * 3] = x;
    controlPoints[index * 3 + 1] = y;
    controlPoints[index * 3 + 2] = z;

    calculateControlsForAngle(index);
    setVertexArray();
    setTextureArray();
    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    setAngleVertexArray();
    //calculateAngle();
}

vec3 Curve::getCoordOfPoint(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getCoordOfPoint(int index)"
                                       << " index = " << index << "\n";

    if (index < 0 || index > 2)
    {
        QMessageBox::critical(0, "Ошибка", "Curve::getCoordOfPoint(int index) index out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "Curve::getCoordOfPoint(int index) index out of range\n";
        QApplication::exit(0);
    }
    if (index >= 0 && index < 3)
    {
        vec3 p(controlPoints[index * 3], controlPoints[index * 3 + 1], controlPoints[index * 3 + 2]);
        return p;
    }
    else
        return vec3(0.0f, 0.0f, 0.0f);
}

void Curve::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

float Curve::getBoardWidth()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getBoardWidth()\n";
    return boardWidth;
}

bool Curve::getBoardShowStatus()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getBoardShowStatus()\n";
    return showBoard;
}

void Curve::setAngleVertexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setAngleVertexArray()\n";
    angleVertexArray.clear();
    float pi = 3.1415926f;

    float xCenter = controlPoints[0];
    float yCenter = controlPoints[1];
    float zCenter = controlPoints[2];

    float xLeft = controlPoints[3];
    float yLeft = controlPoints[4];
    float zLeft = controlPoints[5];

    float xRight = controlPoints[6];
    float yRight = controlPoints[7];
    float zRight = controlPoints[8];

    float dxLeft = xLeft - xCenter;
    float dyLeft = yLeft - yCenter;
    float dxRight = xRight - xCenter;
    float dyRight = yRight - yCenter;
    float r1 = sqrt(dxLeft * dxLeft + dyLeft * dyLeft);
    float r2 = sqrt(dxRight * dxRight + dyRight * dyRight);

    float dx = xRight - xCenter;
    float dy = yRight - yCenter;
    float r = sqrt(dx * dx + dy * dy);
    float t = dx / r;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle1 = acos(t);
    if (dy <= 0)
        angle1 = 2.0f * pi - angle1;

    dx = xLeft - xCenter;
    dy = yLeft - yCenter;
    r = sqrt(dx * dx + dy * dy);
    t = dx / r;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle2 = acos(t);
    if (dy <= 0)
        angle2 = 2.0f * pi - angle2;

    if (angle1 > angle2)
    {
        angle2 += 2.0f * pi;
    }

    r = r1 > r2 ? r2 : r1;

    for (int i = 0; i <= numberOfSides; ++i)
    {

        float angle = (angle1 + (angle2 - angle1) * float(i) / float(numberOfSides));
        float dx = r * cosf(angle);
        float dy = r * sinf(angle);
        angleVertexArray.push_back(xCenter + dx);
        angleVertexArray.push_back(yCenter + dy);
        angleVertexArray.push_back(0.11f);
    }
    //angleRounding = (angle2 - angle1) * 180.0 / pi;
    // emit angleChanged(angleRounding);
}

void Curve::setAngleColorArray(float red, float green, float blue)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setAngleColorArray(float red, float green, float blue)"
                                       << " red = " << red
                                       << " green = " << green
                                       << " blue = " << blue << "\n";
    angleColorArray.clear();
    for (int i = 0; i < angleVertexArray.size() / 3; ++i)
    {
        angleColorArray.push_back(red);
        angleColorArray.push_back(green);
        angleColorArray.push_back(blue);
    }
}

void Curve::setAngleIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setAngleIndexArray()\n";
    angleIndexArray.clear();
    for (int i = 0; i < angleVertexArray.size() / 3; ++i)
    {
        angleIndexArray.push_back(i);
    }
}

void Curve::calculateAngle()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::calculateAngle()\n";
    float pi = 3.1415926f;

    float xCenter = controlPoints[0];
    float yCenter = controlPoints[1];
    float zCenter = controlPoints[2];

    float xLeft = controlPoints[3];
    float yLeft = controlPoints[4];
    float zLeft = controlPoints[5];

    float xRight = controlPoints[6];
    float yRight = controlPoints[7];
    float zRight = controlPoints[8];

    float dxLeft = xLeft - xCenter;
    float dyLeft = yLeft - yCenter;
    float dxRight = xRight - xCenter;
    float dyRight = yRight - yCenter;
    float r1 = sqrt(dxLeft * dxLeft + dyLeft * dyLeft);
    float r2 = sqrt(dxRight * dxRight + dyRight * dyRight);

    float dx = xRight - xCenter;
    float dy = yRight - yCenter;
    float r = sqrt(dx * dx + dy * dy);
    float t = dx / r;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle1 = acos(t);
    if (dy <= 0)
        angle1 = 2.0f * pi - angle1;

    dx = xLeft - xCenter;
    dy = yLeft - yCenter;
    r = sqrt(dx * dx + dy * dy);
    t = dx / r;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle2 = acos(t);
    if (dy <= 0)
        angle2 = 2.0f * pi - angle2;

    if (angle1 > angle2)
    {
        angle2 += 2.0f * pi;
    }
    float temp = angleRounding;
    angleRounding = (angle2 - angle1) * 180.0 / pi;
    if (temp != angleRounding)
        emit angleChanged(angleRounding);
    else
        angleRounding = temp;

}

bool Curve::calculateLinesIntersection(float a1, float b1, float c1,
                                       float a2, float b2, float c2,
                                       float &x, float &y)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::calculateLinesIntersection(float a1, float b1, float c1,float a2, float b2, float c2,float &x, float &y)"
                                       << " a1 = " << a1 << " b1 = " << b1 << " c1 = " << c1
                                       << " a2 = " << a2 << " b2 = " << b2 << " c2 = " << c2 << "\n";
    // Проходит через начало координат
    if (c1 == 0.0f && a1 != 0.0f && b1 != 0.0f)
    {
        if (a2 != 0.0f || b2 != 0.0f)
        {
            x = (b1 * c2) / (a1 * b2 - a2 * b1);
            y = (-1.0f) * a1 * x / b1;
            return true;
        }
        else
            return false;

    }
    else
        // Параллельна Ох
        if (a1 == 0.0f && b1 != 0.0f && c1 != 0.0f)
        {
            if (a2 != 0.0f)
            {
                y = (-1.0f) * c1 / b1;
                x = (-1.0f) * (b2 * y + c2) / a2;
                return true;
            }
            else
                return false;
        }
        else
            // Параллельна Оу
            if (b1 == 0.0f && a1 != 0.0f && c1 != 0.0f)
            {
                if (b2 != 0.0f)
                {
                    x = (-1.0f) * c1 / a1;
                    y = (-1.0f) * (a2 * x + c2) / b2;
                    return true;
                }
                else
                    return false;
            }
            else
                // Совпадает с Оу
                if (b1 == 0.0f && c1 == 0.0f && a1 != 0.0f)
                {
                    if (b2 != 0.0f)
                    {
                        x = 0.0f;
                        y = (-1.0f) * c2 / b2;
                        return true;
                    }
                    else
                        return false;
                }
                else
                    // Совпадает с Ох
                    if (a1 == 0.0f && c1 == 0.0f && b1 != 0.0f)
                    {
                        if (a2 != 0.0f)
                        {
                            y = 0.0f;
                            x = (-1.0f) * c2 / a2;
                            return true;
                        }
                        else
                            return false;
                    }
                    else
                        if (a1 != 0.0f && c1 != 0.0f && b1 != 0.0f)
                        {
                            // Проходит через начало координат
                            if (c2 == 0.0f && a2 != 0.0f && b2 != 0.0f)
                            {
                                x = (b2 * c1) / (a2 * b1 - a1 * b2);
                                y = (-1.0f) * a2 * x / b2;
                                return true;

                            }
                            else
                                // Параллельна Ох
                                if (a2 == 0.0f && b2 != 0.0f && c2 != 0.0f)
                                {
                                    y = (-1.0f) * c2 / b2;
                                    x = (-1.0f) * (b1 * y + c1) / a1;
                                    return true;
                                }
                                else
                                    // Параллельна Оу
                                    if (b2 == 0.0f && a2 != 0.0f && c2 != 0.0f)
                                    {
                                        x = (-1.0f) * c2 / a2;
                                        y = (-1.0f) * (a1 * x + c1) / b1;
                                        return true;
                                    }
                                    else
                                        // Совпадает с Оу
                                        if (b2 == 0.0f && c2 == 0.0f && a2 != 0.0f)
                                        {
                                            x = 0.0f;
                                            y = (-1.0f) * c1 / b1;
                                            return true;
                                        }
                                        else
                                            // Совпадает с Ох
                                            if (a2 == 0.0f && c2 == 0.0f && b2 != 0.0f)
                                            {
                                                y = 0.0f;
                                                x = (-1.0f) * c1 / a1;
                                                return true;

                                            }
                                            else
                                                if (a2 != 0.0f && c2 != 0.0f && b2 != 0.0f)
                                                {
                                                    y = (c1 * a2 - c2 * a1) / (b2 * a1 - b1 * a2);
                                                    x = (-1.0f) * (b1 * y - c1) / a1;
                                                    return true;
                                                }
                                                else
                                                    return false;
                        }


    return false;
}

void Curve::calculateControlsForAngle(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::calculateControlsForAngle(int index)\n";
    float pi = 3.14159265f;
    float xCenter = controlPoints[0];
    float yCenter = controlPoints[1];

    float xLeft = controlPoints[3];
    float yLeft = controlPoints[4];

    float xRight = controlPoints[6];
    float yRight = controlPoints[7];

    float dxLeft = xLeft - xCenter;
    float dyLeft = yLeft - yCenter;
    float rLeft = sqrt(dxLeft*dxLeft + dyLeft*dyLeft);
    float dxRight = xRight - xCenter;
    float dyRight = yRight - yCenter;
    float rRight = sqrt(dxRight*dxRight + dyRight*dyRight);
    float t = dxRight / rRight;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float alpha1 = acos(t);
    if (dyRight < 0)
        alpha1 = 2.0f * pi - alpha1;
    t = dxLeft / rLeft;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float alpha2 = acos(t);
    if (dyLeft < 0)
        alpha2 = 2.0f * pi - alpha2;
    float res = alpha2 - alpha1;
    if (res >= pi)
    {
        switch (index)
        {
        case 0:
            break;
        case 1:
        {

            controlPoints[3] = xCenter + rLeft * cos(alpha1 + pi);
            controlPoints[4] = yCenter + rLeft * sin(alpha1 + pi);
            angleRounding  = 180.0f;

        }
            break;
        case 2:
        {

            controlPoints[6] = xCenter + rRight * cos(alpha2 - pi);
            controlPoints[7] = yCenter + rRight * sin(alpha2 - pi);
            angleRounding  = 180.0f;

        }
            break;
        default:
            break;
        }
    }
}

void Curve::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "Curve::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

bool Curve::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
}

void Curve::setLeftLength(double length)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setLeftLength(double length)"
                                       << " length = " << length << "\n";
    if (this->leftLength == length)
        return;
    float x1 = controlPoints[0];
    float y1 = controlPoints[1];
    float x2 = controlPoints[3];
    float y2 = controlPoints[4];
    float r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float dx = (x2 - x1) / r * length;
    float dy = (y2 - y1) / r * length;
    controlPoints[3] = x1 + dx;
    controlPoints[4] = y1 + dy;
    setVertexArray();
    setTextureArray();
    this->leftLength = length;
    emit leftLengthChanged(length);
}

void Curve::setRightLength(double length)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setRightLength(double length)"
                                       << " length = " << length << "\n";
    if (this->rightLength == length)
        return;

    float x1 = controlPoints[0];
    float y1 = controlPoints[1];
    float x2 = controlPoints[6];
    float y2 = controlPoints[7];
    float r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float dx = (x2 - x1) / r * length;
    float dy = (y2 - y1) / r * length;
    controlPoints[6] = x1 + dx;
    controlPoints[7] = y1 + dy;
    setVertexArray();
    setTextureArray();
    this->rightLength = length;
    emit rightLengthChanged(length);
}

void Curve::setBoardWidth(double width)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setBoardWidth(double width)"
                                       << " width = " << width << "\n";
    if (boardWidth == width)
        return;
    boardWidth = width;
    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    emit boardWidthChanged(width);
}

void Curve::setBoardShowStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setBoardShowStatus(bool status)"
                                       << " status = " << status << "\n";
    if (showBoard == status)
        return;
    showBoard = status;
    emit showBoardStatusChanged(status);
}

void Curve::setAngle(double angle)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setAngle(double angle)"
                                       << " angle = " << angle << "\n";
    float pi = 3.14159265f;

    if (fabs(angleRounding - angle) < 0.01f)
        return;
    angleRounding = angle;

    float xCenter = controlPoints[0];
    float yCenter = controlPoints[1];
    float zCenter = controlPoints[2];

    float xLeft = controlPoints[3];
    float yLeft = controlPoints[4];
    float zLeft = controlPoints[5];

    float xRight = controlPoints[6];
    float yRight = controlPoints[7];
    float zRight = controlPoints[8];

    float dxLeft = xLeft - xCenter;
    float dyLeft = yLeft - yCenter;
    float dxRight = xRight - xCenter;
    float dyRight = yRight - yCenter;
    float r1 = sqrt(dxLeft * dxLeft + dyLeft * dyLeft);
    float r2 = sqrt(dxRight * dxRight + dyRight * dyRight);

    float dx = xRight - xCenter;
    float dy = yRight - yCenter;
    float rRight = sqrt(dx * dx + dy * dy);
    float t = dx / rRight;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle1 = acos(t);
    if (dy <= 0)
        angle1 = 2.0f * pi - angle1;

    dx = xLeft - xCenter;
    dy = yLeft - yCenter;
    float rLeft = sqrt(dx * dx + dy * dy);
    t = dx / rLeft;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle2 = acos(t);
    if (dy <= 0)
        angle2 = 2.0f * pi - angle2;

    if (angle1 > angle2)
    {
        angle2 += 2.0f * pi;
    }

    dx = rLeft * cos(angle1 + angleRounding * pi / 180.0f);
    dy = rLeft * sin(angle1 + angleRounding * pi / 180.0f);

    controlPoints[3] = xCenter + dx;
    controlPoints[4] = yCenter + dy;

    setVertexArray();
    setTextureArray();
    setIndexArray();

    setVertexArrayBoard();
    setTextureArrayBoard(texture2USize, texture2VSize);
    setIndexArrayBoard();
    setAngleVertexArray();
    emit angleChanged(angleRounding);

}


std::vector<vec3> Curve::getCoordOfControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getCoordOfControl(int index)"
                                       << " index = " << index << "\n";
    std::vector<vec3> res;

    switch (index)
    {
    case 0:
    {
        vec3 p(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        res.push_back(p);
    }
        break;
    case 1:
    {
        vec3 p(controlPoints[3], controlPoints[4], controlPoints[5] + 0.001f);
        res.push_back(p);
    }
        break;
    case 2:
    {
        vec3 p(controlPoints[6], controlPoints[7], controlPoints[8] + 0.001f);
        res.push_back(p);
    }
        break;
    case 3:
    {
        for (int i = 0; i < vertexArrayBoard.size() / 3 - 5; i += 10)
        {
            vec3 p(vertexArrayBoard[(i + 4) * 3],
                    vertexArrayBoard[(i + 4) * 3 + 1],
                    vertexArrayBoard[(i + 4) * 3 + 2]);
            vec3 s(vertexArrayBoard[(i + 9) * 3],
                    vertexArrayBoard[(i + 9) * 3 + 1],
                    vertexArrayBoard[(i + 9) * 3 + 2]);
            res.push_back(p);
            res.push_back(s);
        }
    }
        break;
    case 4:
    {
        vec3 p(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        vec3 s(controlPoints[3], controlPoints[4], controlPoints[5] + 0.001f);
        res.push_back(p);
        res.push_back(s);
    }
        break;
    case 5:
    {
        vec3 p(controlPoints[0], controlPoints[1], controlPoints[2] + 0.001f);
        vec3 s(controlPoints[6], controlPoints[7], controlPoints[8] + 0.001f);
        res.push_back(p);
        res.push_back(s);
    }
        break;
    default:
        break;
    }
    return res;
}




RoadElement *Curve::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getCopy()\n";
    Curve* copyElement = new Curve(*this);
    return copyElement;
}


void Curve::setCoordForControl(int index, std::vector<vec3> &controls)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::setCoordForControl(int index, std::vector<vec3> &controls)"
                                       << " index = " << index << "\n";
    switch (index)
    {
    case 0:
    {
        float x, y;
        x = controlPoints[0];
        y = controlPoints[1];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 1:
    {
        float x, y;
        x = controlPoints[3];
        y = controlPoints[4];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 2:
    {
        float x, y;
        x = controlPoints[6];
        y = controlPoints[7];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 3:
    {
        int i = 0;
        float x, y;
        x = vertexArrayBoard[(i + 4) * 3];
        y = vertexArrayBoard[(i + 4) * 3 + 1];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 4:
    {
        float x, y;
        x = controlPoints[0];
        y = controlPoints[1];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    case 5:
    {
        float x, y;
        x = controlPoints[0];
        y = controlPoints[1];
        resizeByControl(index, controls[0].x - x, controls[0].y - y, x, y);
    }
        break;
    default:
        break;
    }
}


void Curve::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::clearProperties(QLayout *layout)\n";
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


QJsonObject Curve::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "Curve::getJSONInfo()\n";
    QJsonObject element;
    element["Name"] = name;
    element["Layer"] = layer;
    element["Texture1Source"] = texture1;
    element["Texture1USize"] = texture1USize;
    element["Texture1VSize"] = texture1VSize;
    element["Texture2Source"] = texture2;
    element["Texture2USize"] = texture2USize;
    element["Texture2VSize"] = texture2VSize;

    element["BoardWidth"] = boardWidth;
    element["AngleRounding"] = angleRounding;
    element["NumberOfSides"] = numberOfSides;

    element["Id"] = Id;

    QJsonArray tempControlPoints;
    for (int i = 0; i < 9; ++i)
        tempControlPoints.append(QJsonValue(controlPoints[i]));
    element["ControlPoints"] = tempControlPoints;

    element["Fixed"] = fixed;
    element["ShowBoard"] = showBoard;

    return element;

}
