#include "railway.h"
#include "model.h"

bool RailWay::log = true;

RailWay::RailWay()
{

}

RailWay::RailWay(float *axisArray, int size,
                 QString textureSource,
                 float textureUsize, float textureVsize)
{
    name = "RailWay";
    selected = fixed = false;
    layer = 1;
    textureID[0] = TextureManager::getInstance()->getID(textureSource);
    this->textureUsize = textureUsize;
    this->textureVsize = textureVsize;
    axisVertexArray.resize(size);
    for (int i = 0; i < size; ++i)
        axisVertexArray[i] = axisArray[i];
    setVertexArray();
    setTextureArray(textureUsize, textureVsize);
    setIndexArray();
}

RailWay::RailWay(QVector<float> &axisArray,
                 QString textureSource,
                 float textureUsize, float textureVsize)
{
    name = "RailWay";
    selected = fixed = false;
    layer = 1;
    textureID[0] = TextureManager::getInstance()->getID(textureSource);
    this->textureUsize = textureUsize;
    this->textureVsize = textureVsize;
    axisVertexArray.resize(axisArray.size());
    for (int i = 0; i < axisArray.size(); ++i)
        axisVertexArray[i] = axisArray[i];
    setVertexArray();
    setTextureArray(textureUsize, textureVsize);
    setIndexArray();
}

RailWay::~RailWay()
{

}

bool RailWay::isSelected()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::isSelected()\n";
    return selected;
}

void RailWay::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
}

void RailWay::drawFigure(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::drawFigure(QGLWidget *render)\n";
    glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray.begin());
    glTexCoordPointer(2, GL_FLOAT, 0, textureArray.begin());
    glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, indexArray.begin());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);

    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }
}

void RailWay::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::drawSelectionFrame()\n";
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.0f);
}

void RailWay::drawMeasurements(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::drawMeasurements(QGLWidget *render)\n";
}

void RailWay::move(float dx, float dy, float dz)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::move(float dx, float dy, float dz)"
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " dz = " << dz << "\n";
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
    {
        axisVertexArray[i * 3] += dx;
        axisVertexArray[i * 3 + 1] += dy;
    }
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3] += dx;
        vertexArray[i * 3 + 1] += dy;
    }
}

void RailWay::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    if (index < 0 || index >= axisVertexArray.size() / 3)
    {
        QMessageBox::critical(0, "Ошибка", "RailWay::drawControlElement(int index, float lineWidth, float pointSize) index out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->warningLog() << "RailWay::drawControlElement(int index, float lineWidth, float pointSize) index out of range\n";
        return;
    }

    glPointSize(pointSize + 5.0f);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(axisVertexArray[index * 3],
            axisVertexArray[index * 3 + 1],
            axisVertexArray[index * 3 + 2]);
    glEnd();
}

QCursor RailWay::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void RailWay::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " x = " << x
                                       << " y = " << y << "\n";
    if (index < 0 || index >= axisVertexArray.size() / 3)
    {
        QMessageBox::critical(0, "Ошибка", "RailWay::resizeByControl(int index, float dx, float dy, float x, float y) index out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->warningLog() << "RailWay::resizeByControl(int index, float dx, float dy, float x, float y) index out of range\n";
        return;
    }
    axisVertexArray[index * 3] += dx;
    axisVertexArray[index * 3 + 1] += dy;

    setVertexArray();
    setTextureArray(textureUsize, textureVsize);
}

int RailWay::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::getNumberOfControls()\n";
    return axisVertexArray.size() / 3;
}

int RailWay::controlsForPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::controlsForPoint()\n";
    return 1;
}

void RailWay::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
}

void RailWay::getProperties(QFormLayout *layout, QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::getProperties(QFormLayout *layout, QGLWidget *render)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "RailWay::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "RailWay::getProperties(QFormLayout *layout, QGLWidget *render) layout = NULL\n";
        QApplication::exit(0);
    }
}

bool RailWay::isFixed()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::isFixed()\n";
    return fixed;
}

int RailWay::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::getLayer()\n";
    return layer;
}

void RailWay::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::clear()\n";
}

