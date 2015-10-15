#include "splitzone.h"
#include <GL/glu.h>
#include <QApplication>

SplitZone::SplitZone()
{

}

SplitZone::SplitZone(float *pointsArray, int size,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     QString description)
{
    this->description = description;
    this->name = "SplitZone";
    line = NULL;
    this->beginRounding = beginRounding;
    this->endRounding = endRounding;
    this->width = width;
    lineWidth = 0.15f;
    selected = fixed = false;
    this->beginRounding = beginRounding;
    this->endRounding = endRounding;
    calculateLine(pointsArray,size,width);
    lineWidth = 0.15f;
    this->layer = 1;
    this->width = width;
    line = new LineBroken(lineWidth,lineAxisArray,this->size,
                          QApplication::applicationDirPath() + "/models/city_roads/solid.png", 6.0f,
                          "LineBroken", 1);
    selected = fixed = false;
}

SplitZone::SplitZone(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     float width,
                     bool beginRounding,
                     bool endRounding,
                     QString description)
{
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

    //line = new LineBroken(lineWidth,lineAxisArray,size,1.0f, 1.0f, 1.0f, 1.0f, "LineBroken", 1);
    line = new LineBroken(lineWidth,lineAxisArray,this->size,
                          QApplication::applicationDirPath() + "/models/city_roads/solid.png", 6.0f,
                          "LineBroken", 1);
    selected = fixed = false;
}

SplitZone::~SplitZone()
{
    delete line;
    line = NULL;

    delete[] lineAxisArray;
    lineAxisArray = NULL;
}



bool SplitZone::isSelected()
{
    return selected;
}

void SplitZone::setSelectedStatus(bool status)
{
    selected = status;
}

void SplitZone::drawFigure(QGLWidget *render)
{

    line->drawFigure(render);
    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
    //qDebug() << "SplitZone: drawFigure()";
}

void SplitZone::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 2.0f, 5.0f);
}

void SplitZone::drawMeasurements(QGLWidget *render)
{
    GLdouble x, y, z;
    GLdouble wx, wy, wz;
    QFont shrift = QFont("Times", 8, QFont::Black);
    float x1, x2, y1, y2;

    // Ширина полосы
    x1 = p2.x;
    y1 = p2.y;
    x2 = p3.x;
    y2 = p3.y;
    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    glColor3f(0.0f, 1.0f, 0.0f);
    float dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, "L=" + QString("%1").arg(dr), shrift);

    x1 = p1.x + (p4.x - p1.x)/4.0f;
    y1 = p1.y + (p4.y - p1.y)/4.0f;
    x2 = p2.x + (p3.x - p2.x)/4.0f;
    y2 = p2.y + (p3.y - p2.y)/4.0f;

    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x1, y1, 0.3f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x2, y2, 0.3f);
    glEnd();

    x = (x1 + x2) / 2.0f;
    y = (y1 + y2) / 2.0f;
    z = 0.0f;
    dr = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    getWindowCoord(x, y, z, wx, wy, wz);
    render->renderText(wx + 5, wy + 5, "W=" + QString("%1").arg(dr), shrift);
}

