#include "model.h"
#include <QApplication>

Model::Model(QObject *parent) : QObject(parent)
{
    numberOfGroups = 5;
    for (int i = 0; i < numberOfGroups; ++i)
    {
        group[i].visible = true;
        group[i].elements.clear();
    }
    modified = false;
    //TextureManager::getInstance()->addTexture(QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg");
    //TextureManager::getInstance()->addTexture(QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg");
}

Model::~Model()
{

    for (int i = 0; i < numberOfGroups; ++i)
    {
        for (int j = 0; j < group[i].elements.size(); ++j)
        {
            if (group[i].elements.at(j))
            {
                delete group[i].elements.at(j);
                group[i].elements.replace(j, NULL);
            }
            else
            {
                QMessageBox::critical(0, "Ошибка", QString("Error while clearing model") +
                                      QString("group = ") + QString::number(i) +
                                      QString(", index = ") + QString::number(j),
                                      QMessageBox::Yes);
                Logger::getLogger()->writeLog(QString("Error while clearing model") +
                                              QString("group = ") + QString::number(i) +
                                              QString(", index = ") + QString::number(j));
            }
        }
        group[i].elements.clear();
    }

}


void Model::addElement(RoadElement *element, int groupIndex)
{
    Logger::getLogger()->writeLog("Model::addElement(RoadElement *element, int groupIndex)");
    if (!element)
    {
        QMessageBox::critical(0, "Ошибка", "Trying to add empty element to the model. Program terminates.",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("Trying to add empty element to the model. Program terminates.");
        QApplication::exit(0);
    }
    if (groupIndex >= numberOfGroups || groupIndex < 0)
    {
        QMessageBox::critical(0, "Ошибка", "Model::addElement(RoadElement *element, int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("Model::addElement(RoadElement *element, int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range");
        QApplication::exit(0);
    }
    group[groupIndex].elements.push_back(element);
    Logger::getLogger()->writeLog("Model::addElement(" + element->getName() + ", " +
                                  QString::number(groupIndex) + ")");
}

void Model::deleteElement(int elementId, int groupIndex)
{
    Logger::getLogger()->writeLog("Model::deleteElement(int elementId = " + QString::number(elementId) +
                                  ", int groupIndex = " + QString::number(groupIndex) + ")");

    if (groupIndex >= numberOfGroups || groupIndex < 0)
    {
        QMessageBox::critical(0, "Ошибка", "Model::deleteElement(int elementId, int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("Model::deleteElement(int elementId, int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range");
        QApplication::exit(0);
    }
    if (elementId >= group[groupIndex].elements.size() || elementId < 0)
    {
        QMessageBox::critical(0, "Ошибка", "In model element index = " + QString::number(elementId) + " out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("In model element index = " + QString::number(elementId) + " out of range");
        QApplication::exit(0);
    }
    delete group[groupIndex].elements.at(elementId);
    group[groupIndex].elements.removeAt(elementId);


}

QList<RoadElement *> &Model::getGroup(int groupIndex)
{
    if (groupIndex < 0 || groupIndex >= numberOfGroups)
    {
        QMessageBox::critical(0, "Ошибка", "Model::getGroup(int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("Model::getGroup(int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range");
        QApplication::exit(0);
    }
    return group[groupIndex].elements;
}

bool Model::isGroupVisible(int index)
{
    if (index >= numberOfGroups || index < 0)
    {
        QMessageBox::critical(0, "Ошибка", "Model::isGroupVisible(int index) In model group index = " + QString::number(index) + " out of range",
                              QMessageBox::Yes);
        Logger::getLogger()->writeLog("Model::isGroupVisible(int index) In model group index = " + QString::number(index) + " out of range");
        QApplication::exit(0);
    }
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
    Logger::getLogger()->writeLog("Model::clear()");

    for (int i = 0; i < numberOfGroups; ++i)
    {
        for (int j = 0; j < group[i].elements.size(); ++j)
        {
            if (group[i].elements.at(j))
            {
                delete group[i].elements.at(j);
            }
            else
            {
                QMessageBox::critical(0, "Ошибка", QString("Error while clearing model") +
                                      QString("group = ") + QString::number(i) +
                                      QString(", index = ") + QString::number(j),
                                      QMessageBox::Yes);
                Logger::getLogger()->writeLog(QString("Error while clearing model") +
                                              QString("group = ") + QString::number(i) +
                                              QString(", index = ") + QString::number(j));
            }
        }
        group[i].elements.clear();
    }
    modified = false;

}

void Model::setRoadVisible(bool status)
{
    Logger::getLogger()->writeLog("Model::setRoadVisible(" + QString::number(status) + ")");
    //group[0].visible = status;
    for (int i = 0; i < group[0].elements.size(); ++i)
        group[0].elements[i]->setDrawRoadStatus(status);
    emit visibilityChanged();
}

void Model::setLinesVilible(bool status)
{
    Logger::getLogger()->writeLog("Model::setLinesVilible(" + QString::number(status) + ")");
    group[1].visible = status;
    for (int i = 0; i < group[0].elements.size(); ++i)
        group[0].elements[i]->setDrawLinesStatus(status);
    emit visibilityChanged();

}

void Model::setMapVisible(bool status)
{
    Logger::getLogger()->writeLog("Model::setMapVisible(" + QString::number(status) + ")");
    group[numberOfGroups - 1].visible = status;
    emit visibilityChanged();
}

void Model::setRulerVisible(bool status)
{
    Logger::getLogger()->writeLog("Model::setRulerVisible(" + QString::number(status) + ")");
    group[numberOfGroups - 2].visible = status;
    emit visibilityChanged();
}