void RailWay::addBreak(bool front)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::addBreak(bool front)"
                                       << " front = " << front << "\n";
    float x, y, z;
    if (front)
    {
        x = axisVertexArray[0];
        y = axisVertexArray[1];
        z = axisVertexArray[2];
        axisVertexArray.push_front(z);
        axisVertexArray.push_front(y);
        axisVertexArray.push_front(x);
    }
    else
    {
        int size = axisVertexArray.size();
        x = axisVertexArray[size - 3];
        y = axisVertexArray[size - 2];
        z = axisVertexArray[size - 1];
        axisVertexArray.push_back(x);
        axisVertexArray.push_back(y);
        axisVertexArray.push_back(z);
    }
    setVertexArray();
    setTextureArray(textureUsize, textureVsize);
    setIndexArray();
}

void RailWay::setVertexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::setVertexArray()\n";
    vertexArray.clear();
    float r = 2.7f / 2.0f;
    for (int i = 0; i < axisVertexArray.size() / 3; ++i)
    {
        // Если теукщий индекс - начало осевой линии,
        // то строим перпендикуляр
        if (i == 0)
        {
            float x1 = axisVertexArray[i * 3];
            float y1 = axisVertexArray[i * 3 + 1];
            float x2 = axisVertexArray[(i + 1) * 3];
            float y2 = axisVertexArray[(i + 1) * 3 + 1];
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

            vertexArray.push_back(x1 + dx);
            vertexArray.push_back(y1 - dy);
            vertexArray.push_back(0.0f);

            vertexArray.push_back(x1 + dx);
            vertexArray.push_back(y1 - dy);
            vertexArray.push_back(0.15f);

            vertexArray.push_back(x1 + dx / r * (0.76f + 0.075f));
            vertexArray.push_back(y1 - dy / r * (0.76f + 0.075f));
            vertexArray.push_back(0.23f);

            vertexArray.push_back(x1 + dx / r * (0.76f + 0.075f));
            vertexArray.push_back(y1 - dy / r * (0.76f + 0.075f));
            vertexArray.push_back(0.41f);

            vertexArray.push_back(x1 + dx / r * 0.76f);
            vertexArray.push_back(y1 - dy / r * 0.76f);
            vertexArray.push_back(0.41f);

            vertexArray.push_back(x1 + dx / r * 0.76f);
            vertexArray.push_back(y1 - dy / r * 0.76f);
            vertexArray.push_back(0.23f);

            ///////////////////////////////////////////////////////////

            vertexArray.push_back(x1 - dx / r * 0.76f);
            vertexArray.push_back(y1 + dy / r * 0.76f);
            vertexArray.push_back(0.23f);

            vertexArray.push_back(x1 - dx / r * 0.76f);
            vertexArray.push_back(y1 + dy / r * 0.76f);
            vertexArray.push_back(0.41f);

            vertexArray.push_back(x1 - dx / r * (0.76f + 0.075f));
            vertexArray.push_back(y1 + dy / r * (0.76f + 0.075f));
            vertexArray.push_back(0.41f);

            vertexArray.push_back(x1 - dx / r * (0.76f + 0.075f));
            vertexArray.push_back(y1 + dy / r * (0.76f + 0.075f));
            vertexArray.push_back(0.23f);

            vertexArray.push_back(x1 - dx);
            vertexArray.push_back(y1 + dy);
            vertexArray.push_back(0.15f);

            vertexArray.push_back(x1 - dx);
            vertexArray.push_back(y1 + dy);
            vertexArray.push_back(0.0f);

        }
        else
        {   // Если теукщий индекс - конец осевой линии,
            // то строим перпендикуляр
            if (i == axisVertexArray.size() / 3 - 1)
            {
                float x1 = axisVertexArray[i * 3];
                float y1 = axisVertexArray[i * 3 + 1];
                float x2 = axisVertexArray[(i - 1) * 3];
                float y2 = axisVertexArray[(i - 1) * 3 + 1];
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

                vertexArray.push_back(x1 - dx);
                vertexArray.push_back(y1 + dy);
                vertexArray.push_back(0.0f);

                vertexArray.push_back(x1 - dx);
                vertexArray.push_back(y1 + dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x1 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y1 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x1 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y1 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x1 - dx / r * 0.76f);
                vertexArray.push_back(y1 + dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x1 - dx / r * 0.76f);
                vertexArray.push_back(y1 + dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                ///////////////////////////////////////////////////////////

                vertexArray.push_back(x1 + dx / r * 0.76f);
                vertexArray.push_back(y1 - dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x1 + dx / r * 0.76f);
                vertexArray.push_back(y1 - dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x1 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y1 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x1 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y1 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x1 + dx);
                vertexArray.push_back(y1 - dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x1 + dx);
                vertexArray.push_back(y1 - dy);
                vertexArray.push_back(0.0f);
            }
            else
            {

                float x1 = axisVertexArray[(i - 1) * 3];
                float y1 = axisVertexArray[(i - 1) * 3 + 1];
                float x2 = axisVertexArray[i * 3];
                float y2 = axisVertexArray[i * 3 + 1];
                float x3 = axisVertexArray[(i + 1) * 3];
                float y3 = axisVertexArray[(i + 1) * 3 + 1];
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

                vertexArray.push_back(x2 - dx);
                vertexArray.push_back(y2 - dy);
                vertexArray.push_back(0.0f);

                vertexArray.push_back(x2 - dx);
                vertexArray.push_back(y2 - dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x2 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 - dx / r * 0.76f);
                vertexArray.push_back(y2 - dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 - dx / r * 0.76f);
                vertexArray.push_back(y2 - dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                ///////////////////////////////////////////////////////////

                vertexArray.push_back(x2 + dx / r * 0.76f);
                vertexArray.push_back(y2 + dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 + dx / r * 0.76f);
                vertexArray.push_back(y2 + dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 + dx);
                vertexArray.push_back(y2 + dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x2 + dx);
                vertexArray.push_back(y2 + dy);
                vertexArray.push_back(0.0f);



                vertexArray.push_back(x2 - dx);
                vertexArray.push_back(y2 - dy);
                vertexArray.push_back(0.0f);

                vertexArray.push_back(x2 - dx);
                vertexArray.push_back(y2 - dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x2 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 - dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 - dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 - dx / r * 0.76f);
                vertexArray.push_back(y2 - dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 - dx / r * 0.76f);
                vertexArray.push_back(y2 - dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                ///////////////////////////////////////////////////////////

                vertexArray.push_back(x2 + dx / r * 0.76f);
                vertexArray.push_back(y2 + dy / r * 0.76f);
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 + dx / r * 0.76f);
                vertexArray.push_back(y2 + dy / r * 0.76f);
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.41f);

                vertexArray.push_back(x2 + dx / r * (0.76f + 0.075f));
                vertexArray.push_back(y2 + dy / r * (0.76f + 0.075f));
                vertexArray.push_back(0.23f);

                vertexArray.push_back(x2 + dx);
                vertexArray.push_back(y2 + dy);
                vertexArray.push_back(0.15f);

                vertexArray.push_back(x2 + dx);
                vertexArray.push_back(y2 + dy);
                vertexArray.push_back(0.0f);


            }
        }
    }
    for (int i = 0; i < vertexArray.size() / 3; ++i)
    {
        vertexArray[i * 3 + 2] -= 0.25f;
    }
}

