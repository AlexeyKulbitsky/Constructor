#include "roadelementmanager.h"
#include <GL/gl.h>

RoadElementManager* RoadElementManager::manager = NULL;

int RoadElementManager::getElementID(const QString &name)
{
    if (!elements.contains(name))
        return -1;
    else
    {
        return elements[name].id;
    }
}

void RoadElementManager::setID(const QString &name, unsigned int ID)
{
    elements[name].id = ID;
}

void RoadElementManager::addReference(const QString &name)
{
    if (elements.contains(name))
        elements[name].count++;
}

void RoadElementManager::deleteReference(const QString &name)
{
    if (elements.contains(name))
    {
        elements[name].count--;
        if (elements[name].count <= 0)
        {
            glDeleteLists(elements[name].id, 1);
            elements.remove(name);
        }
    }
}
