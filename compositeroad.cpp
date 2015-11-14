#include "compositeroad.h"

bool CompositeRoad::log = true;

CompositeRoad::CompositeRoad()
{
    selected = false;
    fixed = false;
    name = "CompositeRoad";
    layer = 0;
    zRadius = 2.0f;
    elementX = elementY = 0.0f;
}

CompositeRoad::CompositeRoad(const CompositeRoad &source)
{
    selected = source.selected;
    fixed = source.fixed;
    name = source.name;
    layer = source.layer;
    for (QList<RoadElement*>::const_iterator it = source.elements.begin();
         it != source.elements.end(); ++it)
    {
        elements.push_back((*it)->getCopy());
    }
    elementX = source.elementX;
    elementY = source.elementY;
}

CompositeRoad::~CompositeRoad()
{
    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i])
            delete elements[i];
        elements[i] = NULL;
    }
}



bool CompositeRoad::isSelected()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::isSelected()\n";
    return selected;
}

void CompositeRoad::setSelectedStatus(bool status)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::setSelectedStatus(bool status)"
                                   << " status = " << status << "\n";
    selected = status;
    for (QList<RoadElement*>::iterator it = elements.begin();
         it != elements.end();
         ++it)
    {
        (*it)->setSelectedStatus(status);
    }
}

void CompositeRoad::drawFigure(QGLWidget *render)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawFigure(QGLWidget *render)\n";

    for (int i = 0; i < 5; ++i)
    {
        for (QList<RoadElement*>::iterator it = elements.begin();
             it != elements.end();
             ++it)
        {
            if ((*it)->getLayer() == i)
                (*it)->drawFigure(render);
        }
    }
    if (selected)
    {
        glDisable(GL_DEPTH_TEST);
        drawSelectionFrame();
        glEnable(GL_DEPTH_TEST);
    }

}

void CompositeRoad::drawSelectionFrame()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawSelectionFrame()\n";
    for (QList<RoadElement*>::iterator it = elements.begin();
         it != elements.end();
         ++it)
    {
        (*it)->drawSelectionFrame();
    }
    drawControlElement(0, 5.0f, 5.0f);
}

void CompositeRoad::drawMeasurements(QGLWidget *render)
{
    if (!showMeasurements)
        return;
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawMeasurements(QGLWidget *render)\n";
}

void CompositeRoad::move(float dx, float dy, float dz)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawSelectionFrame()"
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " dz = " << dz << "\n";
    if (fixed)
    {
        return;
    }
    for (QList<RoadElement*>::iterator it = elements.begin();
         it != elements.end();
         ++it)
    {
        (*it)->move(dx, dy);
    }
    elementX += dx;
    elementY += dy;
    setZRotVertexArray();
}

void CompositeRoad::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawControlElement(int index, float lineWidth, float pointSize)"
                                   << " index = " << index
                                   << " lineWidth = " << lineWidth
                                   << " pointSize = " << pointSize << "\n";
    switch (index)
    {
    case 0:
//
//        glDisableClientState(GL_NORMAL_ARRAY);
//        glDisable(GL_TEXTURE_2D);
//        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//        glEnableClientState(GL_COLOR_ARRAY);
        glLineWidth(lineWidth);
        glColorPointer(3, GL_FLOAT,0,zRotColorArray.begin());
        glVertexPointer(3, GL_FLOAT, 0, zRotVertexArray.begin());
        glDrawElements(GL_LINE_STRIP,zRotIndexArray.size(),GL_UNSIGNED_INT,zRotIndexArray.begin());
        break;
    default:
        break;
    }

}

QCursor CompositeRoad::getCursorForControlElement(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getCursorForControlElement(int index)"
                                   << " index = " << index << "\n";
    switch (index)
    {
    case 0:
        return Qt::CrossCursor;
    default:
        return Qt::SizeAllCursor;
    }

    return Qt::SizeAllCursor;
}

void CompositeRoad::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::resizeByControl(int index, float dx, float dy, float x, float y)"
                                   << " index = " << index
                                   << " dx = " << dx
                                   << " dy = " << dy
                                   << " x = " << x
                                   << " y = " << y << "\n";
    if (fixed)
    {
        return;
    }
    switch (index)
    {
    case 0:
    {
        float X1 = x;
        float Y1 = y;
        float X2 = elementX;
        float Y2 = elementY;
        float X3 = X1 + dx;
        float Y3 = Y1 + dy;
        float dx1 = X1 - X2;
        float dy1 = Y1 - Y2;
        float r1 = sqrt(dx1*dx1 + dy1*dy1);
        float dx2 = X3 - X2;
        float dy2 = Y3 - Y2;
        float r2 = sqrt(dx2*dx2 + dy2*dy2);
        float pi = 3.14159265f;
        float t = dx1 / r1;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle1 = acos(t);
        if (dy1 < 0)
            angle1 = 2.0f * pi - angle1;
        t = dx2 / r2;
        if (t > 1)
            t = 1.0f;
        if (t < -1)
            t = -1.0f;
        float angle2 = acos(t);
        if (dy2 < 0)
            angle2 = 2.0f * pi - angle2;
        float angle = angle2 - angle1;
        //setZRotation(zRot + angle * 180.0f / pi);
    }
        break;
    default:
        break;
    }
}

