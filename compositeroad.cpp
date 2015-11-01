#include "compositeroad.h"

bool CompositeRoad::log = true;

CompositeRoad::CompositeRoad()
{
    selected = false;
    fixed = false;
    name = "CompositeRoad";
    layer = 0;
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
    if (selected)
    {
        drawSelectionFrame();
    }
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
}

void CompositeRoad::drawMeasurements(QGLWidget *render)
{
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
}

void CompositeRoad::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::drawControlElement(int index, float lineWidth, float pointSize)"
                                   << " index = " << index
                                   << " lineWidth = " << lineWidth
                                   << " pointSize = " << pointSize << "\n";
}

QCursor CompositeRoad::getCursorForControlElement(int index)
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getCursorForControlElement(int index)"
                                   << " index = " << index << "\n";
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
}

int CompositeRoad::getNumberOfControls()
{
    if (log)
    Logger::getLogger()->infoLog() << "CompositeRoad::getNumberOfControls()\n";
    return 0;
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