void RailWay::setTextureArray(float textureUsize, float textureVsize)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::setTextureArray(float textureUsize, float textureVsize)"
                                       << " textureUsize = " << textureUsize
                                       << " textureVsize = " << textureVsize << "\n";
    textureArray.clear();
    for (int i = 0; i < vertexArray.size() / 3 - 12; i += 24)
    {
        float x0 = vertexArray[i * 3];
        float y0 = vertexArray[i * 3 + 1];
        float x1 = vertexArray[(i + 1) * 3];
        float y1 = vertexArray[(i + 1) * 3 + 1];
        float x2 = vertexArray[(i + 2) * 3];
        float y2 = vertexArray[(i + 2) * 3 + 1];
        float x3 = vertexArray[(i + 3) * 3];
        float y3 = vertexArray[(i + 3) * 3 + 1];
        float x4 = vertexArray[(i + 4) * 3];
        float y4 = vertexArray[(i + 4) * 3 + 1];
        float x5 = vertexArray[(i + 5) * 3];
        float y5 = vertexArray[(i + 5) * 3 + 1];
        float x6 = vertexArray[(i + 6) * 3];
        float y6 = vertexArray[(i + 6) * 3 + 1];
        float x7 = vertexArray[(i + 7) * 3];
        float y7 = vertexArray[(i + 7) * 3 + 1];
        float x8 = vertexArray[(i + 8) * 3];
        float y8 = vertexArray[(i + 8) * 3 + 1];
        float x9 = vertexArray[(i + 9) * 3];
        float y9 = vertexArray[(i + 9) * 3 + 1];
        float x10 = vertexArray[(i + 10) * 3];
        float y10 = vertexArray[(i + 10) * 3 + 1];
        float x11 = vertexArray[(i + 11) * 3];
        float y11 = vertexArray[(i + 11) * 3 + 1];
        float x12 = vertexArray[(i + 12) * 3];
        float y12 = vertexArray[(i + 12) * 3 + 1];
        float x13 = vertexArray[(i + 13) * 3];
        float y13 = vertexArray[(i + 13) * 3 + 1];
        float x14 = vertexArray[(i + 14) * 3];
        float y14 = vertexArray[(i + 14) * 3 + 1];
        float x15 = vertexArray[(i + 15) * 3];
        float y15 = vertexArray[(i + 15) * 3 + 1];
        float x16 = vertexArray[(i + 16) * 3];
        float y16 = vertexArray[(i + 16) * 3 + 1];
        float x17 = vertexArray[(i + 17) * 3];
        float y17 = vertexArray[(i + 17) * 3 + 1];
        float x18 = vertexArray[(i + 18) * 3];
        float y18 = vertexArray[(i + 18) * 3 + 1];
        float x19 = vertexArray[(i + 19) * 3];
        float y19 = vertexArray[(i + 19) * 3 + 1];
        float x20 = vertexArray[(i + 20) * 3];
        float y20 = vertexArray[(i + 20) * 3 + 1];
        float x21 = vertexArray[(i + 21) * 3];
        float y21 = vertexArray[(i + 21) * 3 + 1];
        float x22 = vertexArray[(i + 22) * 3];
        float y22 = vertexArray[(i + 22) * 3 + 1];
        float x23 = vertexArray[(i + 23) * 3];
        float y23 = vertexArray[(i + 23) * 3 + 1];
        float pi = 3.14159265f;

        textureArray.push_back(0.0f);
        textureArray.push_back(0.0f);

        float r1 = sqrt((x12 - x0) * (x12 - x0) + (y12 - y0) * (y12 - y0));

        float factor;

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x1 - x0) + (y12 - y0) * (y1 - y0))/r1;

        textureArray.push_back(0.04021f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x2 - x0) + (y12 - y0) * (y2 - y0))/r1;

        textureArray.push_back(0.17359f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x3 - x0) + (y12 - y0) * (y3 - y0))/r1;

        textureArray.push_back(0.19119f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x4 - x0) + (y12 - y0) * (y4 - y0))/r1;

        textureArray.push_back(0.20881f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x5 - x0) + (y12 - y0) * (y5 - y0))/r1;

        textureArray.push_back(0.22642f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x6 - x0) + (y12 - y0) * (y6 - y0))/r1;

        textureArray.push_back(0.77736f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x7 - x0) + (y12 - y0) * (y7 - y0))/r1;

        textureArray.push_back(0.79497f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x8 - x0) + (y12 - y0) * (y8 - y0))/r1;

        textureArray.push_back(0.81258f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x9 - x0) + (y12 - y0) * (y9 - y0))/r1;

        textureArray.push_back(0.83019f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x10 - x0) + (y12 - y0) * (y10 - y0))/r1;

        textureArray.push_back(0.95979f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x11 - x0) + (y12 - y0) * (y11 - y0))/r1;

        textureArray.push_back(1.0f);
        textureArray.push_back(factor/textureVsize);

        //////////////////////////////////////////////////////////////

        textureArray.push_back(0.0f);
        textureArray.push_back(r1 / textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x13 - x0) + (y12 - y0) * (y13 - y0))/r1;

        textureArray.push_back(0.04021f);
        textureArray.push_back(r1 / textureVsize);


        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x14 - x0) + (y12 - y0) * (y14 - y0))/r1;

        textureArray.push_back(0.17359f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x15 - x0) + (y12 - y0) * (y15 - y0))/r1;

        textureArray.push_back(0.19119f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x16 - x0) + (y12 - y0) * (y16 - y0))/r1;

        textureArray.push_back(0.20881f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x17 - x0) + (y12 - y0) * (y17 - y0))/r1;

        textureArray.push_back(0.22642f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x18 - x0) + (y12 - y0) * (y18 - y0))/r1;

        textureArray.push_back(0.77736f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x19 - x0) + (y12 - y0) * (y19 - y0))/r1;

        textureArray.push_back(0.79497f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x20 - x0) + (y12 - y0) * (y20 - y0))/r1;

        textureArray.push_back(0.81258f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x21 - x0) + (y12 - y0) * (y21 - y0))/r1;

        textureArray.push_back(0.83019f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x22 - x0) + (y12 - y0) * (y22 - y0))/r1;

        textureArray.push_back(0.95979f);
        textureArray.push_back(factor/textureVsize);

        if (x12 == x0 && y12 == y0)
            factor = 0.0f;
        else
            factor = ((x12 - x0) * (x23 - x0) + (y12 - y0) * (y23 - y0))/r1;

        textureArray.push_back(1.0f);
        textureArray.push_back(factor/textureVsize);
    }
}