int CompositeRoad::getNumberOfControls()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getNumberOfControls()\n";
    return 1;
}

int CompositeRoad::controlsForPoint()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::controlsForPoint()\n";
    return 0;
}

void CompositeRoad::changeColorOfSelectedControl(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::changeColorOfSelectedControl(int index)"
                                   << "index = " << index << "\n";
}

void CompositeRoad::getProperties(QFormLayout *layout, QGLWidget *render)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getProperties(QFormLayout *layout, QGLWidget *render\n)";
}

bool CompositeRoad::isFixed()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::isFixed()\n";
    return fixed;
}

void CompositeRoad::addElement(RoadElement *element)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::addElement(RoadElement *element)\n";
    if (element == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "CompositeRoad::addElement(RoadElement *element) element = NULL",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "CompositeRoad::addElement(RoadElement *element) element = NULL\n";
        return;
    }
    elements.push_back(element);
    float sumX = 0.0f, sumY = 0.0f;
    for (int i = 0; i < elements.size(); ++i)
    {
        sumX += elements[i]->getElementX();
        sumY += elements[i]->getElementY();
    }
    elementX = sumX / float(elements.size());
    elementY = sumY / float(elements.size());

    setZRotVertexArray();
    setZRotColorArray(0.0f, 1.0f, 0.0f);
    setZRotIndexArray();
}

CompositeRoad::setZRotVertexArray()
{
    int numberOfSides = 40;
    if (zRotVertexArray.size() != (numberOfSides + 1) * 3)
        zRotVertexArray.resize((numberOfSides + 1) * 3);
    float pi = 3.14159265f;
    for (int i = 0; i <= numberOfSides; ++i)
    {
        float angle = 2.0f * pi / (float(numberOfSides)) * float(i);
        zRotVertexArray[i * 3] = elementX + zRadius * cosf(angle);
        zRotVertexArray[i * 3 + 1] = elementY + zRadius * sinf(angle);
        zRotVertexArray[i * 3 + 2] = 0.0f;
    }
}

CompositeRoad::setZRotColorArray(float r, float g, float b)
{
    if (zRotColorArray.size() != zRotVertexArray.size())
        zRotColorArray.resize(zRotVertexArray.size());
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotColorArray[i * 3] = r;
        zRotColorArray[i * 3 + 1] = g;
        zRotColorArray[i * 3 + 2] = b;
    }
}

CompositeRoad::setZRotIndexArray()
{
    if (zRotIndexArray.size() != zRotVertexArray.size() / 3)
        zRotIndexArray.resize(zRotVertexArray.size() / 3);
    for (int i = 0; i < zRotVertexArray.size() / 3; ++i)
    {
        zRotIndexArray[i] = i;
    }
}


bool CompositeRoad::setFixed(bool fixed)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::setFixed(bool fixed)"
                                   << " fixed = " << fixed << "\n";
    this->fixed = fixed;
}

void CompositeRoad::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "CompositeRoad::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


int CompositeRoad::getLayer()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getLayer()\n";
    return layer;
}


void CompositeRoad::deleteElement(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::deleteElement(int index)"
                                   << " index = " << index << "\n";
    if (index >= elements.size())
    {
        Logger::getLogger()->errorLog() << "CompositeRoad::deleteElement(int index), index out of range\n";

        QMessageBox::critical(0, "Ошибка", "CompositeRoad::deleteElement(int index), index out of range");
        return;
    }
    elements.removeAt(index);
}

int CompositeRoad::getNumberOfElements()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getNumberOfElements()\n";
    return elements.size();
}

RoadElement *CompositeRoad::getElement(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getElement(int index)"
                                   << " index = " << index << "\n";
    if (index >= elements.size())
    {
        if (log)
        Logger::getLogger()->errorLog() << "CompositeRoad::getElement(int index), index out of range, return NULL pointer\n";
        QMessageBox::critical(0, "Ошибка", "CompositeRoad::getElement(int index), index out of range, return NULL pointer");
        return NULL;
    }
    return elements.at(index);
}


void CompositeRoad::clear()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::clear()\n";
}


RoadElement *CompositeRoad::getCopy()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getCopy()\n";
    CompositeRoad* copyElement = new CompositeRoad(*this);
    return copyElement;
}


void CompositeRoad::clearProperties(QLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "CompositeRoad::clearProperties(QLayout *layout)\n";
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}


QJsonObject CompositeRoad::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "CompositeRoad::getJSONInfo()\n";
    QJsonObject element;

    element["Name"] = name;
    element["Layer"] = layer;
    element["Fixed"] = fixed;
    element["Id"] = Id;
    QJsonArray tempElements;
    for (int i = 0; i < elements.size(); ++i)
    {
        tempElements.append(elements[i]->getJSONInfo());
    }
    element["Elements"] = tempElements;
    return element;
}
