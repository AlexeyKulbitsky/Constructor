#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QList>
class RoadElement;
#include "roadelement.h"
#include "roundingroad.h"
#include "linesimple.h"
#include "roadsimple.h"
#include "linebroken.h"
#include "roadbroken.h"
#include "compositeroad.h"
#include "cube.h"
#include "curve.h"
#include "intersection.h"
#include "splitzone.h"
#include "voltageline.h"
#include "doublevoltageline.h"
#include "railway.h"
#include "pole.h"
#include "beziers.h"
#include "billboard.h"
#include "arch.h"
#include "mapplane.h"

struct Group
{
    QList<RoadElement*> elements;
    bool visible;
};

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    virtual ~Model();

private:
    int numberOfGroups;
    //std::list<RoadElement*> group[4];
    Group group[5];
    // 0 - группа для дорожного полотна
    // 1 - группа для дорожной разметки
    // 2 - группа для тротуаров, разделительных полос
    // 3 - группа для рабочих инструментов
    // 4 - группа для спутниковой подложки
public:
    void addElement(RoadElement* element, int groupIndex);
    void deleteElement(int elementId, int groupIndex);
    int getNumberOfGroups() { return numberOfGroups; }
    QList<RoadElement*>& getGroup(int groupIndex);

    bool isGroupVisible(int index);
    void setModified(bool status);
    bool isModified();
    void clear();
signals:

public slots:
    void setRoadVisible(bool);
    void setLinesVilible(bool);

private:
    bool modified;

};

#endif // MODEL_H