void RailWay::setIndexArray()
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::setIndexArray()\n";
    indexArray.clear();
    for (int i = 0 ; i < vertexArray.size() / 3; i += 24)
    {
        indexArray.push_back(i);
        indexArray.push_back(i + 12);
        indexArray.push_back(i + 13);

        indexArray.push_back(i);
        indexArray.push_back(i + 13);
        indexArray.push_back(i + 1);

        indexArray.push_back(i + 1);
        indexArray.push_back(i + 13);
        indexArray.push_back(i + 14);

        indexArray.push_back(i + 1);
        indexArray.push_back(i + 14);
        indexArray.push_back(i + 2);

        indexArray.push_back(i + 2);
        indexArray.push_back(i + 14);
        indexArray.push_back(i + 15);

        indexArray.push_back(i + 2);
        indexArray.push_back(i + 15);
        indexArray.push_back(i + 3);

        indexArray.push_back(i + 3);
        indexArray.push_back(i + 15);
        indexArray.push_back(i + 16);

        indexArray.push_back(i + 3);
        indexArray.push_back(i + 16);
        indexArray.push_back(i + 4);

        indexArray.push_back(i + 4);
        indexArray.push_back(i + 16);
        indexArray.push_back(i + 17);

        indexArray.push_back(i + 4);
        indexArray.push_back(i + 17);
        indexArray.push_back(i + 5);

        indexArray.push_back(i + 5);
        indexArray.push_back(i + 17);
        indexArray.push_back(i + 18);

        indexArray.push_back(i + 5);
        indexArray.push_back(i + 18);
        indexArray.push_back(i + 6);

        indexArray.push_back(i + 6);
        indexArray.push_back(i + 18);
        indexArray.push_back(i + 19);

        indexArray.push_back(i + 6);
        indexArray.push_back(i + 19);
        indexArray.push_back(i + 7);

        indexArray.push_back(i + 7);
        indexArray.push_back(i + 19);
        indexArray.push_back(i + 20);

        indexArray.push_back(i + 7);
        indexArray.push_back(i + 20);
        indexArray.push_back(i + 8);

        indexArray.push_back(i + 8);
        indexArray.push_back(i + 20);
        indexArray.push_back(i + 21);

        indexArray.push_back(i + 8);
        indexArray.push_back(i + 21);
        indexArray.push_back(i + 9);

        indexArray.push_back(i + 9);
        indexArray.push_back(i + 21);
        indexArray.push_back(i + 22);

        indexArray.push_back(i + 9);
        indexArray.push_back(i + 22);
        indexArray.push_back(i + 10);

        indexArray.push_back(i + 10);
        indexArray.push_back(i + 22);
        indexArray.push_back(i + 23);

        indexArray.push_back(i + 10);
        indexArray.push_back(i + 23);
        indexArray.push_back(i + 11);
    }
}

void RailWay::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "RailWay::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

bool RailWay::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "RailWay::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
    return true;
}

