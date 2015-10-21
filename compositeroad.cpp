#include "compositeroad.h"

CompositeRoad::CompositeRoad()
{
    selected = false;
    fixed = false;
    name = "CompositeRoad";
    layer = 0;
}

CompositeRoad::~CompositeRoad()
{

}



bool CompositeRoad::isSelected()
{
    return selected;
}

void CompositeRoad::setSelectedStatus(bool status)
{
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
    for (QList<RoadElement*>::iterator it = elements.begin();
         it != elements.end();
         ++it)
    {
        (*it)->drawSelectionFrame();
    }
}

void CompositeRoad::drawMeasurements(QGLWidget *render)
{
}

void CompositeRoad::move(float dx, float dy, float dz)
{
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
}

QCursor CompositeRoad::getCursorForControlElement(int index)
{
    return Qt::SizeAllCursor;
}

void CompositeRoad::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
}

int CompositeRoad::getNumberOfControls()
{
    return 1;
}

int CompositeRoad::controlsForPoint()
{
    return 1;
}

void CompositeRoad::changeColorOfSelectedControl(int index)
{
}

void CompositeRoad::getProperties(QFormLayout *layout, QGLWidget *render)
{
}

bool CompositeRoad::isFixed()
{
    return fixed;
}

void CompositeRoad::addElement(RoadElement *element)
{
    elements.push_back(element);
}


bool CompositeRoad::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int CompositeRoad::getLayer()
{
    return layer;
}


void CompositeRoad::deleteElement(int index)
{
    if (index >= elements.size())
        return;
    elements.removeAt(index);
}

int CompositeRoad::getNumberOfElements()
{
    return elements.size();
}

RoadElement *CompositeRoad::getElement(int index)
{
    if (index >= elements.size())
        return NULL;
    return elements.at(index);
}


void CompositeRoad::clear()
{
}
