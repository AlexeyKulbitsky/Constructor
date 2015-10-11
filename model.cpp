#include "model.h"
#include <QApplication>

Model::Model(QObject *parent) : QObject(parent)
{
    numberOfGroups = 4;
    for (int i = 0; i < numberOfGroups; ++i)
    {
        group[i].visible = true;
        group[i].elements.clear();
    }
    modified = false;
    //TextureManager::getInstance()->addTexture(QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg");
    //TextureManager::getInstance()->addTexture(QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg");
}

void Model::addElement(RoadElement *element, int groupIndex)
{
    if (groupIndex >= numberOfGroups)
        return;
    group[groupIndex].elements.push_back(element);
}

void Model::deleteElement(int elementId, int groupIndex)
{
    if (groupIndex >= numberOfGroups)
        return;
    std::list<RoadElement*>::iterator it = group[groupIndex].elements.begin();
    for (int i = 0; i <= elementId; ++i)
        it++;
    group[groupIndex].elements.erase(it);
}

std::list<RoadElement *> &Model::getGroup(int groupIndex)
{

    return group[groupIndex].elements;
}

bool Model::isGroupVisible(int index)
{
    return group[index].visible;
}

void Model::setModified(bool status)
{
    modified = status;
}

bool Model::isModified()
{
    return modified;
}

void Model::clear()
{
    for (int i = 0; i < numberOfGroups; ++i)
    {
        group[i].elements.clear();
    }
    modified = false;
}

void Model::setRoadVisible(bool status)
{
    group[0].visible = status;
}

void Model::setLinesVilible(bool status)
{
    group[1].visible = status;
}