void SplitZone::move(float dx, float dy, float dz)
{
    line->move(dx, dy, dz);
    for (int i = 0; i < size / 3; ++i)
    {
        this->lineAxisArray[i * 3] += dx;
        this->lineAxisArray[i * 3 + 1] += dy;
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
    for (int i = 0; i < axisArray.size() / 3; ++i)
    {
        axisArray[i * 3] += dx;
        axisArray[i * 3 + 1] += dy;
    }

}

void SplitZone::drawControlElement(int index, float lineWidth, float pointSize)
{
    /*
    switch (index)
    {
    case 0:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p1.x, p1.y, p1.z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p2.x, p2.y, p2.z);
        glEnd();
    }
        break;
    case 1:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p3.x, p3.y, p3.z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p4.x, p4.y, p4.z);
        glEnd();
    }
        break;
    case 2:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p2.x, p2.y, p2.z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p3.x, p3.y, p3.z);
        glEnd();
    }
        break;
    case 3:
    {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p4.x, p4.y, p4.z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(p1.x, p1.y, p1.z);
        glEnd();
    }
        break;
    default:
        break;
    }
    */

    if (index >= axisArray.size() / 3)
    {

    }
    else
    {
        glPointSize(pointSize + 5.0f);
        glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(axisArray[index * 3],
                axisArray[index * 3 + 1],
                axisArray[index * 3 + 2]);
        glEnd();
    }
}

QCursor SplitZone::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void SplitZone::resizeByControl(int index, float dx, float dy, float x, float y)
{
    /*
    switch (index)
    {
    case 0:
    {
        pBegin.x += dx;
        pBegin.y += dy;
        delete []lineAxisArray;
        calculateLine(pBegin, pEnd, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);

    }
        break;
    case 1:
    {
        pEnd.x += dx;
        pEnd.y += dy;
        delete []lineAxisArray;
        calculateLine(pBegin, pEnd, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
    }
        break;
    case 2:
    {
        float dr = ((pPerpEnd.x - pPerpBegin.x)*dx + (pPerpEnd.y - pPerpBegin.y)*dy)/
                sqrt((pPerpEnd.x - pPerpBegin.x)*(pPerpEnd.x - pPerpBegin.x) +
                     (pPerpEnd.y - pPerpBegin.y)*(pPerpEnd.y - pPerpBegin.y));

        delete []lineAxisArray;
        width += dr * 2.0f;
        calculateLine(pBegin, pEnd, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
    }
        break;
    case 3:
    {
        float dr = ((pPerpBegin.x - pPerpEnd.x)*dx + (pPerpBegin.y - pPerpEnd.y)*dy)/
                sqrt((pPerpBegin.x - pPerpEnd.x)*(pPerpBegin.x - pPerpEnd.x) +
                     (pPerpBegin.y - pPerpEnd.y)*(pPerpBegin.y - pPerpEnd.y));

        delete []lineAxisArray;
        width += dr * 2.0f;
        calculateLine(pBegin, pEnd, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
    }
        break;
    default:
        break;
    }
    */

    if (index >= axisArray.size() / 3)
    {

    }
    else
    {
        axisArray[index * 3] += dx;
        axisArray[index * 3 + 1] += dy;
        calculateLine(axisArray, width);
        line->setVertexArray(lineWidth, lineAxisArray, size);
    }
}

int SplitZone::getNumberOfControls()
{
    //return 4;
    return axisArray.size() / 3 + 2;
}

int SplitZone::controlsForPoint()
{
    return 1;
}

void SplitZone::changeColorOfSelectedControl(int index)
{
}

void SplitZone::getProperties(QFormLayout *layout, QGLWidget *render)
{
}

bool SplitZone::isFixed()
{
    return fixed;
}

int SplitZone::getLayer()
{
    return layer;
}

void SplitZone::clear()
{
}

void SplitZone::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

bool SplitZone::setFixed(bool fixed)
{
    this->fixed = fixed;
}

void SplitZone::calculateLine(vec3 p1, vec3 p2, float width)
{

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
    float alpha = acos((p1.x - p2.x) / r);

    if ((p1.y - p2.y) < 0)
        alpha = 2.0f * pi - alpha;
    float alpha1 = alpha - pi / 2.0f;
    if (alpha1 < 0)
        alpha1 = 2.0f * pi + alpha1;
    float alpha2 = alpha1 + pi;

    int numberOfSides = 10;
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
            float xR1, yR1, xR2, yR2;
            float pi = 3.1415926f;
            float alpha = acos((x1 - x2) / r);

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
                int numberOfSides = 10;
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
                float xR1, yR1, xR2, yR2;
                float pi = 3.1415926f;
                float alpha = acos((x1 - x2) / r);

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
                    int numberOfSides = 10;
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
                float xR1, yR1, xR2, yR2;
                float pi = 3.1415926f;
                float alpha = acos((x1 - x2) / r);

                if ((x1 - x2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                float alpha2 = alpha1 + pi;
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

void SplitZone::calculateLine(QVector<GLfloat> &pointsArray, float width)
{
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
            float xR1, yR1, xR2, yR2;
            float pi = 3.1415926f;
            float alpha = acos((x1 - x2) / r);

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
                int numberOfSides = 10;
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
                float xR1, yR1, xR2, yR2;
                float pi = 3.1415926f;
                float alpha = acos((x1 - x2) / r);

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
                    int numberOfSides = 10;
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
                float xR1, yR1, xR2, yR2;
                float pi = 3.1415926f;
                float alpha = acos((x1 - x2) / r);

                if ((x1 - x2) < 0)
                    alpha = 2.0f * pi - alpha;
                float alpha1 = alpha - pi / 2.0f;
                if (alpha1 < 0)
                    alpha1 = 2.0f * pi + alpha1;
                float alpha2 = alpha1 + pi;
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

float SplitZone::getWidth()
{
    return width;
}

void SplitZone::setDescription(const QString& description)
{
    this->description = description;
}

void SplitZone::drawDescription(QGLWidget *render, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    if (render && description[0] != '\0')
    {
        GLdouble x, y, z;
        GLdouble wx, wy, wz;
        x = xCenter;
        y = yCenter;
        z = 0.0f;
        QFont shrift = QFont("Times", 8, QFont::Black);
        getWindowCoord(x, y, z, wx, wy, wz);
        render->renderText(wx + 5, wy + 5, description, shrift);
    }
}

void SplitZone::rotate(float angle, float x, float y, float z)
{
    line->rotate(angle, x, y, z);
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
}

void SplitZone::addBreak(bool front)
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
